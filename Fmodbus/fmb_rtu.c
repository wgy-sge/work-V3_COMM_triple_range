/******************** (C) COPYRIGHT 2010 Zidetech *****************************
* File Name          : fmb_rtu.c
* Author             : xxx
* Version            : V1.0
* Date               : 2010.06.21
* Description        : RTU通讯控制函数
********************************************************************************
* Log :
2009.06.21 xxx, initial version
*******************************************************************************/
#include "fmb.h"
#include "stdlib.h"
#include "string.h"

/* ----------------------- Static variables ---------------------------------*/
static  eMBSndState ep1_snd_state;                   //通道1发送状态机
static  eMBRcvState ep1_rcv_state = STATE_RX_IDLE;   //通道1接收状态机

static  USHORT ep1_rcv_posi;                      //通道1接收指针

static  UCHAR *ep1_psnd;                          //通道1发送源地址
static  USHORT ep1_snd_count;                     //通道1发送计数器
static  USHORT ep1_snd_amount;                    //通道1发送字节数目

/*---------------------------- Functions -------------------------------------*/
//获取接收内容长度
u16 GetRecvBuffLen(void)
{
	return ep1_rcv_posi;
}
/*******************************************************************************
* Function Name  : eMBRTUInit
* Description    : 初始化通讯口与定时器
* Input          : None
* Output         : None
* Return         : errcode
*******************************************************************************/
eMBErrorCode eMBRTUInit( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    
        
    /*** 初始化通讯口 ***/
    if( xMBPortSerialInit(  ) != TRUE )
    {
        eStatus = MB_EPORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         * The timer reload value for a character is given by:
         *
         * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
         *             = 11 * Ticks_per_1s / Baudrate
         *             = 220000 / Baudrate
         * The reload for t3.5 is 1.5 times this value and similary
         * for t3.5.
         */
        if( xMBPortTimersInit(  ) != TRUE )
        {
            eStatus = MB_EPORTERR;
        }
    }
    
    return eStatus;
}

/*******************************************************************************
* Function Name  : eMBRTUStart
* Description    : 通讯模块开始工作，打开串口接收中断
* Input          : None
* Output         : None
* Return         : errcode
*******************************************************************************/
void eMBRTUStart( void )
{
    /* Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.
     */
    
    ep1_rcv_state = STATE_RX_IDLE;           //置接收状态机到空闲状态
    
    vMBPortSerialEnable( FALSE, FALSE );    //打开串口接收中断
        
}

/*******************************************************************************
* Function Name  : eMBRTUStop
* Description    : RTU停止工作，关闭串口中断、定时器中断
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void eMBRTUStop( void )
{
    vMBPortSerialEnable( FALSE, FALSE );
    vMBPortTimersDisable(  );
}

/*******************************************************************************
* Function Name  : eMBRTUReceive
* Description    : RTU接收一帧数据，进行校验
* Input          : None
* Output         : None
* Return         : eMBErrorCode
*******************************************************************************/
eMBErrorCode eMBRTUReceive( UCHAR ** pucFrame, USHORT * pusLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    /* Length and CRC check */
    if( ( ep1_rcv_posi >= MB_SER_PDU_SIZE_MIN )
        && ( usMBCRC16( client_rec_buf, ep1_rcv_posi ) == 0 ) )
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        //ucMBAddress = ep1_buf[MB_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength = ( USHORT )( ep1_rcv_posi - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC );

        /* Return the start of the Modbus PDU to the caller. */
        *pucFrame = ( UCHAR * ) & client_rec_buf[MB_SER_PDU_PDU_OFF];        
    }
    else
    {
        eStatus = MB_EIO;
    }

    return eStatus;
}

/*******************************************************************************
* Function Name  : eMBClientReceive
* Description    : 客户端的接收帧函数
* Input          : pucFrame--目标缓存的指针
* Output         : None
* Return         : eMBErrorCode
*******************************************************************************/
eMBErrorCode eMBClientReceive( UCHAR ** pucFrame )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    
    if( ( ep1_rcv_posi >= MB_SER_PDU_SIZE_MIN )
        && ( usMBCRC16( client_rec_buf, ep1_rcv_posi ) == 0 ) )
    {
	*pucFrame = ( UCHAR * ) & client_rec_buf;
    }
    else
    {
	eStatus = MB_EIO;
    }
    return eStatus;
}



/*******************************************************************************
* Function Name  : eMBRTUSend
* Description    : RTU发送一帧PDU数据，
* Input          : None
* Output         : None
* Return         : eMBErrorCode
*******************************************************************************/
eMBErrorCode eMBRTUSend(  const UCHAR * pucFrame, USHORT usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          usCRC16;

    
    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if( ep1_rcv_state == STATE_RX_IDLE )
    {
        /* 将发送缓存赋值 */
        ep1_psnd = (UCHAR*)pucFrame ;
        ep1_snd_count = usLength;
            
        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = usMBCRC16( ( UCHAR * ) ep1_psnd, usLength );
            
        ep1_psnd[ep1_snd_count++] = ( UCHAR )( usCRC16 & 0xFF );
        ep1_psnd[ep1_snd_count++] = ( UCHAR )( usCRC16 >> 8 );
            
        /* Activate the transmitter. */
        ep1_snd_state = STATE_TX_XMIT;
        vMBPortSerialEnable( FALSE, TRUE );
    }
    else
    {
        eStatus = MB_EIO;
    }
        
    return eStatus;
}

