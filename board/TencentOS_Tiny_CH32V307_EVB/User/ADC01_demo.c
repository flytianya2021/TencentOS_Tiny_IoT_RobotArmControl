/**
  ******************************************************************
  * @file    main.c
  * @author  xy,Benue
  * @version V1.0
  * @date    2022-1-19
  * @brief   使用 ADC 外设产生随机值。
  ******************************************************************
  * @attention
  * VeriMake 用于CH32V307例程
  ******************************************************************
  */
#include "ADC01_demo.h"
#include "lcd.h"
#include "debug.h"// 包含 CH32V307 的头文件，C 标准单元库和delay()函数
struct ADC_Data_TypeDef ADC_Data;
/* Global Variable */
s16 Calibrattion_Val = 0;//校准值
u16 val[5];
/********************************************************************
* 函 数 名       : ADC1_Init
* 函数功能    : 初始化ADC
* 输    入          : 无
* 输    出          : 无
********************************************************************/
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
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          //设置AD模式为单独模式，只使用ADC1
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;               //禁用多通道模式，启用单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;          //启动连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //不启用外部触发源，启动软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;      //数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                     //要转换通道数量
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);                                      //使能ADC

    ADC_BufferCmd(ADC1, DISABLE);                               //disable buffer
    //测量ADC校准数据，也可以不使用。
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    Calibrattion_Val = Get_CalibrationValue(ADC1);

    ADC_BufferCmd(ADC1, ENABLE);                                //enable buffer
}

/********************************************************************
* 函 数 名       : Get_ADC_Val()
* 函数功能    : 返回ADCx转换出的结果数据。
* 输    入          : 通道数
* 输    出          : ADCx转换出的结果数据
********************************************************************/
uint16_t Get_ADC_Val(u8 ch)
{
    uint16_t val;

    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
    {
           tos_task_delay(1);
       }
       tos_task_delay(5);
    val = ADC_GetConversionValue(ADC1);

    return val;
}

uint16_t Get_ADC_Val_5Channel(void)
{


    ADC_RegularChannelConfig(ADC1, 8, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
    {
        tos_task_delay(1);
    }
//    tos_task_delay(1);
    val[0] = ADC_GetConversionValue(ADC1);


    ADC_RegularChannelConfig(ADC1, 12, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
    {
           tos_task_delay(1);
       }
//       tos_task_delay(1);
    val[1] = ADC_GetConversionValue(ADC1);


    ADC_RegularChannelConfig(ADC1, 13, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
    {
           tos_task_delay(1);
       }
//       tos_task_delay(1);
    val[2] = ADC_GetConversionValue(ADC1);


    ADC_RegularChannelConfig(ADC1, 14, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
    {
           tos_task_delay(1);
       }
//       tos_task_delay(1);
    val[3] = ADC_GetConversionValue(ADC1);


    ADC_RegularChannelConfig(ADC1, 15, 1, ADC_SampleTime_239Cycles5 );
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
    {
           tos_task_delay(1);
       }
//       tos_task_delay(1);
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
/********************************************************************
* 函 数 名      :  main
* 函数功能   : 主函数
* 输    入         : 无
* 输    出         : 无
*********************************************************************/

void ADC_main(void)
{
 char G_Temp[]={0};

 //   Delay_Init();
   // USART_Printf_Init(115200);
    ADC1_Init();
 //   printf("CalibrattionValue:%d\n", Calibrattion_Val);  //串口打印出校准值

    while(1)
    {
        Get_ADC_Val_5Channel();
        LCD_ShowString(0,175,"ADC01 Running/   ",BLUE,WHITE,24,0);
        sprintf(G_Temp ,"AD1:%d,AD2:%d      ",  ADC_Data.ADC01, ADC_Data.ADC02);
        LCD_ShowString(0,120,G_Temp,BLUE,WHITE,16,0);
        LCD_ShowString(0,175,"ADC01 Running|   ",BLUE,WHITE,24,0);
        sprintf(G_Temp,"AD3:%d,AD4:%d,AD5:%d   ",  ADC_Data.ADC03,ADC_Data.ADC04, ADC_Data.ADC05);
        LCD_ShowString(0,145,G_Temp,BLUE,WHITE,16,0);
        LCD_ShowString(0,175,"ADC01 Running\   ",BLUE,WHITE,24,0);

        tos_task_delay(10);
        LCD_ShowString(0,175,"ADC01 Running--   ",BLUE,WHITE,24,0);
    }
}
