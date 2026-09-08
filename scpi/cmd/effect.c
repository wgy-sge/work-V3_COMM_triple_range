#include "include.h"

short scpi_spe_effect_current_min(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_LOEF_CURR_MIN);	

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}

	return 0;//先返回成功
}

short scpi_spe_effect_current_min_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_LOEF_CURR_MIN,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_effect_current_max(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_LOEF_CURR_MAX);	

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}

	return 0;//先返回成功
}
short scpi_spe_effect_current_max_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_LOEF_CURR_MAX,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_effect_current_normal(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_LOEF_CURR_NORM);	

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}

	return 0;//先返回成功
}
short scpi_spe_effect_current_normal_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_LOEF_CURR_NORM,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;

}

short scpi_spe_effect_test_dewll(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_LOEF_DELAY);	

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}

	return 0;//先返回成功

}

short scpi_spe_effect_test_dewll_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_LOEF_DELAY,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestSCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;

}

short scpi_spe_effect_status_result_quest(void)
{
	unReg32 regd, regd2, regd3;
	char cmdbuf[128];
	char tmp[10];
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_LOEF_REGUL, 3 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_effect_status_result_quest;
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
			GetModbusRegValue(client_rec_buf+3, &regd);//负载效应之调整率
			GetModbusRegValue(client_rec_buf+7, &regd2);//负载效应之电压变化
			GetModbusRegValue(client_rec_buf+11, &regd3);//负载效应之电源内阻

			sprintf(cmdbuf, "%.4f,%.4f,%.4f;", regd2.reg_floa, regd.reg_floa, regd3.reg_floa);	
			
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

	
	
	

	
	
	
	



