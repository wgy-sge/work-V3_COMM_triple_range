/********************    FT8633A  人机接口板嵌入式程序    ********************
Copyright (C), 2010, Zidetech. Co., Ltd.
* File Name          : scpi_processor.c
* Author             : lu guangping
* Version            : V0.0.1
* Date               : 2010/3/15
* Description        : SCPI命令响应执行函数
********************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "include.h"

u16   cmdcnt= 0; //SCPI解析器收到的命令字符数

char OutputBuff[MAXOUTPUTCHARS];//输出缓冲区
u16 outcounter= 0;

short scpi_error_fifo[MAX_ERROR_NUM];//错误码缓存
u8 curerrornum= 0;//错误计数

int serv_req_enable= 0;
int serv_req_enable_cpy= 0;

static int lcl_rmt_flag= 1;

//SCPI输出
short Scpi_Print(char *buf)
{
	if (outcounter+ strlen(buf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, buf, strlen(buf));
	outcounter= outcounter+ strlen(buf);	
	return 0;
}

//报错函数
short ReportError(void)
{
	//Beep_report(BEEP_WARNING);
	int err_tmp;
	char cmdbuf[128];

	err_tmp= scpi_parser_return_code;

	if (err_tmp> ERR_SCPI_MODBUS)
	{
		scpi_parser_return_code= ERR_NO_ERROR;//清除错误
		err_tmp= err_tmp- (ERR_SCPI_MODBUS+ERR_SCPI_MODBUS_OFFSET);//还原错误代码
		if (err_tmp> 0)
		{
			sprintf(cmdbuf, "error,+%d;", err_tmp);
		}
		else
		{
			sprintf(cmdbuf, "error,%d;", err_tmp);
		}

		if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
		{
			return ERR_OUTBUF_OVERFLOWER;
		}
		memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
		outcounter= outcounter+ strlen(cmdbuf);
		return 0;
	}
	PushInError(err_tmp);//存入错误
	//发生错误，解析器不能往下执行命令
	scpi_parser_return_code= ERR_NO_ERROR;
	scpi_parser_is_idle= TRUE;		
	scpi_init();
	return 0;//先返回成功	
}



short Rmt_Lcl(void)
{
	short *perr_code;
	unReg32 fun;
	//Beep_report(BEEP_WARNING);	
	
    	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{
			requ_read_regs( MAP_RB_STATUS_1, 1 );		
			//Send_event= NULL;
			Recv_event= Rmt_Lcl;
			scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
			return 0;//先返回成功
		}
		else if (can_use(COMMUN_CONNECT))//已经收到数据
		{
			Recv_event= NULL;
			scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
			back_idle();//释放通信接口

			GetModbusRegValue(client_rec_buf+3, &fun);

		}
		return 0;
}


typedef struct 
{
	u16 (*GetChar)(void);
	void (*PrintStr)(char*pstr);

	bool  cmd_flag;
	u16   cmd_len;

	u8 *pSCPI_cmdbuff;
}stScpiProc_T;

u8 scpi_lan_cmd_buff[SIZE_OF_RS232_BUFFER];
u8 scpi_uart1_cmd_buff[SIZE_OF_RS232_BUFFER];
u8 scpi_usb_cmd_buff[SIZE_OF_RS232_BUFFER];
u8 scpi_Tcplan_cmd_buff[SIZE_OF_RS232_BUFFER];
u8 scpi_gpib_cmd_buff[SIZE_OF_RS232_BUFFER];
static stScpiProc_T ScpiProcFuncs[AMOUNT_OF_CHANNEL]= 
{
	{
		getchar_LAN,
			LAN_PrintStr,
			FALSE,
			0,
			scpi_lan_cmd_buff
	},

	{
		getchar_uart1,
			COMPrintStr,
			FALSE,
			0,
			scpi_uart1_cmd_buff
		},

		{
			getchar_usb,
				USBPrintStr,
				FALSE,
				0,
				scpi_usb_cmd_buff
		},

		{//TCP占位
			getchar_TcpLAN,
				TcpLAN_PrintStr,
				FALSE,
				0,
				scpi_Tcplan_cmd_buff
			},
		{
			getchar_gpib,
				GPIBPrintStr,
				FALSE,
				0,
				scpi_gpib_cmd_buff
		},	
};

u8 ReadScpiCMD(void)
{
	u8 i;
	unsigned int tempint;

	for (i= 0; i< AMOUNT_OF_CHANNEL; i++)
	{
		while (1)
		{
			tempint= ScpiProcFuncs[i].GetChar();

			if (tempint!= 65535)//当前通道有数据
			{
				if (tempint >= 128)//SCPI收到不可见字符丢弃
				{
					continue;
				}

				if(ScpiProcFuncs[i].cmd_len== SIZE_OF_RS232_BUFFER)
				{
					ScpiProcFuncs[i].cmd_len= 0;
					scpi_parser_return_code= ERR_INPUTBUF_OVERFLOWER;//输入溢出
					ReportError();		
				}

				if ((tempint== '\n'))	
				{
					if ( ScpiProcFuncs[i].cmd_len== 0)							
						return 0xff;

					ScpiProcFuncs[i].pSCPI_cmdbuff[ScpiProcFuncs[i].cmd_len]= '\n'; 
					ScpiProcFuncs[i].pSCPI_cmdbuff[ScpiProcFuncs[i].cmd_len+1]= '\0';
					ScpiProcFuncs[i].cmd_len= 0;
					ScpiProcFuncs[i].cmd_flag= TRUE;
					return i;//收到一条完整的指令
				}
				else
				{
					ScpiProcFuncs[i].pSCPI_cmdbuff[ScpiProcFuncs[i].cmd_len]= (char)tempint;              
					ScpiProcFuncs[i].cmd_len++;
				}
			}
			else
				break;//当前通道数据读完
		}

	}

	return 0xff;//没有收到完整指令
}


void SetLocal( u8 loc)
{
	lcl_rmt_flag= loc;
}


void FreeScpiProc(void)
{
	if (scpi_proc_ava== FALSE)//当前有指令正在处理
	{		
		scpi_parser_return_code= ERR_EXECUTION_ERROR;
		ReportError();		

		Recv_event= NULL;
		scpi_proc_ava= TRUE;
	}
}

void RefLedStatus(void);
//SCPI响应处理主函数，它调用SCPI解析器进行处理
void scpi_main_proc(void)
{
	static u8 scpi_chan= 0;
	u8 stat;

	unsigned int tempint;	

	// Place your code here
	if (scpi_proc_ava)//无命令待处理
	{
	
		if(scpi_parser_is_idle)
		{
			scpi_chan= ReadScpiCMD();
			if (scpi_chan!= 0xff)//收到指令
			{
				RefLedStatus( );
				scpi_parser_is_idle = FALSE;

				strcpy(scpi_input_buffer, ScpiProcFuncs[scpi_chan].pSCPI_cmdbuff);//拷贝指令待处理
				memset(OutputBuff, 0, MAXOUTPUTCHARS);//清空输出缓冲区
				outcounter= 0;
			}		
		}

		if(!scpi_parser_is_idle)
		{  
			if (lcl_rmt_flag== 1)
			{
				lcl_rmt_flag= 0;
				Rmt_Lcl();//调试时暂时关闭
			}
			else
				scpi_parser();
		}

		//执行功能函数
		if(scpi_func_output)
		{
			scpi_parser_return_code= scpi_func_output();
			scpi_func_output = NULL;
		}

		if(scpi_parser_is_idle&&outcounter&&scpi_proc_ava)
		{
			OutputBuff[outcounter-1]= '\n';
			OutputBuff[outcounter]= '\0';
			if (scpi_chan == CHANNEL_GPIB)
			{
				GPIBIntOFF();
				stat=cpd_read_reg(ADDR_STAT1);
				if((stat&0xf0)!=0x80)
				{
					scpi_parser_return_code= ERR_QUERY_ERROR;
					ReportError();
					remote_info.gpib.tx_wp= remote_info.gpib.tx_rp= 0;//清空本地数据	
					cpd_write_reg(ADDR_LOC_MSG, 0x17);//清空GPIB卡的发送数据
					cpd_write_reg(ADDR_LOC_MSG, 0x1D);//清空GPIB卡的发送数据
				}  
				GPIBIntON();	//open GPIB int		
			}

			ScpiProcFuncs[scpi_chan].PrintStr(OutputBuff);
			outcounter= 0;
		}

		

	}
	else//有未完成命令
	{	
		if (can_use(COMMUN_CONNECT)&&(Recv_event!= NULL))//已经收到数据
		{//执行功能函数			
			scpi_parser_return_code= Recv_event();
			//Recv_event = NULL;

			if(scpi_parser_is_idle&&outcounter&&scpi_proc_ava)
			{
				OutputBuff[outcounter-1]= '\n';
				OutputBuff[outcounter]= '\0';
				if (scpi_chan == CHANNEL_GPIB)
				{
					GPIBIntOFF();
					stat=cpd_read_reg(0x05);
					if((stat&0xf0)!=0x80)
					{
						scpi_parser_return_code= ERR_QUERY_ERROR;
						ReportError();
						remote_info.gpib.tx_wp= remote_info.gpib.tx_rp= 0;//清空本地数据
						cpd_write_reg(ADDR_LOC_MSG, 0x17);//清空GPIB卡的发送数据
						cpd_write_reg(ADDR_LOC_MSG, 0x1D);//清空GPIB卡的发送数据
					}  
					GPIBIntON();	//open GPIB int		
				}

				ScpiProcFuncs[scpi_chan].PrintStr(OutputBuff);
				outcounter= 0;
			}
		}			
	}



	// 报错系统 
	//-----------------------------------------------------------------------
	if (scpi_parser_return_code!= ERR_NO_ERROR)
	{
		ReportError();					
	}

	if (remote_info.uart1.rx_err)
	{	
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{	
			remote_info.uart1.rx_err= 0;
			scpi_parser_return_code= ERR_INPUTBUF_OVERFLOWER;//输入溢出	
			ReportError();	
		}		
	}

	if (remote_info.gpib.rx_err)
	{	
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{	
			remote_info.gpib.rx_err= 0;
			scpi_parser_return_code= ERR_INPUTBUF_OVERFLOWER;//输入溢出	
			ReportError();	
		}		
	}

	if (remote_info.gpib.tx_err)
	{	
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{	
			remote_info.gpib.tx_err= 0;
			scpi_parser_return_code= ERR_OUTBUF_OVERFLOWER;//输出溢出	
			ReportError();	
		}		
	}

	if (remote_info.lan.tx_err)
	{	
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{	
			remote_info.lan.tx_err= 0;
			scpi_parser_return_code= ERR_OUTBUF_OVERFLOWER;//输出溢出	
			ReportError();	
		}		
	}

	//--------------------------------------------------------------------------
}




//+++++++++++++++++++++++++++++++++++++++++++++++++++
//
//公共指令
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++

extern u16  stan_even_even;
#ifndef NO_SCPI_COMM_COMMAND
short scpi_com_cls(void)
{
	int cls= 0;
	//short *perr_code;

	stan_even_even= 0;
	ClearErrorQueue();
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		requ_write_an_reg((u8*)&cls, MAP_CLEAR_FAULT);		
		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_com_ese(void)
{
	Scpipara_type std_event_en;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &std_event_en, &err_code);
		if (pscpi_pb== NULL)
			return err_code;		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&std_event_en.reg_int, MAP_SCPI_STAN_ENAB);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;	
}

short scpi_com_ese_quest(void)
{
	//int std_event_en;
	//short *perr_code;
	//char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SCPI_STAN_ENAB,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;

}
short scpi_com_esr_quest(void)
{
	char cmdbuf[128];
	sprintf(cmdbuf, "%d;", stan_even_even);
	if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
	outcounter= outcounter+ strlen(cmdbuf);

	return 0;
}

char BlockManuFactor[50];
char BlockModels[50];


short scpi_com_idn_print_quest(void)
{
	u16 i, len;

	char cmdbuf[200];

	if (can_use(COMMUN_CONNECT))//已经收到数据
	{
        back_idle();//释放通信接口
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令		

		if (client_rec_buf[1]== MB_FUNC_READ_BLOCK_DATA)
		{
			len= client_rec_buf[2];
			len<<= 8;
			len+= client_rec_buf[3];

			for (i= 0; i< len; i++)
			{
				BlockModels[i]= client_rec_buf[4+ i];
			}
			BlockModels[i]= 0;	

			sprintf(cmdbuf, "%s,%s,0,%s;", BlockManuFactor, BlockModels, SOFT_VERSION);
			if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
			{
				return ERR_OUTBUF_OVERFLOWER;
			}
			memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
			outcounter= outcounter+ strlen(cmdbuf);
		}			
	}
	else
	{
		back_idle();//释放通信接口
		RequestReadBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_MODEL );		
		//Send_event= NULL;
		Recv_event= scpi_com_idn_print_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}
	return 0;
	//return ERR_QUERY_ERROR;//返回错误码	
}

/*
short scpi_com_idn_quest_inner_answer1(void)
{


}
*/

