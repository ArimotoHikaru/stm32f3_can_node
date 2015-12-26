
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

//�ꕶ�����M
void transmit_usart1_c(char c);

//�����񑗐M
void transmit_usart1_s(char *s);

//USART1����M���荞�݃n���h��
void USART1_IRQHandler(void);

//USART1�������֐�
void USART1_Configuration(void);

//DMA���M�p
void USART2_ConfigurationDMA(void);

//
void transmit_usart2_dma(char str[]);

//DMA�����݃n���h��
void DMA1_Channel7_IRQHandler(void);

#endif
