#include "include.h"

short scpi_spe_measure_current_quest(void)
{
	//float curr;
	//short *perr_code;
	//char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_CURRENT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_measure_volt_quest(void)
{
	//float volt;
	//short *perr_code;
	//char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_VOLTAGE,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_measure_power_quest(void)
{
	//float power;
	//short *perr_code;
	//char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_POWER,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}


short scpi_spe_measure_resis_quest(void)
{
	//float resis;
	//short *perr_code;
	//char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_RESISTANCE,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}
	return 0;
}

short scpi_spe_measure_temperature_quest(void)
{
	//float temp;
	//short *perr_code;
	//char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_TEMPERATURE,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}


short scpi_spe_measure_charge_quest(void)
{
	//float temp;
	//short *perr_code;
	//char cmdbuf[128];
	char cmdbuf[128];
	unReg32 regdat;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_QUANTITY,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_measure_charge_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &regdat);

			regdat.reg_floa/= 3600;//库伦转换为安时
			sprintf(cmdbuf, "%.4fAH;", regdat.reg_floa);
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



short scpi_spe_measure_energy_quest(void)
{
	//float temp;
	//short *perr_code;
	//char cmdbuf[128];
	char cmdbuf[128];
	unReg32 regdat;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_ENERGY,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_measure_energy_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &regdat);

			regdat.reg_floa/= 3600;//库伦转换为瓦时
			sprintf(cmdbuf, "%.4fWH;", regdat.reg_floa);
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



short scpi_spe_measure_time_quest(void)
{
	//int temp;
	//short *perr_code;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_LOADING_TIME,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	
	return 0;
}

short scpi_spe_fetch_status_quest(void)
{
	unReg32 fun;

	char cmdbuf[128];
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_STATUS_1,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_fetch_status_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &fun);
			sprintf(cmdbuf, "%u;", (u32)fun.reg_int);
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
