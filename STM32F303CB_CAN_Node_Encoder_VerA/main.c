#include "main.h"

#ifdef USE_USB
#include "VCP_F3.c"
#endif


// Unused global variables that have to be included to ensure correct compiling */
// ###### DO NOT CHANGE ######
// ===============================================================================
__IO uint32_t TimingDelay = 0;																	//used with the Delay function
__IO uint8_t DataReady = 0;
__IO uint32_t USBConnectTimeOut = 1000;
__IO uint32_t UserButtonPressed = 0;
__IO uint8_t PrevXferComplete = 1;
// ===============================================================================

uint32_t ticker=0,downticker=0;

int main(void)
{
	char str_buf[200];
	int ledflag=1;

	init();

    while(1)
    {
    	//自動送信
    	Can_Auto_Trans();

    	if(ticker>500){
    		ticker=0;

    		GPIO_WriteBit(GPIOB,GPIO_Pin_13,ledflag);
    		GPIO_WriteBit(GPIOB,GPIO_Pin_14,ledflag);
    		GPIO_WriteBit(GPIOB,GPIO_Pin_15,ledflag);

     		if(ledflag != 0){
    			ledflag=0;
    		}else{
    			ledflag=1;
    		}

    		sprintf(str_buf,"TIM1:%d TIM2:%d TIM3:%d TIM4:%d\n\r", Encoder_Count(TIM1), Encoder_Count(TIM2), Encoder_Count(TIM3), Encoder_Count(TIM4));
     		transmit_usart2_dma(str_buf);
    		//VCP_PutStr(str_buf);//USBの送信は500ms以上の間隔で送信すること
    	}
    }
}

void init(void)
{
	SystemInit();

	if (SysTick_Config(SystemCoreClock / 1000)){}

	LED_configuration();

#ifdef USE_USART2
	USART2_ConfigurationDMA();
#endif

#ifdef USE_ENCODER
	TIM_encoder_Configuration();
#endif

#ifdef USE_CAN
	CAN_Configuration();
#endif

	NVIC_Configuration();

#ifdef USE_USB
	//USB設定
	VCP_ResetPort();//pull the USB D+ line low to disconnect the USB device from the PC
	Delay(500);		//allow a few milliseconds before reconnecting
	VCP_Init();		//initialize the STM32F3-Discovery as a Virtual COM Port - device gets re-detected by the PC
	//送信例： VCP_PutStr(buf);
	Delay(500);
#endif

}

void LED_configuration (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB ,ENABLE);

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_Level_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//消灯
	GPIO_WriteBit(GPIOB,GPIO_Pin_13,0);
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,0);
	GPIO_WriteBit(GPIOB,GPIO_Pin_15,0);

}

// Function to insert a timing delay of nTime
// ###### DO NOT CHANGE ######
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

// Function to Decrement the TimingDelay variable.
// ###### DO NOT CHANGE ######
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

// Unused functions that have to be included to ensure correct compiling
// ###### DO NOT CHANGE ######
// =======================================================================
uint32_t L3GD20_TIMEOUT_UserCallback(void)
{
  return 0;
}

uint32_t LSM303DLHC_TIMEOUT_UserCallback(void)
{
  return 0;
}
// =======================================================================
