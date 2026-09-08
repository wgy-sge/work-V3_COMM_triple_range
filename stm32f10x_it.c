/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V2.0.3
* Date               : 09/22/2008
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "include.h"
u8 gpib_holdon= 0;

volatile u16 scroll_cnt= 0;

volatile u8 rs232_parityerr1= 0;
volatile u8 parity_cnt1= 0;


volatile u8 rs232_parityerr2= 0;
volatile u8 parity_cnt2= 0;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NMIException
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMIException(void)
{
}

/*******************************************************************************
* Function Name  : HardFaultException
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFaultException(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

/*******************************************************************************
* Function Name  : MemManageException
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManageException(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

/*******************************************************************************
* Function Name  : BusFaultException
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFaultException(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

/*******************************************************************************
* Function Name  : UsageFaultException
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFaultException(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMonitor(void)
{
}

/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVCHandler(void)
{
}

/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSVC(void)
{
}

extern u8 gRs232Cnt;
extern u8 gRs485Cnt;
extern WORD volatile gTimCnt;

u8 gRuning_flg= 0;
/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.

设计 为 1ms 进一次  中断 
*******************************************************************************/
#define  TIME_SCROLL_DELAY   20
u32 gDelayTick= 0;
u16 gLedTick= 0;

void RefLedStatus(void)
{
	gLedTick= 0;
	if (gRuning_flg)
	{
		gRuning_flg= 0;		
	} 
	else
	{
		gRuning_flg= 1;		
	}
}


void SysTickHandler(void)
{
	if (gDelayTick)
	{
		gDelayTick--;
	}

	gLedTick++;
	if (gLedTick> 1500)
	{
		RefLedStatus( );
	}

	if (rs232_parityerr2== 1)
	{
		rs232_parityerr2= 2;	
		parity_cnt2= 0;
	}
	else if (rs232_parityerr2== 2)
	{
		parity_cnt2++;
		if (parity_cnt2> (30000/USART_InitStructure.USART_BaudRate))//大于三个字节时间时认为单次输入完毕
		{
			//report_error(E_RS232_PARITY);
			rs232_parityerr2= 0;
			parity_cnt2= 0;
		}

	}

	if ( gRs232Cnt> 0)
	{
		gRs232Cnt--;
		if ( gRs232Cnt== 1 )
		{
			Rs232MbCharTimeover();
		}
	}

	if ( gRs485Cnt> 0)
	{
		gRs485Cnt--;
		if ( gRs485Cnt== 1 )
		{
			Rs485MbCharTimeover();
		}
	}

	//通讯超时计时器
	if( ticks_modbus_timeout > 0 )
	{
		ticks_modbus_timeout --;

		if( ticks_modbus_timeout == 0 )
		{
			//产生通讯超时事件
			post_port_event( EVENT_TIMEOUT ); 
		}
	}


	if (gInnerCommFrameTicks)
	{
		gInnerCommFrameTicks--;

		if (gInnerCommFrameTicks== 0)
		{
			port1_t35_expired( );
		}
	}

		gTimCnt++;
}

