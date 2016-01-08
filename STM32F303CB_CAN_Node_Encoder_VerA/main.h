/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "stm32f30x_it.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_exti.h"
//#include "stm32303c_eval.h"
#include "stm32f30x_can.h"

/* user_hedder --------------------------------------------------------------*/
#include "usart.h"
#include "tim_encoder.h"
#include "nvic.h"
#include "can.h"
/*USB関連*/
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2
} MyLed_TypeDef;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define verA

#ifdef verA
#define LED1_PIN                         GPIO_Pin_13
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOB

#define LED2_PIN                         GPIO_Pin_14
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOB

#define LED3_PIN                         GPIO_Pin_15
#define LED3_GPIO_PORT                   GPIOB
#define LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOB
#else
#define LED1_PIN                         GPIO_Pin_8
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define LED2_PIN                         GPIO_Pin_9
#define LED2_GPIO_PORT                   GPIOA
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define LED3_PIN                         GPIO_Pin_10
#define LED3_GPIO_PORT                   GPIOA
#define LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOA
#endif

#define USE_USART2
#define USE_INTERRUPT_USART2_DMA

//USB関連
//#define USE_USB //USBで通信するときはここのコメントアウトを外す
#define USB_INT_DEFAULT
#define BUTTON_USER BUTTON_NONE
#define USER_BUTTON_EXTI_LINE 0
#define USER_BUTTON_EXTI_IRQn EXTI0_IRQn
//USB関連ここまで

//TIM_Encoder関連
#define USE_ENCODER
#define USE_INTERRUPT_TIM1
#define USE_INTERRUPT_TIM2
#define USE_INTERRUPT_TIM3
#define USE_INTERRUPT_TIM4
//TIM_Encoder関連ここまで

//CAN関連
#define USE_CAN
//#define USE_INTERRUPT_CAN_TX
#define USE_INTERRUPT_CAN_RX


/* Exported functions ------------------------------------------------------- */
void init();
void LED_configuration (void);
void LEDOn(MyLed_TypeDef Led);
void LEDOff(MyLed_TypeDef Led);
void COM_Transmit(char str[]);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
