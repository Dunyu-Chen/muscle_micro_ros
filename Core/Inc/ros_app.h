
#ifndef MUSCLE_MICROROS_V0_ROS_APP_H
#define MUSCLE_MICROROS_V0_ROS_APP_H
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32.h>
#include <muscle_interfaces/msg/muscle_state.h>
#include <muscle_interfaces/msg/unified_input.h>
#include <muscle_interfaces/msg/pwm_valve_input.h>
#include <muscle_interfaces/msg/binary_valve_input.h>
#include "ad7606_driver.h"
#include "mhj_driver.h"

extern struct AD7606_Params adc_instance;
bool cubemx_transport_open(struct uxrCustomTransport * transport);
bool cubemx_transport_close(struct uxrCustomTransport * transport);
size_t cubemx_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err);
size_t cubemx_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);

#endif //MUSCLE_MICROROS_V0_ROS_APP_H