short scpi_com_idn_quest(void)
{	
	u16 i;
	u16 len;
	/*
	char cmdbuf[128];

	sprintf(cmdbuf, "%s,%s,0,%s;", "Aaith", "FT68200A", SOFT_VERSION);
	if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
	outcounter= outcounter+ strlen(cmdbuf);

	return 0;
	*/

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		RequestReadBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_MANUFACTURER );		
		//Send_event= NULL;
		Recv_event= scpi_com_idn_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_BLOCK_DATA)
		{
			len= client_rec_buf[2];
			len<<= 8;
			len+= client_rec_buf[3];

			for (i= 0; i< len; i++)
			{
				BlockManuFactor[i]= client_rec_buf[4+ i];
			}
			BlockManuFactor[i]= 0;	

			RequestReadBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_MODEL );		
			//Send_event= NULL;
			Recv_event= scpi_com_idn_print_quest;
			scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	

		}
		else
			return ERR_QUERY_ERROR;//返回错误码	
	}

	return 0;	
}

short scpi_com_opc(void)
{

	stan_even_even|= 1;
	return 0;
}

short scpi_com_opc_quest(void)
{
	int opc= 1;				
	char cmdbuf[128];

	sprintf(cmdbuf, "%d;", opc);
	if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
	outcounter= outcounter+ strlen(cmdbuf);

	return 0;
}

