/*
 * ADC01_demo.h
 *
 *  Created on: Jul 4, 2022
 *      Author: snailfish DS tech
 */
#include "tos_k.h"
#ifndef HARDWARE01_ADC01_DEMO_H_
#define HARDWARE01_ADC01_DEMO_H_
typedef struct ADC_Data_TypeDef
{
 uint16_t ADC01;
 uint16_t ADC02;
 uint16_t ADC03;
 uint16_t ADC04;
 uint16_t ADC05;
};
extern struct ADC_Data_TypeDef ADC_Data;
void ADC1_Init(void);
uint16_t Get_ADC_Val_5Channel(void);
#endif /* HARDWARE01_ADC01_DEMO_H_ */
