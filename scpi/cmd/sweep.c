
#include "include.h"

short scpi_spe_sweep_current_l1(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_SWEEP_CURR_A);

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}


	return 0; 
}
short scpi_spe_sweep_current_l1_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SWEEP_CURR_A,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}
short scpi_spe_sweep_current_l2(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_SWEEP_CURR_B);

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_sweep_current_l2_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SWEEP_CURR_B,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;
}
short scpi_spe_sweep_current_rise(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_SWEEP_SLEW_UP);

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_sweep_current_rise_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SWEEP_SLEW_UP,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;
}
short scpi_spe_sweep_current_fall(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_SWEEP_SLEW_DN);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_sweep_current_fall_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SWEEP_SLEW_DN,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;
}
short scpi_spe_sweep_frequency_start(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_SWEEP_FSTART);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_sweep_frequency_start_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SWEEP_FSTART,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;
}
short scpi_spe_sweep_frequency_end(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_SWEEP_FEND);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_sweep_frequency_end_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SWEEP_FEND,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;
}
short scpi_spe_sweep_frequency_step(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_SWEEP_FSTEP);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_sweep_frequency_step_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SWEEP_FSTEP,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;
}
short scpi_spe_sweep_wave_duty(void)
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
		if (curr.type== SCPI_FLOAT)
		{
			curr.reg_int= (int)curr.reg_floa;	
		}
		requ_write_an_reg((u8*)&curr.reg_int, MAP_SWEEP_DUTY);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_sweep_wave_duty_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SWEEP_DUTY,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;
}
short scpi_spe_sweep_wave_dweel(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_SWEEP_DWELL);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_sweep_wave_dweel_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SWEEP_DWELL,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;
}
short scpi_spe_sweep_status_result_quest(void)
{
	unReg32 regd[4];
	u8 i;
	char cmdbuf[128];
	char tmp[10];
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SWEEP_VPP_VOLT, 4 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_sweep_status_result_quest;
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
			for (i= 0; i< 4; i++)
			{
				GetModbusRegValue(client_rec_buf+3+ i*4, &regd[i]);	
			}			
            sprintf(cmdbuf, "%.4fV,%.4fHz,%.4fV,%.4fHz;", regd[0].reg_floa, \
				regd[1].reg_floa,regd[2].reg_floa,regd[3].reg_floa);	
			
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