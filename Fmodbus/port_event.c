/******************** (C) COPYRIGHT 2010 Zidetech *****************************
* File Name          : port_event.c
* Author             : xxx
* Version            : V1.0
* Date               : 2010.06.21
* Description        : 事件操作函数
********************************************************************************
* Log :
2009.06.21 xxx, initial version
*******************************************************************************/
#include "fmb.h"


/* ----------------------- Variables ----------------------------------------*/
static UCHAR ev_rece_fifo[MODBUS_EVENT_FIFO_LENGTH];       //接收事件FIFO
static UCHAR ev_fifo_head;                                 //FIFO的队首位置
static UCHAR ev_fifo_wposi;                                //FIFO的入队位置
static UCHAR ev_fifo_length;                               //FIFO的长度




/*---------------------------- Functions -------------------------------------*/

/*******************************************************************************
* Function Name  : init_ev_queue
* Description    : 初始化接收事件队列
* Input          : None
* Output         : None
* Return         : errcode
*******************************************************************************/
BOOL init_ev_queue( void )
{
    ev_fifo_wposi = 0;
    ev_fifo_length = 0;
    ev_fifo_head = 0;
    
    return TRUE;
}

/*******************************************************************************
* Function Name  : post_port_event
* Description    : 入队
* Input          : port_num--通道编号
* Output         : None
* Return         : errcode
*******************************************************************************/
BOOL post_port_event( UCHAR port_num )
{
    BOOL    status = FALSE;
    
    if( ev_fifo_length < MODBUS_EVENT_FIFO_LENGTH )
    {
        ev_rece_fifo[ev_fifo_wposi] = port_num;
        ev_fifo_wposi = (ev_fifo_wposi+1) % MODBUS_EVENT_FIFO_LENGTH;   
        ev_fifo_length ++;
        
        status = TRUE;
    }
    
    return status;
}

/*******************************************************************************
* Function Name  : get_port_event
* Description    : 出队
* Input          : pevent--事件目的地址
* Output         : None
* Return         : errcode
*******************************************************************************/
BOOL get_port_event( UCHAR * pevent )
{
    BOOL    status = FALSE;

    if( ev_fifo_length > 0 )
    {
        *pevent = ev_rece_fifo[ev_fifo_head];
        ev_fifo_head = (ev_fifo_head+1) % MODBUS_EVENT_FIFO_LENGTH;
        ev_fifo_length --;
        
        status = TRUE;
    }
    
    return status;
}
