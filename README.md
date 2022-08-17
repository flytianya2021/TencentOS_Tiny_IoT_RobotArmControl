# 一、概述

​        随之物联网的发展，各类设备都能通过物联网进行控制，本次方案尝试了通过腾讯物联网平台实现设备控制设备的功能，使用了小型机械臂和小车进行测试，验证控制的物联网控制的实时性。机械臂由5个舵机实现5个自由度。小车由一个转向舵机和一个驱动电机组成。控制端采用WCH沁恒RISC-V [TencentOS](https://cloud.tencent.com/product/ts?from=10680) Tiny CH32V_EVB_AIoT RevB02开发套件负责读取电位器和姿态传感器数据并上传到云端，执行端由STM32L431RCT6控制器负责驱动电机和舵机。

# 二、方案框架

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/kalftkk8pl.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=deefe9b9d919e3e146c58e048d9e0f1c4f31a6ad)

方案框架

方案的亮点：

1、实现沁恒开发板对MPU6050传感器的读取。

2、实现5路ADC转换功能，对5个电位器进行读取。

3、实现STM32开发板对5路机械臂舵机、1路小车舵机和电机的正反转功能。

4、实现通过物联网传输数据和远程控制，即沁恒开发板通过获取姿态数据控制STM32开发板的PWM输出和IO控制。

5、通过规则引擎串联两个开发板，实现设备控制设备功能。

6、通过腾讯连连小程序查看数据和控制机械臂和小车。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/rtnphh303e.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=393dc9aa442c528dddebea668077d8da3d0c108a)

沁恒开发板控制端

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/eegsvcp75o.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=ac5c83e5262cb663dc4f16f3a3270130027f3216)

STM32开发板执行端

# 三、硬件设计

## 1、控制端系统

### **1）沁恒开发板套件**

​        CH32V_EVB是腾讯物联网操作系统TencentOS tiny 团队联合沁恒微电子设计的一款物联网评估板，用于TencentOS tiny 基础内核、RISC-V IP核架构和IoT组件功能体验和评估。 开发板实物如下图：

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/hldw6dvb4j.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=d07e12079a5730596ec6d45bc75df285b5c9ac5e)

添加描述

### 开发板特性

- 内置TencentOS Tiny开源物联网操作系统
- 开发板采用沁恒RISC-V MCU CH32V307VCT6芯片，CH32V305/7系列是基于沁恒自研RISC-V架构微处理器青稞V4系列设计的32位工业级互联型微控制器，配备了硬件堆栈区、快速中断入口，在标准RISC-V基础上大大提高了中断响应速度。加入单精度浮点指令集，扩充堆栈区，具有更高的运算性能。扩展串口UART数量到8组，定时器到10组，其中4组高级定时器。提供USB2.0高速接口（480Mbps）并内置了PHY收发器，以太网MAC升级到千兆并集成了10M-PHY模块。
- · 64KB SRAM，256KB Flash
- · 板载Type-C接口WCH-LINK仿真器
- · 板载esp8266 WiFi模组，支持腾讯云固件
- · 板载以太网接口
- · 板载物联网俱乐部WAN Interface接口，可支持NB-IoT、WiFi、4G cat1、LoRa等模组
- · 板载物联网俱乐部E53 Interface接口，可扩展全系E53传感器以及音频模块；
- · 板载标准24P DVP摄像头接口，可支持最高500万像素摄像头；
- · 板载1.54寸 IPS高清显示屏，支持240*240分辨率；
- · 预留SD卡、用户按键、SPI Flash，
- · 扩展IO口，方便开发者扩展硬件模块

### **2）E53模块**

采用E53_SC2智慧井盖模块，使用到上面的姿态传感器MPU6050来获取X轴和Y轴的数据，X轴控制小车的前进和后退。Y轴控制小车的舵机转向。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/cudca9ds8z.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=b58acdb48e4432f370bae17fb8af50bc854a9d14)

添加描述

### 3）电位器

该电位器主要用于模拟机械臂的关节旋转角度，通过控制电位器来控制相应的机械臂的舵机实现摆臂等一系列操作，总共需要5个电位器实现5个自由度。

目前使用如下两轴操作杆电位器模拟电位器数据的读取。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/pu7jmih2pn.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=1124db568b5ecb13dd5ec1da8cbd65f290e8fab4)

添加描述

实际是需要如下电位器安装一个主机械臂的关节上面，然后通过读取摆动机械臂的关节数据上传到云端实现远程控制。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/4ju92o5n7w.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=88dcf3a7f92b9a5e44f25b89215ba23ffe6ea826)

添加描述

## 2、执行端系统

### 1）控制主板

小熊派的STM32L431物联网开发板。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/dpu9qu9kxe.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=30017088a7e0ae7f233d0e56bca958575904875d)

