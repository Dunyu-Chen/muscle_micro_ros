//
// Created by dunyu on 24-6-6.
//

#ifndef MUSCLE_MICRO_ROS_MHJ_DRIVER_H
#define MUSCLE_MICRO_ROS_MHJ_DRIVER_H

#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"

struct MHJ_PWM_Params{
    TIM_HandleTypeDef pwm_timer_handler;
    uint32_t inlet_pwm_channel;
    uint32_t outlet_pwm_channel;
    uint32_t max_pwm_value;
    double period;
    double max_duty;
    double open_dead_zone;
    double close_dead_zone;
};

struct MHJ_Binary_params{
    uint16_t inlet_pin;
    GPIO_TypeDef * inlet_pin_port;
    uint16_t outlet_pin;
    GPIO_TypeDef * out_inlet_pin_port;
};

void MHJ_PWM_init(struct MHJ_PWM_Params* Params);

void MHJ_PWM_step(struct MHJ_PWM_Params* Params,double command);

#endif //MUSCLE_MICRO_ROS_MHJ_DRIVER_H
