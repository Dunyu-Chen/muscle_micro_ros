#include "ros_app.h"

// rcl return flag
rcl_ret_t rc;

//data communication setup
rcl_publisher_t publisher;
rcl_subscription_t subscriber;
muscle_interfaces__msg__MuscleState  pub_msg;
muscle_interfaces__msg__UnifiedInput sub_msg;

// timer callback for periodic publish
void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{
  rc = rcl_publish(&publisher, &pub_msg, NULL);
}

// subscriber callback
void subscriber_callback(const void * msgin)
{
    HAL_GPIO_TogglePin(Led_GPIO_Port,Led_Pin);
}
// entrance of rtos task
void StartDefaultTask(void *argument)
{
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
    ROSIDL_GET_MSG_TYPE_SUPPORT(muscle_interfaces, msg, UnifiedInput),
    "muscle_input");

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

