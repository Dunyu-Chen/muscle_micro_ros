//
// Created by dunyu on 24-6-6.
//

#include "mhj_driver.h"

void MHJ_PWM_init(struct MHJ_PWM_Params* Params){
    // Period //physical period in us
    Params->max_pwm_value = Params->pwm_timer_handler.Init.Period +1;
    HAL_TIM_PWM_Start(&Params->pwm_timer_handler, Params->inlet_pwm_channel);
    HAL_TIM_PWM_Start(&Params->pwm_timer_handler, Params->outlet_pwm_channel);
    __HAL_TIM_SetCompare(&Params->pwm_timer_handler, Params->inlet_pwm_channel,0);
    __HAL_TIM_SetCompare(&Params->pwm_timer_handler, Params->outlet_pwm_channel,0);
}

void MHJ_PWM_step(struct MHJ_PWM_Params* Params,double command){
    // update based on unified update command
    //todo
}
