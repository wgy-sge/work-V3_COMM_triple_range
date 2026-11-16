#include "include.h"



short scpi_spe_input_output_state(void)
{

	Scpipara_type state;
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
		requ_write_an_reg((u8*)&state.reg_int, MAP_RUN_SWITCH);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_input_output_state_quest(void)
{
	unReg32 fun;
	//short *perr_code;
	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_STATUS_1, 1);		
		//Send_event= NULL;
		Recv_event= scpi_spe_input_output_state_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口

		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &fun);

			if (fun.reg_int&BIT_INPUT)
			{
				fun.reg_int= 1;
			}
			else
			{
				fun.reg_int= 0;
			}

			strcpy(cmdbuf, pScpi_param[(fun.reg_int+COM_OFF)<<1]);
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

short scpi_spe_function(void)
{
	Scpipara_type state;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_FUNCTION|SCPI_STR, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		

		if (state.reg_int== -1)//忽略此次命令
		{
			return 0;
		}
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&state.reg_int, MAP_TEST_FUNCTION);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		
	}

	return 0;
}

s32 FT68200_Func2FT6800_Func(s32 ft68200_Func);

extern const char *pScpi_ft6800param[];
short scpi_spe_function_quest(void)
{
	//int state;
	//short *perr_code;
	char cmdbuf[128];
	unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_TEST_FUNCTION,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_function_quest;
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

			//fun.reg_int= FT68200_Func2FT6800_Func(fun.reg_int);
			if (fun.reg_int< 0)
			{
				strcpy(cmdbuf, "none");
				
			}
			//else if(fun.reg_int<= FT6800_FUNC_CAPA)//ELoader具有的功能
			//{
			//	strcpy(cmdbuf, pScpi_ft6800param[(fun.reg_int)<<1]);
			//}
			else//FT68200才具有的功能
			{
                          strcpy(cmdbuf, pScpi_param[(fun.reg_int+COM_CCH)<<1]);
				//strcpy(cmdbuf, pScpi_param[(fun.reg_int-FT6800_FUNC_CAPA+COM_CCH)<<1]);
			}

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

short scpi_spe_input_output_function(void)
{
	Scpipara_type state;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_FUNCTION|SCPI_STR, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&state.reg_int, MAP_TEST_FUNCTION);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}

	return 0;
}

short scpi_spe_input_output_function_quest(void)
{
	//int state;
	//short *perr_code;
	char cmdbuf[128];
	unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_TEST_FUNCTION,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_input_output_function_quest;
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

short scpi_spe_input_output_short(void)
{

	Scpipara_type state;
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
		requ_write_an_reg((u8*)&state.reg_int, MAP_SHORT_CTRL);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	

	return 0;
}

short scpi_spe_input_output_short_quest(void)
{
	//int state;
	//short *perr_code;
	char cmdbuf[128];

	unReg32 fun;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_STATUS_1,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_input_output_short_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &fun);

			if (fun.reg_int&BIT_SHORT)
			{
				fun.reg_int= 1;
			}
			else
			{
				fun.reg_int= 0;
			}

			strcpy(cmdbuf, pScpi_param[(fun.reg_int+COM_OFF)<<1]);
			strcat(cmdbuf, ";");
			//sprintf(cmdbuf, "%d;", fun.reg_int);
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

short scpi_spe_input_output_von_level(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_VON_POINT);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0;   
}
short scpi_spe_input_output_von_level_quest(void)
{
	//float volt;
	//short *perr_code;
	//char cmdbuf[128];
	//unReg32 regd;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_VON_POINT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;
}

short scpi_spe_input_output_voff_level(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_VOFF_POINT);			

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;  //先返回成功
}

short scpi_spe_input_output_voff_level_quest(void)
{
	//float volt;
	//short *perr_code;
	//char cmdbuf[128];
	//unReg32 regd;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_VOFF_POINT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;
}

short scpi_spe_input_output_timer_level(void)
{	
	Scpipara_type timer;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &timer, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&timer.reg_int, MAP_LOAD_TIME);	
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;  
}

short scpi_spe_input_output_timer_level_quest(void)
{
	//int timer;
	//short *perr_code;
	//char cmdbuf[128];
	//unReg32 regd;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_LOAD_TIME,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntSCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}
	return 0;
}

short scpi_spe_input_output_prot_current(void)
{
	Scpipara_type curr;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &curr, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		if (curr.type== SCPI_INT)
		{
			curr.reg_floa= (float)curr.reg_int;	
		}
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_PROT_OVER_CURR);

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	

	return 0;        
}

short scpi_spe_input_output_prot_current_quest(void)
{	
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_PROT_OVER_CURR,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		
	}
	return 0;
}



short scpi_spe_input_output_prot_volt(void)
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
	return 0;        
}

short scpi_spe_input_output_prot_volt_quest(void)
{
	//float volt;
	//short *perr_code;
	//char cmdbuf[128];
//unReg32 regd;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_PROT_OVER_VOLT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		
	}
	return 0;
}


short scpi_spe_input_output_prot_power(void)
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
		requ_write_an_reg((u8*)&power.reg_floa, MAP_PROT_OVER_POW);

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;        
}

short scpi_spe_input_output_prot_power_quest(void)
{
	//float power;
	//short *perr_code;
	//char cmdbuf[128];
        //unReg32 regd;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_PROT_OVER_POW,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}


short scpi_spe_input_output_prot_clear(void)
{
	int lcl= 1;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		requ_write_an_reg((u8*)&lcl, MAP_CLEAR_FAULT);		

		//Send_event= NULL;
		Recv_event= scpi_spe_input_output_prot_clear;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口

		if (client_rec_buf[1]== MB_FUNC_WRITE_REGISTER)
		{
			;			
		}
		else
			return ERR_QUERY_ERROR;//返回错误码
	}

	return 0;     
}
