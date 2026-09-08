#include "include.h"



short scpi_spe_syst_sense_state(void)
{
	Scpipara_type   state;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_ON_OFF|SCPI_STR, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&state.reg_int, MAP_VOLT_SENSE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}
short scpi_spe_syst_sense_state_quest(void)
{
	//int state;
	//short *perr_code;
	char cmdbuf[128];
	unReg32 regdat;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_STATUS_1,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_syst_sense_state_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口


		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &regdat);

			if (regdat.reg_int&BIT_SENSE)
			{
				regdat.reg_int= 1;
			}
			else
			{
				regdat.reg_int= 0;
			}

			strcpy(cmdbuf, pScpi_param[(regdat.reg_int+COM_OFF)<<1]);
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



short scpi_spe_syst_response_volt(void)
{
	//PrintStr("scpi_spe_syst_response_volt");
	//return 0;
	Scpipara_type   state;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_FAST_SLOW|SCPI_STR, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&state.reg_int, MAP_CV_RESPONSE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_syst_response_volt_quest(void)
{	
	//int state;
	//short *perr_code;
	char cmdbuf[128];
	unReg32 regdat;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CV_RESPONSE,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_syst_response_volt_quest;
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
			GetModbusRegValue(client_rec_buf+3, &regdat);

			strcpy(cmdbuf, pScpi_param[(regdat.reg_int+COM_FAST)<<1]);
			strcat(cmdbuf, ";");
			//sprintf(cmdbuf, "%d;", fun);
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

short scpi_spe_syst_error_quest(void)
{  
	int scpi_err= 0;	
	char cmdbuf[128];	

	scpi_err= PullOutError();

	if(scpi_err== ERR_NO_ERROR)
	{
		strcpy(cmdbuf, "+0, No error;");
	}
	else
	{				
		Panel_ReadError(scpi_err);
		sprintf(cmdbuf, "%d, %s;", scpi_err, err_flash_info.p_err_str);				
		err_flash_info.p_err_str= NULL;
	}

	if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
	outcounter= outcounter+ strlen(cmdbuf);	
	return 0;
}

short scpi_spe_syst_version_quest(void)
{
	char cmdbuf[128];

	strcpy(cmdbuf, "1990.0;");
	if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
	outcounter= outcounter+ strlen(cmdbuf);
	return 0;
}

void SetLocal( u8 loc );
short scpi_spe_syst_local(void)
{
	int lcl= 0;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		requ_write_an_reg((u8*)&lcl, MAP_LOCAL_RMT);		
		
		//Send_event= NULL;
		Recv_event= scpi_spe_syst_local;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口

		if (client_rec_buf[1]== MB_FUNC_WRITE_REGISTER)
		{
			SetLocal(1);			
		}
		else
			return ERR_QUERY_ERROR;//返回错误码
	}

	return 0;
}

short scpi_spe_syst_clear_charge(void)
{
	int rwlock= 1;	
	
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		requ_write_an_reg((u8*)&rwlock, MAP_CLEAR_FLOW_PAST);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_syst_clear_time(void)
{
	int rwlock= 1;
	
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		requ_write_an_reg((u8*)&rwlock, MAP_CLEAR_FLOW_PAST);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	
	return 0;
}


short scpi_spe_syst_clear_energy(void)
{
	int rwlock= 1;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		requ_write_an_reg((u8*)&rwlock, MAP_CLEAR_FLOW_PAST);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	

	return 0;
}


short scpi_spe_syst_parallel_numb(void)
{
	//PrintStr("scpi_spe_syst_parallel_numb");
	//return 0;
	Scpipara_type   state;
	short err_code;	
	u8 *pscpi_pb;
	
	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&state.reg_int, MAP_MS_SELECT);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}


short scpi_spe_syst_parallel_numb_quest(void)
{
	//int state;
	//short *perr_code;
	char cmdbuf[128];
	unReg32 regdat;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_MS_SELECT,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_syst_parallel_numb_quest;
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
			GetModbusRegValue(client_rec_buf+3, &regdat);

			//strcpy(cmdbuf, pScpi_param[(regdat.reg_int+COM_FAST)<<1]);
			//strcat(cmdbuf, ";");
			sprintf(cmdbuf, "%d;", regdat.reg_int);
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


short scpi_spe_syst_parallel_slavers(void)
{
	//PrintStr("scpi_spe_syst_parallel_numb");
	//return 0;
	Scpipara_type   state;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&state.reg_int, MAP_MS_SLAVE_NUM);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}


short scpi_spe_syst_parallel_slavers_quest(void)
{
	//int state;
	//short *perr_code;
	char cmdbuf[128];
	unReg32 regdat;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_MS_SLAVE_NUM,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_syst_parallel_slavers_quest;
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
			GetModbusRegValue(client_rec_buf+3, &regdat);

			//strcpy(cmdbuf, pScpi_param[(regdat.reg_int+COM_FAST)<<1]);
			//strcat(cmdbuf, ";");
			sprintf(cmdbuf, "%d;", regdat.reg_int);
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


short scpi_spe_syst_parallel_state(void)
{
	//PrintStr("scpi_spe_syst_parallel_numb");
	//return 0;
	Scpipara_type   state;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_STR|SCPI_ON_OFF, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&state.reg_int, MAP_MS_CTRL);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}


short scpi_spe_syst_parallel_state_quest(void)
{
	char cmdbuf[128];
	unReg32 regdat;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_MS_CTRL,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_syst_parallel_state_quest;
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
			GetModbusRegValue(client_rec_buf+3, &regdat);

			strcpy(cmdbuf, pScpi_param[(regdat.reg_int+COM_OFF)<<1]);
			strcat(cmdbuf, ";");
			//sprintf(cmdbuf, "%d;", regdat.reg_int);
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




short scpi_spe_syst_rwlock(void)
{
	//PrintStr("scpi_spe_syst_parallel_numb");
	return 0;
}







short scpi_spe_syst_battcv_state(void)
{
	//PrintStr("scpi_spe_syst_parallel_numb");
	//return 0;
	Scpipara_type   state;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_STR|SCPI_ON_OFF, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&state.reg_int, MAP_CV_REGU_AHEA);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}


short scpi_spe_syst_battcv_state_quest(void)
{
	char cmdbuf[128];
	unReg32 regdat;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CV_REGU_AHEA,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_syst_battcv_state_quest;
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
			GetModbusRegValue(client_rec_buf+3, &regdat);

			strcpy(cmdbuf, pScpi_param[(regdat.reg_int+COM_OFF)<<1]);
			strcat(cmdbuf, ";");
			//sprintf(cmdbuf, "%d;", regdat.reg_int);
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

