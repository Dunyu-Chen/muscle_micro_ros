//
// Created by dunyu on 24-6-6.
//

#include "mhj_driver.h"

void MHJ_pwm_init(struct MHJ_Params* Params, double Pwm_period){
    Params->open_time = 650.0 ; // us = 0.001 ms
    Params->close_time = 350.0;
    Params->pwm_period = Pwm_period;
    Params->full_pwm_value = Params->timer_handler.Init.Period +1;
    Params->min_duty = Params->open_time/Params->pwm_period;
    Params->max_duty = 1.0 - (Params->close_time/Params->pwm_period);

    // init pwm output
    HAL_TIM_PWM_Start(&Params->timer_handler, Params->inlet_channel);
    HAL_TIM_PWM_Start(&Params->timer_handler, Params->outlet_channel);
    __HAL_TIM_SetCompare(&Params->timer_handler, Params->inlet_channel,0);
    __HAL_TIM_SetCompare(&Params->timer_handler, Params->outlet_channel,0);
}

void MHJ_pwm_step(struct MHJ_Params* Params,double Input_duty,double Output_duty){

    if (Input_duty < Params->min_duty){
        Input_duty = Params->min_duty;
    }
    else if (Input_duty > Params->max_duty){
        Input_duty = Params->max_duty;
    }
    if (Output_duty< Params->min_duty){
        Output_duty = Params->min_duty;
    }
    else if (Output_duty > Params->max_duty){
        Output_duty = Params->max_duty;
    }

    int16_t input_value = Input_duty*Params->full_pwm_value;
    int16_t output_value = Output_duty*Params->full_pwm_value;
    __HAL_TIM_SetCompare(&Params->timer_handler, Params->inlet_channel,input_value);
    __HAL_TIM_SetCompare(&Params->timer_handler, Params->outlet_channel,output_value);
}

void MHJ_binary_init(struct MHJ_Params* Params){
    Params->open_time = 650.0 ; // us = 0.001 ms
    Params->close_time = 350.0;
    Params->full_pwm_value = Params->timer_handler.Init.Period +1;
}

void MHJ_binary_step(struct MHJ_Params* Params, int8_t Input_state,int8_t Output_state){
    if (Input_state ==1 && Output_state ==1){
        Input_state = 0;
        Output_state = 0;
    }
    int16_t input_value = Input_state * Params->full_pwm_value;
    int16_t output_value = Output_state * Params->full_pwm_value;
    __HAL_TIM_SetCompare(&Params->timer_handler, Params->inlet_channel,input_value);
    __HAL_TIM_SetCompare(&Params->timer_handler, Params->outlet_channel,output_value);
}







