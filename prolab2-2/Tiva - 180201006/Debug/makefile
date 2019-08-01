################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include SW-EK-TM4C123GXL-2.1.4.178/utils/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Tiva_C_LaunchPad.hex \

SECONDARY_SIZE += \
Tiva_C_LaunchPad.siz \


# All Target
all: Tiva_C_LaunchPad.elf secondary-outputs

# Tool invocations
Tiva_C_LaunchPad.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T "/home/student/eclipse-workspace/Tiva_C_LaunchPad/src/TM4C.ld" -Xlinker --gc-sections -Wl,-Map,"Tiva_C_LaunchPad.map" -o "Tiva_C_LaunchPad.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Tiva_C_LaunchPad.hex: Tiva_C_LaunchPad.elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "Tiva_C_LaunchPad.elf"  "Tiva_C_LaunchPad.hex"
	@echo 'Finished building: $@'
	@echo ' '

Tiva_C_LaunchPad.siz: Tiva_C_LaunchPad.elf
	@echo 'Invoking: GNU ARM Cross Print Size'
	arm-none-eabi-size --format=berkeley "Tiva_C_LaunchPad.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) Tiva_C_LaunchPad.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
