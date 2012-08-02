################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/master/master_io.c \
../src/master/master_main.c \
../src/master/master_report.c 

OBJS += \
./src/master/master_io.o \
./src/master/master_main.o \
./src/master/master_report.o 

C_DEPS += \
./src/master/master_io.d \
./src/master/master_main.d \
./src/master/master_report.d 


# Each subdirectory must supply rules for building sources it contributes
src/master/%.o: ../src/master/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DUSE_STM32_DISCOVERY -DHSE_VALUE=8000000 -I"//home/pyro/workspace/workspace_STM32/CAN/src" -I"//home/pyro/workspace/workspace_STM32/CAN/lib/Device/STM32F4xx/Include" -I"//home/pyro/workspace/workspace_STM32/CAN/lib/STM32F4xx_StdPeriph_Driver/inc" -I"//home/pyro/workspace/workspace_STM32/CAN/lib/CMSIS/Include" -O3 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


