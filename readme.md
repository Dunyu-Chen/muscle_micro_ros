# Pneumatic Artificial Muscle Micro ROS Firmware 

firmware of pneumatic muscle drive circuit using stm32cube-micro-ros interfaces

- MCU: stm32h743iit6 

- ADC: ad7606 AD converter

- to do

## project setup instruction

1. clone micro_ros_stm32cubemx_utils repo to project folder

> As the project is based on ROS2 Humble switch the repo branch to humble

```bash
git clone -b humble https://github.com/micro-ROS/micro_ros_stm32cubemx_utils.git
cd micro_ros_stm32cubemx_utils
git checkout humble
```

2. clone the self defined ROS2 transmission interfaces to micro-ros build list

```bash
cd cd micro_ros_stm32cubemx_utils/microros_static_library_ide/library_generation/extra_packages/
git clone git@github.com:Dunyu-Chen/muscle_interfaces.git
cd ../.. 
rm -rf libmicroros/
```

3. Open STM32CubeIDE and build the project
