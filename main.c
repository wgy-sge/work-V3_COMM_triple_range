/********************    ELoader  通讯板嵌入式程序    ********************
Copyright (C), 2024, Zidetech. Co., Ltd.
* File Name          : main.c
* Author             : 
* Version            : V0.0.1
* Date               : 2024.9.6
* Description        : 主程序
*************************************************************************
* Log : 注意版本信息修改在 "include.h" 头文件内！！！！！！！！！！！！！

*************************************************************************/


/* Includes -------------------------------------------------------------*/
#include "include.h"

#define    COMMBOOTVERSIONSTR_ADDRESS    0x08000400      //通讯板BOOTLOAD版本信息字符串
#define    MAXBOOTVERSIONSTR_LEN         50


char CommBootVersionStr[MAXBOOTVERSIONSTR_LEN];
u8   CommBootVersionLen= 0;

u8  LoadBootloadVersionStr(char *pBootVersionStr)
{
	int i;
	CommBootVersionLen= 0;
	for ( i= 0; i< MAXBOOTVERSIONSTR_LEN; i++)
	{
		if ( pBootVersionStr[i]== 0 )
		{
			return i;			
		}
		else
		{
			CommBootVersionStr[i] = pBootVersionStr[i];
			CommBootVersionLen++;
		}
	}

	return i;
}



enum
{
	WriteCommInfo,
	WriteCommInfo_Answer,
	WriteCommBootVersionInfo,
	WriteCommBootVersionInfo_Answer,
	WaitMainStart,
	WaitMainStart_Answer,
	LoadConfig,
	LoadConfig_Answer,
	LoadStatusReg,
	LoadStatusReg_Answer,
	SetLoalRmt,
	SetLoalRmt_Answer,
};




static u8 con_state= 0;
bool HaveDoneConnect( UCHAR *fram, USHORT len, eMBErrorCode err )
{

	if (err== MB_ENOERR)
	{
		;
	} 
	else//连接失败重新连接
	{
		back_idle();//释放通信接口
		con_state= WriteCommInfo;
	}

	return TRUE;
}

bool HaveDoneWriteBootInfo( UCHAR *fram, USHORT len, eMBErrorCode err )
{
  // static u8 wr_cnt= 0;
	if (err== MB_ENOERR)
	{
		;
	} 
	else//连接失败重新连接
	{
		back_idle();//释放通信接口
		con_state= WriteCommBootVersionInfo;//重新写入}
	}
	return TRUE;
}



extern u16  stan_even_enab;                 //标准事件使能寄存器
extern u16  serv_requ_enab;                 //服务请求使能寄存器

//extern u16  stat_channel_ptr, stat_channel_ntr, stat_channel_enab;//通道配置

