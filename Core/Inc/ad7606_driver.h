//
// Created by dunyu on 24-5-30.
//

#ifndef TEST_PROJECT_AD7606_DRIVER_H
#define TEST_PROJECT_AD7606_DRIVER_H

#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"

struct AD7606_Params{
    // GPIO external interrupt for busy signal
    uint16_t busy_pin;
    GPIO_TypeDef * busy_pin_port;
    // GPIO output for reset ADC
    uint16_t reset_pin;
    GPIO_TypeDef * reset_pin_port;
    // GPIO output for transmitting process control i.e. CS
    uint16_t cs_pin;
    GPIO_TypeDef * cs_pin_port;

    // PWM driven conversion CA
    TIM_HandleTypeDef ca_timer_handler;
    uint32_t ca1_timer_channel;
    uint32_t ca2_timer_channel;

    // spi transmission
    SPI_HandleTypeDef spi_handler;

    // GPIO output pins for ADC setup

    // sensor range
    uint16_t range_pin;
    GPIO_TypeDef * range_pin_port;

    //transmit setup
    uint16_t ser_pin;
    GPIO_TypeDef * ser_pin_port;
    uint16_t d15_pin;
    GPIO_TypeDef * d15_pin_port;

    // over sampling setup
    uint16_t os0_pin;
    GPIO_TypeDef * os0_pin_port;
    uint16_t os1_pin;
    GPIO_TypeDef * os1_pin_port;
    uint16_t os2_pin;
    GPIO_TypeDef * os2_pin_port;
};

extern void AD7606_reset(struct AD7606_Params* Params);

extern void AD7606_init(
        struct AD7606_Params* Params,
        uint8_t Range,
        uint8_t Transmit_mode,
        uint8_t Oversample_mode);

extern void AD7606_start_conversion_pwm(struct AD7606_Params* Params,double Period);

extern void AD7606_read_data_exti(struct AD7606_Params* Params,uint16_t * Receive_Buffer);

#endif //TEST_PROJECT_AD7606_DRIVER_H
