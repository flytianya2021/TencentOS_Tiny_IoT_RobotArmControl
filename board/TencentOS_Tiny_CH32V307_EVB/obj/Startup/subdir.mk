################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Startup/startup_ch32v30x.S 

OBJS += \
./Startup/startup_ch32v30x.o 

S_UPPER_DEPS += \
./Startup/startup_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.S
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -x assembler-with-cpp -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\Startup" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\TOS_CONFIG" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\arch\risc-v\risc-v3a\common\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\arch\risc-v\risc-v3a\rv32i\gcc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

