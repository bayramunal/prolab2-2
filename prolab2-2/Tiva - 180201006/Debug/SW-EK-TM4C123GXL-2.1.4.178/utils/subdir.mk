################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SW-EK-TM4C123GXL-2.1.4.178/utils/uartstdio.c 

OBJS += \
./SW-EK-TM4C123GXL-2.1.4.178/utils/uartstdio.o 

C_DEPS += \
./SW-EK-TM4C123GXL-2.1.4.178/utils/uartstdio.d 


# Each subdirectory must supply rules for building sources it contributes
SW-EK-TM4C123GXL-2.1.4.178/utils/%.o: ../SW-EK-TM4C123GXL-2.1.4.178/utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DPART_TM4C123GH6PM -I"/home/student/eclipse-workspace/Tiva_C_LaunchPad/SW-EK-TM4C123GXL-2.1.4.178" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


