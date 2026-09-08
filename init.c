/********************    ELoader  通讯板嵌入式程序    ********************
Copyright (C), 2010, Zidetech. Co., Ltd.
* File Name          : init.c
* Author             : lu guangping
* Version            : V0.0.1
* Date               : 2010/9/15
* Description        : 初始化代码
********************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "include.h"


EXTI_InitTypeDef 	 	EXTI_InitStructure;
GPIO_InitTypeDef 		GPIO_InitStructure;
NVIC_InitTypeDef 	 	NVIC_InitStructure;
USART_InitTypeDef 	    USART_InitStructure;
//I2C_InitTypeDef	 	    I2C_InitStructure;




//系统时钟 为 32m ,大约 32个指令 周期 为 1us
void delay_us(vu32 us)
{
	u32 i;
	while(us != 0)
	{
		for(i=0;i<=16;i++);
		us--;
	}
}

void delay_ms(vu32  ms)
{
	//u32 i;
	while(ms != 0)
	{
		delay_us(1000);
		ms--;
	}
}

void GPIBIntOFF(void)
{
	//停止中断
	// Disable the EXTI9_5 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);


}
void GPIBIntON(void)
{
	//清除中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void ResetInit(void)
{
	//开机清屏
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO|
		RCC_APB2Periph_USART1, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//CPD_RST			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	CPD_RST_0;       
	delay_ms(20);       
	CPD_RST_1;
	delay_ms(20);
}



/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
	//RCC_HSICmd(ENABLE);			//使能 内部RC振荡器

	/* Wait till HSE is ready */
	while((HSEStartUpStatus = RCC_WaitForHSEStartUp())!=SUCCESS);

	if(HSEStartUpStatus == SUCCESS)
	{
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);  
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);//RCC_PLLMul_9

		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08);
	}

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO|
		RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
}

void Init_Device(void)
{
	//TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_OCInitTypeDef  TIM_OCInitStructure;  //产生PWM时用的

	//USART_ClockInitTypeDef USART_ClockInitStructure;

	//定义一个RCC_ClocksTypeDef 的结构体
	RCC_ClocksTypeDef get_rcc_clock;   

	RCC_Configuration();

	/* 下面用来初始化 SysTick  */
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) ;  //  30/8=3.75M
	/* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
	SysTick_SetReload(((HSE_Value*9/8)/1000));		//1ms 中中段 一次
	/* Enable SysTick interrupt */
	SysTick_ITConfig(ENABLE);
	/* Enable the SysTick Counter */
	SysTick_CounterCmd(SysTick_Counter_Enable);

	//调用RCC_GetClocksFreq获取系统时钟状态
	RCC_GetClocksFreq(&get_rcc_clock);  

	/* 下面用来初始化 USART 用到的GPIO  */
	GPIO_PinRemapConfig(GPIO_Remap_USART2, DISABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*------------------------ 配置TIM3，定时产生中断 -----------------------*/ 
	//GPIB用到的中断
	/* Connect Key Button EXTI Line to Key Button GPIO Pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	/* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5;		//			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		//			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	//gpib 用到的 io口 初始化
	//------------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin =
		GPIO_Pin_8+GPIO_Pin_9+GPIO_Pin_10+GPIO_Pin_11	
		+GPIO_Pin_12+GPIO_Pin_13+GPIO_Pin_14+GPIO_Pin_15;	//CPD0~CPD7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			//cpd_int	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 
		GPIO_Pin_11	+GPIO_Pin_10;	//CPD_A4 CPD_A5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);



	GPIO_InitStructure.GPIO_Pin = 				
		GPIO_Pin_4		//CPD_RST	
		+GPIO_Pin_5		//CPD_OE	
		+GPIO_Pin_8;	//CPD_RW
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = 		
		GPIO_Pin_8	//CPD_DEOI
		+GPIO_Pin_9;//CPD_EN	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	//gpib 用到的 io口 初始化
	GPIO_InitStructure.GPIO_Pin = 
		GPIO_Pin_14;			//CPD_TRG	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//gpib 用到的 io口 初始化
	GPIO_InitStructure.GPIO_Pin = 		
		GPIO_Pin_6;			//IQ_EMP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//---------------------------------------------------------------------------
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//原来是0
        
	// Enable the USART3 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//鲁光平_20101220
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the USART2 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//鲁光平_20090902
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the USART3 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//鲁光平_20101220
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);



	/* Enable the EXTI0_Interrupt */
	// CPD_INT 占用此中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the TIM3 gloabal Interrupt */	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//GPIB初始化
	gpib_init();

	//-----------------------------------------------------------
	CanInit( );
}

