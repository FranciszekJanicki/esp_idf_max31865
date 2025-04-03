#ifndef MAX31865_CONFIG_HPP
#define MAX31865_CONFIG_HPP

#include "spi_device.hpp"
#include <cstdint>

namespace MAX31865 {

    using SPIDevice = ESP32_Utility::SPIDevice;

    enum struct RA : std::uint8_t {
        CONFIG = 0x00,
        RTD_H = 0x01,
        RTD_L = 0x02,
        HIGH_FAULT_H = 0x03,
        HIGH_FAULT_L = 0x04,
        LOW_FAULT_H = 0x05,
        LOW_FAULT_L = 0x06,
        FAULT_STATUS = 0x07,
    };

    enum struct NWires : std::uint8_t {
        THREE = 1,
        TWO = 0,
        FOUR = 0,
    };

    enum struct FaultDetect : std::uint8_t {
        NO_ACTION = 0b00,
        AUTO_DELAY = 0b01,
        MANUAL_DELAY_CYCLE1 = 0b10,
        MANUAL_DELAY_CYCLE2 = 0b11,
    };

    enum Filter : std::uint8_t {
        HZ50 = 1,
        HZ60 = 0,
    };

    enum struct Error : std::uint8_t {
        NO_ERROR = 0,
        VOLTAGE = 2,
        RTD_IN_LOW,
        REF_LOW,
        REF_HIGH,
        RTD_LOW,
        RTD_HIGH,
    };

    enum struct FaultClear : std::uint8_t {
        AUTO = 1U,
        MANUAL = 0U,
    };

    enum struct ConvMode : std::uint8_t {
        AUTO = 1U,
        ONESHOT = 0U,
    };

}; // namespace MAX31865

#endif // MAX31865_CONFIG_HPP