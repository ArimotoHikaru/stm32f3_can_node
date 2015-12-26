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
/*USB�֘A*/
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
#define USE_USART2
#define USE_INTERRUPT_USART2_DMA

//USB�֘A
//#define USE_USB //USB�ŒʐM����Ƃ��͂����̃R�����g�A�E�g���O��
#define USB_INT_DEFAULT
#define BUTTON_USER BUTTON_NONE
#define USER_BUTTON_EXTI_LINE 0
#define USER_BUTTON_EXTI_IRQn EXTI0_IRQn
//USB�֘A�����܂�

//TIM_Encoder�֘A
#define USE_ENCODER
#define USE_INTERRUPT_TIM1
#define USE_INTERRUPT_TIM2
#define USE_INTERRUPT_TIM3
#define USE_INTERRUPT_TIM4
//TIM_Encoder�֘A�����܂�

//CAN�֘A
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
