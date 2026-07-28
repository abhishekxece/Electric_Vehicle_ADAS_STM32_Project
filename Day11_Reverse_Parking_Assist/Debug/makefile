################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Day11_Reverse_Parking_Assist
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Day11_Reverse_Parking_Assist.elf \

MAP_FILES += \
Day11_Reverse_Parking_Assist.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Day11_Reverse_Parking_Assist.list \

OBJCOPY_BIN += \
Day11_Reverse_Parking_Assist.bin \


# All Target
all: main-build

# Main-build Target
main-build: Day11_Reverse_Parking_Assist.elf secondary-outputs

# Tool invocations
Day11_Reverse_Parking_Assist.elf Day11_Reverse_Parking_Assist.map: $(OBJS) $(USER_OBJS) C:\Users\91953\STM32CubeIDE\workspace_1.19.0\Day11_Reverse_Parking_Assist\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Day11_Reverse_Parking_Assist.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\91953\STM32CubeIDE\workspace_1.19.0\Day11_Reverse_Parking_Assist\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Day11_Reverse_Parking_Assist.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Day11_Reverse_Parking_Assist.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Day11_Reverse_Parking_Assist.list"
	@echo 'Finished building: $@'
	@echo ' '

Day11_Reverse_Parking_Assist.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Day11_Reverse_Parking_Assist.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Day11_Reverse_Parking_Assist.bin Day11_Reverse_Parking_Assist.elf Day11_Reverse_Parking_Assist.list Day11_Reverse_Parking_Assist.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
