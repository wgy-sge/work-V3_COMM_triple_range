/******************** (C) COPYRIGHT 2010 Zidetech *****************************
* File Name          : port_serial.c
* Author             : xxx
* Version            : V1.0
* Date               : 2010.06.21
* Description        : 串口操作函数
********************************************************************************
* Log :
2009.06.21 xxx, initial version
*******************************************************************************/
#include "fmb.h"



/*---------------------------- Functions -------------------------------------*/

/*******************************************************************************
* Function Name  : xMBPortSerialInit
* Description    : 初始化串口参数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
BOOL xMBPortSerialInit( void )
{
    USART_InitTypeDef USART_InitStructure;
    
    USART_InitStructure.USART_BaudRate = MODBUS_BAUD;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    
    USART_ITConfig(USART2, USART_IT_TC, DISABLE);
    USART_ITConfig(USART2, USART_IT_ERR, DISABLE);
                          
    USART_Cmd(USART2, ENABLE);
    
    return TRUE;
}


/*******************************************************************************
* Function Name  : vMBPortSerialEnable
* Description    : 配置指定串口的接收和发送功能
* Input          : xRxEnable--接收中断使能
                   xTxEnable--发送中断使能
* Output         : None
* Return         : None
*******************************************************************************/
void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    if( xRxEnable )
    {
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);    
    }
    else
    {
        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 
    }
    
    if( xTxEnable )
    {
        USART_ITConfig(USART2, USART_IT_TXE, ENABLE);    
    }
    else
    {
        USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    }
    
}






