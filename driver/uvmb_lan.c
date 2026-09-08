/******************** (C) COPYRIGHT 2015 Zidetech *****************************
* File Name          : uvmb_lan.c
* Author             : xxx
* Version            : V1.0
* Date               : 2015.7.30
* Description        : 
********************************************************************************
* Log :

*******************************************************************************/
#include "include.h"
#include "socket_app.h"

#define MB_SER_PDU_SIZE_MIN     4       
#define MB_SER_PDU_SIZE_CRC     2     


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static char  rcv_buff_lan[SIZE_OF_LAN_BUFFER];            //网口接收缓存
static u16 rcv_fram_lan;                                  //收到帧计数
static u8  state_rcv_lan = STATE_RX_IDLE;                 //LAN模仿接收状态
static u16 rcv_posi_lan = 0;                              //LAN模仿接收索引

static char  tran_buff_lan[SIZE_OF_LAN_BUFFER+3];         //网口发送缓存




/*******************************************************************************
* Function Name  : LanMbDetectFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void LanMbDetectFrame( void )
{
	u16 i;

	rcv_posi_lan = SocketUdpReadData( (u8*)rcv_buff_lan, SIZE_OF_LAN_BUFFER );


	if( gDevice_CommProtocol == PROTOCOL_MODBUS )//MODBUS
	{		
		if( rcv_posi_lan > 0 )
		{
			rcv_fram_lan = 1;
		}
	}
	else
	{
		//@lgp,2021.07.26  2.00.13  SCPI解析时收到不可见字符则丢弃该帧
		for (i= 0; i< rcv_posi_lan; i++)
		{
			if (rcv_buff_lan[i]>= 128)
			{
				return;
			}
		}

		for (i= 0; i< rcv_posi_lan; i++)
		{
			// Read one byte from the receive data register 
			if (remote_info.lan.rx_full== FALSE)
			{				
				if(((remote_info.lan.rx_wp+1)%COM_BUF_SIZE) != remote_info.lan.rx_rp)
				{
					remote_info.lan.rx_buf[remote_info.lan.rx_wp++]= rcv_buff_lan[i]; //加缓冲区是否满判断    
						
					remote_info.lan.rx_wp%=COM_BUF_SIZE;	
				}
				else
				{
					if (remote_info.lan.rx_full== FALSE)//输入溢出
					{
						remote_info.lan.rx_full= TRUE;
						remote_info.lan.rx_err++;//输入缓冲溢出错误计数
					}				
				}
			}
			else
				break;//输入溢出抛弃数据
		}
	}

}


/*******************************************************************************
* Function Name  : LanMbReadFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool LanMbReadFrame( char *pdst, u16 length, u16* pcnt )
{
	u16 i,cnt = 0;

	//读走接收帧
	rcv_fram_lan = 0; 	
		if( (rcv_posi_lan >= MB_SER_PDU_SIZE_MIN) &&
			(usMBCRC16(( UCHAR * )rcv_buff_lan, rcv_posi_lan ) == 0) )
		{
			rcv_posi_lan -= MB_SER_PDU_SIZE_CRC;
			for( i=0; i< rcv_posi_lan; i++ )
			{
				if( cnt >= length )
				{
					return FALSE;
				}
				*pdst++ = rcv_buff_lan[i]; 
				cnt ++;
			}	
			*pcnt = cnt ;

			return TRUE;
		}
		else
		{
			return FALSE;
		}
}

/*******************************************************************************
* Function Name  : LanMbDoPlus
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void LanMbDoPlus( void )
{
	return;			
}

/*******************************************************************************
* Function Name  : LanMbIsThereFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool LanMbIsThereFrame( void )
{
	if( rcv_fram_lan > 0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*******************************************************************************
* Function Name  : Rs232IsSendFree
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool LanMbIsTransmitFree( void )
{
	//假定能发送
	if( 1 ) 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*******************************************************************************
* Function Name  : LanMbTransmitFrame
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool LanMbTransmitFrame( char * pframe, u16 len )
{
	bool status = TRUE;
	u16 i;

	if( len < SIZE_OF_RS232_BUFFER-2 )
	{
		for( i=0; i<len; i++ )
		{
			tran_buff_lan[i] = pframe[i];
		}

		USHORT          usCRC16;
		usCRC16 = usMBCRC16( ( UCHAR * ) pframe, len );
		tran_buff_lan[i++] = ( UCHAR )( usCRC16 & 0xFF );
		tran_buff_lan[i++] = ( UCHAR )( usCRC16 >> 8 );
		

		len+= 2;
		if( SocketUdpWriteData((u8*)tran_buff_lan, len) != len )
		{
			status = FALSE;
		}		   
	}

	return status;
}