short scpi_com_psc(void)
{
	Scpipara_type psc;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &psc, &err_code);
		if (pscpi_pb== NULL)
			return err_code;
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&psc.reg_int, MAP_SCPI_PSC_SWITCH);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;

}


short scpi_com_psc_quest(void)
{
	//int psc;
	//short *perr_code;
	//char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SCPI_PSC_SWITCH,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_com_rst(void)//恢复出厂设置
{
	int rst= 1;
	//short *perr_code;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		requ_write_an_reg((u8*)&rst, MAP_RST_FACTORY_SET);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_com_sre(void)//设置状态字使能寄存器
{
	Scpipara_type ser_req_en;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &ser_req_en, &err_code);
		if (pscpi_pb== NULL)
			return err_code;		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		serv_req_enable_cpy= ser_req_en.reg_int;
		requ_write_an_reg((u8*)&ser_req_en.reg_int, MAP_SCPI_SERV_ENAB);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;	
}

short scpi_com_sre_quest(void)//状态字使能寄存器查询
{
	//int ser_req_en;
	//short *perr_code;
	//char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SCPI_SERV_ENAB,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;
}


short scpi_com_stb_quest_answer(void)
{
	unReg32 regdat;
    u8 status_byte= 0;
    char cmdbuf[128];

	if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if ( client_rec_buf[1]== MB_FUNC_READ_REGISTERS)//回读成功
		{
			GetModbusRegValue(client_rec_buf+3, &regdat);//SCPI通道事件总结			

			if (regdat.reg_int)
			{
				status_byte|= 4;//SCPI通道事件总结
			}

			if (outcounter> 0)
			{
				status_byte|= 16;//有数据没有输出
			}

			GetModbusRegValue(client_rec_buf+3+ 4, &regdat);//SCPI标准事件使能

			if (regdat.reg_int&stan_even_even)
			{
				status_byte|= 32;//
			}
			
			GetModbusRegValue(client_rec_buf+3+ 8, &regdat);//SCPI服务请求使能
			if (regdat.reg_int&status_byte)
			{
				status_byte|= 64;//
			}
			
			sprintf(cmdbuf, "%d;", status_byte);
			if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
			{
				return ERR_OUTBUF_OVERFLOWER;
			}
			memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
			outcounter= outcounter+ strlen(cmdbuf);
			
		}			
		else
			return ERR_QUERY_ERROR;//返回错误码
	}

	return 0;
}

