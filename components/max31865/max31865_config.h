#ifndef MAX31865_MAX31865_CONFIG_H
#define MAX31865_MAX31865_CONFIG_H

#include <stddef.h>
#include <stdint.h>

typedef float float32_t;

typedef enum {
    MAX31865_ERR_OK = 0,
    MAX31865_ERR_FAIL = 1 << 0,
    MAX31865_ERR_NULL = 1 << 1,
} max31865_err_t;

typedef enum {
    MAX31865_REG_ADDRESS_CONFIG = 0x00,
    MAX31865_REG_ADDRESS_RTD_H = 0x01,
    MAX31865_REG_ADDRESS_RTD_L = 0x02,
    MAX31865_REG_ADDRESS_HIGH_FAULT_H = 0x03,
    MAX31865_REG_ADDRESS_HIGH_FAULT_L = 0x04,
    MAX31865_REG_ADDRESS_LOW_FAULT_H = 0x05,
    MAX31865_REG_ADDRESS_LOW_FAULT_L = 0x06,
    MAX31865_REG_ADDRESS_FAULT_STATUS = 0x07,
} max31865_reg_address_t;

typedef enum {
    MAX31865_N_WIRES_THREE = 1,
    MAX31865_N_WIRES_TWO = 0,
    MAX31865_N_WIRES_FOUR = 0,
} max31865_n_wires_t;

typedef enum {
    MAX31865_FAULT_DETECT_NO_ACTION = 0b00,
    MAX31865_FAULT_DETECT_AUTO_DELAY = 0b01,
    MAX31865_FAULT_DETECT_MANUAL_DELAY_CYCLE1 = 0b10,
    MAX31865_FAULT_DETECT_MANUAL_DELAY_CYCLE2 = 0b11,
} max31865_fault_detect_t;

typedef enum {
    MAX31865_FILTER_50HZ = 1,
    MAX31865_FILTER_60HZ = 0,
} max31865_filter_t;

typedef enum {
    MAX31865_FAULT_CLEAR_AUTO = 1U,
    MAX31865_FAULT_CLEAR_MANUAL = 0U,
} max31865_fault_clear_t;

typedef enum {
    MAX31865_CONV_MODE_AUTO = 1U,
    MAX31865_CONV_MODE_ONESHOT = 0U,
} max31865_conv_mode_t;

typedef struct {
    float32_t scale;
} max31865_config_t;

typedef struct {
    void* bus_user;
    max31865_err_t (*bus_init)(void*);
    max31865_err_t (*bus_deinit)(void*);
    max31865_err_t (*bus_write)(void*, uint8_t, uint8_t const*, size_t);
    max31865_err_t (*bus_read)(void*, uint8_t, uint8_t*, size_t);
} max31865_interface_t;

#endif // MAX31865_MAX31865_CONFIG_H