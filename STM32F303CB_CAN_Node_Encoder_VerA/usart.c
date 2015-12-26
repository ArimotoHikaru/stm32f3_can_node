#include "usart.h"

#define USART1_BAUDRATE 115200
#define USART2_BAUDRATE 115200

#define USART_TxBufferSize  200//リングバッファの要素数
#define USART_RxBufferSize  200//リングバッファの要素数

char USART1_TxBuffer[USART_TxBufferSize];
char USART1_RxBuffer[USART_RxBufferSize];
int  USART1_TxPtrNow = 0,
     USART1_TxPtrEnd = 1;
int  USART1_RxPtrNow = 0,
     USART1_RxPtrEnd = 1;

char USART2_TxBufferDMA[USART_TxBufferSize];
char USART2_RxBufferDMA[USART_RxBufferSize];
int  USART2_TxPtrNowDMA = 0,
     USART2_TxPtrEndDMA = 1;
int  USART2_RxPtrNowDMA = 0,
     USART2_RxPtrEndDMA = 1;

void USART1_Configuration(void)
{
	/* Define variable ------------------------------------------------------*/
	/* Define InitTypeDef ---------------------------------------------------*/
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;

	/* initialize InitTypeDef -----------------------------------------------*/

	/* Supply clock source --------------------------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Define gpio_config ---------------------------------------------------*/
	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_9;//TX
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_Level_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_10;//RX
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_Level_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);

	USART_DeInit(USART1);

	/* Set up USART1_function --------------------------------------------------*/
	USART_InitStructure.USART_BaudRate 				= USART1_BAUDRATE;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
	USART_InitStructure.USART_Parity 				= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 					= USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);


#ifdef USE_INTERRUPT_USART1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
#endif

	USART_Cmd(USART1, ENABLE);

}
void USART2_ConfigurationDMA(void)
{
	/* Define variable ------------------------------------------------------*/
	/* Define InitTypeDef ---------------------------------------------------*/
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	DMA_InitTypeDef 	DMA_InitStructure;

	/* initialize InitTypeDef -----------------------------------------------*/

	/* Supply clock source --------------------------------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_DMA1, ENABLE);

	/* Define gpio_config ---------------------------------------------------*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_7);

	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_3;//TX
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_Level_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_4;//RX
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_Level_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_DeInit(USART2);

	/* Set up USART2_function --------------------------------------------------*/
	USART_InitStructure.USART_BaudRate 				= USART2_BAUDRATE;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
	USART_InitStructure.USART_Parity 				= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 					= USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &USART_InitStructure);

#ifdef USE_INTERRUPT_USART2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
#endif

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

	USART_Cmd(USART2, ENABLE);

	DMA_DeInit(DMA1_Channel7);

	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&USART2->TDR;
	DMA_InitStructure.DMA_MemoryBaseAddr		= (uint32_t)USART2_TxBufferDMA;
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize 			= sizeof(USART2_TxBufferDMA)/sizeof(uint8_t);
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;//再送信有効
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;

	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel7, ENABLE);

}
void transmit_usart2_dma(char str[])
{
	int j = sizeof(USART2_TxBufferDMA);
	int i=0;
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, DISABLE);

	for(i=0;i<j;i++){
		USART2_TxBufferDMA[i]='\0';//null埋め
	}

	while(*str!=0) // until end of string   文字列の末尾までループ
	{
		USART2_TxBufferDMA[USART2_TxPtrEndDMA] = *(str++);
	    USART2_TxPtrEndDMA++; //ポインタ＋１
	}
	USART2_TxPtrEndDMA=0;

	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel7, ENABLE);

}
void DMA1_Channel7_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC7) != RESET){
		DMA_Cmd(DMA1_Channel7, DISABLE);//再送信が有効になっているためDMAを無効にする　送信関数で再度有効にすると送信再開する
		DMA_ClearFlag(DMA1_FLAG_TC7);
	}

}
//一文字送信
void transmit_usart1_c(char c)
{

#ifdef USE_INTERRUPT_USART1
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);  //送信し終わった割り込みを一時的に禁止
#endif

  USART1_TxBuffer[USART1_TxPtrEnd] = c;    //リングバッファに１文字追加
  USART1_TxPtrEnd++;    //書き込みポインタを＋１

  if(USART1_TxPtrEnd > (USART_TxBufferSize-1)) USART1_TxPtrNow=0; USART1_TxPtrEnd = 1;    //書き込みポインタがオーバーフローしたら０に戻す

#ifdef USE_INTERRUPT_USART1
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  // 送信し終わった割り込みを許可
#endif

}
//文字列送信
void transmit_usart1_s(char s[])
{

#ifdef USE_INTERRUPT_USART1
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
#endif

  while(*s!=0) // until end of string   文字列の末尾までループ
  {
    USART1_TxBuffer[USART1_TxPtrEnd] = *(s++);
    USART1_TxPtrEnd++; //  ポインタ＋１

    if(USART1_TxPtrEnd > (USART_TxBufferSize-1)) USART1_TxPtrEnd = 0;
  }

#ifdef USE_INTERRUPT_USART1
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);   //送信し終わった割り込みを許可
#endif

}

void USART1_IRQHandler(void)
{
	char c;
  
	//受信割り込み
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		c = (char)USART_ReceiveData(USART1);
		transmit_usart1_c(c);
	}
  
	//送信割り込み
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET){  // UART送信フラグチェック
		USART_SendData(USART1, USART1_TxBuffer[USART1_TxPtrNow++]); // １文字送信

		if(USART1_TxPtrNow > (USART_TxBufferSize-1)) USART1_TxPtrNow=0;//ポインタオーバーフローならゼロに戻す

		if(USART1_TxPtrNow == USART1_TxPtrEnd){     //リングバッファが空か？
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE); //送信割り込みをオフ
		}
	}
}
