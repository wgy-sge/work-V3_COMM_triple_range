#include "include.h"

short scpi_spe_discharge_test_mode(void)
{
	Scpipara_type fnum;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_DISCH_MODE|SCPI_STR, &fnum, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&fnum.reg_int, MAP_DISC_MODE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

const char *pDischModeStrings[]=
{	
	"cc",	
	"cr",
	"cp"
};

short scpi_spe_discharge_test_mode_quest(void)
{
	unReg32 mode;

	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_DISC_MODE,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_discharge_test_mode_quest;
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
			strcpy(cmdbuf, pDischModeStrings[mode.reg_int]);
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
short scpi_spe_discharge_test_current(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_DISC_CURR);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_discharge_test_current_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_DISC_CURR,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}
short scpi_spe_discharge_test_resistance(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_DISC_RESI);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_discharge_test_resistance_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_DISC_RESI,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}
short scpi_spe_discharge_test_power(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_DISC_POW);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_discharge_test_power_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_DISC_POW,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestPowerCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}
short scpi_spe_discharge_end_voltage(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_DISC_END_VOLT);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_discharge_end_voltage_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_DISC_END_VOLT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}
short scpi_spe_discharge_end_capacity(void)
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
		requ_write_an_reg((u8*)&curr.reg_floa, MAP_DISC_END_CAPA);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0; 
}
short scpi_spe_discharge_end_capacity_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_DISC_END_CAPA,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}
short scpi_spe_discharge_end_time(void)
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
			curr.reg_int= (int)curr.reg_floa ;	
		}
		requ_write_an_reg((u8*)&curr.reg_int, MAP_DISC_END_TIME);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_discharge_end_time_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_DISC_END_TIME,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	return 0;
}

short scpi_spe_discharge_result_quest(void)
{
	unReg32 regd;
	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_QUANTITY,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_discharge_result_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &regd);

			regd.reg_floa/= 3600;
			sprintf(cmdbuf, "%.4fAH;", regd.reg_floa);
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

short scpi_spe_discharge_status_capacity_quest(void)
{
	unReg32 regd;
	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_QUANTITY,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_discharge_status_capacity_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &regd);
		
			//regd.reg_floa/= 3600;
			sprintf(cmdbuf, "%.4fC;", regd.reg_floa);
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

short scpi_spe_discharge_status_energy_quest(void)
{
	unReg32 regd;
	char cmdbuf[128];
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_RB_ENERGY,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_discharge_status_energy_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &regd);

			regd.reg_floa/= 3600;
			sprintf(cmdbuf, "%.4fWH;", regd.reg_floa);
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

short scpi_spe_discharge_status_time_quest(void)
{
	unReg32 regd;
        char cmdbuf[128];
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_DISC_TIME,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_discharge_status_time_quest;
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
			GetModbusRegValue(client_rec_buf+3, &regd);	
			regd.reg_int/= 1000;//转换为秒
			sprintf(cmdbuf, "%dS;", regd.reg_int);	

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