小熊派开发板

### 2）机械臂

该机械臂由5个舵机组成。实现左右转动，上下，前后和夹紧等动作。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/xmv9tl4u3i.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=3df2daa7ff87967e705f4932f7555caafb9ec843)

机械臂

### 3)WAN模块

采用的是ESP8266模块，实现联网功能。该模块已经烧录腾讯云固件。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/qooc0g3erl.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=574e7f80f6a18125bd51bc0d817e8425d5206ae9)

ESP8266

### 4）电机驱动电路设计

手工焊接了个MOS管全桥驱动，可以实现电机的正反转。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/pllk9jzdvs.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=62536d383355d78de2f630db5ad8ad469c364225)

手工焊接MOS管全桥驱动

原理图如下：

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/0f4dozm2vi.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=b86128f9fa881a9f59de690c111e22920444a4c0)

MOS管驱动电机电路

5）舵机控制转向。

利用舵机实现对小车的转向控制。改造原小车的电机转向传动机构。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/vey1ygu50b.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=98c1b3ee405c3945d33fb490f50cc8ec8b421ade)

添加描述

6）供电电源

​       充电宝或电池供电。小车是由遥控赛车改装而来，本身是电磁供电的。另外我做了外界充电宝的接口，充电宝续航较长，电流大，可以带动电机舵机等耗电大的部件。

# 四、软件设计

## 1、控制端软件设计

通过模拟IIC读取将姿态传感器MPU6050和电位器数据读取创建在一个任务里面，通过结构体将数据传到MQTT任务里面。实现对相关传感器数据的上传。在上传之前已经将数据进行了换算，确保执行控制端可以直接使用，或者减少换算量。

功能如下

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/xv9iy4uwus.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=7328a8a99539275edbee254d1f57837e585927b8)

添加描述

上传数据模板

```javascript
#define REPORT_DATA_TEMPLATE_MPU    "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"MPU6050_Ax\\\":%d\\,\\\"MPU6050_Ay\\\":%d\\}}"
#define REPORT_DATA_TEMPLATE_ADC    "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000002\\\"\\,\\\"params\\\":{\\\"ADC01\\\":%d\\,\\\"ADC02\\\":%d\\,\\\"ADC03\\\":%d\\,\\\"ADC04\\\":%d\\,\\\"ADC05\\\":%d\\}}"
```

MPU6050数据和ADC数据读取

