################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include platform/hal/wch/ch32v30xx/subdir.mk
-include net/tencent_firmware_module_wrapper/subdir.mk
-include net/sal_module_wrapper/subdir.mk
-include net/at/src/subdir.mk
-include kernel/core/subdir.mk
-include fs/vfs/subdir.mk
-include fs/kv/subdir.mk
-include fs/fatfs/subdir.mk
-include examples/subdir.mk
-include devices/esp8266_tencent_firmware/subdir.mk
-include devices/esp8266/subdir.mk
-include c-sdk/services/explorer/service_mqtt/src/file_manage/subdir.mk
-include c-sdk/services/explorer/service_mqtt/src/subdir.mk
-include c-sdk/services/explorer/data_template/src/subdir.mk
-include c-sdk/services/common/system/src/subdir.mk
-include c-sdk/services/common/ota/src/subdir.mk
-include c-sdk/services/common/mqtt_client/src/subdir.mk
-include c-sdk/services/common/log_upload/src/subdir.mk
-include c-sdk/services/common/http_signed/src/subdir.mk
-include c-sdk/services/common/http_client/src/subdir.mk
-include c-sdk/services/common/cos/src/subdir.mk
-include c-sdk/platform/os/TencentOS-Tiny/subdir.mk
-include c-sdk/platform/network/src/subdir.mk
-include c-sdk/common/utils/src/subdir.mk
-include c-sdk/common/mqtt_packet/src/subdir.mk
-include c-sdk/common/cryptology/src/subdir.mk
-include arch/rv32i/gcc/subdir.mk
-include arch/common/subdir.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
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

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
TencentOS-Tiny.hex \

SECONDARY_LIST += \
TencentOS-Tiny.lst \

SECONDARY_SIZE += \
TencentOS-Tiny.siz \


# All Target
all: TencentOS-Tiny.elf secondary-outputs

# Tool invocations
TencentOS-Tiny.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -T "D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"TencentOS-Tiny.map" --specs=nano.specs --specs=nosys.specs -o "TencentOS-Tiny.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
TencentOS-Tiny.hex: TencentOS-Tiny.elf
	@	riscv-none-embed-objcopy -O ihex "TencentOS-Tiny.elf"  "TencentOS-Tiny.hex"
	@	@
TencentOS-Tiny.lst: TencentOS-Tiny.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble "TencentOS-Tiny.elf" > "TencentOS-Tiny.lst"
	@	@
TencentOS-Tiny.siz: TencentOS-Tiny.elf
	@	riscv-none-embed-size --format=berkeley "TencentOS-Tiny.elf"
	@	@
# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) TencentOS-Tiny.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
