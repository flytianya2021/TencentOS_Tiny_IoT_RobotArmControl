#include "mcu_init.h"
#include "tos_k.h"
#include "mqttclient.h"
#include "cJSON.h"
#include "sal_module_wrapper.h"
#include "E53_IA1.h"
#define USE_ESP8266

//#define USE_EC600S

#ifdef USE_ESP8266
#include "esp8266.h"
#endif

#ifdef USE_EC600S
#include "ec600s.h"
#endif

#ifdef USE_ESP8266 
static hal_uart_port_t esp8266_port = HAL_UART_PORT_0;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim16;
void mqtt_set_esp8266_port(hal_uart_port_t port) {
    esp8266_port = port;
}
#endif

k_event_t report_result_event;
k_event_flag_t report_success = 1<<0;
k_event_flag_t report_fail    = 1<<1;
struct ADC_Data{
  uint16_t ADC01;
	uint16_t ADC02;
	uint16_t ADC03;
	uint16_t ADC04;
	uint16_t ADC05;
}ADCValue;
struct MPU6050_Data{
  int8_t Ax;
	int8_t Ay;
	int8_t Az;
	int8_t Gx;
	int8_t Gy;
	int8_t Gz;
}MPU6050;
static void tos_topic_handler(void* client, message_data_t* msg)
{
   (void) client;
   cJSON* cjson_root   = NULL;
   cJSON* cjson_status = NULL;
	 cJSON* cjson_AD01 = NULL;
	 cJSON* cjson_AD02 = NULL;
	 cJSON* cjson_AD03 = NULL;
	 cJSON* cjson_AD04 = NULL;
	 cJSON* cjson_AD05 = NULL;
	 cJSON* cjson_mpu6050_x = NULL;
	 cJSON* cjson_mpu6050_y = NULL;
	 cJSON* cjson_mpu6050_z = NULL;
   char* status = NULL;
   char* status_AD01 = NULL;
	 char* status_AD02 = NULL;
	 char* status_AD03 = NULL;
	 char* status_AD04 = NULL;
	 char* status_AD05 = NULL;
	 char* status_ACC_X = NULL;	 
	 char* status_ACC_Y = NULL;
	 char* status_ACC_Z = NULL;
    k_event_flag_t event_flag = report_fail;

    /* 打印日志 */
    MQTT_LOG_I("-----------------------------------------------------------------------------------");
    MQTT_LOG_I("%s:%d %s()...\ntopic: %s, qos: %d. \nmessage:\n\t%s\n", __FILE__, __LINE__, __FUNCTION__, 
            msg->topic_name, msg->message->qos, (char*)msg->message->payload);
    MQTT_LOG_I("-----------------------------------------------------------------------------------\n");
    
    /* 使用cjson解析上报响应数据 */
    cjson_root = cJSON_Parse((char*)msg->message->payload);
    if (cjson_root == NULL) {
        printf("report reply message parser fail\r\n");
        event_flag = report_fail;
        goto exit;
    }
  
		/* 提取status状态 */
    cjson_status = cJSON_GetObjectItem(cjson_root, "params");
		cjson_AD01 = cJSON_GetObjectItem(cjson_status, "ADC01");
		cjson_AD02 = cJSON_GetObjectItem(cjson_status, "ADC02");
		cjson_AD03 = cJSON_GetObjectItem(cjson_status, "ADC03");
		cjson_AD04 = cJSON_GetObjectItem(cjson_status, "ADC04");
		cjson_AD05 = cJSON_GetObjectItem(cjson_status, "ADC05");
		  if (cjson_AD01 == NULL)
				{
        printf("cjson_AD01 data fail\r\n");
        //goto exitmpu6050;
       }else   {	ADCValue.ADC01=cjson_AD01->valueint;}
		 if (cjson_AD02 == NULL)
				{
        printf("cjson_AD02 data fail\r\n");
       // goto exitmpu6050;
       }else   {ADCValue.ADC02=cjson_AD02->valueint;}
		 if (cjson_AD03 == NULL)
				{
         printf("cjson_AD03 data fail\r\n");
        //goto exitmpu6050;
       }else   {ADCValue.ADC03=cjson_AD03->valueint;}
		 if (cjson_AD04 == NULL)
				{
       printf("cjson_AD04 data fail\r\n");
        //goto exitmpu6050;
       }else   {}ADCValue.ADC04=cjson_AD04->valueint;
		 if (cjson_AD05 == NULL)
				{
        printf("cjson_AD05 data fail\r\n");
        //goto exitmpu6050;
       }else   {ADCValue.ADC05=cjson_AD05->valueint;}
//		printf("cjson_AD01:%d\r\n",ADCValue.ADC01);
//		printf("cjson_AD02:%d\r\n",ADCValue.ADC02);
//		printf("cjson_AD03:%d\r\n",ADCValue.ADC03);
//		printf("cjson_AD04:%d\r\n",ADCValue.ADC04);
//		printf("cjson_AD05:%d\r\n",ADCValue.ADC05);
		//exitmpu6050:
		cjson_status = cJSON_GetObjectItem(cjson_root, "params");
		cjson_mpu6050_x = cJSON_GetObjectItem(cjson_status, "MPU6050_Ax");
		cjson_mpu6050_y = cJSON_GetObjectItem(cjson_status, "MPU6050_Ay");
//		cjson_mpu6050_z = cJSON_GetObjectItem(cjson_status, "MPU6050_Az");
		
		  if (cjson_mpu6050_x == NULL) 
				{
        printf("mpu6050_X data NULL\r\n");
        //goto exit;
        } else{MPU6050.Ax= (int8_t)cjson_mpu6050_x->valueint;}
		 if (cjson_mpu6050_y == NULL) 
				{
        printf("mpu6050_Y data NULL\r\n");
        //goto exit;
        }else{		MPU6050.Ay= (int8_t)cjson_mpu6050_y->valueint;}
//		MPU6050.Az= (int8_t)cjson_mpu6050_z->valueint;
//		printf("cjson_mpu6050_x:%d\r\n",MPU6050.Ax);
//		printf("cjson_mpu6050_y:%d\r\n",MPU6050.Ay);
//		printf("cjson_mpu6050_z:%d\r\n",MPU6050.Az);
    /* 判断status状态 */
    if (strstr(status,"success")) {
        event_flag = report_success;
    }else {
        event_flag = report_fail;
    }
    /*	memset(report_buf,0,sizeof(report_buf));			
			snprintf(report_buf,sizeof(report_buf),"Ax:%d,Ay:%d       ",MPU6050.Ax,MPU6050.Ay);
			LCD_ShowCharStr(10,40,report_buf,BACKCOLOR,BLUE,24);
		
			LCD_ShowCharStr(180,180," | ",BACKCOLOR,RED,32);
		
			snprintf(report_buf,sizeof(report_buf),"ADC01:%d,ADC02:%d  ",ADCValue.ADC01,ADCValue.ADC02);
			LCD_ShowCharStr(10,75,report_buf,BACKCOLOR,BLUE,16);
		
			memset(report_buf,0,sizeof(report_buf));
			snprintf(report_buf,sizeof(report_buf),"ADC03:%d,ADC04:%d   ",ADCValue.ADC03,ADCValue.ADC04);
			LCD_ShowCharStr(10,95,report_buf,BACKCOLOR,BLUE,16);

			memset(report_buf,0,sizeof(report_buf));
			LCD_ShowCharStr(180,180," / ",BACKCOLOR,RED,32);
		
			snprintf(report_buf,sizeof(report_buf),"ADC05:%d   ",ADCValue.ADC05);
			LCD_ShowCharStr(10,115,report_buf,BACKCOLOR,BLUE,16);
			
			LCD_ShowCharStr(180,180,"--",BACKCOLOR,RED,32);
			
			snprintf(report_buf,sizeof(report_buf),"Count:%d   ",count++);
			LCD_ShowCharStr(10,200,report_buf,BACKCOLOR,BLUE,24);
			LCD_ShowCharStr(180,180," \ ",BACKCOLOR,RED,32);*/
exit:
    cJSON_Delete(cjson_root);
    cjson_root = NULL;
    status = NULL;
    
    tos_event_post(&report_result_event, event_flag);
    
    return;
}

