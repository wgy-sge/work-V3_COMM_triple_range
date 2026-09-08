/********************    ELoader  通讯板嵌入式程序    ********************
Copyright (C), 2010, Zidetech. Co., Ltd.
* File Name          : uart.c
* Author             : lu guangping
* Version            : V0.0.1
* Date               : 2010/9/15
* Description        : 串口驱动
*************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "include.h"

//串口1接外部串口，用于外部编程输入
//串口2接主机板，用于内部通讯

void COMPrintStr(char *pstr)
{
	u8 count= outcounter;
	while(count)
	{
		if(remote_info.uart1.tx_full!= TRUE)
		{
			putchar_uart1(*pstr);
			if (remote_info.uart1.tx_full!= TRUE)//发送失败重新发送
			{
				pstr++;
				count--;
			}			
		}
		
	}
}


void USBPrintStr(char *pstr)
{
	u8 count= outcounter;
	while(count)
	{
		if(remote_info.usb.tx_full!= TRUE)
		{
			putchar_usb(*pstr);
			if (remote_info.usb.tx_full!= TRUE)//发送失败重新发送
				pstr++;
			count--;
		}
	}
}

void putchar_uart2(u8 data )
{
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
	//if(((comm_ctrl.uart2.tx_wp+1)& (COM_BUF_SIZE-1))!= comm_ctrl.uart2.tx_rp)
	if(((comm_ctrl.uart2.tx_wp+1)%COM_BUF_SIZE)!= comm_ctrl.uart2.tx_rp)
	{		
		comm_ctrl.uart2.tx_buf[comm_ctrl.uart2.tx_wp++]= data;		
		//comm_ctrl.uart2.tx_wp&= (COM_BUF_SIZE-1);
		comm_ctrl.uart2.tx_wp%= COM_BUF_SIZE;
		//if(remote_info.uart2.tx_wp== ((remote_info.uart2.tx_rp+1)& (COM_BUF_SIZE-1)))
		//	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	}
	else
	{
		if(comm_ctrl.uart2.tx_full!= TRUE)
		{
			comm_ctrl.uart2.tx_full= TRUE;       
			//report_error(E_OUTPUTBUF_OVERFLOW);
		}
	}

	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

}
/*******************************************************************************
* Function Name  : 
* Description    : 获取一个字节数据，若没有则返回65535
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
u16 getchar_uart2(void)
{
	u16 ch;
	u8 len;
	/*if(trig_delay_cnt> 0)//延时触发不相应命令
	{
		return 65535;
	}*/
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	if(comm_ctrl.uart2.rx_rp!= comm_ctrl.uart2.rx_wp)
	{
		ch= comm_ctrl.uart2.rx_buf[comm_ctrl.uart2.rx_rp++];
		//comm_ctrl.uart2.rx_rp&=(COM_BUF_SIZE-1);
		comm_ctrl.uart2.rx_rp%=COM_BUF_SIZE;

		if (comm_ctrl.uart2.rx_full)
		{
			//消除满标志
			if( comm_ctrl.uart2.rx_wp >=comm_ctrl.uart2.rx_rp ) 
			{
				len = comm_ctrl.uart2.rx_wp -comm_ctrl.uart2.rx_rp;
			}
			else
			{
				len = comm_ctrl.uart2.rx_wp + COM_BUF_SIZE - comm_ctrl.uart2.rx_rp;
			}
			if (len< COM_BUF_SIZE-30)
			{
				comm_ctrl.uart2.rx_full= FALSE;
			}
		}
		
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		return ch;
	}
	else
	{
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		return 65535;
	}
}