//波特率选择
const u32 combaud_data[]=
{
	4800,
	9600,
	19200,
	38400,
	115200,
};

const u16 comparity_data[]=
{
	USART_Parity_No,
	USART_Parity_Odd,
	USART_Parity_Even,
};


/*******************************************************************************
* Function Name  : RequestRs485Baud
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
static u32 RequestRs232Baud( void )
{
	return combaud_data[gSystCommInfo.comm_baud];
}

static u16 RequestRs232Parity( void )
{
	return comparity_data[gSystCommInfo.comm_parity];
}




void uart1Init(void)
{
	// DISABLE the USART1
	/* 配置IO口 */

	GPIO_PinRemapConfig( GPIO_Remap_USART1, DISABLE ); 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_Cmd(USART1, DISABLE);

	USART_InitStructure.USART_BaudRate = RequestRs232Baud();		
	USART_InitStructure.USART_Parity = RequestRs232Parity();

	if(gSystCommInfo.comm_parity== NONE_PARITY)
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	else
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;

	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	//USART_IT_PE
	USART_ITConfig(USART1, USART_IT_TC, DISABLE);
	USART_ITConfig(USART1, USART_IT_ERR, DISABLE);

	//Enable USART1 Receive interrupts
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

	// Enable the USART1 
	USART_Cmd(USART1, ENABLE);

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//鲁光平_20090902
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void uart3Init(void)
{
	// DISABLE the USART3
	/* 配置IO口 */	 	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);

	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);

	/* Configure USART3 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  //PC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure USART3 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	USART_Cmd(USART3, DISABLE);

	USART_InitStructure.USART_BaudRate = RequestRs232Baud( );		
	USART_InitStructure.USART_Parity = RequestRs232Parity( );

	if(gSystCommInfo.comm_parity== NONE_PARITY)
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	else
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;

	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	//USART_IT_PE
	USART_ITConfig(USART3, USART_IT_TC, DISABLE);
	USART_ITConfig(USART3, USART_IT_ERR, DISABLE);

	//Enable USART3 Receive interrupts
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);

	// Enable the USART3 
	USART_Cmd(USART3, ENABLE);

	/* Enable the USART3 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//鲁光平_20090902
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//-----------------------------------------------------------------------------
	
}

extern struct net_info gSystNetInfo;

void Init_SystVariables(void)
{		
	//初始化 系统相关变量	
	//真正在系统时需要从主机板读取!!!!!!!!!!!!!!!!!!!!!!!!!
	
	//sys_regs[OFFS_REG_COMM_INTERFACE].reg_int= FT_RS232;
	//sys_regs[OFFS_REG_GPIB_ADDRESS].reg_int= 5;
	//sys_regs[OFFS_REG_COM_BAUD].reg_int= 9600;
	//sys_regs[OFFS_REG_COM_PARITY].reg_int= NONE_PARITY;

	//sys_regs[OFFS_REG_CAL_LEVEL].reg_int= 1;//默认小值点

	//sys_info.local_remote= LOCAL;

	remote_info.uart1.rx_rp= remote_info.uart1.rx_wp= 0;
	remote_info.uart1.tx_rp= remote_info.uart1.tx_wp= 0;
	remote_info.uart1.rx_full= remote_info.uart1.tx_full= FALSE;
	remote_info.uart1.rx_err= 0;
	remote_info.uart1.tx_err= 0;

	remote_info.usb.rx_rp= remote_info.usb.rx_wp= 0;
	remote_info.usb.tx_rp= remote_info.usb.tx_wp= 0;
	remote_info.usb.rx_full= remote_info.usb.tx_full= FALSE;
	remote_info.usb.rx_err= 0;
	remote_info.usb.tx_err= 0;

	remote_info.gpib.rx_wp= remote_info.gpib.rx_rp= 0;
	remote_info.gpib.tx_rp= remote_info.gpib.tx_wp= 0;
	remote_info.gpib.rx_full= remote_info.gpib.tx_full= FALSE;
	remote_info.gpib.rx_err= 0;
	remote_info.gpib.tx_err= 0;


	comm_ctrl.uart2.rx_wp= comm_ctrl.uart2.rx_rp= 0;
	comm_ctrl.uart2.tx_rp= comm_ctrl.uart2.tx_wp= 0;
	comm_ctrl.uart2.rx_full= comm_ctrl.uart2.tx_full= FALSE;
	comm_ctrl.uart2.rx_err= 0;
	comm_ctrl.uart2.tx_err= 0;

	remote_info.lan.rx_rp= remote_info.lan.rx_wp= 0;
	remote_info.lan.tx_rp= remote_info.lan.tx_wp= 0;
	remote_info.lan.rx_full= remote_info.lan.tx_full= FALSE;
	remote_info.lan.rx_err= 0;
	remote_info.lan.tx_err= 0;

	remote_info.tcplan.rx_rp= remote_info.tcplan.rx_wp= 0;
	remote_info.tcplan.tx_rp= remote_info.tcplan.tx_wp= 0;
	remote_info.tcplan.rx_full= remote_info.tcplan.tx_full= FALSE;
	remote_info.tcplan.rx_err= 0;
	remote_info.tcplan.tx_err= 0;

	// scpi 初始化
	scpi_parser_is_idle = TRUE;
	scpi_parser_return_code = ERR_NO_ERROR;

	scpi_proc_ava= TRUE;//可用
	Send_event= NULL;
	Recv_event= NULL;

	outcounter= 0;
/*
	if (sys_regs[OFFS_REG_COMM_INTERFACE].reg_int== FT_GPIB)
	{
		PrintStr= GPIBPrintStr;
		GetChar= getchar_gpib;	
	}
	else if(sys_regs[OFFS_REG_COMM_INTERFACE].reg_int== FT_RS232)
	{
		PrintStr= COMPrintStr;
		GetChar= getchar_uart1;
	}
	else if(sys_regs[OFFS_REG_COMM_INTERFACE].reg_int== FT_USB)
	{
		PrintStr= USBPrintStr;
		GetChar= getchar_usb;
	}
*/
	gSystCommInfo.comm_baud= BAUD_9600;
	gSystCommInfo.comm_parity= NONE_PARITY;

	gSystCommInfo.gpib_addr= 5;

	gSystCommInfo.comm_protocol= PROTOCOL_SCPI;
	gSystCommInfo.device_addr= 10;

	gSystCommInfo.comm_dog_time= 0;

	gSystCommInfo.pNetInfo= &gSystNetInfo;
}


/*******************************************************************************
* Function Name  : init_ctrl_params
* Description    :初始化CTRL板的校准系数，初始状态等参数
* Return         : 
*******************************************************************************/
void init_ctrl_params(void)
{


}

void InitScpiREG(void)
{	
	InitStatusVariable();
	ClearErrorQueue( );
}

void ChangeGPIBAddr(void)
{
	//停止中断
	GPIBIntOFF();
	cpd_write_reg(ADDR_PADDR, gSystCommInfo.gpib_addr);		//primary Address
	GPIBIntON();
}