#define REPORT_DATA_TEMPLATE "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"MPU6050_Ax\\\":%d\\,\\\"MPU6050_Ay\\\":%d\\}}"

char report_buf[200];
#define WIFINAME "CMCC-lin-----"
#define WIFIPASSWORD "linjia@15120613059"
#define BACKCOLOR  BLACK
void mqttclient_task(void)
{
    int error;
    int16_t PWM01=0,PWM02=0,TIM1_PWM01=0,AccY=0;
    uint16_t ArmPWM01=0,ArmPWM02=0,ArmPWM03=0,ArmPWM04=0,ArmPWM05=0;
    int lightness = 0;
    
    mqtt_client_t *client = NULL;
    
    mqtt_message_t msg;
    
    k_event_flag_t match_flag;
    
    char  host_ip[20];
    char  temp[4]={0};
		
	//	HAL_GPIO_WritePin(IA1_Motor_GPIO_Port,IA1_Motor_Pin,GPIO_PIN_SET);
    memset(&msg, 0, sizeof(msg));
    	LCD_ShowCharStr(1,1,"Robot Arm Driver",BACKCOLOR,RED,24);
#ifdef USE_ESP8266 
    esp8266_sal_init(esp8266_port);
	LCD_ShowCharStr(1,30,"Connecting WIFI... ",BACKCOLOR,RED,24);
    esp8266_join_ap(WIFINAME, WIFIPASSWORD);
		LCD_ShowCharStr(1,30,"Connected WIFI OK  ",BACKCOLOR,GREEN,24);
#endif

#ifdef USE_EC600S
    ec600s_sal_init(HAL_UART_PORT_2);
#endif

    mqtt_log_init();

    client = mqtt_lease();
    
    tos_event_create(&report_result_event, (k_event_flag_t)0u);
    
    /* Domain Format: <your product ID>.iotcloud.tencentdevices.com */
    tos_sal_module_parse_domain("fwr8pgacus.iotcloud.tencentdevices.com",host_ip,sizeof(host_ip));
    
    /*
        These infomation is generated by mqtt_config_gen.py tool in "TencentOS-tiny\tools" directory.
    */
    mqtt_set_port(client, "1883");
    mqtt_set_host(client, host_ip);
    mqtt_set_client_id(client, "8IT0644V94SmartCar_RobotArm");
    mqtt_set_user_name(client, "8IT0644V94SmartCar_RobotArm;12010126;VMG4M;1660064650");//mOAvDZcEgBFhz5pC7Uw2GA==
    mqtt_set_password(client, "467c6ce286ebe0175fe5180e98b6a180bd281d2297e6cce53bc8f673ec721ede;hmacsha256");
    mqtt_set_clean_session(client, 1);

    error = mqtt_connect(client);
    
    MQTT_LOG_D("mqtt connect error is %#0x", error);
    
    error = mqtt_subscribe(client, "$thing/down/property/8IT0644V94/SmartCar_RobotArm", QOS0, tos_topic_handler);
    
    MQTT_LOG_D("mqtt subscribe error is %#0x", error);
    uint8_t count=0;
		LCD_ShowCharStr(1,30,"                   ",BACKCOLOR,RED,24);
		
	//HAL_GPIO_WritePin(IA1_Motor_GPIO_Port,IA1_Motor_Pin,GPIO_PIN_RESET);
	
	ADCValue.ADC01=60;
	ADCValue.ADC02=120;
	ADCValue.ADC03=120;
	ADCValue.ADC04=150;
	ADCValue.ADC05=90;
	
    while (1) 
	{
	
			//电机控制
		/*全速模式*/
		#if 1
		if(MPU6050.Ax>15&&MPU6050.Ax<=80)//前进
		{
     PWM01=MPU6050.Ax*10;
     if(PWM01>2000)PWM01=2000;
      LCD_ShowCharStr(10,100,"FORWARD     ",BACKCOLOR,RED,24);
      HAL_GPIO_WritePin(CAR_MOTOR_F_Port,CAR_MOTOR_F_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(CAR_MOTOR_R_Port,CAR_MOTOR_R_Pin,GPIO_PIN_RESET);
		}
		else if(MPU6050.Ax<-15&&MPU6050.Ax>=-80)//后退
		{
     PWM01=MPU6050.Ax*10;
     if(PWM01>2000)PWM01=2000;
      LCD_ShowCharStr(10,100,"BACK     ",BACKCOLOR,RED,24);
      HAL_GPIO_WritePin(CAR_MOTOR_F_Port,CAR_MOTOR_F_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(CAR_MOTOR_R_Port,CAR_MOTOR_R_Pin,GPIO_PIN_SET);
		}
		else
		{
			LCD_ShowCharStr(10,100,"STOP     ",BACKCOLOR,RED,24);
		 HAL_GPIO_WritePin(CAR_MOTOR_F_Port,CAR_MOTOR_F_Pin,GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(CAR_MOTOR_R_Port,CAR_MOTOR_R_Pin,GPIO_PIN_RESET);
		}
		#else
		/*变速模式*/
		if(MPU6050.Ax>5)
		{
     PWM01=MPU6050.Ax*10;
     if(PWM01>2000)PWM01=2000;

      HAL_GPIO_WritePin(CAR_MOTOR_F_Port,CAR_MOTOR_F_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(CAR_MOTOR_R_Port,CAR_MOTOR_R_Pin,GPIO_PIN_RESET);

			HAL_GPIO_WritePin(BTS7960_EN_Port,BTS7960_EN_Pin,GPIO_PIN_RESET);
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, PWM01);//PA0
	    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);//PA1
		}
   else if(MPU6050.Ax<-5)
		{

     PWM02=MPU6050.Ax*10;
     if(PWM02<-2000)PWM02=-2000;
       HAL_GPIO_WritePin(BTS7960_EN_Port,BTS7960_EN_Pin,GPIO_PIN_RESET);
     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	   __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, -PWM02);

		}
		else{
			HAL_GPIO_WritePin(BTS7960_EN_Port,BTS7960_EN_Pin,GPIO_PIN_SET);
			 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	     __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
		}
		#endif
		//车舵机控制
		AccY=MPU6050.Ay+90;//*3.2;
		if(AccY>=110)AccY=110;
		else if(AccY<=65)AccY=65;
	 if(AccY<110&&AccY>65)
		{
			TIM1_PWM01=AccY*3.2;
			if(TIM1_PWM01>=180)TIM1_PWM01=0;
     	snprintf(report_buf,sizeof(report_buf),"SERVER_PWM01:%d     ",AccY);
			LCD_ShowCharStr(10,150,report_buf,BACKCOLOR,GREEN,24);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, TIM1_PWM01);	   //PA8
		}else
		{ 		
			LCD_ShowCharStr(10,150,"   PWM ERRO          ",BACKCOLOR,RED,24);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 288);	
		}
	//机械臂舵机控制
	  /*
		PA11    ------> TIM1_CH4 控制机械臂舵机1
    PA2     ------> TIM2_CH3  控制机械臂舵机2
    PA3     ------> TIM2_CH4  控制机械臂舵机3
		PB14    ------> TIM15_CH1  控制机械臂舵机4
		PA6     ------> TIM16_CH1   控制机械臂舵机5
		*/
		//ArmPWM01=ADCValue.ADC01*3.2;
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, ArmPWM03);	 //PA3
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, ArmPWM03);	 //PA3
		if(ADCValue.ADC01>90)
		{
		  ADCValue.ADC01=90;
		}
		if(ADCValue.ADC02>90)
		{
			ADCValue.ADC02=90;
		}
		if(ADCValue.ADC03>90)
		{
			ADCValue.ADC03=90;
		}
		if(ADCValue.ADC04>150)
		{
			ADCValue.ADC04=150;
	  }
	  if(ADCValue.ADC05>90)
	 {
			ADCValue.ADC05=90;
		}
   if(ADCValue.ADC01>0&&ADCValue.ADC01<=90)		
	 {
		 ArmPWM01=ADCValue.ADC01*3.2;
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, ArmPWM01);	  //PA0
		  
	 }
	 if(ADCValue.ADC02>0&&ADCValue.ADC02<=90)		
	 {
	  ArmPWM02=ADCValue.ADC02*3.2;
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, ArmPWM02);	   //PA1
	 }
	 if(ADCValue.ADC03>0&&ADCValue.ADC03<=90)		
	 {
	  ArmPWM03=ADCValue.ADC03*3.2;
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, ArmPWM03);	 //PA2
	 }
	 if(ADCValue.ADC04>0&&ADCValue.ADC04<=90)		
	 {
	  ArmPWM04=ADCValue.ADC04*3.2;
	  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, ArmPWM04);	 //PA3  
	 }
	 if(ADCValue.ADC05>0&&ADCValue.ADC05<=90)		
	 {
	  ArmPWM05=ADCValue.ADC05*3.2;
	  __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, ArmPWM05);	//PA6  
	 }
