#ifndef MAX31865_MAX31865_H
#define MAX31865_MAX31865_H

#include "max31865_config.h"
#include "max31865_registers.h"
#include <stdbool.h>

typedef struct {
    max31865_config_t config;
    max31865_interface_t interface;
} max31865_t;

max31865_err_t max31865_initialize(max31865_t* max31865,
                                   max31865_config_t const* config,
                                   max31865_interface_t const* interface);
max31865_err_t max31865_deinitialize(max31865_t* max31865);

max31865_err_t max31865_get_resistance_data_raw(max31865_t const* max31865, uint16_t* raw);
max31865_err_t max31865_get_resistance_data_scaled(max31865_t const* max31865, float32_t* scaled);

max31865_err_t max31865_set_config_reg(max31865_t const* max31865,
                                       max31865_config_reg_t const* reg);
max31865_err_t max31865_get_config_reg(max31865_t const* max31865, max31865_config_reg_t* reg);

max31865_err_t max31865_set_high_fault_reg(max31865_t const* max31865,
                                           max31865_high_fault_reg_t const* reg);
max31865_err_t max31865_get_high_fault_reg(max31865_t const* max31865,
                                           max31865_high_fault_reg_t* reg);

max31865_err_t max31865_set_low_fault_reg(max31865_t const* max31865,
                                          max31865_low_fault_reg_t const* reg);
max31865_err_t max31865_get_low_fault_reg(max31865_t const* max31865,
                                          max31865_low_fault_reg_t* reg);

max31865_err_t max31865_get_rtd_reg(max31865_t const* max31865, max31865_rtd_reg_t* reg);

max31865_err_t max31865_set_vbias(max31865_t const* max31865, bool vbias);
max31865_err_t max31865_start_one_shot_conversion(max31865_t const* max31865);

#endif // MAX31865_MAX31865_H
