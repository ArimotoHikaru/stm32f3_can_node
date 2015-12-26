#ifndef EXTI_H
#define EXTI_H
#include <stdio.h>
#include "stm32f30x.h"
#include "stm32f30x_can.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_misc.h"
#include "main.h"

//EXTIèâä˙âªä÷êî
void CAN_Configuration(void);
void USB_LP_CAN1_RX0_IRQHandler(void);
void Encoder_int_to_char(TIM_TypeDef* TIMx, int value);
void Encoder_into_CANflame(TIM_TypeDef* TIMx);
void CAN_Receive_Check (CanRxMsg* RxMessage);
void Can_Auto_Trans(void);
void CAN_TX_TEST(int num);
#endif
