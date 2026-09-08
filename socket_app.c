/******************** (C) COPYRIGHT 2015 Zidetech *****************************
* File Name          : socket_app.c
* Author             : xxx
* Version            : V1.0
* Date               : 2015.10.9
* Description        : 
********************************************************************************
* Log :

*******************************************************************************/
#include "socket_app.h"
#include "w5500.h"
#include "socket.h"
#include "include.h"
#include "net_config.h"
#include "sys_timer.h"

//#include "uart_dma.h"


/* Private typedef -----------------------------------------------------------*/
enum
{
	NET_TYPE_TCP      = 0,
	NET_TYPE_TCPBACK  = 1,
	NET_TYPE_UDP      = 2,  
};

/* Private define ------------------------------------------------------------*/
#define    NET_BUF_RXSIZE        2048
#define    NET_BUF_TXSIZE        2048
#define    SOCKET_TICK_TIME      5000

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static struct System_Info * net_sys_info = NULL;
static u8   net_rxbuf[NET_BUF_RXSIZE];
//static u8   net_txbuf[NET_BUF_TXSIZE];
static u16  len_rcv= 0;
static u32  net_addr;
static u16  net_port;

//TCP通讯用
static u8   tcpnet_rxbuf[NET_BUF_RXSIZE];
//static u8   tcpnet_txbuf[NET_BUF_TXSIZE];
static u16  tcplen_rcv= 0;


struct net_info gSystNetInfo= { 
  {0xfe, 0x35, 0x1c, 0xa2, 0xdd, 0x55 }, 
{192,168,1,10}, 
{255,255,255,0}, 
{192,168,1,1}, 
7000
};


#define UNIQUE_ID_BASEADDR    0x1FFFF7E8
void socket_app_init(void)
{
	uint8 *mac_id;

	net_port_config();	

	mac_id     = (uint8 *)( UNIQUE_ID_BASEADDR + 6 );

	memcpy(gSystNetInfo.mac, mac_id, 6);
	//memcpy(gSystNetInfo.iptab,  sys_regs[MAP_IP_ADDRESS].reg_buff, 4);
	//memcpy(gSystNetInfo.netmask,  sys_regs[MAP_SUBNET_MASK].reg_buff, 4);
	/*
	gSystNetInfo.iptab[0]= 192;
	gSystNetInfo.iptab[1]= 168;
	gSystNetInfo.iptab[2]= 0;
	gSystNetInfo.iptab[3]= 123;

	gSystNetInfo.netmask[0]= 255;
	gSystNetInfo.netmask[1]= 255;
	gSystNetInfo.netmask[2]= 255;
	gSystNetInfo.netmask[3]= 0;


	gSystNetInfo.gatewall[0]= 192;
	gSystNetInfo.gatewall[1]= 168;
	gSystNetInfo.gatewall[2]= 0;
	gSystNetInfo.gatewall[3]= 1;
	*/
	while(1)
	{
		gSystNetInfo.mac[0]= 0;//改为单播MAC地址
		gSystNetInfo.mac[2]= gSystNetInfo.iptab[0];//强制修改MAC使MAC不一样
		gSystNetInfo.mac[3]= gSystNetInfo.iptab[1];//强制修改MAC使MAC不一样
		gSystNetInfo.mac[4]= gSystNetInfo.iptab[2];//强制修改MAC使MAC不一样
		gSystNetInfo.mac[5]= gSystNetInfo.iptab[3];//强制修改MAC使MAC不一样

		if( set_network( gSystNetInfo.mac,
                     gSystNetInfo.iptab,
                     gSystNetInfo.netmask,
                     gSystNetInfo.gatewall ) == TRUE )
		break;
	}

	/*****打开W5500的Keepalive功能*****/
	setkeepalive(0);    
}

void socket_timetick_pool(void)
{
  static uint32 socket_tick_stamp = 0;
  if(sys_timer_getstamp_between(socket_tick_stamp)  >=  SOCKET_TICK_TIME)
  {
    socket_tick_stamp = sys_timer_Getstamp();
    socket_app_pool();
  }
}


