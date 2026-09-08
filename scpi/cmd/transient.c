#include "include.h"

//瞬态功率
short scpi_spe_transient_power_fashion(void)
{
	Scpipara_type fashion;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_STR|SCPI_CONT|SCPI_PULSE_TOGG , &fashion, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 			
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&fashion.reg_int, MAP_CPD_WAY);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;
}
short scpi_spe_transient_power_fashion_quest(void)
{
	//int fashion;
	//short *perr_code;
	char cmdbuf[128];
	unReg32 regd;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CPD_WAY,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_transient_power_fashion_quest;
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
		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &regd);		

			strcpy(cmdbuf, pScpi_param[(regd.reg_int+COM_CONT)<<1]);
			strcat(cmdbuf, ";");

			//sprintf(cmdbuf, "%d;", fun.reg_int);
			if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
			{
				return ERR_OUTBUF_OVERFLOWER;
			}
			memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
			outcounter= outcounter+ strlen(cmdbuf);
			return 0;
		}
		else
			return ERR_QUERY_ERROR;//返回错误码
	}

	return 0;
}
short scpi_spe_transient_power_mlevel(void)
{
	Scpipara_type power;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &power, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (power.type== SCPI_INT)
		{
			power.reg_floa= (float)power.reg_int;	
		}
		requ_write_an_reg((u8*)&power.reg_floa, MAP_CPD_L1);

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;        
}
short scpi_spe_transient_power_mlevel_quest(void)
{
	//float power;
	//short *perr_code;
	//char cmdbuf[128];
	//unReg32 regd;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CPD_L1,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	return 0;
}
short scpi_spe_transient_power_mwidth(void)
{
	Scpipara_type mwidth;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_FLOAT, &mwidth, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (mwidth.type== SCPI_INT)
		{
			mwidth.reg_floa= (float)mwidth.reg_int;
		}
		requ_write_an_reg((u8*)&mwidth.reg_floa, MAP_CPD_T1);	
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}


	return 0;  
}
short scpi_spe_transient_power_mwidth_quest(void)
{
	//float mwidth;
	//short *perr_code;
	//char cmdbuf[128];
	//unReg32 regd;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CPD_T1,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestMSCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}
short scpi_spe_transient_power_tlevel(void)
{
	Scpipara_type power;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &power, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		if (power.type== SCPI_INT)
		{
			power.reg_floa= (float)power.reg_int;	
		}
		requ_write_an_reg((u8*)&power.reg_floa, MAP_CPD_L2);

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}	

	return 0;        
}
short scpi_spe_transient_power_tlevel_quest(void)
{
	//float power;
	//short *perr_code;
	//char cmdbuf[128];
	//unReg32 regd;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CPD_L2,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}


	return 0;
}
short scpi_spe_transient_power_twidth(void)
{
	Scpipara_type twidth;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_FLOAT, &twidth, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (twidth.type== SCPI_INT)
		{
			twidth.reg_floa= (float)twidth.reg_int;
		}
		requ_write_an_reg((u8*)&twidth.reg_floa, MAP_CPD_T2);	
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;  
}
short scpi_spe_transient_power_twidth_quest(void)
{
	//float twidth;
	//short *perr_code;
	//char cmdbuf[128];
	//unReg32 regd;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CPD_T2,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestMSCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	return 0;
}