/*******************************************************************************
* Function Name  : eMBClientSend
* Description    : 客户端的发送帧函数，
* Input          : usLength--发送帧的字节个数，不包括CRC
* Output         : None
* Return         : eMBErrorCode
*******************************************************************************/
eMBErrorCode eMBClientSend(  USHORT usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          usCRC16;

    
    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if( ep1_rcv_state == STATE_RX_IDLE )
    {
        /* 将发送缓存赋值 */
        ep1_psnd = (UCHAR*)client_snd_buf ;
        ep1_snd_count = usLength;
            
        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = usMBCRC16( ( UCHAR * ) ep1_psnd, usLength );
            
        ep1_psnd[ep1_snd_count++] = ( UCHAR )( usCRC16 & 0xFF );
        ep1_psnd[ep1_snd_count++] = ( UCHAR )( usCRC16 >> 8 );
        
        ep1_snd_amount = ep1_snd_count;
        /* Activate the transmitter. */
        ep1_snd_state = STATE_TX_XMIT;
        ticks_modbus_timeout = MODBUS_TIMEOUT_TICKS;    //超时
        
        vMBPortSerialEnable( FALSE, TRUE );
    }
    else
    {
        eStatus = MB_EIO;
    }
        
    return eStatus;     
}


/*******************************************************************************
* Function Name  : eMBClientReSend
* Description    : 现有消息帧重新发送
* Input          : None
* Output         : None
* Return         : eMBErrorCode
*******************************************************************************/
eMBErrorCode eMBClientReSend( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    
    if( ep1_rcv_state == STATE_RX_IDLE )
    {
        /* 将发送缓存赋值 */
        ep1_psnd = (UCHAR*)client_snd_buf ;
        ep1_snd_count = ep1_snd_amount;
        
        /* Activate the transmitter. */
        ep1_snd_state = STATE_TX_XMIT;
		ticks_modbus_timeout= MODBUS_TIMEOUT_TICKS;//超时应答计时器
        vMBPortSerialEnable( FALSE, TRUE );
    }
    else
    {
        eStatus = MB_EIO;
    }
        
    return eStatus;       
  
}



/*******************************************************************************
* Function Name  : port1_receive_fsm
* Description    : 端口1接收状态机
* Input          : None
* Output         : None
* Return         : void
*******************************************************************************/
void port1_receive_fsm( void )
{
    UCHAR           ucByte;

    /* Always read the port1 character. */
    ucByte = USART_ReceiveData( USART2 );

    switch ( ep1_rcv_state )
    {
        /* If we have received a character in the init state we have to
         * wait until the frame is finished.
         */
    case STATE_RX_INIT:
        vMBPortTimersEnable(  );
        break;

        /* In the error state we wait until all characters in the
         * damaged frame are transmitted.
         */
    case STATE_RX_ERROR:
        vMBPortTimersEnable(  );
        break;

        /* In the idle state we wait for a new character. If a character
         * is received the t1.5 and t3.5 timers are started and the
         * receiver is in the state STATE_RX_RECEIVCE.
         */
    case STATE_RX_IDLE:
        ep1_rcv_posi = 0;
        client_rec_buf[ep1_rcv_posi++] = ucByte;
        ep1_rcv_state = STATE_RX_RCV;

        /* Enable t3.5 timers. */
        vMBPortTimersEnable(  );
        break;

        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         */
    case STATE_RX_RCV:
        if( ep1_rcv_posi < MB_SER_PDU_SIZE_MAX )
        {
            client_rec_buf[ep1_rcv_posi++] = ucByte;
        }
        else
        {
            ep1_rcv_state = STATE_RX_ERROR;
        }
        vMBPortTimersEnable(  );
        break;
    }
    
      vMBPortTimersEnable(  );
    return ;
}

/*******************************************************************************
* Function Name  : port1_transmit_fsm
* Description    : 端口1发送状态机
* Input          : None
* Output         : None
* Return         : void
*******************************************************************************/
void port1_transmit_fsm( void )
{
    switch ( ep1_snd_state )
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_TX_IDLE:
        /* enable receiver/disable transmitter. */
        vMBPortSerialEnable( TRUE, FALSE );
        break;

    case STATE_TX_XMIT:
        /* check if we are finished. */
        if( ep1_snd_count > 0 )
        {
            USART_SendData(USART2, ( CHAR )*ep1_psnd );
            
            ep1_psnd++;  /* next byte in sendbuffer. */
            ep1_snd_count--;
        }
        else
        {
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            vMBPortSerialEnable( TRUE, FALSE );
            ep1_snd_state = STATE_TX_IDLE;
            
            //设置超时计时器
            ticks_modbus_timeout = MODBUS_TIMEOUT_TICKS;
            
        }
        break;
    }

    return ;
}

/*******************************************************************************
* Function Name  : port1_t35_expired
* Description    : 端口1的计时定时器
* Input          : None
* Output         : None
* Return         : void
*******************************************************************************/
void port1_t35_expired( void )
{
    
    switch ( ep1_rcv_state )
    {
        /* Timer t35 expired. Startup phase is finished. */
    case STATE_RX_INIT:
        //xMBPortEventPost( EV_READY );
        break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
    case STATE_RX_RCV:
        post_port_event( EVENT_PORT_1_RECEIVE );
		
        break;

        /* An error occured while receiving the frame. */
    case STATE_RX_ERROR:
        break;

        /* Function called in an illegal state. */
    default:
        //assert( ( eRcvState == STATE_RX_INIT ) ||
              //  ( eRcvState == STATE_RX_RCV ) || ( eRcvState == STATE_RX_ERROR ) );
        break;
    }

    vMBPortTimersDisable(  );
    /* 关闭接收，置接收状态机 */
    vMBPortSerialEnable( FALSE,FALSE );
    ep1_rcv_state = STATE_RX_IDLE;

    return ;
}
