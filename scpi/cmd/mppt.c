#include "include.h"

// MPPT 步长单位为 V，调整周期以秒保存，时间换算由主控完成。

short scpi_spe_mppt_step(void)
{
	Scpipara_type value;
	short err_code;
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &value, &err_code);
		if (pscpi_pb== NULL)
			return err_code;
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (value.type== SCPI_INT)
		{
			value.reg_floa= (float)value.reg_int;
		}
		requ_write_an_reg((u8*)&value.reg_floa, MAP_MPPT_STEP_SIZE);
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//等待主控应答后继续处理命令
	}
	return 0;
}

short scpi_spe_mppt_step_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_MPPT_STEP_SIZE,1 );
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;
	}
	return 0;
}

short scpi_spe_mppt_period(void)
{
	Scpipara_type value;
	short err_code;
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &value, &err_code);
		if (pscpi_pb== NULL)
			return err_code;
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (value.type== SCPI_INT)
		{
			value.reg_floa= (float)value.reg_int;
		}
		requ_write_an_reg((u8*)&value.reg_floa, MAP_MPPT_REGU_PERIOD);
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//等待主控应答后继续处理命令
	}
	return 0;
}

short scpi_spe_mppt_period_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_MPPT_REGU_PERIOD,1 );
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;
	}
	return 0;
}