/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : This function handles WWDG interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : PVD_IRQHandler
* Description    : This function handles PVD interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PVD_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TAMPER_IRQHandler
* Description    : This function handles Tamper interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TAMPER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : FLASH_IRQHandler
* Description    : This function handles Flash interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RCC_IRQHandler
* Description    : This function handles RCC interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External interrupt Line 0 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
	u8 int_vec;
	u8 cnt;
	u8 tmp;
	u8 ch;

	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		// Toggle GPIO_LED pin 6 
		//GPIO_WriteBit(GPIO_LED, GPIO_Pin_6, (BitAction)((1-GPIO_ReadOutputDataBit(GPIO_LED, GPIO_Pin_6))));
		// Clear the Key Button EXTI line pending bit 
		int_vec=cpd_read_reg(ADDR_INT_VEC);
		//P2IFG = 0x00;         //清空中断标志寄存器
		switch(int_vec)
		{
		case VEC_IFC:

			break;
		case VEC_DCLR:
			remote_info.gpib.tx_rp=remote_info.gpib.tx_wp;	//清除发送缓冲区
			remote_info.gpib.tx_full=FALSE;
			break;
		case VEC_DTRIG:
			//scpi_com_trg();		
			break;
		case VEC_REM:
			tmp=cpd_read_reg(ADDR_STAT0);
			if(tmp&0x08)
			{//remote
				//if((sys_info.cal_state== ON)&&(sys_info.local_remote== LOCAL))
				{
					//ReportError(ERR_EXECUTION_ERROR);
				}
				//else if(sys_info.local_remote== LOCAL)
				{					
					//ReturnToRemote();
				}
			}
			else 
			{//local
				//if(sys_info.local_remote== REMOTE)
				//returntolocal();					
			}
			break;
		case VEC_TXC:
			cnt=cpd_read_reg(ADDR_STAT1) >>4;		//idle tx buffer
			tmp=remote_info.gpib.tx_rp;
			while((tmp!=remote_info.gpib.tx_wp)&&(cnt!=0))
			//while((tmp!=remote_info.gpib.tx_wp)&&(cnt> 6))
			{
				cpd_write_reg(ADDR_OUT_Q, remote_info.gpib.tx_buf[tmp]);
				//tmp=(tmp+1)&(COM_BUF_SIZE-1);
				tmp=(tmp+1)%COM_BUF_SIZE;
				cnt--;
			}
			remote_info.gpib.tx_rp=tmp;
			if(remote_info.gpib.tx_rp!=remote_info.gpib.tx_wp)
			{
				cpd_write_reg(ADDR_LOC_MSG, 0x1c+0x80);
			}
			else//若MCU缓冲区空，则清除mav位
			{			
				cpd_write_reg(ADDR_LOC_MSG,0x1c+0x00);	//mav off
			}

			remote_info.gpib.tx_full= FALSE;
			break;
		case VEC_RDY:
			cnt=cpd_read_reg(ADDR_STAT1) & 0x0f;	//ready data count
			tmp=remote_info.gpib.rx_wp;
			//while( ( ( (tmp+1)&(COM_BUF_SIZE-1) ) != remote_info.gpib.rx_rp ) && (cnt!=0) )
			while( ( ( (tmp+1)%COM_BUF_SIZE ) != remote_info.gpib.rx_rp ) && (cnt!=0) )
			{
				ch=cpd_read_reg(ADDR_IN_Q);
				cnt--;
						
					remote_info.gpib.rx_buf[tmp]=ch&0x7f;
					//tmp=(tmp+1)&(COM_BUF_SIZE-1);
					tmp=(tmp+1)%COM_BUF_SIZE;

					if(ch&0x80)
					{
						remote_info.gpib.rx_buf[tmp]=0x0a;
						//tmp=(tmp+1)&(COM_BUF_SIZE-1);
						tmp=(tmp+1)%COM_BUF_SIZE;

					}				
			}

			remote_info.gpib.rx_wp=tmp;

			//RFD HOLDON/OFF检测
			if( remote_info.gpib.rx_wp >= remote_info.gpib.rx_rp ) 
			{
				tmp = remote_info.gpib.rx_wp - remote_info.gpib.rx_rp;
			}
			else
			{
				tmp = remote_info.gpib.rx_wp + COM_BUF_SIZE - remote_info.gpib.rx_rp;
			}

			if( tmp > ((COM_BUF_SIZE*7)/8) ) 
			{
				if( !gpib_holdon ) 	
				{
					cpd_write_reg(ADDR_LOC_MSG, 0x80|GPIB_MSG_HOLD);
					gpib_holdon = 1;	
					remote_info.gpib.rx_full= TRUE;//输入溢出
					remote_info.gpib.rx_err++;

				}
			}
			break;
		}

		//clear FT488D interrupt
		cpd_write_reg(ADDR_INT_VEC, 0);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}

}

/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External interrupt Line 1 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : This function handles External interrupt Line 2 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : This function handles External interrupt Line 4 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : This function handles DMA1 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel3_IRQHandler
* Description    : This function handles DMA1 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel4_IRQHandler
* Description    : This function handles DMA1 Channel 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel4_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : DMA1_Channel5_IRQHandler
* Description    : This function handles DMA1 Channel 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel6_IRQHandler
* Description    : This function handles DMA1 Channel 6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel7_IRQHandler
* Description    : This function handles DMA1 Channel 7 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC1_2_IRQHandler
* Description    : This function handles ADC1 and ADC2 global interrupts requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC1_2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_HP_CAN_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN_TX_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN_RX0_IRQHandler(void)
{
	can_rx_irq( );
}

