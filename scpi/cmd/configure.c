#include "include.h"




short scpi_spe_conf_volt_sense(void)
{
	Scpipara_type   state;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_LOCAL_REMOTE|SCPI_STR, &state, &err_code);
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

short scpi_spe_conf_volt_sense_quest(void)
{
	//int state;
	//short *perr_code;
	char cmdbuf[128];
	unReg32 regdat;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_STATUS_1,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_conf_volt_sense_quest;
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

			strcpy(cmdbuf, pScpi_param[(regdat.reg_int+COM_LOCAL)<<1]);
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




short scpi_spe_conf_volt_on(void)
{
	return scpi_spe_input_output_von_level( );
}
short scpi_spe_conf_volt_on_quest(void)
{
	return scpi_spe_input_output_von_level_quest( );
}
short scpi_spe_conf_volt_off(void)
{
	return scpi_spe_input_output_voff_level( );
}
short scpi_spe_conf_volt_off_quest(void)
{
	return scpi_spe_input_output_voff_level_quest( );
}
short scpi_spe_conf_volt_upper(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_CV_UPPER);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}
short scpi_spe_conf_volt_upper_quest(void)
{
	//char cmdbuf[128];
	//unReg32 regd;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CV_UPPER,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_conf_volt_lower(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_CV_LOWER);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}
short scpi_spe_conf_volt_lower_quest(void)
{
	///char cmdbuf[128];
	//unReg32 regd;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CV_LOWER,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_conf_current_upper(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_CC_UPPER);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}
short scpi_spe_conf_current_upper_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CC_UPPER,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_conf_current_lower(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_CC_LOWER);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}
short scpi_spe_conf_current_lower_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CC_LOWER, 1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_conf_resistance_upper(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_CR_UPPER);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}
short scpi_spe_conf_resistance_upper_quest(void)
{
	
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CR_UPPER,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_conf_resistance_lower(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_CR_LOWER);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}
short scpi_spe_conf_resistance_lower_quest(void)
{
	
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CR_LOWER,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		
	}
	return 0;
}
short scpi_spe_conf_power_upper(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_CP_UPPER);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}
short scpi_spe_conf_power_upper_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CP_UPPER,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_conf_power_lower(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_CP_LOWER);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}
short scpi_spe_conf_power_lower_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CP_LOWER,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}
short scpi_spe_conf_protect_ovp(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_PROT_OVER_VOLT);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		
	}
        
        return 0;//先返回成功
}
short scpi_spe_conf_protect_ovp_quest(void)
{
	

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_PROT_OVER_VOLT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_conf_protect_ocp(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_PROT_OVER_CURR);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}
short scpi_spe_conf_protect_ocp_quest(void)
{
	

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_PROT_OVER_CURR, 1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_conf_protect_opp(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_PROT_OVER_POW);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}
short scpi_spe_conf_protect_opp_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_PROT_OVER_POW, 1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_conf_protect_lvp(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_PROT_UNDER_VOLT);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		
	}
        
        return 0;//先返回成功
}

short scpi_spe_conf_protect_lvp_quest(void)
{
	

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_PROT_UNDER_VOLT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_conf_protect_time(void)
{
	Scpipara_type volt;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &volt, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&volt.reg_int, MAP_LOAD_TIME);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		
	}
        
        return 0;//先返回成功
}
short scpi_spe_conf_protect_time_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_LOAD_TIME,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntSCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_conf_limit_curr(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_CURR_LMT);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}
short scpi_spe_conf_limit_curr_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CURR_LMT, 1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_conf_external_inhibit(void)
{

	Scpipara_type state;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_EXT_INHIBIT|SCPI_STR, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&state.reg_int, MAP_INHIBIT);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_conf_external_inhibit_quest(void)
{	
	unReg32 mode;

	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_INHIBIT,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_conf_external_inhibit_quest;
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
			GetModbusRegValue(client_rec_buf+3, &mode);
			strcpy(cmdbuf, pExtInhibitStrings[mode.reg_int]);
			strcat(cmdbuf, ";");	

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