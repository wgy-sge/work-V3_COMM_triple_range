#include "include.h"


//定态电压
short scpi_spe_static_volt_level_answer(void)
{
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;
	Scpipara_type volt;
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

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能
					
		if(fun.reg_int== FUNC_CVL)
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CVL_VOLT_SET);
		}
		else
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CVH_VOLT_SET);
			
		}

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}


	return 0;  
}
short scpi_spe_static_volt_level(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_volt_level_answer);      
}
short scpi_spe_static_volt_level_quest_answer(void)
{
	unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能		
		if(fun.reg_int== FUNC_CVL)
		{
			requ_read_regs( MAP_CVL_VOLT_SET, 1 );	
		}
		else
		{
			requ_read_regs( MAP_CVH_VOLT_SET, 1 );			
		}

		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;
}
short scpi_spe_static_volt_level_quest(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_volt_level_quest_answer);  
}

short scpi_spe_static_volt_range_answer(void)
{
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;

	Scpipara_type range;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &range, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}	

	fun= GetPreRegDat( );
	fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能
	if ((fun.reg_int== FUNC_CVH)||(fun.reg_int== FUNC_CVL))
	{
		if(range.reg_int)
		{
			fun.reg_int= FUNC_CVL;
		}
		else
		{
			fun.reg_int= FUNC_CVH;
		}
		requ_write_an_reg((u8*)&fun.reg_int, MAP_TEST_FUNCTION);
	}	
	else
	{
		return 0;//ERR_EXECUTION_ERROR;//当前功能不对
	}

	//Send_event= NULL;
	Recv_event= ScpiSetCmdAnswer;
	scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	return 0;
}

short scpi_spe_static_volt_range(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_volt_range_answer); 
}

short scpi_spe_static_volt_range_quest_answer(void)
{
	int range;
	unReg32 fun;
	char cmdbuf[128];


	fun= GetPreRegDat( );
	fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		
	if(fun.reg_int== FUNC_CVL)
	{
		range= 1;	
	}
	else
	{
		range= 0;
		
	}

	sprintf(cmdbuf, "%d;", range);
	if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
	outcounter= outcounter+ strlen(cmdbuf);
	return 0;
}

short scpi_spe_static_volt_range_quest(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_volt_range_quest_answer); 
}

short scpi_spe_static_volt_rise_answer(void)
{
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;
	Scpipara_type volt;
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

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能
			
		if(fun.reg_int== FUNC_CVL)
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CVL_SLEW_UP);
		}
		else
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CVH_SLEW_UP);			
		}

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}


	return 0;  
}
short scpi_spe_static_volt_rise(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_volt_rise_answer);  
}
short scpi_spe_static_volt_rise_quest_answer(void)
{
	unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能	
			
		if(fun.reg_int== FUNC_CVL)
		{
			requ_read_regs( MAP_CVL_SLEW_UP, 1 );	
		}
		else
		{
			requ_read_regs( MAP_CVH_SLEW_UP, 1 );			
		}

		//Send_event= NULL;
		Recv_event= ScpiQuestVoltSlewCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_static_volt_rise_quest(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_volt_rise_quest_answer); 
}
short scpi_spe_static_volt_fall_answer(void)
{
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;
	Scpipara_type volt;
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

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能
			
		if(fun.reg_int== FUNC_CVL)
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CVL_SLEW_DN);
		}
		else
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CVH_SLEW_DN);
			
		}

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}


	return 0;  
}
short scpi_spe_static_volt_fall(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_volt_fall_answer);  
}
short scpi_spe_static_volt_fall_quest_answer(void)
{
	unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能
				
		if(fun.reg_int== FUNC_CVL)
		{
			requ_read_regs( MAP_CVL_SLEW_DN, 1 );	
		}
		else
		{
			requ_read_regs( MAP_CVH_SLEW_DN, 1 );				
		}

		//Send_event= NULL;
		Recv_event= ScpiQuestVoltSlewCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_static_volt_fall_quest(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_volt_fall_quest_answer); 
}


