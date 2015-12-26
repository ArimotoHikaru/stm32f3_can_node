#include "tim_encoder.h"

int 			TIM1_over_flow = 0, TIM1_under_flow = 0,
				TIM2_over_flow = 0, TIM2_under_flow = 0,
				TIM3_over_flow = 0, TIM3_under_flow = 0,
				TIM4_over_flow = 0, TIM4_under_flow = 0;


//TIM1 2 3 4
void TIM_encoder_Configuration(void)
{
	/* Define variable ------------------------------------------------------*/
	uint16_t PrescalerValue = 0;

	/* Define InitTypeDef ---------------------------------------------------*/
	GPIO_InitTypeDef 		GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* initialize InitTypeDef -----------------------------------------------*/
	TIM_DeInit(TIM1);
	TIM_DeInit(TIM2);
	TIM_DeInit(TIM3);
	TIM_DeInit(TIM4);

	/* Supply clock source --------------------------------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

	//AF_xの数字はSTM32F303のデータシートを参照
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_6);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_6);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_2);

	/* Define gpio_config ---------------------------------------------------*/
	GPIO_InitStructure.GPIO_Pin 	= 	  GPIO_Pin_0
										| GPIO_Pin_1
										| GPIO_Pin_6
										| GPIO_Pin_7
										| GPIO_Pin_8
										| GPIO_Pin_9
										;

	GPIO_InitStructure.GPIO_Mode 	=	GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType	=	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd	=	GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin 	= 	  GPIO_Pin_6
										| GPIO_Pin_7
										;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*
	 * TIM1_CH1:PA8
	 * TIM1_CH2:PA9
	 * TIM2_CH1:PA0
	 * TIM2_CH2:PA1
	 * TIM3_CH1:PA6
	 * TIM3_CH2:PA7
	 * TIM4_CH1:PB6
	 * TIM4_CH2:PB7
	 */

	/* Set up TIM_encoder function --------------------------------------------------*/
	//メンバの値の設定
	TIM_TimeBaseStructure.TIM_Period		= 0xffff-1;
	TIM_TimeBaseStructure.TIM_Prescaler		= 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//プリスケーラの値の設定
	PrescalerValue = (uint16_t) ((SystemCoreClock ) / 36000000) - 1;

	//プリスケーラの設定
	TIM_PrescalerConfig(TIM1, PrescalerValue, TIM_PSCReloadMode_Immediate);
	TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);
	TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
	TIM_PrescalerConfig(TIM4, PrescalerValue, TIM_PSCReloadMode_Immediate);

	//エンコーダーインターフェースモードに設定
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	//更新割り込み設定
#ifdef USE_INTERRUPT_TIM1
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE );
#endif

#ifdef USE_INTERRUPT_TIM2
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );
#endif

#ifdef USE_INTERRUPT_TIM3
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );
#endif

#ifdef USE_INTERRUPT_TIM4
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );
#endif

	/* TIM enable counter */
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}



void Encoder_ClearCount(TIM_TypeDef* TIMx)
{
	TIMx->CNT = 0;

	if(TIMx == TIM1){
		TIM1_over_flow = 0;
		TIM1_under_flow = 0;
	}else if(TIMx == TIM2){
		TIM2_over_flow = 0;
		TIM2_under_flow = 0;
	}else if(TIMx == TIM3){
		TIM3_over_flow = 0;
		TIM3_under_flow = 0;
	}else if(TIMx == TIM4){
		TIM4_over_flow = 0;
		TIM4_under_flow = 0;
	}
}

int TIMx_over_flow(TIM_TypeDef* TIMx)
{
	int value = 0;

	if(TIMx == TIM1){
		value = TIM1_over_flow;
	}else if(TIMx == TIM2){
		value = TIM2_over_flow;
	}else if(TIMx == TIM3){
		value = TIM3_over_flow;
	}else if(TIMx == TIM4){
		value = TIM4_over_flow;
	}
	return value;
}

int TIMx_under_flow(TIM_TypeDef* TIMx)
{
	int value = 0;

	if(TIMx == TIM1){
		value = TIM1_under_flow;
	}else if(TIMx == TIM2){
		value = TIM2_under_flow;
	}else if(TIMx == TIM3){
		value = TIM3_under_flow;
	}else if(TIMx == TIM4){
		value = TIM4_under_flow;
	}
	return value;
}

int Encoder_Count(TIM_TypeDef* TIMx)
{
	int value = 0;

	if(TIMx == TIM1){
		value = ( TIM_GetCounter(TIM1) + TIM1_over_flow * 65536 - ( TIM1_under_flow * 65536 ) );
	}else if(TIMx == TIM2){
		value = ( TIM_GetCounter(TIM2) + TIM2_over_flow * 65536 - ( TIM2_under_flow * 65536 ) );
	}else if(TIMx == TIM3){
		value = ( TIM_GetCounter(TIM3) + TIM3_over_flow * 65536 - ( TIM3_under_flow * 65536 ) );
	}else if(TIMx == TIM4){
		value = ( TIM_GetCounter(TIM4) + TIM4_over_flow * 65536 - ( TIM4_under_flow * 65536 ) );
	}
	return value;
}
/*******************************************************************************
* Function Name  : TIM1_IRQHandler
* Description    : This function handles TIM1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_TIM16_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){
		if(((TIM1->CR1)&0x10)>>4 == 0){
			TIM1_over_flow++;
		}else{
			TIM1_under_flow++;
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
     if(((TIM2->CR1)&0x10)>>4 == 0){
         TIM2_over_flow++;
     }else{
         TIM2_under_flow++;
     }
     TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}
/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM8_UP_TIM3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
	  if(((TIM3->CR1)&0x10)>>4 == 0){
		 TIM3_over_flow++;
     }else{
         TIM3_under_flow++;
     }
	  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}
/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		if(((TIM4->CR1)&0x10)>>4 == 0){
			TIM4_over_flow++;
		}else{
			TIM4_under_flow++;
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}



