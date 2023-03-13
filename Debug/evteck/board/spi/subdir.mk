################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../evteck/board/spi/spi.c 

OBJS += \
./evteck/board/spi/spi.o 

C_DEPS += \
./evteck/board/spi/spi.d 


# Each subdirectory must supply rules for building sources it contributes
evteck/board/spi/%.o evteck/board/spi/%.su: ../evteck/board/spi/%.c evteck/board/spi/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/Components/dp83848 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I"C:/Users/MinhNhan/STM32CubeIDE/workspace_1.10.1/EVTeck/evteck/app" -I"C:/Users/MinhNhan/STM32CubeIDE/workspace_1.10.1/EVTeck/evteck/board" -I"C:/Users/MinhNhan/STM32CubeIDE/workspace_1.10.1/EVTeck/evteck/components" -I"C:/Users/MinhNhan/STM32CubeIDE/workspace_1.10.1/EVTeck/evteck/service" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-evteck-2f-board-2f-spi

clean-evteck-2f-board-2f-spi:
	-$(RM) ./evteck/board/spi/spi.d ./evteck/board/spi/spi.o ./evteck/board/spi/spi.su

.PHONY: clean-evteck-2f-board-2f-spi

