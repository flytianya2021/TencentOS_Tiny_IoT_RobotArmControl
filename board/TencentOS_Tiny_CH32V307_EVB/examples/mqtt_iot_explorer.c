#include "tos_k.h"
#include "esp8266_tencent_firmware.h"
#include "tencent_firmware_module_wrapper.h"
#include "ADC01_demo.h"
#include "MPU6050.h"
#define PRODUCT_ID              "8IT0644V94"//产品ID
#define DEVICE_NAME             "SmartCar_Controler"    //设备名称
#define DEVICE_KEY              "k3gzdfIibFQPpm/urywLTg=="//设备秘钥
extern struct MPU6050_Struct_Typedef MPU6050_Data;
extern struct ADC_Data_TypeDef ADC_Data;
//#define REPORT_DATA_TEMPLATE_MPU    "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"MPU6050_Ax\\\":%d\\,\\\"MPU6050_Ay\\\":%d\\,\\\"MPU6050_Az\\\":%d\\}}"
//#define REPORT_DATA_TEMPLATE_ADC    "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000002\\\"\\,\\\"params\\\":{\\\"ADC01\\\":%d\\,\\\"ADC02\\\":%d\\,\\\"ADC03\\\":%d\\,\\\"ADC04\\\":%d\\,\\\"ADC05\\\":%d\\}}"

#define REPORT_DATA_TEMPLATE_MPU    "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"MPU6050_Ax\\\":%d\\,\\\"MPU6050_Ay\\\":%d\\}}"
#define REPORT_DATA_TEMPLATE_ADC    "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000002\\\"\\,\\\"params\\\":{\\\"ADC01\\\":%d\\,\\\"ADC02\\\":%d\\,\\\"ADC03\\\":%d\\,\\\"ADC04\\\":%d\\,\\\"ADC05\\\":%d\\}}"


void default_message_handler(mqtt_message_t* msg)
{
    printf("callback:\r\n");
    printf("---------------------------------------------------------\r\n");
    printf("\ttopic:%s\r\n", msg->topic);
    printf("\tpayload:%s\r\n", msg->payload);
    printf("---------------------------------------------------------\r\n");
}

