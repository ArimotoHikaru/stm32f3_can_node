
#ifndef USART_H
#define USART_H
#include <stdio.h>
#include "stm32f30x.h"
#include "stm32f30x_usart.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_dma.h"
#include "stm32f30x_misc.h"
#include "main.h"

//一文字送信
void transmit_usart1_c(char c);

//文字列送信
void transmit_usart1_s(char *s);

//USART1送受信割り込みハンドラ
void USART1_IRQHandler(void);

//USART1初期化関数
void USART1_Configuration(void);

//DMA送信用
void USART2_ConfigurationDMA(void);

//
void transmit_usart2_dma(char str[]);

//DMA割込みハンドラ
void DMA1_Channel7_IRQHandler(void);

#endif
