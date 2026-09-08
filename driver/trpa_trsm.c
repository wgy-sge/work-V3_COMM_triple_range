#include "include.h"
/* Private typedef -----------------------------------------------------------*/


typedef struct
{
	bool (*pfIsThereFram)(void);
	bool (*pfReadFram)(char *pdst, u16 length, u16* pcnt);
	void (*pfDoPlus)(void);
	bool (*pfIsTranFree)(void);
	bool (*pfTranFram)( char * pframe, u16 len);

}stTrpaTrsmFuncs;

/* Private variables ---------------------------------------------------------*/

//内部通讯
static u8 fsm_trpa_in = TRSM_IDLE;             //传输状态机
//static u8 fsm_trpa_out = TRSM_IDLE;            //传输状态机
static char ucbuff_in[SIZE_OF_RS232_BUFFER];   //传入缓存

static char ucbuff_out[SIZE_OF_RS232_BUFFER];   //传入缓存
static u16  uslen_out= 0;


static u16 gReg_Addr;

//内部通讯
//static u16 rcv_fram_inner;                                  //内端收到有效帧计数



const static stTrpaTrsmFuncs trpa_funcs[AMOUNT_OF_CHANNEL] = 
{
	//CHANNEL_LAN
	{
		LanMbIsThereFrame,
			LanMbReadFrame,
			LanMbDoPlus,
			LanMbIsTransmitFree,
			LanMbTransmitFrame,
	},

	//CHANNEL_RS232
	{
		Rs232MbIsThereFrame,
			Rs232MbReadFrame,
			Rs232MbDoPlus,
			Rs232MbIsTransmitFree,
			Rs232MbTransmitFrame,
		},

		//CHANNEL_RS485
		{
			Rs485MbIsThereFrame,
				Rs485MbReadFrame,
				Rs485MbDoPlus,
				Rs485MbIsTransmitFree,
				Rs485MbTransmitFrame,
		},

		//CHANNEL_LAN_TCP
		{
			TcpLanMbIsThereFrame,
				TcpLanMbReadFrame,
				TcpLanMbDoPlus,
				TcpLanMbIsTransmitFree,
				TcpLanMbTransmitFrame,
			},
};


