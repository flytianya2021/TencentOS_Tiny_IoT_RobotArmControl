################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ADC01_demo.c \
../User/HzLib.c \
../User/ch32v30x_it.c \
../User/iic.c \
../User/lcd.c \
../User/lcd_init.c \
../User/main.c \
../User/mpu6050.c \
../User/sdio.c \
../User/spi_flash.c \
../User/spi_flash_kv.c \
../User/system_ch32v30x.c \
../User/usart.c 

OBJS += \
./User/ADC01_demo.o \
./User/HzLib.o \
./User/ch32v30x_it.o \
./User/iic.o \
./User/lcd.o \
./User/lcd_init.o \
./User/main.o \
./User/mpu6050.o \
./User/sdio.o \
./User/spi_flash.o \
./User/spi_flash_kv.o \
./User/system_ch32v30x.o \
./User/usart.o 

C_DEPS += \
./User/ADC01_demo.d \
./User/HzLib.d \
./User/ch32v30x_it.d \
./User/iic.d \
./User/lcd.d \
./User/lcd_init.d \
./User/main.d \
./User/mpu6050.d \
./User/sdio.d \
./User/spi_flash.d \
./User/spi_flash_kv.d \
./User/system_ch32v30x.d \
./User/usart.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\Debug" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\include\common" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\include\config" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\include\services\common" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\include\services\explorer" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\common\cryptology\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\common\mqtt_packet\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\common\utils\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\platform\network\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\services\explorer\data_template\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\services\explorer\service_mqtt\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\services\common\log_upload\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\connectivity\iot-hub-device-c-sdk\services\common\mqtt_client\inc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\net\sal_module_wrapper" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\devices\esp8266" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\fs\kv\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\fs\fatfs\wrapper\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\components\fs\vfs\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\kernel\hal\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\net\at\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\net\tencent_firmware_module_wrapper" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\devices\esp8266_tencent_firmware" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\TOS_CONFIG" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\arch\risc-v\risc-v3a\common\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\arch\risc-v\risc-v3a\rv32i\gcc" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\kernel\core\include" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\Core" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\User" -I"D:\ALL_IoTGame\TencentOSGame\TencentOS-v2.4.5\TencentOS-tiny\board\TencentOS_Tiny_CH32V307_EVB\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

