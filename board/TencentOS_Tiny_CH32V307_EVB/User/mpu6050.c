#include "mpu6050.h"
#include "ADC01_demo.h"
#include "debug.h"


struct MPU6050_Struct_Typedef MPU6050_Data;
//��ʼ��MPU6050
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Init(void)
{
    u8 res;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);     //ʹ��AFIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);    //��ʹ������IO PORTAʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;              // �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                  //�����趨������ʼ��GPIOA

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); //��ֹJTAG,�Ӷ�PA15��������ͨIOʹ��,����PA15��������ͨIO!!!

    MPU_AD0_L;                              //����MPU6050��AD0��Ϊ�͵�ƽ,�ӻ���ַΪ:0X68

    IIC_Init();                             //��ʼ��IIC����
    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80); //��λMPU6050
    tos_task_delay(100);
    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00); //����MPU6050
    MPU_Set_Gyro_Fsr(3);                    //�����Ǵ�����,��2000dps
    MPU_Set_Accel_Fsr(0);                   //���ٶȴ�����,��2g
    MPU_Set_Rate(50);                       //���ò�����50Hz
    MPU_Write_Byte(MPU_INT_EN_REG,0X00);    //�ر������ж�
    MPU_Write_Byte(MPU_USER_CTRL_REG,0X00); //I2C��ģʽ�ر�
    MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);   //�ر�FIFO
    MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80); //INT���ŵ͵�ƽ��Ч
    res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
    if(res==MPU_ADDR)//����ID��ȷ
    {
        MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01); //����CLKSEL,PLL X��Ϊ�ο�
        MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00); //���ٶ��������Ƕ�����
        MPU_Set_Rate(50);                       //���ò�����Ϊ50Hz
    }else return 1;
    return 0;
}

//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
    return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ
}

//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
    return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ
}

//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_LPF(u16 lpf)
{
    u8 data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���
}

//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
u8 MPU_Set_Rate(u16 rate)
{
    u8 data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);  //�������ֵ�ͨ�˲���
    return MPU_Set_LPF(rate/2); //�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(void)
{
    u8 buf[2];
    short raw;
    float temp;
    MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((u16)buf[0]<<8)|buf[1];
    temp=36.53+((double)raw)/340;
    return temp*100;;
}

//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;
    res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
    if(res==0)
    {
        *gx=((u16)buf[0]<<8)|buf[1];
        *gy=((u16)buf[2]<<8)|buf[3];
        *gz=((u16)buf[4]<<8)|buf[5];
       // printf("GYRO:  X=%d   Y=%d   Z=%d  \n",*gx,*gy,*gz);
    }
    return res;;
}
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;
    res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
    if(res==0)
    {
        *ax=((u16)buf[0]<<8)|buf[1];
        *ay=((u16)buf[2]<<8)|buf[3];
        *az=((u16)buf[4]<<8)|buf[5];
       // printf("ACC:  X=%d   Y=%d   Z=%d  \n",*ax,*ay,*az);
    }
    return res;;
}

//IIC����д
//addr:������ַ
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
    u8 i;
    IIC_Start();
    IIC_SendByte((addr<<1)|0);  //����������ַ+д����
    if(IIC_WaitAck())           //�ȴ�Ӧ��
    {
        IIC_Stop();
        return 1;
    }
    IIC_SendByte(reg); //д�Ĵ�����ַ
    IIC_WaitAck();     //�ȴ�Ӧ��
    for(i=0;i<len;i++)
    {
        IIC_SendByte(buf[i]);  //��������
        if(IIC_WaitAck())      //�ȴ�ACK
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_Stop();
    return 0;
}

//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
    IIC_Start();
    IIC_SendByte((addr<<1)|0);//����������ַ+д����
    if(IIC_WaitAck())  //�ȴ�Ӧ��
    {
        IIC_Stop();
        return 1;
    }
    IIC_SendByte(reg); //д�Ĵ�����ַ
    IIC_WaitAck();     //�ȴ�Ӧ��
    IIC_Start();
    IIC_SendByte((addr<<1)|1);//����������ַ+������
    IIC_WaitAck();     //�ȴ�Ӧ��
    while(len)
    {
        if(len==1)*buf=IIC_ReadByte(0);//������,����nACK
        else *buf=IIC_ReadByte(1);     //������,����ACK
        len--;
        buf++;
    }
    IIC_Stop(); //����һ��ֹͣ����
    return 0;
}

