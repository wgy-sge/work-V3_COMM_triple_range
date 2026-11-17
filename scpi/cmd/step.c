#include "include.h"

// STEP 模式：0=CV、1=CC、2=CP、3=CR；起始值、步长和终止值单位随模式确定。
// 触发方式：0=定时、1=触发；定时间隔以秒保存，参数范围由主控检查。

short scpi_spe_step_mode(void)
{
	Scpipara_type value;
	short err_code;
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &value, &err_code);
		if (pscpi_pb== NULL)
			return err_code;
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&value.reg_int, MAP_STEP_RUN_MODE);
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//等待主控应答后继续处理命令
	}
	return 0;
}

short scpi_spe_step_mode_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_STEP_RUN_MODE,1 );
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;
	}
	return 0;
}

short scpi_spe_step_start(void)
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
		requ_write_an_reg((u8*)&value.reg_floa, MAP_STEP_START);
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//等待主控应答后继续处理命令
	}
	return 0;
}

short scpi_spe_step_start_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_STEP_START,1 );
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;
	}
	return 0;
}

short scpi_spe_step_delta(void)
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
		requ_write_an_reg((u8*)&value.reg_floa, MAP_STEP_DELTA);
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//等待主控应答后继续处理命令
	}
	return 0;
}

short scpi_spe_step_delta_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_STEP_DELTA,1 );
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;
	}
	return 0;
}

short scpi_spe_step_stop(void)
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
		requ_write_an_reg((u8*)&value.reg_floa, MAP_STEP_STOP);
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//等待主控应答后继续处理命令
	}
	return 0;
}

short scpi_spe_step_stop_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_STEP_STOP,1 );
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;
	}
	return 0;
}

short scpi_spe_step_trigger_mode(void)
{
	Scpipara_type value;
	short err_code;
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &value, &err_code);
		if (pscpi_pb== NULL)
			return err_code;
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_write_an_reg((u8*)&value.reg_int, MAP_STEP_TRI_MODE_SELECT);
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//等待主控应答后继续处理命令
	}
	return 0;
}

short scpi_spe_step_trigger_mode_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_STEP_TRI_MODE_SELECT,1 );
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;
	}
	return 0;
}

short scpi_spe_step_timer(void)
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
		requ_write_an_reg((u8*)&value.reg_floa, MAP_STEP_TIMER);
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//等待主控应答后继续处理命令
	}
	return 0;
}

short scpi_spe_step_timer_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_STEP_TIMER,1 );
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;
	}
	return 0;
}