```javascript
#define BACKCOLOR BLACK
void MPU6050_main(void *arg)
{
    int X=0,Y=0,Z=0;
    int X1=0,Y1=0;
    short ACC_x,ACC_y,ACC_z;
    char A_Temp[]={0};
    char G_Temp[]={0};
    char T_Temp[]={0};
    LCD_ShowString(0,200,"MPU6060 Task Init...  ",BLUE,BACKCOLOR,24,0);
    Delay_Init();
    MPU_Init();                //初始化MPU6050
    ADC1_Init();
    LCD_ShowString(0,200,"MPU6050 Init OK!       ",BLUE,BACKCOLOR,24,0);

    while(1)
    {

        Get_ADC_Val_5Channel();//5个AD通道
        LCD_ShowString(0,200,"MPU6050/AD Running/",BLUE,BACKCOLOR,24,0);
        MPU6050_Data.temp=MPU_Get_Temperature(); //得到温度值
        LCD_ShowString(0,200,"MPU6050/AD Running--",BLUE,BACKCOLOR,24,0);
        MPU_Get_Accelerometer(& ACC_x,& ACC_y, &ACC_z);   //得到加速度传感器数据
        MPU6050_Data.aacx=ACC_x/140;
        MPU6050_Data.aacy=ACC_y/140;
        MPU6050_Data.aacz=ACC_z/140;
        LCD_ShowString(0,200,"MPU6050/AD Running\ ",BLUE,BACKCOLOR,24,0);
        sprintf(T_Temp,"Temperature:%d.%d    ", MPU6050_Data.temp/100,MPU6050_Data.temp%100);
        LCD_ShowString(0,30,T_Temp,BLUE,BACKCOLOR,24,0);

        sprintf(A_Temp,"Ax:%d,Ay:%d      ",  MPU6050_Data.aacx, MPU6050_Data.aacy);
        LCD_ShowString(0,55,A_Temp,BLUE,BACKCOLOR,24,0);
        sprintf(G_Temp ,"AD1:%d,AD2:%d      ",  ADC_Data.ADC01, ADC_Data.ADC02);
        LCD_ShowString(0,120,G_Temp,BLUE,BACKCOLOR,16,0);

        sprintf(G_Temp,"AD3:%d,AD4:%d,AD5:%d   ",  ADC_Data.ADC03,ADC_Data.ADC04, ADC_Data.ADC05);
        LCD_ShowString(0,145,G_Temp,BLUE,BACKCOLOR,16,0);
        LCD_ShowString(0,200,"MPU6050/AD Running| ",BLUE,BACKCOLOR,24,0);
           tos_task_delay(5);
          }
}
#include "ADC01_demo.h"
#include "lcd.h"
#include "debug.h" 
struct ADC_Data_TypeDef ADC_Data;
/* Global Variable */
s16 Calibrattion_Val = 0;//校准值
u16 val[5];

void ADC1_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2, ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);                           //初始化ADC时钟，设置时钟为PCLK2的8分频，最大时钟为14MHz

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;           
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;               
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;          
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //不启用外部触发源，启动软件触发
    ADC_InitStructureADC_DataAlign = ADC_DataAlign_Right;      
        ADC_InitStructure.ADC_NbrOfChannel = 1;                    
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);                                       
    ADC_BufferCmd(ADC1, DISABLE);                               
    //测量ADC校准数据，也可以不使用。
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    Calibrattion_Val = Get_CalibrationValue(ADC1);

    ADC_BufferCmd(ADC1, ENABLE);                               
}

 
uint16_t Get_ADC_Val_5Channel(void)
{


    ADC_RegularChannelConfig(ADC1, 8, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
    {
        tos_task_delay(1);
    }
    val[0] = ADC_GetConversionValue(ADC1);

    ADC_RegularChannelConfig(ADC1, 12, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
    {
           tos_task_delay(1);
    }
    val[1] = ADC_GetConversionValue(ADC1);


    ADC_RegularChannelConfig(ADC1, 13, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
    {
           tos_task_delay(1);
     }
    val[2] = ADC_GetConversionValue(ADC1);

    ADC_RegularChannelConfig(ADC1, 14, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
    {
           tos_task_delay(1);
       }
    val[3] = ADC_GetConversionValue(ADC1);

    ADC_RegularChannelConfig(ADC1, 15, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
    {
           tos_task_delay(1);
       }
    val[4] = ADC_GetConversionValue(ADC1);


    ADC_Data.ADC01=val[0]/11.3;
    ADC_Data.ADC02=val[1]/11.3;
    ADC_Data.ADC03=val[2]/11.3;
    ADC_Data.ADC04=val[3]/11.3;
    ADC_Data.ADC05=val[4]/11.3;
//    printf("-->ADC_Value8:%d\r\n",  ADC_Data.ADC01);                              //返回ADC转换结果。
//    printf("++>ADC_Value12:%d\r\n", ADC_Data.ADC02);                              //返回ADC转换结果。
//    printf("-->ADC_Value13:%d\r\n", ADC_Data.ADC03);                              //返回ADC转换结果。
//    printf("++>ADC_Value14:%d\r\n", ADC_Data.ADC04);                              //返回ADC转换结果。
//    printf("++>ADC_Value15:%d\r\n", ADC_Data.ADC05);                              //返回ADC转换结果。

}
```

## 2、执行端软件设计：

​        通过连接MQTT，订阅相关Topic，不断读取云台数据，解析Json数据后对机械臂舵机，机械臂舵机的控制是通过PWM来控制的，那么改变这些PWM值的是获取控制端上传的5路ADC值。而小车电机是通过PWM或IO来控制MOS全桥驱动电路实现前进后退功能和小车舵机控制也是由PWM控制，改变状态或数据的是控制端上传的MPU6050的数据实现，中间也做了些换算。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/6ghy7tiq49.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=bf0cb1fffd55b9c791e43e34e32854fc90b3971f)

添加描述

JSON数据解析代码：

```js
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
       }else   {	ADCValue.ADC01=cjson_AD01->valueint;}
		 if (cjson_AD02 == NULL)
				{
        printf("cjson_AD02 data fail\r\n");
       }else   {ADCValue.ADC02=cjson_AD02->valueint;}
		 if (cjson_AD03 == NULL)
				{
         printf("cjson_AD03 data fail\r\n");
       }else   {ADCValue.ADC03=cjson_AD03->valueint;}
		 if (cjson_AD04 == NULL)
				{
       printf("cjson_AD04 data fail\r\n");
       }else   {}ADCValue.ADC04=cjson_AD04->valueint;
		 if (cjson_AD05 == NULL)
				{
        printf("cjson_AD05 data fail\r\n");
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
 
exit:
    cJSON_Delete(cjson_root);
    cjson_root = NULL;
    status = NULL;
    
    tos_event_post(&report_result_event, event_flag);
    
    return;
}
```

控制电机和舵机代码：

```js
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

    }
}
```

