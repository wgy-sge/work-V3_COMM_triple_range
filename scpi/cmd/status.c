#include "include.h"


/* External variables --------------------------------------------------------*/
u16  stan_even_even;                 //标准事件寄存器


u16  stat_chan_enab;                 //通道事件使能寄存器

u16  stan_even_enab;                 //标准事件使能寄存器
u16  serv_requ_enab;                 //服务请求使能寄存器
u16  status_byte;                    //状态字节


//u16  stat_channel_ptr, stat_channel_ntr, stat_channel_enab;//通道配置





/* Private variables ---------------------------------------------------------*/

void InitStatusVariable(void)
{
	stan_even_even= 0;
	stat_chan_enab= 0;
	stan_even_enab= 0;
	serv_requ_enab= 0;
	status_byte= 0;

}


/*******************************************************************************
* Function Name  : PutBitStatusByte
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/


void PutBitStatusByte( u8 bit )
{
	stan_even_even |= bit;
}


//SCPI状态寄存器

short scpi_spe_status_channel_condition_quest(void)
{
	int condition;
	short *perr_code;
	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SCPI_CHAN_COND,1);	
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}
	return 0;
}
short scpi_spe_status_channel_event_quest(void)
{
	int source;
	short *perr_code;
	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SCPI_CHAN_EVEN,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_status_channel_enable(void)
{
	Scpipara_type   enable;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &enable, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&enable.reg_int, MAP_SCPI_CHAN_ENAB);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_status_channel_enable_quest(void)
{
	int source;
	short *perr_code;
	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SCPI_CHAN_ENAB, 1);		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_status_questionable_condition_quest(void)
{
	return scpi_spe_status_channel_condition_quest();
}
short scpi_spe_status_questionable_enable(void)
{
	return scpi_spe_status_channel_enable();
}
short scpi_spe_status_questionable_enable_quest(void)
{
	return scpi_spe_status_channel_enable_quest();
}

short scpi_spe_status_questionable_event_quest(void)
{
	return scpi_spe_status_channel_event_quest();
}



short StatusChannelPtransition(void)
{
	Scpipara_type   enable;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &enable, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&enable.reg_int, MAP_SCPI_CHAN_PTR);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short StatusChannelPtransitionAsk(void)
{
	int source;
	short *perr_code;
	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SCPI_CHAN_PTR, 1);		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short StatusChannelNtransition(void)
{
	Scpipara_type   enable;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &enable, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&enable.reg_int, MAP_SCPI_CHAN_NTR);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short StatusChannelNtransitionAsk(void)
{
	int source;
	short *perr_code;
	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SCPI_CHAN_NTR, 1);		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}




short scpi_spe_trigger_imm(void)
{
	return 0;   
}

short scpi_spe_trigger_source(void)
{
	return 0;   
}
short scpi_spe_trigger_source_quest(void)
{
	char cmdbuf[128];

	sprintf(cmdbuf, "%d;", 1);
	if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
	outcounter= outcounter+ strlen(cmdbuf);
	return 0;  
}