/*******************************************************************************
* Function Name  : CAN_RX1_IRQHandler
* Description    : This function handles CAN RX1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_RX1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN_SCE_IRQHandler
* Description    : This function handles CAN SCE interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_SCE_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	/*
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{		//AEN
	key_pressed=TRUE;
	cur_key=KEY_SCR_L;
	cur_kclass=KEYC_SCROLL;	
	}	
	else if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{		//BEN
	key_pressed=TRUE;
	cur_key=KEY_SCR_R;
	cur_kclass=KEYC_SCROLL;
	}
	//停止中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_Init(&EXTI_InitStructure);
	// Disable the EXTI9_5 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	//清除Pending的中断
	EXTI_ClearITPendingBit(EXTI_Line5);	
	EXTI_ClearITPendingBit(EXTI_Line6);
	//重置计数器，20ms后重新开启中断
	stop_scroll_int_cnt=0;
	*/
}

/*******************************************************************************
* Function Name  : TIM1_BRK_IRQHandler
* Description    : This function handles TIM1 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : This function handles TIM1 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_TRG_COM_IRQHandler
* Description    : This function handles TIM1 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_CC_IRQHandler
* Description    : This function handles TIM1 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 

	}
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
		//port1_t35_expired( );	

	}
}

/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	sys_timer_Irq();
}

/*******************************************************************************
* Function Name  : I2C1_EV_IRQHandler
* Description    : This function handles I2C1 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_ER_IRQHandler
* Description    : This function handles I2C1 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_ER_IRQHandler
* Description    : This function handles I2C2 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI1_IRQHandler
* Description    : This function handles SPI1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI2_IRQHandler
* Description    : This function handles SPI2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_IRQHandler(void)
{
}





/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
每100ms 接受 一次 主控单片机 传过来的数据
数据 为 3个 通道的  回读 电压 和电流值 与 状态 cc or cv
*******************************************************************************/
ITStatus  ErrorItStatus;