//		memset(temp,0x00,sizeof(temp));
//	sprintf(temp,"E%d\r",TIM1_PWM01);	
////		
//    HAL_UART_Transmit(&huart3,(uint8_t*)temp,sizeof(temp),0xff);
//    HAL_UART_Transmit(&huart1,(uint8_t*)temp,sizeof(temp),0xff);
// 
 
		/*	memset(report_buf,0,sizeof(report_buf));			
			snprintf(report_buf,sizeof(report_buf),"Ax:%d,Ay:%d       ",MPU6050.Ax,MPU6050.Ay);
			LCD_ShowCharStr(10,40,report_buf,BACKCOLOR,BLUE,24);
		
			LCD_ShowCharStr(180,180," | ",BACKCOLOR,RED,32);
		
			snprintf(report_buf,sizeof(report_buf),"ADC01:%d,ADC02:%d  ",ADCValue.ADC01,ADCValue.ADC02);
			LCD_ShowCharStr(10,75,report_buf,BACKCOLOR,BLUE,16);
		
			memset(report_buf,0,sizeof(report_buf));
			snprintf(report_buf,sizeof(report_buf),"ADC03:%d,ADC04:%d   ",ADCValue.ADC03,ADCValue.ADC04);
			LCD_ShowCharStr(10,95,report_buf,BACKCOLOR,BLUE,16);

			memset(report_buf,0,sizeof(report_buf));
			LCD_ShowCharStr(180,180," / ",BACKCOLOR,RED,32);
		
			snprintf(report_buf,sizeof(report_buf),"ADC05:%d   ",ADCValue.ADC05);
			LCD_ShowCharStr(10,115,report_buf,BACKCOLOR,BLUE,16);
			
			LCD_ShowCharStr(180,180,"--",BACKCOLOR,RED,32);
			
			snprintf(report_buf,sizeof(report_buf),"Count:%d   ",count++);
			LCD_ShowCharStr(10,200,report_buf,BACKCOLOR,BLUE,24);
			LCD_ShowCharStr(180,180," \ ",BACKCOLOR,RED,32);*/
			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
      tos_task_delay(100); 
        memset(&msg, 0, sizeof(msg));
