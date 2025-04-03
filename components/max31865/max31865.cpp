#include "max31865.hpp"
#include "max31865_registers.hpp"
#include "spi_device.hpp"
#include <cmath>
#include <optional>
#include <utility>

namespace MAX31865 {

    MAX31865::MAX31865(SPIDevice&& spi_device,
                       std::int16_t const threshold_min,
                       std::int16_t const threshold_max,
                       NWires const nwires,
                       FaultDetect const fault_detect,
                       FaultClear const fault_clear,
                       Filter const filter,
                       ConvMode const conv_mode) noexcept :
        spi_device_{std::forward<SPIDevice>(spi_device)}
    {
        this->initialize(threshold_min, threshold_max, nwires, fault_detect, fault_clear, filter, conv_mode);
    }

    MAX31865::~MAX31865() noexcept
    {
        this->deinitialize();
    }

    std::optional<std::int16_t> MAX31865::get_resistance_raw() noexcept
    {
        if (!this->initialized_) {
            return std::optional<std::int16_t>{std::nullopt};
        }

        this->set_vbias(true);
        vTaskDelay(pdMS_TO_TICKS(10));

        if (this->get_config_register().conv_mode == std::to_underlying(ConvMode::ONESHOT)) {
            this->start_one_shot_conversion();
            vTaskDelay(pdMS_TO_TICKS(50));
        }

        if (this->get_config_register().oneshot) {
            return std::optional<std::int16_t>{std::nullopt};
        }

        auto const rtd = this->get_rtd_registers();

        this->set_vbias(false);

        return rtd.fault ? std::optional<std::int16_t>{std::nullopt} : std::optional<std::int16_t>{rtd.rtd};
    }

    std::optional<float> MAX31865::get_resistance_scaled() noexcept
    {
        return this->get_resistance_raw().transform([](std::int16_t const raw) { return static_cast<float>(raw); });
    }

    void MAX31865::initialize(std::int16_t const threshold_min,
                              std::int16_t const threshold_max,
                              NWires const nwires,
                              FaultDetect const fault_detect,
                              FaultClear const fault_clear,
                              Filter const filter,
                              ConvMode const conv_mode) noexcept
    {
        this->set_config(nwires, fault_detect, fault_clear, filter, conv_mode);
        this->set_high_fault(threshold_max);
        this->set_low_fault(threshold_min);
        this->initialized_ = true;
    }

    void MAX31865::deinitialize() noexcept
    {
        this->initialized_ = false;
    }

    void MAX31865::set_vbias(bool const vbias) const noexcept
    {
        auto config{this->get_config_register()};
        config.vbias = vbias;
        config.fault_clear = false;
        config.fault_detect = false;
        config.oneshot = false;
        this->set_config_register(config);
    }

    void MAX31865::set_config_register(CONFIG const config) const noexcept
    {
        this->spi_device_.write_byte(std::to_underlying(RA::CONFIG), std::bit_cast<std::uint8_t>(config));
    }

    CONFIG MAX31865::get_config_register() const noexcept
    {
        return std::bit_cast<CONFIG>(this->spi_device_.read_byte(std::to_underlying(RA::CONFIG)));
    }

    void MAX31865::set_config(NWires const nwires,
                              FaultDetect const fault_detect,
                              FaultClear const fault_clear,
                              Filter const filter,
                              ConvMode const conv_mode) const noexcept
    {
        this->set_config_register(CONFIG{.vbias = false,
                                         .conv_mode = std::to_underlying(conv_mode),
                                         .oneshot = false,
                                         .nwires = std::to_underlying(nwires),
                                         .fault_detect = std::to_underlying(fault_detect),
                                         .fault_clear = std::to_underlying(fault_clear),
                                         .mainsfilter = std::to_underlying(filter)});
    }

    void MAX31865::set_high_fault(std::int16_t const threshold_max) const noexcept
    {
        this->set_high_fault_registers(std::bit_cast<HIGH_FAULT>(threshold_max));
    }

    void MAX31865::set_low_fault(std::int16_t const threshold_min) const noexcept
    {
        this->set_low_fault_registers(std::bit_cast<LOW_FAULT>(threshold_min));
    }

    void MAX31865::set_high_fault_registers(HIGH_FAULT const high_fault) const noexcept
    {
        this->spi_device_.write_bytes(std::to_underlying(RA::HIGH_FAULT_H),
                                      std::bit_cast<std::array<std::uint8_t, sizeof(HIGH_FAULT)>>(high_fault));
    }

    HIGH_FAULT MAX31865::get_high_fault_registers() const noexcept
    {
        return std::bit_cast<HIGH_FAULT>(
            this->spi_device_.read_bytes<sizeof(HIGH_FAULT)>(std::to_underlying(RA::HIGH_FAULT_H)));
    }

    void MAX31865::set_low_fault_registers(LOW_FAULT const low_fault) const noexcept
    {
        this->spi_device_.write_bytes(std::to_underlying(RA::LOW_FAULT_H),
                                      std::bit_cast<std::array<std::uint8_t, sizeof(LOW_FAULT)>>(low_fault));
    }

    LOW_FAULT MAX31865::get_low_fault_registers() const noexcept
    {
        return std::bit_cast<LOW_FAULT>(
            this->spi_device_.read_bytes<sizeof(LOW_FAULT)>(std::to_underlying(RA::LOW_FAULT_H)));
    }

    RTD MAX31865::get_rtd_registers() const noexcept
    {
        return std::bit_cast<RTD>(this->spi_device_.read_bytes<sizeof(RTD)>(std::to_underlying(RA::RTD_H)));
    }

    void MAX31865::start_one_shot_conversion() const noexcept
    {
        auto config = this->get_config_register();
        config.oneshot = true;
        this->set_config_register(config);
    }

}; // namespace MAX31865