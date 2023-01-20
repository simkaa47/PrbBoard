################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/analog.c \
../Core/Src/control_bit.c \
../Core/Src/dio.c \
../Core/Src/ethernet_task.c \
../Core/Src/fram.c \
../Core/Src/freertos.c \
../Core/Src/keyboard.c \
../Core/Src/lcd.c \
../Core/Src/main.c \
../Core/Src/main_process.c \
../Core/Src/modbus.c \
../Core/Src/probotbornik.c \
../Core/Src/settings.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/timer.c \
../Core/Src/uart_task.c \
../Core/Src/universal_control.c \
../Core/Src/vozvratKovshaFb.c 

OBJS += \
./Core/Src/analog.o \
./Core/Src/control_bit.o \
./Core/Src/dio.o \
./Core/Src/ethernet_task.o \
./Core/Src/fram.o \
./Core/Src/freertos.o \
./Core/Src/keyboard.o \
./Core/Src/lcd.o \
./Core/Src/main.o \
./Core/Src/main_process.o \
./Core/Src/modbus.o \
./Core/Src/probotbornik.o \
./Core/Src/settings.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/timer.o \
./Core/Src/uart_task.o \
./Core/Src/universal_control.o \
./Core/Src/vozvratKovshaFb.o 

C_DEPS += \
./Core/Src/analog.d \
./Core/Src/control_bit.d \
./Core/Src/dio.d \
./Core/Src/ethernet_task.d \
./Core/Src/fram.d \
./Core/Src/freertos.d \
./Core/Src/keyboard.d \
./Core/Src/lcd.d \
./Core/Src/main.d \
./Core/Src/main_process.d \
./Core/Src/modbus.d \
./Core/Src/probotbornik.d \
./Core/Src/settings.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/timer.d \
./Core/Src/uart_task.d \
./Core/Src/universal_control.d \
./Core/Src/vozvratKovshaFb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DSTM32_THREAD_SAFE_STRATEGY=4 -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Core/ThreadSafe -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/analog.d ./Core/Src/analog.o ./Core/Src/analog.su ./Core/Src/control_bit.d ./Core/Src/control_bit.o ./Core/Src/control_bit.su ./Core/Src/dio.d ./Core/Src/dio.o ./Core/Src/dio.su ./Core/Src/ethernet_task.d ./Core/Src/ethernet_task.o ./Core/Src/ethernet_task.su ./Core/Src/fram.d ./Core/Src/fram.o ./Core/Src/fram.su ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/keyboard.d ./Core/Src/keyboard.o ./Core/Src/keyboard.su ./Core/Src/lcd.d ./Core/Src/lcd.o ./Core/Src/lcd.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/main_process.d ./Core/Src/main_process.o ./Core/Src/main_process.su ./Core/Src/modbus.d ./Core/Src/modbus.o ./Core/Src/modbus.su ./Core/Src/probotbornik.d ./Core/Src/probotbornik.o ./Core/Src/probotbornik.su ./Core/Src/settings.d ./Core/Src/settings.o ./Core/Src/settings.su ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_hal_timebase_tim.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/timer.d ./Core/Src/timer.o ./Core/Src/timer.su ./Core/Src/uart_task.d ./Core/Src/uart_task.o ./Core/Src/uart_task.su ./Core/Src/universal_control.d ./Core/Src/universal_control.o ./Core/Src/universal_control.su ./Core/Src/vozvratKovshaFb.d ./Core/Src/vozvratKovshaFb.o ./Core/Src/vozvratKovshaFb.su

.PHONY: clean-Core-2f-Src

