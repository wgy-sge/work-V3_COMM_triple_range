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
static UCHAR  rcv_buff_outer[SIZE_OF_OUTER_BUFFER_IN];      //RS232外端接收缓存
static u16 rcv_posi_outer = 0;                              //RS232外端接收索引
static u16 rcv_fram_outer;                                  //RS232外端收到有效帧计数

//static 
static u8  tran_buff_outer[SIZE_OF_OUTER_BUFFER_OUT];       //RS232外端发送缓存
static u16 bytes_tran_outer;                                //RS232外端发送计数 



static char  *putran_outer;


static bool EnableCount= FALSE;
static u16  Cnt= 0;

static bool SendBusying= FALSE;

//外部通讯RS232用
//定时器中断，软定时器
void Rs232TimerIrq(void)
{

}


/*******************************************************************************
* Function Name  : EnableTimerPortOuter
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EnableTimerRS232( void )
{	

}


/*******************************************************************************
* Function Name  : DisableTimerPortOuter
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DisableTimerRS232( void )
{

}


/*******************************************************************************
* Function Name  : Rs232MbCharTimeover
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool Rs232MbCharTimeover( void )
{	
	Port3SerialEnable(0, 0);	//停止接收与发送

	rcv_fram_outer = 1;//收到一帧
	return TRUE;
}

u8 gRs232Cnt= 0;
void Rs232RecvContInit(void)
{
	gRs232Cnt= 5;
}
/*******************************************************************************
* Function Name  : Rs232MbReceiveFsm
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void Rs232MbReceiveFsm( void )
{
	UCHAR           ucByte;

	// Always read the port1 character. 
	ucByte = USART_ReceiveData( USART3 );

	if (rcv_posi_outer >= SIZE_OF_OUTER_BUFFER_IN)
	{
		rcv_posi_outer= 0;
	}

	rcv_buff_outer[rcv_posi_outer]= ucByte;
	rcv_posi_outer++;	
	Rs232RecvContInit();
}

/*******************************************************************************
* Function Name  : Rs232MbReadFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool Rs232MbReadFrame( char *pdst, u16 length, u16* pcnt )
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
* Function Name  : Rs232MbDoPlus
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void Rs232MbDoPlus( void )
{    
	if (SendBusying)
	{ return; }

	Port3SerialEnable(1, 0);	//使能接收
}



/*******************************************************************************
* Function Name  : Rs232MbIsThereFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool Rs232MbIsThereFrame( void )
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
* Function Name  : Rs232MbIsTransmitFree
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool Rs232MbIsTransmitFree( void )
{	
	return TRUE;	
}


/*******************************************************************************
* Function Name  : Rs232MbTransmitFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool Rs232MbTransmitFrame( char * pframe, u16 len )
{
	bool status = TRUE;
	u16 i;

	if( len < SIZE_OF_RS232_BUFFER-2 )
	{
		for( i=0; i<len; i++ )		
			tran_buff_outer[i] = pframe[i];		

		USHORT          usCRC16;
		usCRC16 = usMBCRC16( ( UCHAR * ) pframe, len );
		tran_buff_outer[i++] = ( UCHAR )( usCRC16 & 0xFF );
		tran_buff_outer[i++] = ( UCHAR )( usCRC16 >> 8 );

		bytes_tran_outer= i;
		SendBusying= TRUE;
		Port3SerialEnable(0, 1);	//使能发送

	}
	else
		status = FALSE;

	return status;

}



/*******************************************************************************
* Function Name  : Rs232MbTransmitFsm
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void Rs232MbTransmitFsm( void )
{
	static u16 i= 0;

	if (i< bytes_tran_outer)
	{
		USART_SendData(USART3, tran_buff_outer[i]);
	}
	else
	{
		i= 0;
		bytes_tran_outer= 0;
		SendBusying= FALSE;
		Port3SerialEnable(1, 0);	//使能接收
		return;
	}
	i++;
}