extern pFuncCommType pfResp;
extern u32 gDelayTick;
u8 Is_Connect(void)
{	
	static u8 req_wr_commbootinfo_cnt= 0;
	unReg32 regdat;
	u8 len;
	//int local= 1;
	switch(con_state)
	{
	case WriteCommInfo:		
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{
			RequestWriteBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_COMM_SOFT, SOFT_INFO, strlen( SOFT_INFO )); 
			pfResp= HaveDoneConnect;
			con_state= WriteCommInfo_Answer;
		}
		break;

	case WriteCommInfo_Answer:
		if (can_use(COMMUN_CONNECT))//已经收到数据
		{
			back_idle();//释放通信接口
			if ( client_rec_buf[1]== MB_FUNC_WRITE_BLOCK_DATA)//回读成功
			{
				con_state= WriteCommBootVersionInfo;	
			}			
			else
				con_state= WriteCommInfo;
		}
		break;

	case WriteCommBootVersionInfo:
		LoadBootloadVersionStr((char*)COMMBOOTVERSIONSTR_ADDRESS);
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{
			RequestWriteBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_COMM_BOOT_VERS, CommBootVersionStr, CommBootVersionLen); 
			pfResp= HaveDoneConnect;
			con_state= WriteCommBootVersionInfo_Answer;
		}

		break;

	case WriteCommBootVersionInfo_Answer:
		if (can_use(COMMUN_CONNECT))//已经收到数据
		{
			back_idle();//释放通信接口
			if ( client_rec_buf[1]== MB_FUNC_WRITE_BLOCK_DATA)//不管是否写成功都不再写
			{
				con_state= WaitMainStart;	
			}			
			else
			{
				req_wr_commbootinfo_cnt++;
				if (req_wr_commbootinfo_cnt> 5)				
					con_state= WaitMainStart;					
				else
					con_state= WriteCommBootVersionInfo;
			}
		}
		break;

	case WaitMainStart:
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{
			requ_read_regs(MAP_RB_STATUS_1, 1);
			pfResp= HaveDoneConnect;			
			con_state= WaitMainStart_Answer;
		}
		break;

	case WaitMainStart_Answer:
		if (can_use(COMMUN_CONNECT))//已经收到数据
		{
			back_idle();//释放通信接口
			if ( client_rec_buf[1]== MB_FUNC_READ_REGISTERS)//回读成功
			{
				len= client_rec_buf[2];
				if (len== 4)//读取5个寄存器共20个字节
				{
					regdat.reg_buff[0]= client_rec_buf[3+3];
					regdat.reg_buff[1]= client_rec_buf[3+2];
					regdat.reg_buff[2]= client_rec_buf[3+1];
					regdat.reg_buff[3]= client_rec_buf[3+0];

					//if (regdat.reg_int&BIT_START_APP)//等待进入应用程序
					{
						con_state= LoadConfig;
					}
					//else
					//{
					//	delay_ms(10);//等待10毫秒继续读
					//	con_state= WaitMainStart;
					//}
				}
				else//数据格式有问题
				{
					con_state= WaitMainStart;
				}							
			}
			else//回读失败
			{
				con_state= WaitMainStart;
			}			
		}
		break;



	case LoadConfig:
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{
			requ_read_regs(MAP_IP_ADDRESS, 9);
			pfResp= HaveDoneConnect;			
			con_state= LoadConfig_Answer;
		}
		break;

	case LoadConfig_Answer:
		if (can_use(COMMUN_CONNECT))//已经收到数据
		{
			back_idle();//释放通信接口
			if ( client_rec_buf[1]== MB_FUNC_READ_REGISTERS)//回读成功
			{
				len= client_rec_buf[2];
				if (len== 36)//读取5个寄存器共36个字节
				{
					gSystCommInfo.pNetInfo->iptab[0]= client_rec_buf[3+3];
					gSystCommInfo.pNetInfo->iptab[1]= client_rec_buf[3+2];
					gSystCommInfo.pNetInfo->iptab[2]= client_rec_buf[3+1];
					gSystCommInfo.pNetInfo->iptab[3]= client_rec_buf[3+0];

					gSystCommInfo.pNetInfo->netmask[0]= client_rec_buf[7+3];
					gSystCommInfo.pNetInfo->netmask[1]= client_rec_buf[7+2];
					gSystCommInfo.pNetInfo->netmask[2]= client_rec_buf[7+1];
					gSystCommInfo.pNetInfo->netmask[3]= client_rec_buf[7+0];

					gSystCommInfo.pNetInfo->gatewall[0]= client_rec_buf[11+3];
					gSystCommInfo.pNetInfo->gatewall[1]= client_rec_buf[11+2];
					gSystCommInfo.pNetInfo->gatewall[2]= client_rec_buf[11+1];
					gSystCommInfo.pNetInfo->gatewall[3]= client_rec_buf[11+0];

					gSystCommInfo.comm_baud= client_rec_buf[15+3];
					gSystCommInfo.comm_parity= client_rec_buf[19+3];

					gSystCommInfo.gpib_addr= client_rec_buf[23+3];
					gSystCommInfo.comm_protocol= client_rec_buf[27+3];
					gDevice_CommProtocol= gSystCommInfo.comm_protocol;
					gSystCommInfo.device_addr= client_rec_buf[31+3];
					gDeviceAddr= gSystCommInfo.device_addr;

					SetODNodeID(gSystCommInfo.device_addr);//设置CANOPEN设备地址
					gSystCommInfo.comm_dog_time= client_rec_buf[35+3];
					gSystCommInfo.comm_dog_time<<= 8;
					gSystCommInfo.comm_dog_time+= client_rec_buf[35+2];
					gSystCommInfo.comm_dog_time<<= 8;
					gSystCommInfo.comm_dog_time+= client_rec_buf[35+1];
					gSystCommInfo.comm_dog_time<<= 8;
					gSystCommInfo.comm_dog_time+= client_rec_buf[35+0];		

					con_state= LoadStatusReg;	
				}
				else//数据格式有问题
				{
					con_state= LoadConfig;
				}							
			}
			else//回读失败
			{
				con_state= LoadConfig;
			}		
		}
		break;




	case  LoadStatusReg:	
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{
			requ_read_regs(MAP_SCPI_CHAN_COND, 9);
			pfResp= HaveDoneConnect;			
			con_state= LoadStatusReg_Answer;
		}
		break;


	case LoadStatusReg_Answer:
		if (can_use(COMMUN_CONNECT))//已经收到数据
		{
			back_idle();//释放通信接口
			if ( client_rec_buf[1]== MB_FUNC_READ_REGISTERS)//回读成功
			{
				GetModbusRegValue(client_rec_buf+3+4*5, &regdat);	
				if (regdat.reg_int== 0)//PSC为0则使用保存配置，否则清零
				{
					GetModbusRegValue(client_rec_buf+3+4*3, &regdat);	
					stan_even_enab= regdat.reg_int;                 //标准事件使能寄存器
					GetModbusRegValue(client_rec_buf+3+4*4, &regdat);	
					serv_requ_enab= regdat.reg_int;                 //服务请求使能寄存器

					
					//GetModbusRegValue(client_rec_buf+3+4*6, &regdat);	
					//stat_channel_ptr= regdat.reg_int;                 //通道状态PTR

					//GetModbusRegValue(client_rec_buf+3+4*7, &regdat);	
					//stat_channel_ntr= regdat.reg_int;                 //通道状态NTR

					
				}
				con_state= SetLoalRmt;	
			}			
			else
				con_state= LoadStatusReg;
		}
		break;

	case SetLoalRmt:
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{
			unReg32 local;
			local.reg_int= 0;
			requ_write_an_reg( (u8*)&local,MAP_LOCAL_RMT);
			pfResp= HaveDoneConnect;
			con_state= SetLoalRmt_Answer;
		}
		break;

	case SetLoalRmt_Answer:
		if (can_use(COMMUN_CONNECT))//已经收到数据
		{
			back_idle();//释放通信接口
			if ( client_rec_buf[1]== MB_FUNC_WRITE_REGISTER)//写成功
			{
				con_state= LoadConfig;
				return TRUE;

			}			
			else
				con_state= SetLoalRmt;
		}
		break;

	default:
		break;
	}

	return 0;
}