/*******************************************************************************
* Function Name  : 
* Description    : COMM板与CTRL通信使用UART1，此函数用于发送数据
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
//PC通过此串口进行通讯
void putchar_uart1(u8 data )
{
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	//if(((remote_info.uart1.tx_wp+1)& (COM_BUF_SIZE-1))!= remote_info.uart1.tx_rp)
	if(((remote_info.uart1.tx_wp+1)% COM_BUF_SIZE)!= remote_info.uart1.tx_rp)
	{		
		remote_info.uart1.tx_buf[remote_info.uart1.tx_wp++]= data;		
		//remote_info.uart1.tx_wp&= (COM_BUF_SIZE-1);
		remote_info.uart1.tx_wp%= COM_BUF_SIZE;

	}
	else	
	{
		if(remote_info.uart1.tx_full!= TRUE)
		{
			remote_info.uart1.tx_full= TRUE;       
			//report_error(E_OUTPUTBUF_OVERFLOW);
		}
	}

	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

}

/*******************************************************************************
* Function Name  : 
* Description    : 获取一个字节数据，若没有则返回65535
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
u16 getchar_uart1(void)
{
	u16 ch;
	u8 len;

	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	if(remote_info.uart1.rx_rp!=remote_info.uart1.rx_wp)
	{
		ch=remote_info.uart1.rx_buf[remote_info.uart1.rx_rp++];
		//remote_info.uart1.rx_rp&=(COM_BUF_SIZE-1);	
		remote_info.uart1.rx_rp%=COM_BUF_SIZE;	

		if (remote_info.uart1.rx_full)
		{
			//消除满标志
			if( remote_info.uart1.rx_wp >=remote_info.uart1.rx_rp ) //循环BUF计算
			{
				len = remote_info.uart1.rx_wp -remote_info.uart1.rx_rp;
			}
			else
			{
				len = remote_info.uart1.rx_wp + COM_BUF_SIZE - remote_info.uart1.rx_rp;
			}

			if (len< COM_BUF_SIZE-30)//如果满了，则要等腾出30个空位后再清空标志
			{
				remote_info.uart1.rx_full= FALSE;
			}
		}

		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		return ch;
	}
	else
	{
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		return 65535;
	}
}


/*******************************************************************************
* Function Name  : 
* Description    : COMM板与CTRL通信使用UART1，此函数用于发送数据
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
//PC通过此串口进行通讯
void putchar_usb(u8 data )
{
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	//if(((remote_info.usb.tx_wp+1)& (COM_BUF_SIZE-1))!= remote_info.usb.tx_rp)
	if(((remote_info.usb.tx_wp+1)% COM_BUF_SIZE)!= remote_info.usb.tx_rp)
	{		
		remote_info.usb.tx_buf[remote_info.usb.tx_wp++]= data;		
		//remote_info.usb.tx_wp&= (COM_BUF_SIZE-1);
		remote_info.usb.tx_wp%= COM_BUF_SIZE;

	}
	else	
	{
		if(remote_info.usb.tx_full!= TRUE)
		{
			remote_info.usb.tx_full= TRUE;       
			//report_error(E_OUTPUTBUF_OVERFLOW);
		}
	}

	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

}

/*******************************************************************************
* Function Name  : 
* Description    : 获取一个字节数据，若没有则返回65535
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
u16 getchar_usb(void)
{
	u16 ch;
	u8 len;

	USART_ITConfig( USART3, USART_IT_RXNE, DISABLE );
	if( remote_info.usb.rx_rp!= remote_info.usb.rx_wp )
	{
		ch= remote_info.usb.rx_buf[remote_info.usb.rx_rp++];
		//remote_info.usb.rx_rp&=(COM_BUF_SIZE-1);	
		remote_info.usb.rx_rp%= COM_BUF_SIZE;

		if (remote_info.usb.rx_full)
		{
			//消除满标志
			if( remote_info.usb.rx_wp >=remote_info.usb.rx_rp ) //循环BUF计算
			{
				len = remote_info.usb.rx_wp -remote_info.usb.rx_rp;
			}
			else
			{
				len = remote_info.usb.rx_wp + COM_BUF_SIZE - remote_info.usb.rx_rp;
			}

			if (len< COM_BUF_SIZE-30)//如果满了，则要等腾出30个空位后再清空标志
			{
				remote_info.usb.rx_full= FALSE;
			}
		}

		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		return ch;
	}
	else
	{
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		return 65535;
	}
}