/*******************************************************************************
* Function Name  : 
* Description    : 获取一个字节数据，若没有则返回65535
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
u16 getchar_LAN(void)
{
	u16 ch;
	u8 len;

	if(remote_info.lan.rx_rp!=remote_info.lan.rx_wp)
	{
		ch= remote_info.lan.rx_buf[remote_info.lan.rx_rp++];
		//remote_info.uart1.rx_rp&=(COM_BUF_SIZE-1);	
		remote_info.lan.rx_rp%= COM_BUF_SIZE;	

		if (remote_info.lan.rx_full)
		{
			//消除满标志
			if( remote_info.lan.rx_wp >=remote_info.lan.rx_rp ) //循环BUF计算
			{
				len = remote_info.lan.rx_wp -remote_info.lan.rx_rp;
			}
			else
			{
				len = remote_info.lan.rx_wp + COM_BUF_SIZE - remote_info.lan.rx_rp;
			}

			if (len< COM_BUF_SIZE-30)//如果满了，则要等腾出30个空位后再清空标志
			{
				remote_info.lan.rx_full= FALSE;
			}
		}		
		return ch;
	}
	else
	{		
		return 65535;
	}
}

//
void LAN_PrintStr(char *pstr)
{	
	SocketUdpWriteData( pstr, strlen(pstr) );
}



/*******************************************************************************
* Function Name  : 
* Description    : 获取一个字节数据，若没有则返回65535
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
u16 getchar_TcpLAN(void)
{
	u16 ch;
	u8 len;

	if(remote_info.tcplan.rx_rp!=remote_info.tcplan.rx_wp)
	{
		ch= remote_info.tcplan.rx_buf[remote_info.tcplan.rx_rp++];
		//remote_info.uart1.rx_rp&=(COM_BUF_SIZE-1);	
		remote_info.tcplan.rx_rp%= COM_BUF_SIZE;	

		if (remote_info.tcplan.rx_full)
		{
			//消除满标志
			if( remote_info.tcplan.rx_wp >=remote_info.tcplan.rx_rp ) //循环BUF计算
			{
				len = remote_info.tcplan.rx_wp -remote_info.tcplan.rx_rp;
			}
			else
			{
				len = remote_info.tcplan.rx_wp + COM_BUF_SIZE - remote_info.tcplan.rx_rp;
			}

			if (len< COM_BUF_SIZE-30)//如果满了，则要等腾出30个空位后再清空标志
			{
				remote_info.tcplan.rx_full= FALSE;
			}
		}		
		return ch;
	}
	else
	{		
		return 65535;
	}
}


//
void TcpLAN_PrintStr(char *pstr)
{	
	SocketTcpWriteData( pstr, strlen(pstr) );
}

#define UDP_SERVER  1
#define TCP_SERVER  1

/*******************************************************************************
* Function Name  : SocketPoll
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void SocketPoll(void)
{
	static bool net_state = TRUE;	

	int len;

	/* 检查链接状态 */
	if( net_linkconfig( ) == FALSE )
	{
		if( net_state == TRUE )
		{
			net_state = FALSE;
			disconnect(NET_TYPE_TCP);
			disconnect(NET_TYPE_TCPBACK);
		}
	}
	else
	{
		net_state = TRUE;

#if TCP_SERVER
		if( gDevice_CommProtocol == PROTOCOL_MODBUS )//TCP MODBUS
		{	
			switch(getSn_SR(NET_TYPE_TCP))/*获取socket0的状态*/
			{
			case SOCK_INIT:/*socket初始化完成*/
				listen(NET_TYPE_TCP);/*在TCP模式下向服务器发送连接请求 */
				break;

			case SOCK_ESTABLISHED:/*socket连接建立*/
				if(getSn_IR(NET_TYPE_TCP) & Sn_IR_CON)
				{
					setSn_IR(NET_TYPE_TCP, Sn_IR_CON);/*Sn_IR的第0位置1*/
					disconnect(NET_TYPE_TCPBACK);
				}
				len=getSn_RX_RSR(NET_TYPE_TCP);/*len为已接收数据的大小*/
				if(len>0)
				{
					len = recv(NET_TYPE_TCP, tcpnet_rxbuf, len);
					if(len > 0)
					{
						tcplen_rcv= len;
						//len = net_rxdata_analy(net_rxbuf,len,net_txbuf,NET_BUF_TXSIZE,true);
						//if(len)
						//{
						//	send(NET_TYPE_TCP,net_rxbuf,len);//len);/*W5500向Client发送数据*/.
						//send(NET_TYPE_TCP,net_txbuf,len);//len);/*W5500向Client发送数据*/.
						//}
					}				
				}
				break;
			case SOCK_CLOSE_WAIT:/*socket等待关闭状态*/
				close(NET_TYPE_TCP);
				break;
			case SOCK_CLOSED:/*socket关闭*/
				socket(NET_TYPE_TCP, Sn_MR_TCP, TCP_MODBUS_PORT, Sn_MR_ND);/*打开socket0的一个端口*/
				break;
			default:
				break;
			}
		}
		else//TCP  SCPI
		{
			
			switch(getSn_SR(NET_TYPE_TCP))/*获取socket0的状态*/
			{
			case SOCK_INIT:/*socket初始化完成*/
				listen(NET_TYPE_TCP);/*在TCP模式下向服务器发送连接请求 */
				break;

			case SOCK_ESTABLISHED:/*socket连接建立*/
				if(getSn_IR(NET_TYPE_TCP) & Sn_IR_CON)
				{
					setSn_IR(NET_TYPE_TCP, Sn_IR_CON);/*Sn_IR的第0位置1*/
					disconnect(NET_TYPE_TCPBACK);
				}
				len=getSn_RX_RSR(NET_TYPE_TCP);/*len为已接收数据的大小*/
				if(len>0)
				{
					len = recv(NET_TYPE_TCP, tcpnet_rxbuf, len);
					if(len > 0)
					{
						tcplen_rcv= len;
						//len = net_rxdata_analy(net_rxbuf,len,net_txbuf,NET_BUF_TXSIZE,true);
						//if(len)
						//{
						//	send(NET_TYPE_TCP,net_rxbuf,len);//len);/*W5500向Client发送数据*/.
						//send(NET_TYPE_TCP,net_txbuf,len);//len);/*W5500向Client发送数据*/.
						//}
					}				
				}
				break;
			case SOCK_CLOSE_WAIT:/*socket等待关闭状态*/
				close(NET_TYPE_TCP);
				break;
			case SOCK_CLOSED:/*socket关闭*/
				socket(NET_TYPE_TCP, Sn_MR_TCP, TCP_SCPI_PORT, Sn_MR_ND);/*打开socket0的一个端口*/
				break;
			default:
				break;
			}
		}


#endif


#if UDP_SERVER
		switch( getSn_SR(NET_TYPE_UDP) )    //获取socket0的状态
		{
		//socket的套接字初始化完成
		case SOCK_UDP:
			if( getSn_IR(NET_TYPE_UDP) & Sn_IR_RECV )
			{
				setSn_IR(NET_TYPE_UDP, Sn_IR_RECV);  //Sn_IR的第0位置1
			}
			len_rcv = getSn_RX_RSR(NET_TYPE_UDP);

			if( len_rcv > 0 )
			{
				len_rcv = recvfrom( NET_TYPE_UDP, net_rxbuf, len_rcv, (uint8 *)&net_addr, &net_port );

				/*
				memcpy(net_txbuf, net_rxbuf, len_rcv);				
				//sendto(NET_TYPE_UDP, net_txbuf, len_rcv, (uint8 *)&net_addr, net_port);//W5500把接收到的数据返回

				
				if(len)
				{
					net_sys_info->debug_view_info->remote_addr  = remote_addr;
					net_sys_info->debug_view_info->remote_port  = remote_port;
					if(memcmp((uint8 *)&remote_addr,net_sys_info->debug_save_info.net.iptab,3) != 0)
					{
						remote_addr  = BROARD_IP;
					}
				}*/
			}
			/*
			len = tx_data_read(net_txbuf,NET_BUF_TXSIZE, TRANS_TYPE_UDP);
			if(len && remote_addr != 0 && remote_port != 0)
			{
			sendto(NET_TYPE_UDP, net_txbuf,len, (uint8 *)&remote_addr,remote_port);//W5200把接收到的数据返回
			net_sys_info->temp_information->trans_count++;
			}
			*/
			break;

			//socket关闭
		case SOCK_CLOSED://????????????????????????
			socket(NET_TYPE_UDP,Sn_MR_UDP, 5525, 0);
			break;

		default:
			break;
		}

#endif
	}
}

