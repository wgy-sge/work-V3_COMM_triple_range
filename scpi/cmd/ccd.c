#include "include.h"

// 中量程使用表尾扩展寄存器，按当前测试功能选择读写地址。


//瞬态电流
short scpi_spe_transient_current_fashion_answer(void)
{
	Scpipara_type fashion;
	short err_code;	
	u8 *pscpi_pb;

	unReg32 fun;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_STR|SCPI_CONT|SCPI_PULSE_TOGG , &fashion, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 			
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_write_an_reg((u8*)&fashion.reg_int, MAP_CCDL_WAY);	
		}
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_write_an_reg((u8*)&fashion.reg_int, MAP_CCDM_WAY);
		}
		else
		{
			requ_write_an_reg((u8*)&fashion.reg_int, MAP_CCDH_WAY);	
		}

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_transient_current_fashion(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_fashion_answer);      
}
short scpi_spe_transient_current_fashion_quest_answer(void)
{
	unReg32 fun;

	char cmdbuf[128];
	unReg32 regd;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_read_regs( MAP_CCDL_WAY,1 );
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_read_regs( MAP_CCDM_WAY,1 );
		}
		else
		{
			requ_read_regs( MAP_CCDH_WAY,1 );
		}

		//Send_event= NULL;
		Recv_event= scpi_spe_transient_current_fashion_quest_answer;
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
			GetModbusRegValue(client_rec_buf+3, &regd);		

			strcpy(cmdbuf, pScpi_param[(regd.reg_int+COM_CONT)<<1]);
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
short scpi_spe_transient_current_fashion_quest(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_fashion_quest_answer);      
}

short scpi_spe_transient_current_mlevel_answer(void)
{
	Scpipara_type curr;
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;

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

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDL_L1);	
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDM_L1);
		}
		else
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDH_L1);	
		}
		
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}
	return 0;        
}

short scpi_spe_transient_current_mlevel(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_mlevel_answer);      
}

short scpi_spe_transient_current_mlevel_quest_answer(void)
{
	unReg32 fun;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_read_regs( MAP_CCDL_L1,1 );	
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_read_regs( MAP_CCDM_L1,1 );
		}
		else
		{
			requ_read_regs( MAP_CCDH_L1,1 );	
		}				
	
		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_transient_current_mlevel_quest(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_mlevel_quest_answer);      
}
short scpi_spe_transient_current_mwidth_answer(void)
{
	Scpipara_type mwidth;
	short err_code;	
	u8 *pscpi_pb;	
	unReg32 fun;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_FLOAT, &mwidth, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (mwidth.type== SCPI_INT)
		{
			mwidth.reg_floa= (float)mwidth.reg_int;
		}
	
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_write_an_reg((u8*)&mwidth.reg_floa, MAP_CCDL_T1);		
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_write_an_reg((u8*)&mwidth.reg_floa, MAP_CCDM_T1);
		}
		else
		{
			requ_write_an_reg((u8*)&mwidth.reg_floa, MAP_CCDH_T1);	
		}	
		
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	

	return 0;  
}

short scpi_spe_transient_current_mwidth(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_mwidth_answer);      
}
short scpi_spe_transient_current_mwidth_quest_answer(void)
{
	unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_read_regs( MAP_CCDL_T1,1 );
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_read_regs( MAP_CCDM_T1,1 );
		}
		else
		{
			requ_read_regs( MAP_CCDH_T1,1 );	
		}		
		Recv_event= ScpiQuestMSCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_transient_current_mwidth_quest(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_mwidth_quest_answer);      
}

short scpi_spe_transient_current_tlevel_answer(void)
{
	Scpipara_type curr;
	short err_code;	
	u8 *pscpi_pb;	
unReg32 fun;
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
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDL_L2);		
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDM_L2);
		}
		else
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDH_L2);
		}	
		
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}

	return 0;  
}

short scpi_spe_transient_current_tlevel(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_tlevel_answer);      
}

