#include "ros_app.h"

// ad7606 setup
struct AD7606_Params adc_instance;
uint16_t rxBuffer[8];

//mhj10 valve
struct MHJ_Params valve_instance;

// rcl return flag
rcl_ret_t rc;
//data communication setup
rcl_publisher_t publisher;
rcl_subscription_t subscriber;
muscle_interfaces__msg__MuscleState  pub_msg;
//muscle_interfaces__msg__PwmValveInput sub_msg;
muscle_interfaces__msg__BinaryValveInput sub_msg;

// ad7606 busy pin interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (HAL_GPIO_ReadPin(adc_instance.busy_pin_port,adc_instance.busy_pin) == GPIO_PIN_RESET){
        AD7606_read_data_exti(&adc_instance,(uint16_t *)rxBuffer);
    }
}

// timer callback for periodic publish
void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{
  pub_msg.seq+=1;
  pub_msg.pressure = rxBuffer[0] / 65535.0 ;
  //pub_msg.position = rxBuffer[2] / 65535.0;
  rc = rcl_publish(&publisher, &pub_msg, NULL);
}

// subscriber callback
void subscriber_callback(const void * msgin)
{
    HAL_GPIO_TogglePin(Led_GPIO_Port,Led_Pin);
    MHJ_binary_step(&valve_instance, (int8_t) sub_msg.inlet_state,(int8_t)sub_msg.outlet_state);
}

// entrance of rtos task
void StartDefaultTask(void *argument)
{
  // valve
  valve_instance.timer_handler = htim3;
  valve_instance.inlet_channel = TIM_CHANNEL_1;
  valve_instance.outlet_channel = TIM_CHANNEL_2;
  MHJ_binary_init(&valve_instance);
  // adc
  adc_instance.range_pin_port = RANGE_GPIO_Port;
  adc_instance.range_pin = RANGE_Pin;

  adc_instance.ser_pin_port = SER_GPIO_Port;
  adc_instance.ser_pin = SER_Pin;

  adc_instance.d15_pin_port = D15_GPIO_Port;
  adc_instance.d15_pin = D15_Pin;

  adc_instance.os0_pin_port = OS0_GPIO_Port;
  adc_instance.os0_pin = OS0_Pin;

  adc_instance.os1_pin_port =OS1_GPIO_Port;
  adc_instance.os1_pin = OS1_Pin;

  adc_instance.os2_pin_port = OS2_GPIO_Port;
  adc_instance.os2_pin = OS2_Pin;

  adc_instance.busy_pin_port = BUSY_GPIO_Port;
  adc_instance.busy_pin= BUSY_Pin;

  adc_instance.reset_pin_port= RST_GPIO_Port;
  adc_instance.reset_pin=RST_Pin;

  adc_instance.cs_pin_port = CS_GPIO_Port;
  adc_instance.cs_pin = CS_Pin;

  adc_instance.ca_timer_handler=htim2;
  adc_instance.ca1_timer_channel = TIM_CHANNEL_1;
  adc_instance.ca2_timer_channel = TIM_CHANNEL_2;

  adc_instance.spi_handler = hspi1;
  AD7606_init(
    &adc_instance,
    10,
    's',
    16);
  AD7606_start_conversion_pwm(&adc_instance,1000);
  pub_msg.seq = 0;
  //micro ros transport init
  rmw_uros_set_custom_transport(
    true,
    (void *) &huart1,
    cubemx_transport_open,
    cubemx_transport_close,
    cubemx_transport_write,
    cubemx_transport_read);

  // allocator
  rcl_allocator_t allocator;
  allocator = rcl_get_default_allocator();
  // support
  rclc_support_t support;
  rc = rclc_support_init(&support, 0, NULL, &allocator);
  // node
  rcl_node_t node;
  rclc_node_init_default(&node, "muscle_mcu", "", &support);

  //publisher
  rc = rclc_publisher_init_best_effort(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(muscle_interfaces, msg, MuscleState),
    "muscle_state");

    //subscriber
    rc = rclc_subscription_init_best_effort(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(muscle_interfaces, msg, PwmValveInput),
    "valve_input");

    //timer
    rcl_timer_t timer;
    rc = rclc_timer_init_default(
      &timer,
      &support,
      RCL_MS_TO_NS(1),
      timer_callback);

    // executor
    rclc_executor_t executor;
    executor = rclc_executor_get_zero_initialized_executor();
    rclc_executor_init(
      &executor,
      &support.context,
      2, // timer and subscriber
      &allocator);

    // add subscriber to executor
    rc = rclc_executor_add_subscription(
      &executor,
      &subscriber,
      &sub_msg,
      &subscriber_callback,
      ON_NEW_DATA);

    // add timer to executor
    rc = rclc_executor_add_timer(
      &executor,
      &timer);
    // start the executor
    rclc_executor_spin(&executor);
}
