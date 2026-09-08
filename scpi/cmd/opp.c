
//OPP测试
#include "include.h"

short scpi_spe_opp_start_voltage(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_OPP_VON_VOLT);	

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}

	return 0;//先返回成功
}

short scpi_spe_opp_start_voltage_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_OPP_VON_VOLT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;//先返回成功
}

short scpi_spe_opp_start_delay(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_OPP_VON_DELAY);	

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;//先返回成功
}

short scpi_spe_opp_start_delay_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_OPP_VON_DELAY,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestSCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;//先返回成功
}

short scpi_spe_opp_original_power(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_OPP_ORIG_POW);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;        
}

short scpi_spe_opp_original_power_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_OPP_ORIG_POW,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_opp_step_power(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_OPP_DELT_POW);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;        
}

short scpi_spe_opp_step_power_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_OPP_DELT_POW,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_opp_step_dwell(void)
{
	Scpipara_type awhile;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &awhile, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		if (awhile.type== SCPI_INT)
		{
			awhile.reg_floa= (float)awhile.reg_int;	
		}
		requ_write_an_reg((u8*)&awhile.reg_floa, MAP_OPP_DWELL_TIME);	
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;        
}

short scpi_spe_opp_step_dwell_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_OPP_DWELL_TIME,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestSCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_opp_end_power(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_OPP_END_POW);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;        
}

short scpi_spe_opp_end_power_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_OPP_END_POW,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_opp_end_voltage(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_OPP_END_VOLT);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;        
}

short scpi_spe_opp_end_voltage_quest(void)
{


	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_OPP_END_VOLT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;
}

short scpi_spe_opp_trip_upper(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_OPP_MAX_TRIP);	

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}

	return 0;//先返回成功
}

short scpi_spe_opp_trip_upper_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_OPP_MAX_TRIP,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_opp_trip_lower(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_OPP_MIN_TRIP);	

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;//先返回成功
}

short scpi_spe_opp_trip_lower_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_OPP_MIN_TRIP,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_opp_status_result_quest(void)
{
	unReg32 regd, regd2;
	char cmdbuf[128];
	char tmp[10];
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_OPP_RESULT_WORD, 2 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_opp_status_result_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	else if(can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口	

		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &regd);//检查信息	
			GetModbusRegValue(client_rec_buf+7, &regd2);//测量值

			if (regd.reg_int== 2)//通过
			{
				strcpy(cmdbuf, "PASS");
			}
			else if(regd.reg_int== 3)//失败
			{
				strcpy(cmdbuf, "FAIL");
			}
			else if (regd.reg_int== 1)//错误
			{
				strcpy(cmdbuf, "ERROR;");
			}
			else//无结果
			{
				strcpy(cmdbuf, "NULL;");
			}

			tmp[0]= 0;
			if ((regd.reg_int==2) || (regd.reg_int== 3))
			{				
				sprintf(tmp, ",%.4fW;", regd2.reg_floa);					
			}

			strcat(cmdbuf, tmp);
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
