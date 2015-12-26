
#ifndef ENCODER_H
#define ENCODER_H
#include <stdio.h>
#include "stm32f30x.h"
#include "stm32f30x_tim.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_misc.h"
#include "main.h"

void TIM_encoder_Configuration(void);
void TIM_encoder_Configuration_onlyTIM8(void);
void Encoder_ClearCount(TIM_TypeDef* TIMx);
int Encoder_Count(TIM_TypeDef* TIMx);
void TIM1_UP_TIM16_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
int TIMx_over_flow(TIM_TypeDef* TIMx);
int TIMx_under_flow(TIM_TypeDef* TIMx);
#endif
