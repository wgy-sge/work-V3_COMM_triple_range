
#include "include.h"

u8 gMessagePending;	
extern CAN_MSG gRxCAN;	

bool gCanOpenPollBusy= FALSE;//是否正在处理通讯帧

//释放处理器
void CanOpenPollFree(void)
{
	gCanOpenPollBusy= FALSE;
}

//开始处理
void CanOpenPollStart(void)
{
	gCanOpenPollBusy= TRUE;
}

/****************************************************************************
* 名称：CAN_Configration( void )
* 功能：CAN配置
* 入口参数：BaudRate
* 出口参数：无
****************************************************************************/
void CAN_Configration( void )
{
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;


	CAN_DeInit();
	CAN_StructInit(&CAN_InitStructure);
	//关闭时间触发模式
	CAN_InitStructure.CAN_TTCM=DISABLE;
	//关闭自动离线管理
	CAN_InitStructure.CAN_ABOM=DISABLE;
	//关闭自动唤醒模式
	CAN_InitStructure.CAN_AWUM=DISABLE;
	//禁止报文自动重传
	CAN_InitStructure.CAN_NART=DISABLE;
	//FIFO 溢出时报文覆盖源文件
	CAN_InitStructure.CAN_RFLM=DISABLE;
	//报文发送优先级取决于ID号
	CAN_InitStructure.CAN_TXFP=DISABLE;
	//正常的工作模式
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	//设置CAN 波特率=36000000/CAN_Prescaler/(CAN_SJW+CAN_BS1+CAN_BS2)
	//2022.4.6 @luguangping 将CAN波特率由500K改为250K
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//250K//CAN_SJW_1tq;//500
	CAN_InitStructure.CAN_BS1=CAN_BS1_6tq;//CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_5tq;//CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler = 12;	
	//初始化CAN
	CAN_Init( &CAN_InitStructure );

	/*
	// CAN filter init 过滤器初始化 
	CAN_FilterInitStructure.CAN_FilterNumber=0; // 指定了待初始化的过滤器 0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;     //指定了过滤器将被初始化到的模式为标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;    //给出了过滤器位宽 1 个 32 位过滤器

	//31~21  为识别码
	CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;                  // 用来设定过滤器标识符（32 位位宽时为其高段位，16 位位宽时为第一个）	
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;                    // 用来设定过滤器标识符 （32 位位宽时为其低段位，16 位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;               // 用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16 位位宽时为第一个
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;                // 用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16 位位宽时为第二个
	
	
	//31~21  为识别码
	CAN_FilterInitStructure.CAN_FilterIdHigh= gSystCommInfo.device_addr;        //用来设定过滤器标识符（32 位位宽时为其高段位，16 位位宽时为第一个）	
	CAN_FilterInitStructure.CAN_FilterIdHigh<<= 7;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;                    //用来设定过滤器标识符 （32 位位宽时为其低段位，16 位位宽时为第二个
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x00f0;               // 用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16 位位宽时为第一个	
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh<<= 3;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;                // 用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16 位位宽时为第二个
	

	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;        // 设定了指向过滤器的 FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;               // 使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	*/
	
	//使能接收中断
	CAN_ITConfig(CAN_IT_FMP0, ENABLE);
}

//初始化CAN硬件接口
void CanInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN,ENABLE);

	//CAN  I/O初始化
	//PA11 For CAN1-RX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//RX
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//PA12 For CAN1-TX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;//TX
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

	CAN_Configration();//配置CAN
}


/*******************************************************************************
* Function Name  : CAN_Receive_UC
* Description    : Receives a message.
* Input          : FIFONumber: Receive FIFO number, CAN_FIFO0 or CAN_FIFO1.
* Output         : RxMessage: pointer to a structure which contains CAN Id,
*                  CAN DLC, CAN datas and FMI number.
* Return         : None.
*******************************************************************************/
void CAN_Receive_UC(CAN_TypeDef* CANx,u8 FIFONumber, u32* pStdId, u32* pDLC, u32* pDataH, u32* pDataL)
{
	/* Check the parameters */
	//assert_param(IS_CAN_ALL_PERIPH(CANx));
	assert_param(IS_CAN_FIFO(FIFONumber));

	/* Get the Id */
	*pStdId = (u32)0x000007FF & (CANx->sFIFOMailBox[FIFONumber].RIR >> 21);

	/* Get the DLC */
	*pDLC = (u32)0x0000000F & CANx->sFIFOMailBox[FIFONumber].RDTR;

	/* Get the data field */
	*pDataL = CANx->sFIFOMailBox[FIFONumber].RDLR;
	*pDataH = CANx->sFIFOMailBox[FIFONumber].RDHR;

	/* Release the FIFO */
	CAN_FIFORelease(FIFONumber);
}

/****************************************************************************
* 名称：Crx(void)
* 功能：CAN接收数据
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  Crx(void)
{
	u8 i;
	u32 DataL,DataH;
	CAN_MSG MEM_FAR rxcanTmp;
	for(i = 0; i < gMessagePending; i++)
	{
		if (gCanOpenPollBusy== FALSE)//处理器闲
		{			
			CAN_Receive_UC(CAN,CAN_FIFO0,(u32*)(&gRxCAN.ID), (u32*)(&gRxCAN.LEN), &DataH, &DataL);
			gRxCAN.BUF[0] = (u32)0x000000FF & DataL;
			gRxCAN.BUF[1] = (u32)0x000000FF & (DataL >> 8);
			gRxCAN.BUF[2] = (u32)0x000000FF & (DataL >> 16);
			gRxCAN.BUF[3] = (u32)0x000000FF & (DataL >> 24);

			gRxCAN.BUF[4] = (u32)0x000000FF & DataH;
			gRxCAN.BUF[5] = (u32)0x000000FF & (DataH >> 8);
			gRxCAN.BUF[6] = (u32)0x000000FF & (DataH >> 16);
			gRxCAN.BUF[7] = (u32)0x000000FF & (DataH >> 24);
			CanOpenRxPres();
		}
		else
		{
			CAN_Receive_UC(CAN,CAN_FIFO0,(u32*)(&rxcanTmp.ID), (u32*)(&rxcanTmp.LEN), &DataH, &DataL);
		}		
		
	}
	gMessagePending = 0;
}


/****************************************************************************
* 名称：Ctx(void)
* 功能：CAN发送数据
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  Ctx(void)
{
	MCO_ProcessStack();
}


/****************************************************************************
* 名称：can_rx_irq(void)
* 功能：CAN接受中断服务函数
* 入口参数：无
* 出口参数：无
****************************************************************************/
void can_rx_irq(void)
{
	gMessagePending = CAN_MessagePending(CAN_FIFO0);

	Crx();
}


