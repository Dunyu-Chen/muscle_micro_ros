//
// Created by dunyu on 24-6-6.
//

#ifndef MUSCLE_MICRO_ROS_MHJ_DRIVER_H
#define MUSCLE_MICRO_ROS_MHJ_DRIVER_H

#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"

struct MHJ_Params{
    TIM_HandleTypeDef timer_handler;
    uint32_t inlet_channel;
    uint32_t outlet_channel;
    uint16_t full_pwm_value;

    double open_time;
    double close_time;
    double pwm_period;
    double max_duty;
    double min_duty;
};

void MHJ_pwm_init(struct MHJ_Params* Params, double Pwm_period);

void MHJ_pwm_step(struct MHJ_Params* Params,double Input_duty,double Output_duty);

void MHJ_binary_init(struct MHJ_Params* Params);

void MHJ_binary_step(struct MHJ_Params* Params, int8_t Input_state,int8_t Output_state);

#endif //MUSCLE_MICRO_ROS_MHJ_DRIVER_H