void USART1_IRQHandler(void)
{  
	u32 temp;

	if( gDevice_CommProtocol == PROTOCOL_MODBUS )//MODBUS
	{
		//接收 
		if((ErrorItStatus=USART_GetITStatus(USART1, USART_IT_RXNE)) != RESET)
		{	
			USART_ClearITPendingBit( USART1,USART_IT_RXNE );
			if(USART_GetFlagStatus(USART1, USART_FLAG_PE )== RESET)//奇偶校验错误判断
			{
				if (rs232_parityerr1)//发生奇偶校验错误则整个语句都将放弃
				{
					goto pe_err0;
				}
				// Read one byte from the receive data register 
				Rs485MbReceiveFsm( );	
			}
			else//若发生奇偶校验错误则只读出数据，不处理
			{
pe_err0:	USART_ClearFlag( USART1, USART_FLAG_PE );
				USART_ReceiveData(USART1);			
				rs232_parityerr1= 1;
			}				
		}

		//发送
		if((ErrorItStatus=USART_GetITStatus(USART1, USART_IT_TXE)) != RESET)
		{   	
			USART_ClearITPendingBit( USART1,USART_IT_TXE );
			Rs485MbTransmitFsm( );
		}

		//溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
		if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
		{
			USART_ClearFlag(USART1,USART_FLAG_ORE);    //读SR
			USART_ReceiveData(USART1);                //读DR
		}

	
	} 
	else//SCPI
	{
		//接收 
		if((ErrorItStatus=USART_GetITStatus(USART1, USART_IT_RXNE)) != RESET)
		{	
			USART_ClearITPendingBit( USART1,USART_IT_RXNE );	
			if(USART_GetFlagStatus(USART1, USART_FLAG_PE )== RESET)//奇偶校验错误判断
			{
				if (rs232_parityerr1)//发生奇偶校验错误则整个语句都将放弃
				{
					goto pe_err;
				}							
					// Read one byte from the receive data register 
					if (remote_info.uart1.rx_full== FALSE)
					{
						//if(((remote_info.uart1.rx_wp+1)&(COM_BUF_SIZE-1)) != remote_info.uart1.rx_rp)
						if(((remote_info.uart1.rx_wp+1)%COM_BUF_SIZE) != remote_info.uart1.rx_rp)
						{
							remote_info.uart1.rx_buf[remote_info.uart1.rx_wp++]= USART_ReceiveData(USART1); //加缓冲区是否满判断    
							//remote_info.uart1.rx_wp&=(COM_BUF_SIZE-1);	
							remote_info.uart1.rx_wp%=COM_BUF_SIZE;	
						}
						else
						{
							if (remote_info.uart1.rx_full== FALSE)//输入溢出
							{
								remote_info.uart1.rx_full= TRUE;
								remote_info.uart1.rx_err++;//输入缓冲溢出错误计数
							}

							USART_ReceiveData(USART1);//输入溢出抛弃数据
						}
					}
					else
						USART_ReceiveData(USART1);//输入溢出抛弃数据
				
			}
			else//若发生奇偶校验错误则只读出数据，不处理
			{
				USART_ClearFlag( USART1, USART_FLAG_PE );
pe_err:		USART_ReceiveData(USART1);			
				rs232_parityerr1= 1;

			}

		}

		//发送
		if((ErrorItStatus=USART_GetITStatus(USART1, USART_IT_TXE)) != RESET)
		{   	
			USART_ClearITPendingBit( USART1,USART_IT_TXE );
			if(remote_info.uart1.tx_wp!= remote_info.uart1.tx_rp)
			{
				USART_SendData(USART1, remote_info.uart1.tx_buf[remote_info.uart1.tx_rp++]);			
				//remote_info.uart1.tx_rp&= (COM_BUF_SIZE-1);
				remote_info.uart1.tx_rp%= COM_BUF_SIZE;
				if(remote_info.uart1.tx_full== TRUE)
				{
					remote_info.uart1.tx_full= FALSE;  
				}
			}
			else
			{
				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			}
		}



		//溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
		if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
		{
			USART_ClearFlag(USART1,USART_FLAG_ORE);    //读SR
			USART_ReceiveData(USART1);                //读DR
		}
	}


}





