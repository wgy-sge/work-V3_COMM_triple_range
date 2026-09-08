

#include "include.h"
//编辑序列
short scpi_spe_seq_edit_file_number(void)
{
	Scpipara_type fnum;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &fnum, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&fnum.reg_int, MAP_SEQ_EDIT_FILE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_seq_edit_file_number_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_EDIT_FILE,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_seq_edit_file_length(void)
{
	Scpipara_type flength;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &flength, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&flength.reg_int, MAP_SEQ_EDIT_LENGTH);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_seq_edit_file_length_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_EDIT_LENGTH,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		
	}
	return 0;
}

short scpi_spe_seq_edit_cycle(void)
{
	Scpipara_type flength;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &flength, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&flength.reg_int, MAP_SEQ_EDIT_CYCLE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_seq_edit_cycle_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_EDIT_CYCLE,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}
short scpi_spe_seq_edit_link(void)
{
	Scpipara_type flength;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &flength, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&flength.reg_int, MAP_SEQ_EDIT_LINK);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_seq_edit_link_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_EDIT_LINK,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;
}
short scpi_spe_seq_edit_step(void)
{
	Scpipara_type step;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &step, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&step.reg_int, MAP_SEQ_EDIT_STEP);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_seq_edit_step_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_EDIT_STEP,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	return 0;
}

short scpi_spe_seq_edit_level(void)
{
	Scpipara_type dpara;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &dpara, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (dpara.type== SCPI_INT)
		{
			dpara.reg_floa= (float)dpara.reg_int;	
		}
		requ_write_an_reg((u8*)&dpara.reg_floa, MAP_SEQ_EDIT_LOAD);

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;        
}

short scpi_spe_seq_edit_level_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_EDIT_LOAD,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_seq_edit_width(void)
{
	Scpipara_type width;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_FLOAT, &width, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (width.type== SCPI_INT)
	{
		width.reg_floa= width.reg_int;
	} 

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&width.reg_floa, MAP_SEQ_EDIT_DWELL);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_seq_edit_width_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_EDIT_DWELL,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;
}

short scpi_spe_seq_edit_slew(void)
{
	Scpipara_type width;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_FLOAT, &width, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (width.type== SCPI_INT)
	{
		width.reg_floa= width.reg_int;
	} 

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&width.reg_floa, MAP_SEQ_EDIT_SLEW);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_seq_edit_slew_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_EDIT_SLEW,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;
}

short scpi_spe_seq_edit_mode(void)
{
	Scpipara_type width;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_SEQ_MODE|SCPI_STR, &width, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&width.reg_int, MAP_SEQ_EDIT_MODE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_seq_edit_mode_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_EDIT_MODE,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestEnumStringAnswer;
		EnumStringType= SEQ_MODE_STRING;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;
}

short scpi_spe_seq_edit_save(void)
{
	Scpipara_type seq_save;
		
	seq_save.reg_int= 1;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&seq_save.reg_int, MAP_SEQ_SAVE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

//运行序列
short scpi_spe_seq_test_file(void)
{
	Scpipara_type fnum;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &fnum, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&fnum.reg_int, MAP_SEQ_RUN_FILE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_seq_test_file_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_RUN_FILE,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_seq_status_step_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_STAT_STEP,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	return 0;
}

short scpi_spe_seq_status_circle_quest(void)
{	

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_SEQ_STAT_CYCL,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	return 0;
}