char payload_adc[256] = {0};
char payload_mpu[256] = {0};
static char report_topic_name[TOPIC_NAME_MAX_SIZE] = {0};
static char report_reply_topic_name[TOPIC_NAME_MAX_SIZE] = {0};
//extern void MPU6050_main(void *arg);
//extern void ADC_main(void *arg);
//#define APPLICATION_TASK_STK_SIZE02       1024*1
//k_task_t MPU6050_task;
//__aligned(4) uint8_t MPU6050_task_stk[APPLICATION_TASK_STK_SIZE02];
void mqtt_demo_task(void)
{
    int ret = 0;
    int size = 0;
    int lightness = 0;
    mqtt_state_t state;

    char *product_id = PRODUCT_ID;
    char *device_name = DEVICE_NAME;
    char *key = DEVICE_KEY;
    int X=0,Y=0,Z=0;
     int X1=0,Y1=0;
     char A_Temp[]={0};
     char G_Temp[]={0};
     char T_Temp[]={0};
    device_info_t dev_info;
    memset(&dev_info, 0, sizeof(device_info_t));

    /**
     * Please Choose your AT Port first, default is HAL_UART_2(USART2)
    */
    ret = esp8266_tencent_firmware_sal_init(HAL_UART_PORT_2);

    if (ret < 0) {
        printf("esp8266 tencent firmware sal init fail, ret is %d\r\n", ret);
    }
 restart:
     //LCD_ShowString(0,145,G_Temp,BLUE,BACKCOLOR,16,0);
        LCD_ShowString(0,165,"Connecting WIFI...",BLUE,BLACK,24,0);
    esp8266_tencent_firmware_join_ap("CMCC-lin-----", "linjia@15120613059");
    //esp8266_tencent_firmware_join_ap("CMCC-lin", "linjia@15120613059");

    strncpy(dev_info.product_id, product_id, PRODUCT_ID_MAX_SIZE);
    strncpy(dev_info.device_name, device_name, DEVICE_NAME_MAX_SIZE);
    strncpy(dev_info.device_serc, key, DEVICE_SERC_MAX_SIZE);
    tos_tf_module_info_set(&dev_info, TLS_MODE_PSK);

    mqtt_param_t init_params = DEFAULT_MQTT_PARAMS;
    if (tos_tf_module_mqtt_conn(init_params) != 0) {
        printf("module mqtt conn fail\n");
    } else {
        printf("module mqtt conn success\n");
    }

    if (tos_tf_module_mqtt_state_get(&state) != -1) {
        printf("MQTT: %s\n", state == MQTT_STATE_CONNECTED ? "CONNECTED" : "DISCONNECTED");
    }

    size = snprintf(report_reply_topic_name, TOPIC_NAME_MAX_SIZE, "$thing/down/property/%s/%s", product_id, device_name);

    if (size < 0 || size > sizeof(report_reply_topic_name) - 1) {
        printf("sub topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(report_reply_topic_name));
    }
    if (tos_tf_module_mqtt_sub(report_reply_topic_name, QOS0, default_message_handler) != 0) {
        printf("module mqtt sub fail\n");
    } else {
        printf("module mqtt sub success\n");
    }

    memset(report_topic_name, 0, sizeof(report_topic_name));
    size = snprintf(report_topic_name, TOPIC_NAME_MAX_SIZE, "$thing/up/property/%s/%s", product_id, device_name);

    if (size < 0 || size > sizeof(report_topic_name) - 1) {
        printf("pub topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(report_topic_name));
    }
    LCD_ShowString(0,165,"Connected WIFI OK! ",GREEN,BLACK,24,0);
//        MPU_Init();                //初始化MPU6050
//        ADC1_Init();
//        printf("SystemClk:%d\r\n",SystemCoreClock);
//        printf("MPU6050 TEST\r\n");
//
//
//        tos_task_create(&MPU6050_task, "MPU6050_task", MPU6050_main, NULL, 4, MPU6050_task_stk, APPLICATION_TASK_STK_SIZE02, 0);
        while(1)
        {

//            Get_ADC_Val_5Channel();//5个AD通道
//            MPU6050_Data.temp=MPU_Get_Temperature(); //得到温度值
//            MPU_Get_Accelerometer(& MPU6050_Data.aacx,& MPU6050_Data.aacy, &MPU6050_Data.aacz);   //得到加速度传感器数据
//            MPU_Get_Gyroscope(& MPU6050_Data.gyrox,& MPU6050_Data.gyroy,& MPU6050_Data.gyroz);    //得到陀螺仪数据
////            memset(T_Temp,0x00,sizeof(A_Temp))  ;
////            memset(A_Temp,0x00,sizeof(A_Temp))  ;
////            memset(G_Temp,0x00,sizeof(G_Temp))  ;
//           // sprintf(T_Temp,"Temperature:%d\r\n", MPU6050_Data.temp);
//          // printf("Ax:%d,Ay:%d,Az:%d\r\n",  MPU6050_Data.aacy, MPU6050_Data.aacy,MPU6050_Data.aacy);
//           //printf("Gx:%d,Gy:%d,Gz:%d\r\n",  MPU6050_Data.gyrox, MPU6050_Data.gyroy,MPU6050_Data.gyroz);
////            sprintf(T_Temp,"Temperature:%d", MPU6050_Data.temp);
////            LCD_ShowString(0,30,T_Temp,WHITE,BLACK,16,0);
//            sprintf(A_Temp,"Ax:%d,Ay:%d,Az:%d",  MPU6050_Data.aacy, MPU6050_Data.aacy,MPU6050_Data.aacy);
//            LCD_ShowString(0,50,A_Temp,WHITE,BLACK,16,0);
//            sprintf(G_Temp,"Gx:%d,Gy:%d,Gz:%d",  MPU6050_Data.gyrox, MPU6050_Data.gyroy,MPU6050_Data.gyroz);
//            LCD_ShowString(0,70,G_Temp,WHITE,BLACK,16,0);


        /* use AT+PUB AT command */
        memset(payload_adc, 0, sizeof(payload_adc));
   tos_sleep_ms(200);
        snprintf(payload_adc, sizeof(payload_adc), REPORT_DATA_TEMPLATE_ADC, ADC_Data.ADC01,ADC_Data.ADC02,ADC_Data.ADC03,ADC_Data.ADC04,ADC_Data.ADC04);

        if (tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload_adc) != 0) {
            printf("ADC data module mqtt pub fail\n");
            goto restart;
        } else {

            printf("ADC data module mqtt pub success\n");
        }
        tos_sleep_ms(200);

        memset(payload_mpu, 0, sizeof(payload_mpu));
        snprintf(payload_mpu, sizeof(payload_mpu), REPORT_DATA_TEMPLATE_MPU,MPU6050_Data.aacx,MPU6050_Data.aacy);

        if (tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload_mpu) != 0) {
                   printf("MPU6050 datamodule mqtt pub fail\n");
                   continue;
               } else {
                   printf("MPU6050 module mqtt pub success\n");
               }
    }
}

void application_entry(void *arg)
{
    mqtt_demo_task();
    while (1) {
        printf("This is a mqtt demo!\r\n");
        tos_task_delay(1000);
    }
}