short scpi_com_stb_quest(void)//查询状态字概要寄存器
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs(MAP_SCPI_CHAN_EVEN_CSUM, 3);
		Recv_event= scpi_com_stb_quest_answer;			
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;
}

short scpi_com_sav(void)
{
	Scpipara_type sav;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &sav, &err_code);
		if (pscpi_pb== NULL)
			return err_code;		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&sav.reg_int, MAP_SAVE_RAPID);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	
	return 0;
}


short scpi_com_trg(void)
{
	int trg= 1;
	short *perr_code;	
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		requ_write_an_reg((u8*)&trg, MAP_TRIG_SIGNAL);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}
	
	return 0;
}


short scpi_com_rcl(void)
{
	Scpipara_type rcl;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &rcl, &err_code);
		if (pscpi_pb== NULL)
			return err_code;		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&rcl.reg_int, MAP_RECALL_RAPID);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_com_tst_quest(void)
{
	int opc= 0;				
	char cmdbuf[128];

	sprintf(cmdbuf, "%d;", opc);
	if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
	outcounter= outcounter+ strlen(cmdbuf);


	/*
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		requ_write_an_reg((u8*)&tst, OFFS_REG_IEEE4882_TST);		

		//Send_event= NULL;
		Recv_event= scpi_com_tst_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		//client_rec_buf
		perr_code= (short*)client_rec_buf;	
		if (*perr_code== ERR_NO_ERROR)
		{
			tst= *(int *)(client_rec_buf+2);				

			sprintf(cmdbuf, "%d;", tst);
			if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
			{
				return ERR_OUTBUF_OVERFLOWER;
			}
			memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
			outcounter= outcounter+ strlen(cmdbuf);
		}	

		return *perr_code;//返回错误码
	}
*/
	return 0;
}

