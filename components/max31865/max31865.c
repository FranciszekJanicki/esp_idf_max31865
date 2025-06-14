#include "max31865.h"
#include "max31865_registers.h"
#include <assert.h>
#include <string.h>

static max31865_err_t max31865_bus_init(max31865_t const* max31865)
{
    return max31865->interface.bus_init ? max31865->interface.bus_init(max31865->interface.bus_user)
                                        : MAX31865_ERR_NULL;
}

static max31865_err_t max31865_bus_deinit(max31865_t const* max31865)
{
    return max31865->interface.bus_deinit
               ? max31865->interface.bus_deinit(max31865->interface.bus_user)
               : MAX31865_ERR_NULL;
}

static max31865_err_t max31865_bus_write(max31865_t const* max31865,
                                         uint8_t address,
                                         uint8_t const* data,
                                         size_t data_size)
{
    return max31865->interface.bus_init
               ? max31865->interface.bus_write(max31865->interface.bus_user,
                                               address,
                                               data,
                                               data_size)
               : MAX31865_ERR_NULL;
}

static max31865_err_t max31865_bus_read(max31865_t const* max31865,
                                        uint8_t address,
                                        uint8_t* data,
                                        size_t data_size)
{
    return max31865->interface.bus_read ? max31865->interface.bus_read(max31865->interface.bus_user,
                                                                       address,
                                                                       data,
                                                                       data_size)
                                        : MAX31865_ERR_NULL;
}

max31865_err_t max31865_initialize(max31865_t* max31865,
                                   max31865_config_t const* config,
                                   max31865_interface_t const* interface)
{
    assert(max31865 && config && interface);

    memset(max31865, 0, sizeof(*max31865));
    memcpy(&max31865->config, config, sizeof(*config));
    memcpy(&max31865->interface, interface, sizeof(*interface));

    return max31865_bus_init(max31865);
}

max31865_err_t max31865_deinitialize(max31865_t* max31865)
{
    assert(max31865);

    max31865_err_t err = max31865_bus_deinit(max31865);

    memset(max31865, 0, sizeof(*max31865));

    return err;
}

max31865_err_t max31865_set_vbias(max31865_t const* max31865, bool vbias)
{
    assert(max31865);

    max31865_config_reg_t reg = {};

    max31865_err_t err = max31865_get_config_reg(max31865, &reg);
    reg.fault_clear = false;
    reg.fault_detect = false;
    reg.oneshot = false;
    reg.vbias = vbias;
    err |= max31865_set_config_reg(max31865, &reg);

    return err;
}

max31865_err_t max31865_start_one_shot_conversion(max31865_t const* max31865)
{
    assert(max31865);

    max31865_config_reg_t reg = {};

    max31865_err_t err = max31865_get_config_reg(max31865, &reg);
    reg.oneshot = true;
    err |= max31865_set_config_reg(max31865, &reg);

    return err;
}

max31865_err_t max31865_get_resistance_data_raw(max31865_t const* max31865, uint16_t* raw)
{
    assert(max31865 && raw);

    max31865_rtd_reg_t reg = {};

    max31865_err_t err = max31865_set_vbias(max31865, true);
    err |= max31865_get_rtd_reg(max31865, &reg);
    err = max31865_set_vbias(max31865, false);

    *raw = reg.rtd;

    if (reg.fault) {
        err |= MAX31865_ERR_FAIL;
    }

    return err;
}

max31865_err_t max31865_get_resistance_data_scaled(max31865_t const* max31865, float32_t* scaled)
{
    assert(max31865 && scaled);

    uint16_t raw = {};

    max31865_err_t err = max31865_get_resistance_data_raw(max31865, &raw);

    *scaled = (float32_t)raw * max31865->config.scale;

    return err;
}

max31865_err_t max31865_set_config_reg(max31865_t const* max31865, max31865_config_reg_t const* reg)
{
    assert(max31865 && reg);

    uint8_t data = {};

    data |= (reg->vbias & 0x01U) << 7U;
    data |= (reg->conv_mode & 0x01U) << 6U;
    data |= (reg->oneshot & 0x01U) << 5U;
    data |= (reg->nwires & 0x01U) << 4U;
    data |= (reg->fault_detect & 0x03U) << 2U;
    data |= (reg->fault_clear & 0x01U) << 1U;
    data |= reg->mainsfilter & 0x01U;

    return max31865_bus_write(max31865, MAX31865_REG_ADDRESS_CONFIG, &data, sizeof(data));
}

max31865_err_t max31865_get_config_reg(max31865_t const* max31865, max31865_config_reg_t* reg)
{
    assert(max31865 && reg);

    uint8_t data = {};

    max31865_err_t err =
        max31865_bus_read(max31865, MAX31865_REG_ADDRESS_CONFIG, &data, sizeof(data));

    reg->vbias = (data >> 7U) & 0x01U;
    reg->conv_mode = (data >> 6U) & 0x01U;
    reg->oneshot = (data >> 5U) & 0x01U;
    reg->nwires = (data >> 4U) & 0x01U;
    reg->fault_detect = (data >> 2U) & 0x03U;
    reg->fault_clear = (data >> 1U) & 0x01U;
    reg->mainsfilter = data & 0x01U;

    return err;
}

max31865_err_t max31865_set_high_fault_reg(max31865_t const* max31865,
                                           max31865_high_fault_reg_t const* reg)
{
    assert(max31865 && reg);

    uint8_t data[2] = {};

    data[0] = (reg->high_fault >> 8U) & 0xFFU;
    data[1] = reg->high_fault & 0xFFU;

    return max31865_bus_write(max31865, MAX31865_REG_ADDRESS_HIGH_FAULT_H, data, sizeof(data));
}

max31865_err_t max31865_get_high_fault_reg(max31865_t const* max31865,
                                           max31865_high_fault_reg_t* reg)
{
    assert(max31865 && reg);

    uint8_t data[2] = {};

    max31865_err_t err =
        max31865_bus_read(max31865, MAX31865_REG_ADDRESS_HIGH_FAULT_H, data, sizeof(data));

    reg->high_fault = (uint16_t)(((data[0] & 0xFFU) << 8U) | (data[1] & 0xFFU));

    return err;
}

max31865_err_t max31865_set_low_fault_reg(max31865_t const* max31865,
                                          max31865_low_fault_reg_t const* reg)
{
    assert(max31865 && reg);

    uint8_t data[2] = {};

    data[0] = (reg->low_fault >> 8U) & 0xFFU;
    data[1] = reg->low_fault & 0xFFU;

    return max31865_bus_write(max31865, MAX31865_REG_ADDRESS_LOW_FAULT_H, data, sizeof(data));
}

max31865_err_t max31865_get_low_fault_reg(max31865_t const* max31865, max31865_low_fault_reg_t* reg)
{
    assert(max31865 && reg);

    uint8_t data[2] = {};

    max31865_err_t err =
        max31865_bus_read(max31865, MAX31865_REG_ADDRESS_LOW_FAULT_H, data, sizeof(data));

    reg->low_fault = (uint16_t)(((data[0] & 0xFFU) << 8U) | (data[1] & 0xFFU));

    return err;
}

max31865_err_t max31865_get_rtd_reg(max31865_t const* max31865, max31865_rtd_reg_t* reg)
{
    assert(max31865 && reg);

    uint8_t data[2] = {};

    max31865_err_t err =
        max31865_bus_read(max31865, MAX31865_REG_ADDRESS_RTD_H, data, sizeof(data));

    reg->rtd = (uint16_t)((((data[0] & 0xFFU) << 7U) | ((data[1] >> 1U) & 0x7FU)));

    return err;
}
