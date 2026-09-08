/******************** (C) COPYRIGHT 2015 Zidetech *****************************
* File Name          : uvmb_rs232.c
* Author             : xxx
* Version            : V1.0
* Date               : 2015.7.29
* Description        : 
********************************************************************************
* Log :

*******************************************************************************/
#include "include.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
//static 
static u8   rcv_buff_outer[SIZE_OF_OUTER_BUFFER_IN];         //RS485外端接收缓存
static u16  rcv_posi_outer = 0;                              //RS485外端接收索引
static u16  rcv_fram_outer;                                  //RS485外端收到有效帧计数

//static 
static u8   tran_buff_outer[SIZE_OF_OUTER_BUFFER_OUT];       //RS485外端发送缓存
static u16  bytes_tran_outer;                                //RS485外端发送计数 

static char *putran_outer;
static bool EnableCount= FALSE;
static u16  Cnt= 0;

static bool SendBusying= FALSE;

//外部通讯RS485用
//定时器中断，软定时器
void Rs485TimerIrq(void)
{

}


/*******************************************************************************
* Function Name  : EnableTimerPortOuter
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EnableTimerRS485( void )
{	

}


/*******************************************************************************
* Function Name  : DisableTimerPortOuter
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DisableTimerRS485( void )
{

}


/*******************************************************************************
* Function Name  : Rs485MbCharTimeover
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool Rs485MbCharTimeover( void )
{	
	Port1SerialEnable(0, 0);	//停止接收与发送

	rcv_fram_outer = 1;//收到一帧
	return TRUE;
}


u8 gRs485Cnt= 0;
void Rs485RecvContInit(void)
{
	gRs485Cnt= 5;
}

/*******************************************************************************
* Function Name  : Rs485MbReceiveFsm
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void Rs485MbReceiveFsm( void )
{
	UCHAR           ucByte;

	// Always read the port1 character. 
	ucByte = USART_ReceiveData( USART1 );

	if (rcv_posi_outer >= SIZE_OF_OUTER_BUFFER_IN)
	{
		rcv_posi_outer= 0;
	}

	rcv_buff_outer[rcv_posi_outer]= ucByte;
	rcv_posi_outer++;	
	Rs485RecvContInit();
}

/*******************************************************************************
* Function Name  : Rs485MbReadFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool Rs485MbReadFrame( char *pdst, u16 length, u16* pcnt )
{
	u16 i,cnt = 0;

	//读走接收帧
	rcv_fram_outer = 0; 

	if( (rcv_posi_outer >= MB_SER_PDU_SIZE_MIN) &&
		(usMBCRC16(( UCHAR * )rcv_buff_outer, rcv_posi_outer ) == 0) )
	{
		rcv_posi_outer -= MB_SER_PDU_SIZE_CRC;
		for( i=0; i< rcv_posi_outer; i++ )
		{
			if( cnt >= length )
			{
				rcv_posi_outer= 0;
				return FALSE;
			}
			*pdst++ = rcv_buff_outer[i]; 
			cnt ++;
		}	
		*pcnt = cnt ;

		rcv_posi_outer= 0;
		return TRUE;
	}
	else
	{
		rcv_posi_outer= 0;
		return FALSE;
	}
}

/*******************************************************************************
* Function Name  : Rs485MbDoPlus
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void Rs485MbDoPlus( void )
{    
	if (SendBusying)
	{
		return;
	}

	Port1SerialEnable(1, 0);	//使能接收
}



/*******************************************************************************
* Function Name  : Rs485MbIsThereFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool Rs485MbIsThereFrame( void )
{
	if( rcv_fram_outer > 0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



/*******************************************************************************
* Function Name  : Rs485MbIsTransmitFree
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool Rs485MbIsTransmitFree( void )
{

	return TRUE;

}





/*******************************************************************************
* Function Name  : Rs485MbTransmitFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool Rs485MbTransmitFrame( char * pframe, u16 len )
{
	bool status = TRUE;
	u16 i;

	if( len < SIZE_OF_RS232_BUFFER-2 )
	{
		for( i=0; i<len; i++ )
		{
			tran_buff_outer[i] = pframe[i];
		}

		USHORT  usCRC16;
		usCRC16 = usMBCRC16( ( UCHAR * ) pframe, len );
		tran_buff_outer[i++] = ( UCHAR )( usCRC16 & 0xFF );
		tran_buff_outer[i++] = ( UCHAR )( usCRC16 >> 8 );

		bytes_tran_outer= i;
		SendBusying= TRUE;
		Port1SerialEnable(0, 1);	//使能发送

	}
	else
		status = FALSE;

	return status;

}



/*******************************************************************************
* Function Name  : Rs485MbTransmitFsm
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void Rs485MbTransmitFsm( void )
{
	static u16 i= 0;

	if (i< bytes_tran_outer)
	{
		USART_SendData(USART1, tran_buff_outer[i]);
	}
	else
	{
		i= 0;
		bytes_tran_outer= 0;
		SendBusying= FALSE;
		Port1SerialEnable(1, 0);	//使能接收
		return;
	}
	i++;
}