short scpi_com_wai(void)//无操作
{
	//OutputBuff
	//PrintStr("wai\n");

	return 0;
}
#endif








/*
//触发
#ifndef NO_SCPI_SPE_TRIGGER_COMMAND
short scpi_spe_trigger_imm(void)
{
	Scpipara_type auto_save;
	short err_code;	

	auto_save.reg_int= 1;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&auto_save.reg_int, OFFS_REG_TRIG_SEAMP);		
		//Send_event= NULL;
		Recv_event= scpi_spe_trigger_imm;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		//client_rec_buf
		err_code= *(short*)client_rec_buf;	
		return err_code;//返回错误码
	}

	return 0;
}

short scpi_spe_trigger_source(void)
{
	Scpipara_type   source;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_STR|SCPI_EXT_KEY_BUS , &source, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&source.reg_int, OFFS_REG_TRIG_SOURCE);		
		//Send_event= NULL;
		Recv_event= scpi_spe_trigger_source;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		//client_rec_buf
		err_code= *(short*)client_rec_buf;	
		return err_code;//返回错误码
	}

	return 0;
}

short scpi_spe_trigger_source_quest(void)
{
	int source;
	short *perr_code;
	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( OFFS_REG_TRIG_SOURCE,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_trigger_source_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		//
		perr_code= (short*)client_rec_buf;

		if (*perr_code== ERR_NO_ERROR)
		{
			source= *(int *)(client_rec_buf+2);
			strcpy(cmdbuf, pScpi_param[(source+COM_KEY)<<1]);
			strcat(cmdbuf, ";");
			//sprintf(cmdbuf, "%d;", source);
			if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
			{
				return ERR_OUTBUF_OVERFLOWER;
			}
			memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
			outcounter= outcounter+ strlen(cmdbuf);
		}		
		return *perr_code;//返回错误码
	}

	return 0;
}

#endif

*/