/*******************************************************************************
* Function Name  : SocketUdpReadData
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
u16 SocketUdpReadData( u8 *pdst, u16 length )
{
	u16 read_len = 0;
	u8 *pch = pdst;

	if( (len_rcv > 0) && (length >= len_rcv) )
	{
		for( read_len=0; read_len<len_rcv; read_len++ )
		{
			*pch ++ = net_rxbuf[read_len]; 	
		}
	}

	return read_len;
}


/*******************************************************************************
* Function Name  : SocketTcpReadData
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
u16 SocketTcpReadData( u8 *pdst, u16 length )
{
	u16 read_len = 0;
	u8 *pch = pdst;

	if( (tcplen_rcv > 0) && (length >= tcplen_rcv) )
	{
		for( read_len=0; read_len< tcplen_rcv; read_len++ )
		{
			*pch ++ = tcpnet_rxbuf[read_len]; 	
		}
	}

	tcplen_rcv= 0;//数据读完则清零
	return read_len;
}


/*******************************************************************************
* Function Name  : SocketUdpWriteData
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
u16 SocketUdpWriteData( u8 *psrc, u16 length )
{
	u16 len_tx;
	
	len_tx = sendto(NET_TYPE_UDP, psrc,length, (uint8 *)&net_addr,net_port);

	return len_tx;
}



/*******************************************************************************
* Function Name  : SocketTcpWriteData
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
u16 SocketTcpWriteData( u8 *psrc, u16 length )
{
	//u16 len_tx;

    send(NET_TYPE_TCP,psrc,length);//len);/*W5500向Client发送数据*/.

	//len_tx = sendto(NET_TYPE_UDP, psrc,length, (uint8 *)&net_addr,net_port);

	return length;
}


