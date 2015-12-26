#include "can.h"

#define LIST 0
#define MASK 1
#define BIT16 16
#define BIT32 32

typedef struct _CAN_f{
	int FNE;	//FilterNumberEnable
	int MODE;	//LISTorMASK
	int SCALE;	//16bitor32bit
	int STID[4];	//
	int RTR[4];	//
	int	IDE[4];	//
	int EXID[4];	//
}CAN_f;
//RTR 0:�f�[�^�t���[���@1:�����[�g�t���[��
CAN_f CAN_fm[14]={
//FNE	MODE	SCALE	STID    					RTR 		IDE			EXID
 {1,	LIST,   BIT16,	{0x400,0x401,0x402,0x403},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//0
 {1,	LIST,   BIT16,	{0x440,0x441,0x442,0x443},	{1,1,1,1},	{0,0,0,0},	{0,0,0,0}	},//1
 {0,	LIST,   BIT16,	{0x000,0x000,0x000,0x000},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//2
 {0,	MASK,   BIT32,	{0x000,0x000},				{0,0},		{0,0},		{0,0}		},//3
 {0,	LIST,   BIT16,	{0x000,0x00A,0x00B,0x00C},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//4
 {0,	LIST,   BIT16,	{0x000,0x00A,0x00B,0x00C},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//5
 {0,	LIST,   BIT16,	{0x000,0x00A,0x00B,0x00C},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//6
 {0,	LIST,   BIT16,	{0x000,0x00A,0x00B,0x00C},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//7
 {0,	LIST,   BIT16,	{0x000,0x00A,0x00B,0x00C},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//8
 {0,	LIST,   BIT16,	{0x000,0x00A,0x00B,0x00C},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//9
 {0,	LIST,   BIT16,	{0x000,0x00A,0x00B,0x00C},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//10
 {0,	LIST,   BIT16,	{0x000,0x00A,0x00B,0x00C},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//11
 {0,	LIST,   BIT16,	{0x000,0x00A,0x00B,0x00C},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//12
 {0,	LIST,   BIT16,	{0x000,0x00A,0x00B,0x00C},	{0,0,0,0},	{0,0,0,0},	{0,0,0,0}	},//13
};
/* ���̐���
 *�@FNE:���̃t�B���^�[���g�����ۂ�
 * RTR �f�[�^�t���[��:0 �����[�g�t���[��:1
 * STID�ARTR�AIDE,EXID�̔z��[0][1][2][3]��
 * LIST,BIT16�̂Ƃ�	[0]:filter 	[1]:filter		[2]:filter 	[3]:filter
 * MASK,BIT16�̂Ƃ�	[0]:filter 	[1]:[0]��mask  	[2]:filter 	[3]:[2]��mask
 * LIST,BIT32�̂Ƃ�	[0]:filter 	[1]:filter 		[2]:���g�p 	[3]:���g�p
 * MASK,BIT32�̂Ƃ�	[0]:filter 	[1]:[0]��mask 	[2]:���g�p 	[3]:���g�p
 * �Ƃ����\���ɂȂ��Ă���
 */
uint32_t tx_interval[4];//�G���R�[�_�[�̐������p��

//���M����郁�b�Z�[�W���i�[�����ϐ�
CanTxMsg can_tx_flame[4];
CanRxMsg can_rx_flame;

unsigned short interval_time[4] = {0};//�G���R�[�_�[�̐������p��
int can_enc_mode[4] = {0};//�G���R�[�_�[�̐������p��

//���M����f�[�^�t���[����ID
#define CANID_TIM1_TX 0x440
#define CANID_TIM2_TX 0x441
#define CANID_TIM3_TX 0x442
#define CANID_TIM4_TX 0x443

//��M����f�[�^�t���[����ID
#define CANID_TIM1_OPTION 0x400
#define CANID_TIM2_OPTION 0x401
#define CANID_TIM3_OPTION 0x402
#define CANID_TIM4_OPTION 0x403

//��M���郊���[�g�t���[����ID
#define CANID_TIM1_REQUEST 0x440
#define CANID_TIM2_REQUEST 0x441
#define CANID_TIM3_REQUEST 0x442
#define CANID_TIM4_REQUEST 0x443

void CAN_Configuration(void)
{
	/* Define variable ------------------------------------------------------*/
	int filter_num = 0;

	/* Define InitTypeDef ---------------------------------------------------*/
	GPIO_InitTypeDef 		GPIO_InitStructure;
	CAN_InitTypeDef 		CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;

	/* initialize InitTypeDef -----------------------------------------------*/
	

	/* CAN Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* Connect CAN pins to AF9 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_9);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_9);

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		/* CAN register init */
	CAN_DeInit(CAN1);

	CAN_StructInit(&CAN_InitStructure);

	/* Set up CAN function -------------------------------------------------*/
	/* �^�C���g���K�ʐM���[�h�̗L�����E��������ݒ肷�� */
	CAN_InitStructure.CAN_TTCM 		= ENABLE;
	/* �����o�X�I�t�Ǘ�(Automatic Bus-Off Management)�̗L�����E��������ݒ肷�� */
	CAN_InitStructure.CAN_ABOM 		= DISABLE;
	/* �����ċN�����[�h�̗L�����E��������ݒ肷��  */
	CAN_InitStructure.CAN_AWUM 		= DISABLE;
	/* �����đ��M�֎~��L�����E���������� DISABLE: �����đ��M�֎~�𖳌���(�܂�đ��M�͗L��) ENABLE: �����đ��M�֎~�B����ɑ��M����Ȃ��Ă����M��1�񂾂��s���� */
	CAN_InitStructure.CAN_NART 		= DISABLE;
	/* ��MFIFO���b�N���[�h�̗L�����E��������ݒ肷�� */
	CAN_InitStructure.CAN_RFLM 		= DISABLE;
	/* ���MFIFO�̑��M�������w�肷��BDISABLE:���b�Z�[�WID�ő��M���������肳���  ENABLE:�\�t�g�E�F�A�ő��M�v�����������ꂽ���Ԃő��M����� */
	CAN_InitStructure.CAN_TXFP 		= DISABLE;
	/* CAN��Mode��ݒ肷�� */
	CAN_InitStructure.CAN_Mode 		= CAN_Mode_Normal;
	/* �ē����W�����v��(CAN�n�[�h�E�F�A���ē������s���ۂ̃r�b�g��)�����ԒP�ʂ̐��Őݒ肷�� */
	CAN_InitStructure.CAN_SJW 		= CAN_SJW_1tq;
	/* CAN�r�b�g�^�C�~���O���W�X�^(CAN_BTR)��TS1[3:0]��ݒ肷��B */
	CAN_InitStructure.CAN_BS1 		= CAN_BS1_5tq;
	/* CAN�r�b�g�^�C�~���O���W�X�^(CAN_BTR)��TS2[2:0]��ݒ肷�� */
	CAN_InitStructure.CAN_BS2 		= CAN_BS2_6tq;
	/* �{�[���[�g�v���X�P�[���ݒ肷�� 1�`1024 APB1=36MHz*/
	CAN_InitStructure.CAN_Prescaler	= 3;
	//CAN�{�[���[�g = 1Mbps
	CAN_Init(CAN1, &CAN_InitStructure);

	/* Set up CAN Filter function -------------------------------------------------*/
	while(filter_num < 14){//�t�B���^�[�̐������J��Ԃ�0~13

		CAN_FilterInitStructure.CAN_FilterNumber = filter_num;//CAN2�̂Ƃ�+14

		CAN_FilterInitStructure.CAN_FilterFIFOAssignment	= 0;

		if(CAN_fm[filter_num].FNE != 0){//�t�B���^�[�̐ݒ�������Ă���ꍇ

			CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

			if(CAN_fm[filter_num].MODE == MASK){/* �t�B���^�[���[�h�ݒ� */
				CAN_FilterInitStructure.CAN_FilterMode	= CAN_FilterMode_IdMask;
			}else if(CAN_fm[filter_num].MODE == LIST){
				CAN_FilterInitStructure.CAN_FilterMode	= CAN_FilterMode_IdList;
			}

			if(CAN_fm[filter_num].SCALE == BIT16){

				CAN_FilterInitStructure.CAN_FilterScale			= CAN_FilterScale_16bit;

				CAN_FilterInitStructure.CAN_FilterIdLow			= 	(CAN_fm[filter_num].STID[0] << 5) |
																	(CAN_fm[filter_num].RTR[0] << 4) |
																	(CAN_fm[filter_num].IDE[0] << 3) |
																	(CAN_fm[filter_num].EXID[0] >> 15);

				CAN_FilterInitStructure.CAN_FilterMaskIdLow		= 	(CAN_fm[filter_num].STID[1] << 5) |
																	(CAN_fm[filter_num].RTR[1] << 4) |
																	(CAN_fm[filter_num].IDE[1] << 3) |
																	(CAN_fm[filter_num].EXID[1] >> 15);

				CAN_FilterInitStructure.CAN_FilterIdHigh		= 	(CAN_fm[filter_num].STID[2] << 5) |
																	(CAN_fm[filter_num].RTR[2] << 4) |
																	(CAN_fm[filter_num].IDE[2] << 3) |
																	(CAN_fm[filter_num].EXID[2] >> 15);

				CAN_FilterInitStructure.CAN_FilterMaskIdHigh	= 	(CAN_fm[filter_num].STID[3] << 5) |
																	(CAN_fm[filter_num].RTR[3] << 4) |
																	(CAN_fm[filter_num].IDE[3] << 3) |
																	(CAN_fm[filter_num].EXID[3] >> 15);

			}else if(CAN_fm[filter_num].SCALE == BIT32){

				CAN_FilterInitStructure.CAN_FilterScale			= CAN_FilterScale_32bit;

				CAN_FilterInitStructure.CAN_FilterIdLow			= 	((CAN_fm[filter_num].EXID[0]&0x1FFF)<<3) |
																	(CAN_fm[filter_num].IDE[0] << 2) |
																	(CAN_fm[filter_num].RTR[0] << 1);

				CAN_FilterInitStructure.CAN_FilterMaskIdLow		= 	(CAN_fm[filter_num].STID[0] << 5) |
																	(CAN_fm[filter_num].EXID[0] >> 13);

				CAN_FilterInitStructure.CAN_FilterIdHigh		= 	((CAN_fm[filter_num].EXID[1]&0x1FFF)<<3) |
																	(CAN_fm[filter_num].IDE[1] << 2) |
																	(CAN_fm[filter_num].RTR[1] << 1);

				CAN_FilterInitStructure.CAN_FilterMaskIdHigh	= 	(CAN_fm[filter_num].STID[1] << 5) |
																	(CAN_fm[filter_num].EXID[1] >> 13);
			}

		}else{//�t�B���^�[�̐ݒ�������Ă��Ȃ��ꍇ���̃t�B���^�[�i���o�[�̓m���A�N�e�B�u

			CAN_FilterInitStructure.CAN_FilterActivation 	= DISABLE;
		}

		CAN_FilterInit(&CAN_FilterInitStructure);

		filter_num++;
	}

#ifdef USE_INTERRUPT_CAN_RX
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//message pending Interrupt
#endif

#ifdef USE_INTERRUPT_CAN_TX
	CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);//Transmit mailbox empty Interrupt
#endif

}




void USB_LP_CAN1_RX0_IRQHandler(void)
{
	int tmp=0;
#ifdef USE_USB
	USB_Istr();
#endif

	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)){//�V�������b�Z�[�W����M������Ăяo�����
		CAN_Receive(CAN1, CAN_FIFO0, &can_rx_flame);//��M
		CAN_Receive_Check(&can_rx_flame);
	}
}

void Encoder_int_to_char(TIM_TypeDef* TIMx, int value)
{
	if(TIMx == TIM1){
		can_tx_flame[0].StdId = CANID_TIM1_TX;
		can_tx_flame[0].RTR	= 0;
		can_tx_flame[0].IDE	= 0;
		can_tx_flame[0].DLC	= 4;
		can_tx_flame[0].Data[0] = (char)(value & 0x000000FF);
		can_tx_flame[0].Data[1] = (char)((value & 0x0000FF00) >> 8);
		can_tx_flame[0].Data[2] = (char)((value & 0x00FF0000) >> 16);
		can_tx_flame[0].Data[3] = (char)((value & 0xFF000000) >> 24);
	}else if(TIMx == TIM2){
		can_tx_flame[1].StdId = CANID_TIM2_TX;
		can_tx_flame[1].RTR	= 0;
		can_tx_flame[1].IDE	= 0;
		can_tx_flame[1].DLC	= 4;
		can_tx_flame[1].Data[0] = (char)(value & 0x000000FF);
		can_tx_flame[1].Data[1] = (char)((value & 0x0000FF00) >> 8);
		can_tx_flame[1].Data[2] = (char)((value & 0x00FF0000) >> 16);
		can_tx_flame[1].Data[3] = (char)((value & 0xFF000000) >> 24);
	}else if(TIMx == TIM3){
		can_tx_flame[2].StdId = CANID_TIM3_TX;
		can_tx_flame[2].RTR	= 0;
		can_tx_flame[2].IDE	= 0;
		can_tx_flame[2].DLC	= 4;
		can_tx_flame[2].Data[0] = (char)(value & 0x000000FF);
		can_tx_flame[2].Data[1] = (char)((value & 0x0000FF00) >> 8);
		can_tx_flame[2].Data[2] = (char)((value & 0x00FF0000) >> 16);
		can_tx_flame[2].Data[3] = (char)((value & 0xFF000000) >> 24);
	}else if(TIMx == TIM4){
		can_tx_flame[3].StdId = CANID_TIM4_TX;
		can_tx_flame[3].RTR	= 0;
		can_tx_flame[3].IDE	= 0;
		can_tx_flame[3].DLC	= 4;
		can_tx_flame[3].Data[0] = (char)(value & 0x000000FF);
		can_tx_flame[3].Data[1] = (char)((value & 0x0000FF00) >> 8);
		can_tx_flame[3].Data[2] = (char)((value & 0x00FF0000) >> 16);
		can_tx_flame[3].Data[3] = (char)((value & 0xFF000000) >> 24);
	}
}

void Encoder_into_CANflame(TIM_TypeDef* TIMx)
{
	Encoder_int_to_char(TIMx,Encoder_Count(TIMx));
}

void CAN_Receive_Check (CanRxMsg* RxMessage)
{
	switch(RxMessage->StdId){

//�G���R�[�_�[�l�̗v��
	//TIM1
	case CANID_TIM1_REQUEST://�G���R�[�_�[�l�̗v��

		Encoder_into_CANflame(TIM1);
		CAN_Transmit(CAN1, &can_tx_flame[0]);//���M

		break;
	//TIM2
	case CANID_TIM2_REQUEST://�G���R�[�_�[�l�̗v��
		Encoder_into_CANflame(TIM2);
		CAN_Transmit(CAN1, &can_tx_flame[1]);//���M

		break;
	//TIM3
	case CANID_TIM3_REQUEST://�G���R�[�_�[�l�̗v��

		Encoder_into_CANflame(TIM3);
		CAN_Transmit(CAN1, &can_tx_flame[2]);//���M

		break;

	//TIM4
	case CANID_TIM4_REQUEST://�G���R�[�_�[�l�̗v��

		Encoder_into_CANflame(TIM4);
		CAN_Transmit(CAN1, &can_tx_flame[3]);//���M

		break;

//���M�I�v�V�����ύX
	//TIM1
	case CANID_TIM1_OPTION://���M�I�v�V�����ύX

		if(RxMessage->Data[0] == 0x00){//�G���R�[�_�[�l���Z�b�g
			Encoder_ClearCount(TIM1);
		}else if(RxMessage->Data[0] == 0x01){//�������M�ݒ�
			interval_time[0] = (unsigned short)(((RxMessage->Data[2] << 8)&0xFF00) | ((RxMessage->Data[1])&0x00FF));
			if(interval_time[0] == 65535){
				can_enc_mode[0] = 0;//�������M��~
			}else{
				can_enc_mode[0] = 1;//�������M�J�n
				tx_interval[0] = 0;
			}
		}
		break;

	case CANID_TIM2_OPTION://���M�I�v�V�����ύX

		if(RxMessage->Data[0] == 0x00){//�G���R�[�_�[�l���Z�b�g
			Encoder_ClearCount(TIM2);
		}else if(RxMessage->Data[0] == 0x01){//�������M�ݒ�
			interval_time[1] = (unsigned short)(((RxMessage->Data[2] << 8)&0xFF00) | ((RxMessage->Data[1])&0x00FF));
			if(interval_time[1] == 65535){
				can_enc_mode[1] = 0;//�������M��~
			}else{
				can_enc_mode[1] = 1;//�������M�J�n
				tx_interval[1] = 0;
			}
		}
		break;

	case CANID_TIM3_OPTION://���M�I�v�V�����ύX

		if(RxMessage->Data[0] == 0x00){//�G���R�[�_�[�l���Z�b�g
			Encoder_ClearCount(TIM3);
		}else if(RxMessage->Data[0] == 0x01){//�������M�ݒ�
			interval_time[2] = (unsigned short)(((RxMessage->Data[2] << 8)&0xFF00) | ((RxMessage->Data[1])&0x00FF));
			if(interval_time[2] == 65535){
				can_enc_mode[2] = 0;//�������M��~
			}else{
				can_enc_mode[2] = 1;//�������M�J�n
				tx_interval[2] = 0;
			}
		}
		break;

	case CANID_TIM4_OPTION://���M�I�v�V�����ύX

		if(RxMessage->Data[0] == 0x00){//�G���R�[�_�[�l���Z�b�g
			Encoder_ClearCount(TIM4);
		}else if(RxMessage->Data[0] == 0x01){//�������M�ݒ�
			interval_time[3] = (unsigned short)(((RxMessage->Data[2] << 8)&0xFF00) | ((RxMessage->Data[1])&0x00FF));
			if(interval_time[3] == 65535){
				can_enc_mode[3] = 0;//�������M��~
			}else{
				can_enc_mode[3] = 1;//�������M�J�n
				tx_interval[3] = 0;
			}
		}
		break;

	default:
		break;
	}
}

void Can_Auto_Trans(void)
{
	//TIM1
	switch(can_enc_mode[0]){

		case 0:

			break;

		case 1://�������M����
			if(tx_interval[0] >= interval_time[0]){
				tx_interval[0] = 0;
				Encoder_into_CANflame(TIM1);
				CAN_Transmit(CAN1, &can_tx_flame[0]);//���M
			}
			break;
	}

	//TIM2
	switch(can_enc_mode[1]){

		case 0:

			break;

		case 1://�������M����
			if(tx_interval[1] >= interval_time[1]){
				tx_interval[1] = 0;
				Encoder_into_CANflame(TIM2);
				CAN_Transmit(CAN1, &can_tx_flame[1]);//���M
			}
			break;
	}

	//TIM3
	switch(can_enc_mode[2]){

		case 0:

			break;

		case 1://�������M����
			if(tx_interval[2] >= interval_time[2]){
				tx_interval[2] = 0;
				Encoder_into_CANflame(TIM3);
				CAN_Transmit(CAN1, &can_tx_flame[2]);//���M
			}
			break;
	}

	//TIM4
	switch(can_enc_mode[3]){

		case 0:

			break;

		case 1://�������M����
			if(tx_interval[3] >= interval_time[3]){
				tx_interval[3] = 0;
				Encoder_into_CANflame(TIM4);
				CAN_Transmit(CAN1, &can_tx_flame[3]);//���M
				}
			break;
	}
}

void CAN_TX_TEST(int num)
{
	switch(num){

	case 0:
		Encoder_into_CANflame(TIM1);
		CAN_Transmit(CAN1, &can_tx_flame[0]);//���M
		break;
	case 1:
		Encoder_into_CANflame(TIM2);
		CAN_Transmit(CAN1, &can_tx_flame[1]);//���M
		break;
	case 2:
		Encoder_into_CANflame(TIM3);
		CAN_Transmit(CAN1, &can_tx_flame[2]);//���M
		break;
	case 3:
		Encoder_into_CANflame(TIM4);
		CAN_Transmit(CAN1, &can_tx_flame[3]);//���M
		break;
	}
}

