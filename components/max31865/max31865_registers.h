#ifndef MAX31865_MAX31865_REGISTERS_H
#define MAX31865_MAX31865_REGISTERS_H

#include <stdint.h>

typedef struct {
    uint8_t vbias : 1;
    uint8_t conv_mode : 1;
    uint8_t oneshot : 1;
    uint8_t nwires : 1;
    uint8_t fault_detect : 2;
    uint8_t fault_clear : 1;
    uint8_t mainsfilter : 1;
} max31865_config_reg_t;

typedef struct RTD {
    uint16_t rtd : 15;
    uint8_t fault : 1;
} max31865_rtd_reg_t;

typedef struct HIGH_FAULT {
    uint16_t high_fault : 16;
} max31865_high_fault_reg_t;

typedef struct LOW_FAULT {
    uint16_t low_fault : 16;
} max31865_low_fault_reg_t;

typedef struct FAULT_STATUS {
    uint8_t fault_status : 8;
} max31865_fault_status_reg_t;

#endif // MAX31865_MAX31865_REGISTERS_H