//IICдһ���ֽ�
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
u8 MPU_Write_Byte(u8 reg,u8 data)
{
    IIC_Start();
    IIC_SendByte((MPU_ADDR<<1)|0);//����������ַ+д����
    if(IIC_WaitAck())  //�ȴ�Ӧ��
    {
        IIC_Stop();
        return 1;
    }
    IIC_SendByte(reg); //д�Ĵ�����ַ
    IIC_WaitAck();     //�ȴ�Ӧ��
    IIC_SendByte(data);//��������
    if(IIC_WaitAck())  //�ȴ�ACK
    {
        IIC_Stop();
        return 1;
    }
    IIC_Stop();
    return 0;
}

//IIC��һ���ֽ�
//reg:�Ĵ�����ַ
//����ֵ:����������
u8 MPU_Read_Byte(u8 reg)
{
    u8 res;
    IIC_Start();
    IIC_SendByte((MPU_ADDR<<1)|0);//����������ַ+д����
    IIC_WaitAck();     //�ȴ�Ӧ��
    IIC_SendByte(reg); //д�Ĵ�����ַ
    IIC_WaitAck();     //�ȴ�Ӧ��
    IIC_Start();
    IIC_SendByte((MPU_ADDR<<1)|1);//����������ַ+������
    IIC_WaitAck();     //�ȴ�Ӧ��
    res=IIC_ReadByte(0);//��ȡ����,����nACK
    IIC_Stop();         //����һ��ֹͣ����
    return res;
}



/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
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
    MPU_Init();                //��ʼ��MPU6050
    ADC1_Init();
    LCD_ShowString(0,200,"MPU6050 Init OK!       ",BLUE,BACKCOLOR,24,0);

    while(1)
    {

        Get_ADC_Val_5Channel();//5��ADͨ��
        LCD_ShowString(0,200,"MPU6050/AD Running/",BLUE,BACKCOLOR,24,0);
        MPU6050_Data.temp=MPU_Get_Temperature(); //�õ��¶�ֵ
        LCD_ShowString(0,200,"MPU6050/AD Running--",BLUE,BACKCOLOR,24,0);
        MPU_Get_Accelerometer(& ACC_x,& ACC_y, &ACC_z);   //�õ����ٶȴ���������
        MPU6050_Data.aacx=ACC_x/140;
        MPU6050_Data.aacy=ACC_y/140;
        MPU6050_Data.aacz=ACC_z/140;
        LCD_ShowString(0,200,"MPU6050/AD Running\ ",BLUE,BACKCOLOR,24,0);
// MPU_Get_Gyroscope(& MPU6050_Data.gyrox,& MPU6050_Data.gyroy,& MPU6050_Data.gyroz);    //�õ�����������
//        memset(T_Temp,0x00,sizeof(A_Temp))  ;
//        memset(A_Temp,0x00,sizeof(A_Temp))  ;
//        memset(G_Temp,0x00,sizeof(G_Temp))  ;
//        printf(T_Temp,"Temperature:%d", MPU6050_Data.temp);
//       printf("Ax:%d,Ay:%d,Az:%d",  MPU6050_Data.aacy, MPU6050_Data.aacy,MPU6050_Data.aacy);
//       printf("Gx:%d,Gy:%d,Gz:%d",  MPU6050_Data.gyrox, MPU6050_Data.gyroy,MPU6050_Data.gyroz);
        sprintf(T_Temp,"Temperature:%d.%d    ", MPU6050_Data.temp/100,MPU6050_Data.temp%100);
        LCD_ShowString(0,30,T_Temp,BLUE,BACKCOLOR,24,0);

        sprintf(A_Temp,"Ax:%d,Ay:%d      ",  MPU6050_Data.aacx, MPU6050_Data.aacy);
        LCD_ShowString(0,55,A_Temp,BLUE,BACKCOLOR,24,0);

//        sprintf(G_Temp,"Gx:%d,Gy:%d      ", MPU6050_Data.gyrox, MPU6050_Data.gyroy);
//        LCD_ShowString(0,95,G_Temp,BLUE,WHITE,24,0);

        sprintf(G_Temp ,"AD1:%d,AD2:%d      ",  ADC_Data.ADC01, ADC_Data.ADC02);
        LCD_ShowString(0,120,G_Temp,BLUE,BACKCOLOR,16,0);

        sprintf(G_Temp,"AD3:%d,AD4:%d,AD5:%d   ",  ADC_Data.ADC03,ADC_Data.ADC04, ADC_Data.ADC05);
        LCD_ShowString(0,145,G_Temp,BLUE,BACKCOLOR,16,0);
        LCD_ShowString(0,200,"MPU6050/AD Running| ",BLUE,BACKCOLOR,24,0);
           tos_task_delay(5);
          }


}