extern u8 gRuning_flg;
void main(void)
{
	//char cmdbuf[128];
	ResetInit( );	
	Init_SystVariables();
	scpi_init( );//SCPI初始化
	InitScpiREG( );

	Init_Device( );

	//delay_ms(200);

	gDelayTick= 100;

	while ( 1 )
	{
		if (gDelayTick== 0)
		{
			break;
		}
	}

	//InitScpiREG();
	eMBInitClient( );
	eMBRTUStart( );


	//检测是否连接，并从主机板读取初始化信息
	while(1)
	{
		eMBClientPoll( );
		if (Is_Connect( ))
		{
			break;
		}

		if (gRuning_flg)
		{			
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		} 
		else
		{			
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}
	} 
        
	pfResp= NULL;
	serv_req_enable= 0;//sys_regs[OFFS_REG_SERV_REQU_ENABLE].reg_int;

	//gDevice_CommProtocol = PROTOCOL_MODBUS;
	//gDeviceAddr= 160;
	sys_timer_init( );

	uart1Init( );	
	Port1SerialEnable(1, 0);	//使能接收	

	uart3Init( );	
	Port3SerialEnable(1, 0);	//使能接收	

	socket_app_init();	


	ChangeGPIBAddr(); //修改GPIB地址为已存地址

	MCOUSER_ResetCommunication();//CANOPEN 协议初始化
	while(1)
	{	
		if (gRuning_flg)
		{			
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		} 
		else
		{			
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}

		SocketPoll( );

		EnginePoll( );			

		eMBClientPoll( );	//Fmodbus Client

	}
}