bool OutMbRequestProc(char * pframe, u16 len)
{
	if (gDeviceAddr== pframe[0])//是本设备消息则处理
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


bool InnerReadRegsAnswer( UCHAR *fram, USHORT len, eMBErrorCode err )
{
	if (err== MB_ENOERR)//收到正确回答
	{
		uslen_out= len- 2;
		memcpy(ucbuff_out, fram, uslen_out);
		ucbuff_out[0]= gDeviceAddr;	
	}
	else
	{
		ucbuff_out[0]= gDeviceAddr;	
		ucbuff_out[1] = 0x03+ 0x80;
		ucbuff_out[2] = 0x04;

		uslen_out= 3;
	}
	/*
	u8 i, j;
	u16 func, regnum= 0;

	if (err== MB_ENOERR)//收到正确回答
	{	
	func = fram[1];
	ucbuff_out[0]= gDeviceAddr;
	ucbuff_out[1]= 0x03;

	if( func == 0x03 )
	{	
	ucbuff_out[2]= fram[2];
	memcpy(ucbuff_out+3, fram+3, fram[2]);
	uslen_out= 3+ ucbuff_out[2];
	}
	else
	{			
	ucbuff_out[1]+= 0x80;
	ucbuff_out[2] = 0x02;
	uslen_out= 3;			
	}	
	}
	else//异常回答
	{
	ucbuff_out[0]= gDeviceAddr;	
	ucbuff_out[1] = 0x03+ 0x80;
	ucbuff_out[2] = 0x04;

	uslen_out= 3;
	}
	*/
	fsm_trpa_in = TRSM_TRANSFER; //内部通讯完成
	back_idle();//释放内部通讯
	return TRUE;
}


bool InnerReadFifoAnswer( UCHAR *fram, USHORT len, eMBErrorCode err )
{
	if (err== MB_ENOERR)//收到正确回答
	{
		uslen_out= len- 2;
		memcpy(ucbuff_out, fram, uslen_out);
		ucbuff_out[0]= gDeviceAddr;	
	}
	else//异常回答
	{
		ucbuff_out[0]= gDeviceAddr;	
		ucbuff_out[1] = MB_FUNC_READ_FIFO+ 0x80;
		ucbuff_out[2] = 0x04;

		uslen_out= 3;
	}


	/*
	u8 i, j;
	u16 func, dlen= 0;

	if (err== MB_ENOERR)//收到正确回答
	{	
	func = fram[1];
	ucbuff_out[0]= gDeviceAddr;

	ucbuff_out[1]= MB_FUNC_READ_FIFO;

	if( func == MB_FUNC_READ_FIFO )
	{	
	dlen= fram[2];//长度
	dlen<<= 8;
	dlen+= fram[3];

	memcpy(ucbuff_out+ 2, fram+2, dlen+2);
	uslen_out= 2+2+ dlen;
	}
	else
	{			
	ucbuff_out[1]+= 0x80;
	ucbuff_out[2] = 0x02;
	uslen_out= 3;			
	}	
	}
	else//异常回答
	{
	ucbuff_out[0]= gDeviceAddr;	
	ucbuff_out[1] = MB_FUNC_READ_FIFO+ 0x80;
	ucbuff_out[2] = 0x04;

	uslen_out= 3;
	}
	*/
	fsm_trpa_in = TRSM_TRANSFER; //内部通讯完成
	back_idle();//释放内部通讯
	return TRUE;
}


bool InnerWriteRegsAnswer( UCHAR *fram, USHORT len, eMBErrorCode err )
{	
	if (err== MB_ENOERR)//收到正确回答
	{
		uslen_out= len- 2;
		memcpy(ucbuff_out, fram, uslen_out);
		ucbuff_out[0]= gDeviceAddr;	
	}
	else//异常回答
	{
		ucbuff_out[0]= gDeviceAddr;	
		ucbuff_out[1] = MB_FUNC_WRITE_REGISTER+ 0x80;
		ucbuff_out[2] = 0x04;

		uslen_out= 3;
	}

	/*
	u16 func;	
	if (err== MB_ENOERR)//收到正确回答
	{
	func = fram[1];
	ucbuff_out[0]= gDeviceAddr;
	ucbuff_out[1]= 0x10;		

	if( func == 0x10 )
	{				
	memcpy(ucbuff_out+2, fram+2, 4);
	uslen_out= 6;			
	}
	else
	{	
	memcpy(ucbuff_out+1, fram+1, 2);
	uslen_out= 3;		
	}		
	}
	else//异常回答
	{
	ucbuff_out[0]= gDeviceAddr;	
	ucbuff_out[1] = 0x03+ 0x80;
	ucbuff_out[2] = 0x04;

	uslen_out= 3;
	}
	*/
	fsm_trpa_in = TRSM_TRANSFER; //内部通讯完成
	back_idle();//释放内部通讯
	return TRUE;
}

bool InnerReadRegBoundAnswer( UCHAR *fram, USHORT len, eMBErrorCode err )
{
	if (err== MB_ENOERR)//收到正确回答
	{
		uslen_out= len- 2;
		memcpy(ucbuff_out, fram, uslen_out);
		ucbuff_out[0]= gDeviceAddr;	
	}
	else//异常回答
	{
		ucbuff_out[0]= gDeviceAddr;	
		ucbuff_out[1] = MB_FUNC_READ_BOUND_REGISTERS+ 0x80;
		ucbuff_out[2] = 0x04;

		uslen_out= 3;
	}
	/*
	u8 i;
	u16 code;

	if (err== MB_ENOERR)//收到正确回答
	{
	code = *(fram);
	code += *(fram+1) << 8;

	ucbuff_out[0]= gDeviceAddr;
	ucbuff_out[1]= 0x41;		

	if( code == ERR_NO_ERROR )
	{			
	for (i= 0; i< 4; i++)
	{
	ucbuff_out[2+i]= fram[2+4-i-1];
	}

	for (i= 0; i< 4; i++)
	{
	ucbuff_out[6+i]= fram[2+8-i-1];
	}

	uslen_out= 10;			
	}
	else
	{	
	ucbuff_out[1]+= 0x80;			
	ucbuff_out[2] = 0x02;
	uslen_out= 3;		

	uslen_out= 3;		
	}
	}
	else//异常回答
	{
	ucbuff_out[0]= gDeviceAddr;	
	ucbuff_out[1] = 0x03+ 0x80;
	ucbuff_out[2] = 0x04;

	uslen_out= 3;
	}
	*/
	fsm_trpa_in = TRSM_TRANSFER; //内部通讯完成
	back_idle();//释放内部通讯
	return TRUE;
}



bool InnerReadBlockDataAnswer( UCHAR *fram, USHORT len, eMBErrorCode err )
{
	if (err== MB_ENOERR)//收到正确回答
	{
		uslen_out= len- 2;
		memcpy(ucbuff_out, fram, uslen_out);
		ucbuff_out[0]= gDeviceAddr;	
	}
	else//异常回答
	{
		ucbuff_out[0]= gDeviceAddr;	
		ucbuff_out[1] = MB_FUNC_READ_BLOCK_DATA+ 0x80;
		ucbuff_out[2] = 0x04;

		uslen_out= 3;
	}
	/*
	u8 i;

	if (err== MB_ENOERR)//收到正确回答
	{
	ucbuff_out[0]= gDeviceAddr;
	ucbuff_out[1]= 0x42;		

	if( fram[1] == 0x42 )//功能码判断
	{				
	for (i= 0; i< fram[2]+1; i++)
	{
	ucbuff_out[2+i]= fram[2+i];
	}
	uslen_out= ucbuff_out[2]+ 3;			
	}
	else
	{	
	ucbuff_out[1]+= 0x80;			
	ucbuff_out[2] = 0x02;
	uslen_out= 3;				
	}
	}
	else//异常回答
	{
	ucbuff_out[0]= gDeviceAddr;	
	ucbuff_out[1] = 0x42+ 0x80;
	ucbuff_out[2] = 0x04;

	uslen_out= 3;
	}
	*/
	fsm_trpa_in = TRSM_TRANSFER; //内部通讯完成
	back_idle();//释放内部通讯
	return TRUE;
}

bool InnerWriteBlockDataAnswer( UCHAR *fram, USHORT len, eMBErrorCode err )
{
	if (err== MB_ENOERR)//收到正确回答
	{
		uslen_out= len- 2;
		memcpy(ucbuff_out, fram, uslen_out);
		ucbuff_out[0]= gDeviceAddr;	
	}
	else//异常回答
	{
		ucbuff_out[0]= gDeviceAddr;	
		ucbuff_out[1] = MB_FUNC_WRITE_BLOCK_DATA+ 0x80;
		ucbuff_out[2] = 0x04;

		uslen_out= 3;
	}

	fsm_trpa_in = TRSM_TRANSFER; //内部通讯完成
	back_idle();//释放内部通讯
	return TRUE;
}

void RequestReadBlockData( UCHAR devi, 	USHORT addr );
extern pFuncCommType pfResp;
/*******************************************************************************
* Function Name  : FormatStandardToNon
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
static void FormatStandardToNon( char *pdst, u16 buff_len )
{
	u8 i;
	char *pch = pdst;
	u8 addr = pch[0];
	u8 code = pch[1];

	u16 lreg_addr, lreg_num;
	unReg32	reg_val;

	if ( addr!= gDeviceAddr )//首先判断地址,地址错误直接抛弃
	{
		if ( addr== BROADCAST_ADDRESS )
		{
			gBroadcastComm= 1;
		}
		else
		{
			gBroadcastComm= 0;
		    return;
		}
	}

	switch(code)
	{
		//读寄存器
	case 0x03:	
		if (buff_len!= 6)//接收缓冲区长度判断
		{
			break;
		}
		if (gBroadcastComm)
		{
			break;
		}

		lreg_addr = pch[2] << 8;
		lreg_addr |= pch[3];
		lreg_addr >>= 1;

		lreg_num = pch[4] << 8;
		lreg_num |= pch[5];
		lreg_num >>= 1;
		requ_read_regs(lreg_addr, lreg_num);
		pfResp= InnerReadRegsAnswer;
		fsm_trpa_in = TRSM_WAIT; 
		return;

		//写寄存器
	case 0x10:	
		if (buff_len!= 11)
		{
			break;
		}

		lreg_addr = pch[2] << 8;
		lreg_addr |= pch[3];
		lreg_addr >>= 1;

		gReg_Addr= lreg_addr;

		lreg_num = pch[4] << 8;
		lreg_num |= pch[5];
		lreg_num >>= 1;
		if ((lreg_num!= 1)|| (pch[6]!= 4))
		{
			break;
		}

		for (i= 0; i< 4; i++)
		{
			reg_val.reg_buff[i]= pch[6+4-i];
		}	

		requ_write_an_reg((u8*)&reg_val, lreg_addr);
		pfResp= InnerWriteRegsAnswer;
		fsm_trpa_in = TRSM_WAIT;	
		return;

		//读寄存器范围
	case 0x041:
		if (buff_len!= 4)
		{
			break;
		}
		if (gBroadcastComm)
		{
			break;
		}
		lreg_addr = pch[2] << 8;
		lreg_addr |= pch[3];
		lreg_addr >>= 1;
		requ_read_bound_regs(lreg_addr);
		pfResp= InnerReadRegBoundAnswer;
		fsm_trpa_in = TRSM_WAIT; 
		return;

		//读块数据
	case MB_FUNC_READ_BLOCK_DATA:
		if (gBroadcastComm)
		{
			break;
		}
		lreg_addr = pch[2] << 8;
		lreg_addr |= pch[3];
		RequestReadBlockData( MAIN_BOARD_DEVICE_ADDRESS, lreg_addr );

		pfResp= InnerReadBlockDataAnswer;
		fsm_trpa_in = TRSM_WAIT; 
		return;

		//写块数据
	case MB_FUNC_WRITE_BLOCK_DATA:		
		lreg_addr = pch[2] << 8;
		lreg_addr |= pch[3];		 
		RequestWriteBlockData( MAIN_BOARD_DEVICE_ADDRESS,  lreg_addr,  pch+6,  buff_len-6 );
		pfResp= InnerWriteBlockDataAnswer;
		fsm_trpa_in = TRSM_WAIT; 
		return;

	case MB_FUNC_READ_FIFO:
		if (gBroadcastComm)
		{
			break;
		}
		pdst[0]= MAIN_BOARD_DEVICE_ADDRESS;
		client_send(pdst, buff_len);

		pfResp= InnerReadFifoAnswer;
		fsm_trpa_in = TRSM_WAIT; 
		return;	

	default:
		break;
	}

	if (gBroadcastComm== 0)//非广播通讯才有返回
	{
		ucbuff_out[0]= gDeviceAddr;	
		ucbuff_out[1] = code+ 0x80;
		ucbuff_out[2] = 0x01;//不支持的功能码
		uslen_out= 3;

		fsm_trpa_in = TRSM_TRANSFER; //内部通讯完成
	}


}

void RefLedStatus(void);
/*******************************************************************************
* Function Name  : TrpaTrsmPoll
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void TrpaTrsmPoll( void )
{
	static u8 comm_chan = 0;//CHANNEL_RS232
	static u16 uslen_in;

	u8 i;

	/* 消息传入 */
	switch(fsm_trpa_in)
	{
	case TRSM_IDLE:
		if( request_comm_service(COMMUN_CONNECT) == FALSE )
		{
			break;
		}

		for(i= 0; i< 4; i++)//AMOUNT_OF_CHANNEL
		{
			if (i== CHANNEL_GPIB)//GPIB接口不能使用MODBUS协议
			{
				continue;
			}

			if( trpa_funcs[i].pfIsThereFram( ) == TRUE )
			{
				comm_chan = i;
				fsm_trpa_in = TRSM_TRANSFER; 

				if (trpa_funcs[i].pfReadFram( ucbuff_in, SIZE_OF_RS232_BUFFER, &uslen_in )== FALSE)
				{
					fsm_trpa_in = TRSM_IDLE; 
					trpa_funcs[i].pfDoPlus( );//释放端口重新接收
				}
				else
				{	
					uslen_out= 0;//@lgp2021.03.29  防止异常返回
					FormatStandardToNon( ucbuff_in, uslen_in );//外部协议转内部协议
					RefLedStatus( );
					break;
				}
			}
		}
		break;

	case TRSM_TRANSFER:
		if( trpa_funcs[comm_chan].pfIsTranFree( ) == TRUE )
		{			
			if (gBroadcastComm== 0)//非广播通讯方式才有返回
			{
				if(uslen_out> 0)//@lgp2021.03.29  防止异常返回
					trpa_funcs[comm_chan].pfTranFram( ucbuff_out, uslen_out );	
			}
			fsm_trpa_in= TRSM_IDLE;//释放端口重新接收
			trpa_funcs[comm_chan].pfDoPlus( );
		}
		break;


	case TRSM_WAIT:

		break;

	default:
		break;
	}

}