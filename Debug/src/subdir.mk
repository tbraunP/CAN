################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

S_UPPER_SRCS += \
../src/startup_stm32f4xx.S 

OBJS += \
./src/main.o \
./src/startup_stm32f4xx.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/main.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 

S_UPPER_DEPS += \
./src/startup_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DUSE_STM32_DISCOVERY -DHSE_VALUE=8000000 -I"//home/pyro/workspace/workspace_STM32/CAN/src" -I"//home/pyro/workspace/workspace_STM32/CAN/lib/Device/STM32F4xx/Include" -I"//home/pyro/workspace/workspace_STM32/CAN/lib/STM32F4xx_StdPeriph_Driver/inc" -I"//home/pyro/workspace/workspace_STM32/CAN/lib/CMSIS/Include" -O3 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"//home/pyro/workspace/workspace_STM32/CAN/lib/Device/STM32F4xx/Include" -I"//home/pyro/workspace/workspace_STM32/CAN/src" -I"//home/pyro/workspace/workspace_STM32/CAN/lib/STM32F4xx_StdPeriph_Driver/inc" -I"//home/pyro/workspace/workspace_STM32/CAN/lib/CMSIS/Include" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


