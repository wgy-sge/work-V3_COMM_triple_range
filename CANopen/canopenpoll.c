#include "include.h"

u8 EventName= 0;
u16 Index;
u8 EventParam[500];
u16 EventParamLen= 0;

void PostEventVirtulOIDReg(u8 EName, u16 index, u8 *pEParam, u16 len)
{
	//u16 i;
    EventName= EName;

	Index= index>>1;
	EventParamLen= len;
	if ((len> 0)&& (pEParam!= NULL))
	{
		memcpy(EventParam, pEParam, len);
	}
	

	//gSystCommInfo.rem_flag= TRUE;//远程标志		
	//ClearCommDogTickes( );
}





//内部通讯
static u8 fsm_trpa_in = TRSM_IDLE;             //传输状态机

extern CAN_MSG MEM_FAR gTxSDO;
extern pFuncCommType pfResp;

bool CanOpenInnerReadRegAnswer( UCHAR *fram, USHORT len, eMBErrorCode err )
{	
	u8 sdbuff[4];	
	if (err== MB_ENOERR)//收到正确回答
	{		
		if( fram[1] == 0x03 )
		{	
			sdbuff[0]= fram[3+3];//大端转小端
			sdbuff[1]= fram[3+2];
			sdbuff[2]= fram[3+1];
			sdbuff[3]= fram[3+0];

			MCO_ReplyWith(((BYTE*)sdbuff),4);		
		}
		else
		{			
			gTxSDO.BUF[0]= 0x80;//异常响应
			if (!MCOHW_PushMessage(&gTxSDO))
			{
				MCOUSER_FatalError(0x8808);
			}	
		}	
	}
	else//异常回答
	{
		gTxSDO.BUF[0]= 0x80;//异常响应	
		if (!MCOHW_PushMessage(&gTxSDO))
		{
			MCOUSER_FatalError(0x8808);
		}	
	}

	fsm_trpa_in = TRSM_TRANSFER; //内部通讯完成
	back_idle();//释放内部通讯
	return TRUE;
}


bool CanOpenInnerWriteRegAnswer( UCHAR *fram, USHORT len, eMBErrorCode err )
{
	u8 sdbuff[4];

	if (err== MB_ENOERR)//收到正确回答
	{		
		if( fram[1] == 0x10 )
		{	
			// write response
			gTxSDO.BUF[0] = 0x60;	
			if (!MCOHW_PushMessage(&gTxSDO))
			{
				MCOUSER_FatalError(0x8808);
			}		
		}
		else
		{			
			gTxSDO.BUF[0]= 0x80;//异常响应	
			if (!MCOHW_PushMessage(&gTxSDO))
			{
				MCOUSER_FatalError(0x8808);
			}	
		}	
	}
	else//异常回答
	{
		gTxSDO.BUF[0]= 0x80;//异常响应
		if (!MCOHW_PushMessage(&gTxSDO))
		{
			MCOUSER_FatalError(0x8808);
		}	
	}

	fsm_trpa_in = TRSM_TRANSFER; //内部通讯完成
	back_idle();//释放内部通讯
	return TRUE;
}

void CanOpenPoll(void)
{

	/* 消息传入 */
	switch(fsm_trpa_in)
	{
	case TRSM_IDLE:
		if( request_comm_service(COMMUN_CONNECT) == FALSE )//内部通讯空闲
		{
			break;
		}

		switch(EventName)
		{
		case EVENT_CANOPEN_READ_ONE_REG://读一个寄存器
			EventName= EVENT_CANOPEN_NONE;//事件清除
			/*
			if ((Index>= MAP_RB_STATUS_1)&&(Index<= MAP_RB_TEMPERATURE))//此信息通讯板已经存在
			{
				//MCO_ReplyWith(((BYTE*)&(gMainBoardDemapRegs[Index-MAP_RB_STATUS_1].reg_buff)),4);

				fsm_trpa_in = TRSM_TRANSFER; //发送数据
				return;
			}*/
			requ_read_regs(Index, 1);
			pfResp= CanOpenInnerReadRegAnswer;
			fsm_trpa_in = TRSM_WAIT; 
			break;

		case EVENT_CANOPEN_WRITE_ONE_REG://写一个寄存器
			EventName= EVENT_CANOPEN_NONE;//事件清除
			requ_write_an_reg((u8*)EventParam, Index);			
			pfResp= CanOpenInnerWriteRegAnswer;
			fsm_trpa_in = TRSM_WAIT; 
			break;

		default:
			break;
		}


		
		break;

	case TRSM_TRANSFER:
		CanOpenPollFree();
		fsm_trpa_in= TRSM_IDLE;

		break;

	case TRSM_WAIT:
		break;

	default:
		break;
	}
}