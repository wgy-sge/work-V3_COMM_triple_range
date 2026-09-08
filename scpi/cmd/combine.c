//COMBINE

#include "include.h"


short scpi_spe_combine_mode(void)
{
	Scpipara_type state;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_COMB_MODE|SCPI_STR, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		state.reg_int= state.reg_int+ FUNC_CVCC;//功能码偏移增加
		requ_write_an_reg((u8*)&state.reg_int, MAP_TEST_FUNCTION);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}

	return 0;
}



short scpi_spe_combine_mode_quest(void)
{
	//int state;
	//short *perr_code;
	char cmdbuf[128];
	unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_TEST_FUNCTION,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_combine_mode_quest;
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
			GetModbusRegValue(client_rec_buf+3, &fun);
			if (fun.reg_int< FUNC_CVCC)
			{
				fun.reg_int= FUNC_CVCC;
			}

			strcpy(cmdbuf, pScpi_param[(fun.reg_int+COM_CCH)<<1]);
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

short scpi_spe_combine_cvcc_voltage(void)
{
	Scpipara_type volt;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &volt, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (volt.type== SCPI_INT)
		{
			volt.reg_floa= (float)volt.reg_int;	
		}
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_COMB_CVCC_VOLT);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0;  
}

short scpi_spe_combine_cvcc_voltage_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_COMB_CVCC_VOLT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;
}

short scpi_spe_combine_cvcc_current(void)
{
	Scpipara_type volt;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &volt, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (volt.type== SCPI_INT)
		{
			volt.reg_floa= (float)volt.reg_int;	
		}
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_COMB_CVCC_CURR);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0;  
}

short scpi_spe_combine_cvcc_current_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_COMB_CVCC_CURR,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}

short scpi_spe_combine_crcc_current(void)
{
	Scpipara_type volt;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &volt, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (volt.type== SCPI_INT)
		{
			volt.reg_floa= (float)volt.reg_int;	
		}
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_COMB_CRCC_CURR);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0;  
}

short scpi_spe_combine_crcc_current_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_COMB_CRCC_CURR,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}

short scpi_spe_combine_crcc_resistance(void)
{
	Scpipara_type volt;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &volt, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (volt.type== SCPI_INT)
		{
			volt.reg_floa= (float)volt.reg_int;	
		}
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_COMB_CRCC_RESI);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0;  
}

short scpi_spe_combine_crcc_resistance_quest(void)
{
	
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{
			requ_read_regs( MAP_COMB_CRCC_RESI,1 );		
			//Send_event= NULL;
			Recv_event= ScpiQuestFloatCmdAnswer;
			scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

		}
		return 0;
}

short scpi_spe_combine_cpcc_current(void)
{
	Scpipara_type volt;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &volt, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (volt.type== SCPI_INT)
		{
			volt.reg_floa= (float)volt.reg_int;	
		}
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_COMB_CPCC_CURR);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0;  
}

short scpi_spe_combine_cpcc_current_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_COMB_CPCC_CURR,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}

short scpi_spe_combine_cpcc_power(void)
{
	Scpipara_type volt;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &volt, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (volt.type== SCPI_INT)
		{
			volt.reg_floa= (float)volt.reg_int;	
		}
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_COMB_CPCC_POW);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0;  
}

short scpi_spe_combine_cpcc_power_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_COMB_CPCC_POW,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
	
}