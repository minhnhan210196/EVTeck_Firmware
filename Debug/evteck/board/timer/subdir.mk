################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../evteck/board/timer/timer.c 

OBJS += \
./evteck/board/timer/timer.o 

C_DEPS += \
./evteck/board/timer/timer.d 


# Each subdirectory must supply rules for building sources it contributes
evteck/board/timer/timer.o: ../evteck/board/timer/timer.c evteck/board/timer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/EVTeck_Firmware/evteck/app/afe_app" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/EVTeck_Firmware/evteck/app/setting" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/EVTeck_Firmware/evteck/app" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/EVTeck_Firmware/evteck/board" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/EVTeck_Firmware/evteck/board/gpio" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/EVTeck_Firmware/evteck/board/spi" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/EVTeck_Firmware/evteck/board/timer" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/EVTeck_Firmware/evteck/components/afe" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/EVTeck_Firmware/evteck/components/ate" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.6.0/EVTeck_Firmware/evteck/service/cli" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"evteck/board/timer/timer.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