//        memset(&report_buf, 0, sizeof(report_buf));
//        snprintf(report_buf, sizeof(report_buf), REPORT_DATA_TEMPLATE, MPU6050.Ax,MPU6050.Ay);
//        
//        if (lightness > 100) {
//            lightness = 0;
//        }
//       
//        msg.qos = QOS0;
//        msg.payload = (void *) report_buf;
//        
//        error = mqtt_publish(client, "$thing/up/property/8IT0644V94/SmartCar_RobotArm", &msg);
//        
//        MQTT_LOG_D("mqtt publish error is %#0x", error);
//        
//        tos_event_pend(&report_result_event, 
//                       report_success|report_fail,
//                       &match_flag,
//                       TOS_TIME_FOREVER,
//                       TOS_OPT_EVENT_PEND_ANY | TOS_OPT_EVENT_PEND_CLR);
//        
//        if (match_flag == report_success) {
//            printf("report to Tencent IoT Explorer success\r\n");
//        
//        }else if (match_flag == report_fail){
//            printf("report to Tencent IoT Explorer fail\r\n");
//        }
//        
       
    }
}

void application_entry(void *arg)
{
    mqttclient_task();
    while (1) {
        printf("This is a mqtt demo!\r\n");
        tos_task_delay(1000);
    }
}