/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
这个内部通讯用 ，非标类MODBUS协议
*******************************************************************************/
void USART2_IRQHandler(void)
{	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit( USART2,USART_IT_RXNE );
		if( USART_GetFlagStatus( USART2,USART_FLAG_PE ) == RESET )
		{
			port1_receive_fsm(  );
		}
		else
		{
			//等待RXNE标志位置1
			//while( USART_GetFlagStatus( USART2,USART_FLAG_RXNE ) == RESET ) ;
			//清除USART_FLAG_PE标志
			USART_ReceiveData(USART2);
		}
	}

	if( USART_GetITStatus(USART2, USART_IT_TXE ) != RESET )
	{   	
		USART_ClearITPendingBit( USART2,USART_IT_TXE );
		port1_transmit_fsm( );
	}	

}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
这个 usb的 中断 ，主要用来解析 scpi
*******************************************************************************/
void USART3_IRQHandler(void)
{
	u32 temp;

	if( gDevice_CommProtocol == PROTOCOL_MODBUS )//MODBUS
	{
		//接收 
		if((ErrorItStatus=USART_GetITStatus(USART3, USART_IT_RXNE)) != RESET)
		{	
			USART_ClearITPendingBit( USART3,USART_IT_RXNE );
			if(USART_GetFlagStatus(USART3, USART_FLAG_PE )== RESET)//奇偶校验错误判断
			{
				if (rs232_parityerr2)//发生奇偶校验错误则整个语句都将放弃
				{
					goto pe_err1;
				}
				// Read one byte from the receive data register 
				Rs232MbReceiveFsm( );		
			}
			else//若发生奇偶校验错误则只读出数据，不处理
			{
pe_err1:	USART_ClearFlag( USART3, USART_FLAG_PE );
				USART_ReceiveData(USART3);			
				rs232_parityerr2= 1;
			} 
				
		}

		//发送
		if((ErrorItStatus=USART_GetITStatus(USART3, USART_IT_TXE)) != RESET)
		{   	
			USART_ClearITPendingBit( USART3,USART_IT_TXE );
			Rs232MbTransmitFsm( );
		}

		//溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
		if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
		{
			USART_ClearFlag( USART3, USART_FLAG_ORE );    //读SR
			USART_ReceiveData( USART3 );                  //读DR
		}
	} 
	else//SCPI
	{
		//接收 
		if((ErrorItStatus=USART_GetITStatus(USART3, USART_IT_RXNE)) != RESET)
		{	
			USART_ClearITPendingBit( USART3,USART_IT_RXNE );	
			if(USART_GetFlagStatus(USART3, USART_FLAG_PE )== RESET)//奇偶校验错误判断
			{
				if (rs232_parityerr2)//发生奇偶校验错误则整个语句都将放弃
				{
					goto pe_err;
				}
				// Read one byte from the receive data register 
				if (remote_info.usb.rx_full== FALSE)
				{
					//if(((remote_info.usb.rx_wp+1)&(COM_BUF_SIZE-1)) != remote_info.usb.rx_rp)
					if(((remote_info.usb.rx_wp+1)%COM_BUF_SIZE) != remote_info.usb.rx_rp)
					{
						remote_info.usb.rx_buf[remote_info.usb.rx_wp++] = USART_ReceiveData(USART3); //加缓冲区是否满判断    
						//remote_info.usb.rx_wp&= (COM_BUF_SIZE-1);	
						remote_info.usb.rx_wp%= COM_BUF_SIZE;	
					}
					else
					{
						if (remote_info.usb.rx_full== FALSE)//输入溢出
						{
							remote_info.usb.rx_full= TRUE;
							remote_info.usb.rx_err++;//输入缓冲溢出错误计数
						}

						USART_ReceiveData(USART3);//输入溢出抛弃数据
					}
				}
				else
					USART_ReceiveData(USART3);//输入溢出抛弃数据
			}
			else//若发生奇偶校验错误则只读出数据，不处理
			{
				USART_ClearFlag( USART3, USART_FLAG_PE );
pe_err:		    USART_ReceiveData(USART3);			
				rs232_parityerr2= 1;
			}
		}

		//发送
		if((ErrorItStatus=USART_GetITStatus(USART3, USART_IT_TXE)) != RESET)
		{   	
			USART_ClearITPendingBit( USART3, USART_IT_TXE );
			if(remote_info.usb.tx_wp!= remote_info.usb.tx_rp)
			{
				USART_SendData(USART3, remote_info.usb.tx_buf[remote_info.usb.tx_rp++]);			
				//remote_info.usb.tx_rp&= (COM_BUF_SIZE-1);
				remote_info.usb.tx_rp%= COM_BUF_SIZE;
				if(remote_info.usb.tx_full== TRUE)
				{
					remote_info.usb.tx_full= FALSE;  
				}
			}
			else
			{
				USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
			}
		}

		//溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
		if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
		{
			USART_ClearFlag(USART3,USART_FLAG_ORE);    //读SR
			USART_ReceiveData(USART3);                 //读DR
		}
	}

}

/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USBWakeUp_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBWakeUp_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_BRK_IRQHandler
* Description    : This function handles TIM8 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_UP_IRQHandler
* Description    : This function handles TIM8 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_UP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_TRG_COM_IRQHandler
* Description    : This function handles TIM8 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_CC_IRQHandler
* Description    : This function handles TIM8 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC3_IRQHandler
* Description    : This function handles ADC3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : FSMC_IRQHandler
* Description    : This function handles FSMC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : This function handles TIM5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI3_IRQHandler
* Description    : This function handles SPI3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : This function handles UART5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM6_IRQHandler
* Description    : This function handles TIM6 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM7_IRQHandler
* Description    : This function handles TIM7 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel1_IRQHandler
* Description    : This function handles DMA2 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel2_IRQHandler
* Description    : This function handles DMA2 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel3_IRQHandler
* Description    : This function handles DMA2 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel4_5_IRQHandler
* Description    : This function handles DMA2 Channel 4 and DMA2 Channel 5
*                  interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel4_5_IRQHandler(void)
{
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
