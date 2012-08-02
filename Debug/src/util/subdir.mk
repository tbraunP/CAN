################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/util/linked_list.c \
../src/util/stm32f4_discovery.c \
../src/util/uart.c 

OBJS += \
./src/util/linked_list.o \
./src/util/stm32f4_discovery.o \
./src/util/uart.o 

C_DEPS += \
./src/util/linked_list.d \
./src/util/stm32f4_discovery.d \
./src/util/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/util/%.o: ../src/util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DUSE_STM32_DISCOVERY -DHSE_VALUE=8000000 -I"//home/pyro/workspace/workspace_STM32/CAN/src" -I"//home/pyro/workspace/workspace_STM32/CAN/lib/Device/STM32F4xx/Include" -I"//home/pyro/workspace/workspace_STM32/CAN/lib/STM32F4xx_StdPeriph_Driver/inc" -I"//home/pyro/workspace/workspace_STM32/CAN/lib/CMSIS/Include" -O3 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