/*
u8 qur_cal_item= 0;
//校准查询
short scpi_spe_query_calibrate_quest(void)
{
	float flt_cal_dat;
	int   int_cal_dat;

	short *perr_code;
	char cmdbuf[128];


	Scpipara_type qrpam;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &qrpam, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (qrpam.type== SCPI_FLOAT)
		{
			qrpam.reg_int = (int)qrpam.reg_floa;	
		}
		qur_cal_item= (u8)qrpam.reg_int;
		requ_write_an_reg((u8*)&qrpam.reg_int, OFFS_REG_QUERY_CALIBRATE);	

		//Send_event= NULL;
		Recv_event= scpi_spe_query_calibrate_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		//
		perr_code= (short*)client_rec_buf;

		if (*perr_code== ERR_NO_ERROR)
		{
			switch(qur_cal_item)
			{
			case 1:	//1，型号索引（整形）；			
			case 13://13，恒阻高量程电流偏置（整形）			
			case 15://15，恒阻低量程电流偏置（整形）
				int_cal_dat= *(int *)(client_rec_buf+2);	
				sprintf(cmdbuf, "%d;", int_cal_dat);
				if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
				{
					return ERR_OUTBUF_OVERFLOWER;
				}
				memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
				outcounter= outcounter+ strlen(cmdbuf);
				break;

			case 2://2，温度修正（浮点）；
				flt_cal_dat= *(float *)(client_rec_buf+2);	
				sprintf(cmdbuf, "%.4f;", flt_cal_dat);
				if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
				{
					return ERR_OUTBUF_OVERFLOWER;
				}
				memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
				outcounter= outcounter+ strlen(cmdbuf);
				break;					

			case 3://3，电流回显高量程k（浮点）+b（浮点）
			case 4://4，电流回显低量程k（浮点）+b（浮点）
			case 5://5，电压回显高量程k（浮点）+b（浮点）
			case 6://6，电压回显低量程k（浮点）+b（浮点）
			case 7://7，恒流功能高量程k（浮点）+b（浮点）
			case 8://8，恒流功能低量程k（浮点）+b（浮点）
			case 9://9，恒压功能高量程k（浮点）+b（浮点）
			case 10://10，恒压功能低量程k（浮点）+b（浮点）
			case 12://12，限流k（浮点）+b（浮点）				
				sprintf(cmdbuf, "%.4f,%.4f;", *(float *)(client_rec_buf+2), *(float *)(client_rec_buf+6));
				if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
				{
					return ERR_OUTBUF_OVERFLOWER;
				}
				memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
				outcounter= outcounter+ strlen(cmdbuf);
				break;

			case 14://14，恒阻高量程计算系数k（浮点）+电压偏置（整形）
			case 16://16，恒阻低量程计算系数k（浮点）+电压偏置（整形）
				flt_cal_dat= *(float *)(client_rec_buf+2);	
				int_cal_dat= *(int *)(client_rec_buf+6);	
				sprintf(cmdbuf, "%.4f,%d;", flt_cal_dat, int_cal_dat);
				if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
				{
					return ERR_OUTBUF_OVERFLOWER;
				}
				memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
				outcounter= outcounter+ strlen(cmdbuf);
				break;

			default:
				break;
			}		
		}		
		return *perr_code;//返回错误码
	}

	return 0;
}*/