short scpi_spe_transient_current_tlevel_quest_answer(void)
{
unReg32 fun;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_read_regs( MAP_CCDL_L2,1 );
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_read_regs( MAP_CCDM_L2,1 );
		}
		else
		{
			requ_read_regs( MAP_CCDH_L2,1 );	
		}

		Recv_event= ScpiQuestCurrCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_transient_current_tlevel_quest(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_tlevel_quest_answer);      
}

short scpi_spe_transient_current_twidth_answer(void)
{
	Scpipara_type twidth;
	short err_code;	
	u8 *pscpi_pb;	
unReg32 fun;
	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_FLOAT, &twidth, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (twidth.type== SCPI_INT)
		{
			twidth.reg_floa= (float)twidth.reg_int;
		}
		
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_write_an_reg((u8*)&twidth.reg_floa, MAP_CCDL_T2);			
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_write_an_reg((u8*)&twidth.reg_floa, MAP_CCDM_T2);
		}
		else
		{
			requ_write_an_reg((u8*)&twidth.reg_floa, MAP_CCDH_T2);	
		}	
		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}


	return 0;  
}
short scpi_spe_transient_current_twidth(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_twidth_answer);      
}

short scpi_spe_transient_current_twidth_quest_answer(void)
{
	unReg32 fun;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_read_regs( MAP_CCDL_T2,1 );
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_read_regs( MAP_CCDM_T2,1 );
		}
		else
		{
			requ_read_regs( MAP_CCDH_T2,1 );	
		}			
		
		Recv_event= ScpiQuestMSCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	return 0;
}

short scpi_spe_transient_current_twidth_quest(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_twidth_quest_answer);      
}

short scpi_spe_transient_current_negative_answer(void)
{
	Scpipara_type curr;
	short err_code;	
	u8 *pscpi_pb;	
unReg32 fun;
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

		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDL_SLEW_DN);		
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDM_SLEW_DN);
		}
		else
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDH_SLEW_DN);
		}	
		
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}	
	return 0;        
}

short scpi_spe_transient_current_negative(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_negative_answer);      
}

short scpi_spe_transient_current_negative_quest_answer(void)
{
	unReg32 fun;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_read_regs( MAP_CCDL_SLEW_DN,1 );	
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_read_regs( MAP_CCDM_SLEW_DN,1 );
		}
		else
		{
			requ_read_regs( MAP_CCDH_SLEW_DN,1 );		
		}				
		
		Recv_event= ScpiQuestCurrSlewCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	return 0;
}
short scpi_spe_transient_current_negative_quest(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_negative_quest_answer);      
}
short scpi_spe_transient_current_positive_answer(void)
{
	Scpipara_type curr;
	short err_code;	
	u8 *pscpi_pb;	
unReg32 fun;
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
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDL_SLEW_UP);	
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDM_SLEW_UP);
		}
		else
		{
			requ_write_an_reg((u8*)&curr.reg_floa, MAP_CCDH_SLEW_UP);
		}		
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;  

}
short scpi_spe_transient_current_positive(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_positive_answer);      
}
short scpi_spe_transient_current_positive_quest_answer(void)
{
  unReg32 fun;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		fun= GetPreRegDat( );
		fun.reg_int= (fun.reg_int&BITS_FUNCTION)>> SHIFT_FUNCTION;//当前测试功能

		if(fun.reg_int== FUNC_CCDL)
		{
			requ_read_regs( MAP_CCDL_SLEW_UP,1 );	
		}		
		else if(fun.reg_int== FUNC_CCDM)
		{
			requ_read_regs( MAP_CCDM_SLEW_UP,1 );
		}
		else
		{
			requ_read_regs( MAP_CCDH_SLEW_UP,1 );	
		}					
		
		Recv_event= ScpiQuestCurrSlewCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;	
}
short scpi_spe_transient_current_positive_quest(void)
{	
	return PreLoadRegAsk(MAP_RB_STATUS_1, scpi_spe_transient_current_positive_quest_answer);      
}
