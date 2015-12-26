#include "usart.h"

#define USART1_BAUDRATE 115200
#define USART2_BAUDRATE 115200

#define USART_TxBufferSize  200//�����O�o�b�t�@�̗v�f��
#define USART_RxBufferSize  200//�����O�o�b�t�@�̗v�f��

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
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;//�đ��M�L��
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
		USART2_TxBufferDMA[i]='\0';//null����
	}

	while(*str!=0) // until end of string   ������̖����܂Ń��[�v
	{
		USART2_TxBufferDMA[USART2_TxPtrEndDMA] = *(str++);
	    USART2_TxPtrEndDMA++; //�|�C���^�{�P
	}
	USART2_TxPtrEndDMA=0;

	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel7, ENABLE);

}
void DMA1_Channel7_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC7) != RESET){
		DMA_Cmd(DMA1_Channel7, DISABLE);//�đ��M���L���ɂȂ��Ă��邽��DMA�𖳌��ɂ���@���M�֐��ōēx�L���ɂ���Ƒ��M�ĊJ����
		DMA_ClearFlag(DMA1_FLAG_TC7);
	}

}
//�ꕶ�����M
void transmit_usart1_c(char c)
{

#ifdef USE_INTERRUPT_USART1
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);  //���M���I��������荞�݂��ꎞ�I�ɋ֎~
#endif

  USART1_TxBuffer[USART1_TxPtrEnd] = c;    //�����O�o�b�t�@�ɂP�����ǉ�
  USART1_TxPtrEnd++;    //�������݃|�C���^���{�P

  if(USART1_TxPtrEnd > (USART_TxBufferSize-1)) USART1_TxPtrNow=0; USART1_TxPtrEnd = 1;    //�������݃|�C���^���I�[�o�[�t���[������O�ɖ߂�

#ifdef USE_INTERRUPT_USART1
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  // ���M���I��������荞�݂�����
#endif

}
//�����񑗐M
void transmit_usart1_s(char s[])
{

#ifdef USE_INTERRUPT_USART1
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
#endif

  while(*s!=0) // until end of string   ������̖����܂Ń��[�v
  {
    USART1_TxBuffer[USART1_TxPtrEnd] = *(s++);
    USART1_TxPtrEnd++; //  �|�C���^�{�P

    if(USART1_TxPtrEnd > (USART_TxBufferSize-1)) USART1_TxPtrEnd = 0;
  }

#ifdef USE_INTERRUPT_USART1
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);   //���M���I��������荞�݂�����
#endif

}

void USART1_IRQHandler(void)
{
	char c;
  
	//��M���荞��
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		c = (char)USART_ReceiveData(USART1);
		transmit_usart1_c(c);
	}
  
	//���M���荞��
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET){  // UART���M�t���O�`�F�b�N
		USART_SendData(USART1, USART1_TxBuffer[USART1_TxPtrNow++]); // �P�������M

		if(USART1_TxPtrNow > (USART_TxBufferSize-1)) USART1_TxPtrNow=0;//�|�C���^�I�[�o�[�t���[�Ȃ�[���ɖ߂�

		if(USART1_TxPtrNow == USART1_TxPtrEnd){     //�����O�o�b�t�@���󂩁H
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE); //���M���荞�݂��I�t
		}
	}
}