# 五、云端设备

  创建了两个云端设备，1是执行端的设备，2是控制端的设备。他们两个共用同一个数据模板，但是控制不冲突。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/adpig7lfog.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710765;1659717965&q-key-time=1659710765;1659717965&q-header-list=&q-url-param-list=&q-signature=8ea7b52cef563dab1fc76c6a546465dc3e648e73)

数据模板

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/827bbgutra.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710814;1659718014&q-key-time=1659710814;1659718014&q-header-list=&q-url-param-list=&q-signature=a8beca35d5df1d2180e0c98d336c452fd6dd3ca0)

创建两个设备

```js
{
  "version": "1.0",
  "properties": [
    {
      "id": "MPU6050_Temp",
      "name": "姿态仪_温度",
      "desc": "",
      "mode": "rw",
      "define": {
        "type": "float",
        "min": "-100",
        "max": "100",
        "start": "0",
        "step": "0.01",
        "unit": "℃"
      },
      "required": false
    },
    {
      "id": "MPU6050_Ax",
      "name": "姿态仪_加速计Ax",
      "desc": "",
      "mode": "rw",
      "define": {
        "type": "int",
        "min": "-130",
        "max": "130",
        "start": "0",
        "step": "1",
        "unit": ""
      },
      "required": false
    },
    {
      "id": "ADC01",
      "name": "机械臂关节01",
      "desc": "",
      "mode": "rw",
      "define": {
        "type": "int",
        "min": "0",
        "max": "380",
        "start": "0",
        "step": "1",
        "unit": ""
      },
      "required": false
    },
    {
      "id": "ADC04",
      "name": "机械臂关节04",
      "desc": "",
      "mode": "rw",
      "define": {
        "type": "int",
        "min": "0",
        "max": "380",
        "start": "0",
        "step": "1",
        "unit": ""
      },
      "required": false
    },
    {
      "id": "ADC05",
      "name": "机械臂关节05",
      "desc": "",
      "mode": "rw",
      "define": {
        "type": "int",
        "min": "0",
        "max": "380",
        "start": "0",
        "step": "1",
        "unit": ""
      },
      "required": false
    },
    {
      "id": "ADC02",
      "name": "机械臂关节02",
      "desc": "",
      "mode": "rw",
      "define": {
        "type": "int",
        "min": "0",
        "max": "380",
        "start": "0",
        "step": "1",
        "unit": ""
      },
      "required": false
    },
    {
      "id": "ADC03",
      "name": "机械臂关节03",
      "desc": "",
      "mode": "rw",
      "define": {
        "type": "int",
        "min": "0",
        "max": "380",
        "start": "0",
        "step": "1",
        "unit": ""
      },
      "required": false
    },
    {
      "id": "MPU6050_Ay",
      "name": "姿态仪_加速计Ay",
      "desc": "",
      "mode": "rw",
      "define": {
        "type": "int",
        "min": "-20",
        "max": "20",
        "start": "0",
        "step": "1",
        "unit": ""
      },
      "required": false
    }
  ],
  "events": [],
  "actions": [],
  "profile": {
    "ProductId": "8IT0644V94",
    "CategoryId": "1"
  }
}
```

# 六、规则引擎使用

打开规则引擎，将两个设备通过Topic传递数据，实现设备端到设备端的数据交互。也是本方案的关键控制环节。

![img](https://ask8088-private-1251520898.cn-south.myqcloud.com/developer-images/article/6078590/6udjbfs305.png?q-sign-algorithm=sha1&q-ak=AKID2uZ1FGBdx1pNgjE3KK4YliPpzyjLZvug&q-sign-time=1659710727;1659717927&q-key-time=1659710727;1659717927&q-header-list=&q-url-param-list=&q-signature=2311df2e125935e9c1674445d0a20b9559839141)

添加描述

规则必须禁止后才能编辑，执行端会获取params字段的json格式的数据，然后进行解析。





# 七、腾讯连连使用

采用了腾讯连连的可视化页面编辑，可以多个页面编辑，实现翻页。如下图是主页面，可以进行机械臂控制。如下图创建页面。



主页面，可以看到控制端上传的控制数据，如果没有控制端的控制时，腾讯连连就成为控制端对执行端进行控制。



点击右上角的小车图标，跳转到机械臂的控制页面对小车进行控制。如下图是翻页页面。



# 八、总结

​        本方案尝试一把物联网端对端的控制，因为上传数据和下发数据的延迟原因，控制上很不顺畅。但这是跨出的第一步，什么时候可以将数据实时传输进行实时控制，那才是物联网控制的终极目标，如果用4G或5G模块也许会更好些。但是只要有数据的中转就会存在延迟。

关于硬件的搭建都是一点一点搭起来的，利用一切可以利用的资源，我觉得这很符合开发者成长激励计划该有的样子了，那杜邦线飞来飞去的感觉回到了那年的智能车比赛。