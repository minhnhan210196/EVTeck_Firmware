################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LWIP/Target/ethernetif.c 

OBJS += \
./LWIP/Target/ethernetif.o 

C_DEPS += \
./LWIP/Target/ethernetif.d 


# Each subdirectory must supply rules for building sources it contributes
LWIP/Target/ethernetif.o: ../LWIP/Target/ethernetif.c LWIP/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/app/afe_app" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/app/setting" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/app" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/board" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/board/gpio" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/board/spi" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/board/timer" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/components/afe" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/components/ate" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/service/cli" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/service/json" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/app/mqtt" -I"C:/Users/MinhNhan/STM32CubeIDE/myWorkspace/EvTeckFirmware/evteck/app/ota_app" -O0 -ffunction-sections -fdata-sections -Wall -u _printf_float -fstack-usage -MMD -MP -MF"LWIP/Target/ethernetif.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