//定态电流
short scpi_spe_static_current_level_answer(void)
{
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;
	Scpipara_type volt;
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

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能	
			
		if(fun.reg_int== FUNC_CCL)
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CCL_CURR_SET);
		}
		else
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CCH_CURR_SET);
			
		}

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}


	return 0;  
}
short scpi_spe_static_current_level(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_current_level_answer);      
}
short scpi_spe_static_current_level_quest_answer(void)
{
	unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能
				
		if(fun.reg_int== FUNC_CCL)
		{
			requ_read_regs( MAP_CCL_CURR_SET, 1 );	
		}
		else
		{
			requ_read_regs( MAP_CCH_CURR_SET, 1 );	
		}

		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;
}
short scpi_spe_static_current_level_quest(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_current_level_quest_answer);  
}

short scpi_spe_static_current_range_answer(void)
{
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;

	Scpipara_type range;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &range, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}	

	fun= GetPreRegDat( );
	fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

	if ((fun.reg_int== FUNC_CCH)||(fun.reg_int== FUNC_CCL))
	{
		if(range.reg_int)
		{
			fun.reg_int= FUNC_CCL;
		}
		else
		{
			fun.reg_int= FUNC_CCH;
		}
		requ_write_an_reg((u8*)&fun.reg_int, MAP_TEST_FUNCTION);
	}	
	else
	{
		return 0;//当前功能不对
	}

	//Send_event= NULL;
	Recv_event= ScpiSetCmdAnswer;
	scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	return 0;
}
short scpi_spe_static_current_range(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_current_range_answer); 
}
short scpi_spe_static_current_range_quest_answer(void)
{
	int range;
	unReg32 fun;
	char cmdbuf[128];

	fun= GetPreRegDat( );
	fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

	if(fun.reg_int== FUNC_CCL)
	{
		range= 1;	
	}
	else
	{
		range= 0;	//当前功能不对
	}

	sprintf(cmdbuf, "%d;", range);
	if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
	outcounter= outcounter+ strlen(cmdbuf);
	return 0;
}
short scpi_spe_static_current_range_quest(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_current_range_quest_answer); 
}
short scpi_spe_static_current_rise_answer(void)
{
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;
	Scpipara_type volt;
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

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能
	
		if(fun.reg_int== FUNC_CCL)
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CCL_SLEW_UP);
		}
		else
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CCH_SLEW_UP);//当前功能不对
		}

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}


	return 0;  
}
short scpi_spe_static_current_rise(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_current_rise_answer);  
}
short scpi_spe_static_current_rise_quest_answer(void)
{
	unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能
			
		if(fun.reg_int== FUNC_CCL)
		{
			requ_read_regs( MAP_CCL_SLEW_UP, 1 );	
		}
		else
		{
			requ_read_regs( MAP_CCH_SLEW_UP, 1 );//当前功能不对
		}

		//Send_event= NULL;
		Recv_event= ScpiQuestCurrSlewCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_static_current_rise_quest(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_current_rise_quest_answer); 
}
short scpi_spe_static_current_fall_answer(void)
{
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;
	Scpipara_type volt;
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

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能				
		if(fun.reg_int== FUNC_CCL)
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CCL_SLEW_DN);
		}
		else
		{
			requ_write_an_reg((u8*)&volt.reg_floa, MAP_CCH_SLEW_DN);//当前功能不对
		}

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}


	return 0;  
}
short scpi_spe_static_current_fall(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_current_fall_answer);  
}
short scpi_spe_static_current_fall_quest_answer(void)
{
	unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能
	
			if(fun.reg_int== FUNC_CCL)
		{
			requ_read_regs( MAP_CCL_SLEW_DN, 1 );	
		}
		else
		{
			requ_read_regs( MAP_CCH_SLEW_DN, 1 );//当前功能不对
		}

		//Send_event= NULL;
		Recv_event= ScpiQuestCurrSlewCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;
}
short scpi_spe_static_current_fall_quest(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_current_fall_quest_answer); 
}




//定态功率
short scpi_spe_static_power_level(void)
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
		requ_write_an_reg((u8*)&power.reg_floa, MAP_CP_POW_SET);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;        
}
short scpi_spe_static_power_level_quest(void)
{
	//float power;
	//short *perr_code;
	//char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CP_POW_SET,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

//定态阻值
short scpi_spe_static_resistance_level_answer(void)
{
	Scpipara_type resis;
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &resis, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (resis.type== SCPI_INT)
		{
			resis.reg_floa= (float)resis.reg_int;	
		}

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能	

		if(fun.reg_int== FUNC_CRL)
		{
			requ_write_an_reg((u8*)&resis.reg_floa, MAP_CRL_RESI_SET);
		}
		else
		{
			requ_write_an_reg((u8*)&resis.reg_floa, MAP_CRH_RESI_SET);
		}

		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;        
}
short scpi_spe_static_resistance_level(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_resistance_level_answer);      
}
short scpi_spe_static_resistance_level_quest_answer(void)
{
	unReg32 fun;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能	

		if(fun.reg_int== FUNC_CRL)
		{
			requ_read_regs( MAP_CRL_RESI_SET, 1);
		}
		else
		{
			requ_read_regs( MAP_CRH_RESI_SET, 1);
		}

		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}	
	return 0;
}
short scpi_spe_static_resistance_level_quest(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_resistance_level_quest_answer);      
}


