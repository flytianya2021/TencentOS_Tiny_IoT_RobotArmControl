/**
  ******************************************************************
  * @file    main.c
  * @author  xy,Benue
  * @version V1.0
  * @date    2022-1-19
  * @brief   ʹ�� ADC ����������ֵ��
  ******************************************************************
  * @attention
  * VeriMake ����CH32V307����
  ******************************************************************
  */
#include "ADC01_demo.h"
#include "lcd.h"
#include "debug.h"// ���� CH32V307 ��ͷ�ļ���C ��׼��Ԫ���delay()����
struct ADC_Data_TypeDef ADC_Data;
/* Global Variable */
s16 Calibrattion_Val = 0;//У׼ֵ
u16 val[5];
/********************************************************************
* �� �� ��       : ADC1_Init
* ��������    : ��ʼ��ADC
* ��    ��          : ��
* ��    ��          : ��
********************************************************************/
void ADC1_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2, ENABLE );
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);                           //��ʼ��ADCʱ�ӣ�����ʱ��ΪPCLK2��8��Ƶ�����ʱ��Ϊ14MHz

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          //����ADģʽΪ����ģʽ��ֻʹ��ADC1
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;               //���ö�ͨ��ģʽ�����õ�ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;          //��������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�������ⲿ����Դ�������������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;      //�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;                     //Ҫת��ͨ������
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);                                      //ʹ��ADC

    ADC_BufferCmd(ADC1, DISABLE);                               //disable buffer
    //����ADCУ׼���ݣ�Ҳ���Բ�ʹ�á�
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    Calibrattion_Val = Get_CalibrationValue(ADC1);

    ADC_BufferCmd(ADC1, ENABLE);                                //enable buffer
}

/********************************************************************
* �� �� ��       : Get_ADC_Val()
* ��������    : ����ADCxת�����Ľ�����ݡ�
* ��    ��          : ͨ����
* ��    ��          : ADCxת�����Ľ������
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
//    printf("-->ADC_Value8:%d\r\n",  ADC_Data.ADC01);                              //����ADCת�������
//    printf("++>ADC_Value12:%d\r\n", ADC_Data.ADC02);                              //����ADCת�������
//    printf("-->ADC_Value13:%d\r\n", ADC_Data.ADC03);                              //����ADCת�������
//    printf("++>ADC_Value14:%d\r\n", ADC_Data.ADC04);                              //����ADCת�������
//    printf("++>ADC_Value15:%d\r\n", ADC_Data.ADC05);                              //����ADCת�������

}
/********************************************************************
* �� �� ��      :  main
* ��������   : ������
* ��    ��         : ��
* ��    ��         : ��
*********************************************************************/

void ADC_main(void)
{
 char G_Temp[]={0};

 //   Delay_Init();
   // USART_Printf_Init(115200);
    ADC1_Init();
 //   printf("CalibrattionValue:%d\n", Calibrattion_Val);  //���ڴ�ӡ��У׼ֵ

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
