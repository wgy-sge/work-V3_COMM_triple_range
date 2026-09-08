/******************** (C) COPYRIGHT 2016 Faith *********************************
* File Name          : port_serial.c
* Author             : xxx
* Version            : V1.0
* Date               : 2016.11.14
* Description        : 与硬件相关的串口操作
********************************************************************************
* Log :

*******************************************************************************/
#include "include.h"



/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern UCHAR rcv_buff_outer[SIZE_OF_OUTER_BUFFER_IN];
extern UCHAR tran_buff_outer[SIZE_OF_OUTER_BUFFER_OUT];



/*---------------------------- Functions -------------------------------------*/


/*******************************************************************************
* Function Name  : PortSerialEnable
* Description    : 使能或失能串口接收功能和发送功能
* Input          : xRxEnable--接收功能使能
xTxEnable--发送功能使能
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void Port1SerialEnable( bool xRxEnable, bool xTxEnable )
{
	if( xRxEnable )
	{	
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//使能接收
	}
	else
	{
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);	//关闭接收	

	}

	if( xTxEnable )
	{
		
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//使能发送  
	}
	else
	{
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);//关闭发送  

	}

}


/*******************************************************************************
* Function Name  : PortSerialGetByte
* Description    : 从串口接收一个字节
* Input          : 接收字节的保存地址
* Output         : None
* Return         : None
*******************************************************************************/
void Port1SerialGetByte( CHAR * pucByte )
{   
	*pucByte = USART1->DR;
}

/*******************************************************************************
* Function Name  : PortSerialPutByte
* Description    : 发送一个字节
* Input          : 待发送的字节
* Output         : None
* Return         : None
*******************************************************************************/
void Port1SerialPutByte( CHAR ucByte )
{
	USART1->DR = ucByte;
}