short scpi_spe_static_resistance_rise_answer(void)
{
	Scpipara_type resis;
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &resis, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (resis.type== SCPI_INT)
		{
			resis.reg_floa= (float)resis.reg_int;	
		}

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能	

		if(fun.reg_int== FUNC_CRL)
		{
			requ_write_an_reg((u8*)&resis.reg_floa, MAP_CRL_SLEW_UP);
		}
		else
		{
			requ_write_an_reg((u8*)&resis.reg_floa, MAP_CRH_SLEW_UP);
		}

		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;        
}
short scpi_spe_static_resistance_rise(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_resistance_rise_answer);      
}
short scpi_spe_static_resistance_rise_quest_answer(void)
{
	unReg32 fun;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能	

		if(fun.reg_int== FUNC_CRL)
		{
			requ_read_regs( MAP_CRL_SLEW_UP, 1);
		}
		else
		{
			requ_read_regs( MAP_CRH_SLEW_UP, 1);
		}

		Recv_event= ScpiQuestCurrSlewCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}	
	return 0;
}
short scpi_spe_static_resistance_rise_quest(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_resistance_rise_quest_answer);      
}

short scpi_spe_static_resistance_fall_answer(void)
{
	Scpipara_type resis;
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &resis, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (resis.type== SCPI_INT)
		{
			resis.reg_floa= (float)resis.reg_int;	
		}

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能	

		if(fun.reg_int== FUNC_CRL)
		{
			requ_write_an_reg((u8*)&resis.reg_floa, MAP_CRL_SLEW_DN);
		}
		else
		{
			requ_write_an_reg((u8*)&resis.reg_floa, MAP_CRH_SLEW_DN);
		}

		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;        
}
short scpi_spe_static_resistance_fall(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_resistance_fall_answer);      
}
short scpi_spe_static_resistance_fall_quest_answer(void)
{
	unReg32 fun;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能	

		if(fun.reg_int== FUNC_CRL)
		{
			requ_read_regs( MAP_CRL_SLEW_DN, 1);
		}
		else
		{
			requ_read_regs( MAP_CRH_SLEW_DN, 1);
		}

		Recv_event= ScpiQuestCurrSlewCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}	
	return 0;
}
short scpi_spe_static_resistance_fall_quest(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_resistance_fall_quest_answer);      
}

	

short scpi_spe_static_resistance_range_answer(void)
{
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;

	Scpipara_type range;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &range, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}	

	fun= GetPreRegDat( );
	fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

	if ((fun.reg_int== FUNC_CRH)||(fun.reg_int== FUNC_CRL))
	{
		if(range.reg_int)
		{
			fun.reg_int= FUNC_CRL;
		}
		else
		{
			fun.reg_int= FUNC_CRH;
		}
		requ_write_an_reg((u8*)&fun.reg_int, MAP_TEST_FUNCTION);
	}	
	else
	{
		return 0;//当前功能不对
	}

	//Send_event= NULL;
	Recv_event= ScpiSetCmdAnswer;
	scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	return 0;
}
short scpi_spe_static_resistance_range(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_resistance_range_answer); 
}
short scpi_spe_static_resistance_range_quest_answer(void)
{
	int range;
	unReg32 fun;
	char cmdbuf[128];


	fun= GetPreRegDat( );
	fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

	if(fun.reg_int== FUNC_CRL)
	{
		range= 1;	
	}
	else
	{
		range= 0;	//当前功能不对
	}

	sprintf(cmdbuf, "%d;", range);
	if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
	outcounter= outcounter+ strlen(cmdbuf);
	return 0;
}
short scpi_spe_static_resistance_range_quest(void)
{
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_static_resistance_range_quest_answer); 
}