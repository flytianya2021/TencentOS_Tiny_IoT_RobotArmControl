################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../examples/qcloud_csdk_iot_explorer/data_template_app.c \
../examples/qcloud_csdk_iot_explorer/data_template_config.c 

OBJS += \
./examples/qcloud_csdk_iot_explorer/data_template_app.o \
./examples/qcloud_csdk_iot_explorer/data_template_config.o 

C_DEPS += \
./examples/qcloud_csdk_iot_explorer/data_template_app.d \
./examples/qcloud_csdk_iot_explorer/data_template_config.d 


# Each subdirectory must supply rules for building sources it contributes
examples/qcloud_csdk_iot_explorer/%.o: ../examples/qcloud_csdk_iot_explorer/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\Debug" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\include\common" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\include\config" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\include\services\common" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\include\services\explorer" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\common\cryptology\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\common\mqtt_packet\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\common\utils\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\platform\network\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\services\explorer\data_template\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\services\explorer\service_mqtt\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\services\common\log_upload\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\services\common\mqtt_client\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\net\sal_module_wrapper" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\devices\esp8266" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\fs\kv\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\fs\fatfs\wrapper\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\fs\vfs\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\kernel\hal\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\net\at\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\net\tencent_firmware_module_wrapper" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\devices\esp8266_tencent_firmware" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\TOS_CONFIG" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\arch\risc-v\risc-v3a\common\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\arch\risc-v\risc-v3a\rv32i\gcc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\kernel\core\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\Core" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\User" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

