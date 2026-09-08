
#include "include.h"



//编辑自动测试文件
short scpi_spe_auto_edit_file_number(void)
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
		requ_write_an_reg((u8*)&fnum.reg_int, MAP_AUTO_EDIT_FILE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}
	
	return 0;
}

short scpi_spe_auto_edit_file_number_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_EDIT_FILE,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_auto_edit_file_length(void)
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
		requ_write_an_reg((u8*)&flength.reg_int, MAP_AUTO_EDIT_LENGTH);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_auto_edit_file_length_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_EDIT_LENGTH,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_auto_edit_step(void)
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
		requ_write_an_reg((u8*)&step.reg_int, MAP_AUTO_EDIT_STEP);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_auto_edit_step_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_EDIT_STEP,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_auto_edit_mode(void)
{
	Scpipara_type mode;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_MODE|SCPI_STR , &mode, &err_code);
		if (pscpi_pb== NULL)
			return err_code;			
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&mode.reg_int, MAP_AUTO_EDIT_MODE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

const char *pModeStrings[]=
{	
	"cc",
	"cv",
	"cr",
	"cp"
};

short scpi_spe_auto_edit_mode_quest(void)
{
	unReg32 mode;
	
	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_EDIT_MODE,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_auto_edit_mode_quest;
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
			strcpy(cmdbuf, pModeStrings[mode.reg_int]);
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

short scpi_spe_auto_edit_level(void)
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
		requ_write_an_reg((u8*)&dpara.reg_floa, MAP_AUTO_EDIT_LOAD);

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;        
}

short scpi_spe_auto_edit_level_quest(void)
{
	

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_EDIT_LOAD,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_auto_edit_short(void)
{
	Scpipara_type state;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_ON_OFF|SCPI_STR, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&state.reg_int, MAP_AUTO_EDIT_SHORT);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	return 0;
}

short scpi_spe_auto_edit_short_quest(void)
{
	unReg32 state;
	
	char cmdbuf[128];
	
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_EDIT_SHORT, 1);		
		//Send_event= NULL;
		Recv_event= scpi_spe_auto_edit_short_quest;
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
			GetModbusRegValue(client_rec_buf+3, &state);					
			strcpy(cmdbuf, pScpi_param[((COM_OFF+state.reg_int)<< 1)+1]);			
			strcat(cmdbuf, ";");
			//sprintf(cmdbuf, "%d;", state);
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

short scpi_spe_auto_edit_rbwhat(void)
{
	Scpipara_type rbwhat;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &rbwhat, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&rbwhat.reg_int, MAP_AUTO_EDIT_CHECK);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_auto_edit_rbwhat_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_EDIT_CHECK,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}
	return 0;
}

short scpi_spe_auto_edit_llimit(void)
{
	Scpipara_type limit;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &limit, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (limit.type== SCPI_INT)
		{
			limit.reg_floa= (float)limit.reg_int;	
		}
		requ_write_an_reg((u8*)&limit.reg_floa, MAP_AUTO_EDIT_LMDN);	

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;        
}

short scpi_spe_auto_edit_llimit_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_EDIT_LMDN,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_auto_edit_ulimit(void)
{
	Scpipara_type limit;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &limit, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (limit.type== SCPI_INT)
		{
			limit.reg_floa= (float)limit.reg_int;	
		}
		requ_write_an_reg((u8*)&limit.reg_floa, MAP_AUTO_EDIT_LMUP);	

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;        
}
short scpi_spe_auto_edit_ulimit_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_EDIT_LMUP,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestFloatCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_auto_edit_delay(void)
{
	Scpipara_type delay;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &delay, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (delay.type== SCPI_INT)
		{
			delay.reg_floa= (float)delay.reg_int;	
		}
		requ_write_an_reg((u8*)&delay.reg_floa, MAP_AUTO_EDIT_DWELL);	

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;        
}

short scpi_spe_auto_edit_delay_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_EDIT_DWELL,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestSCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_auto_edit_save(void)
{
	Scpipara_type auto_save;
	
	auto_save.reg_int= 1;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&auto_save.reg_int, MAP_AUTO_EDIT_SAVE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_auto_edit_range(void)
{
	return 0;
}
short scpi_spe_auto_edit_range_quest(void)
{
	char cmdbuf[128];

	sprintf(cmdbuf, "%d;", 1);
	if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
	{
		return ERR_OUTBUF_OVERFLOWER;
	}
	memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
	outcounter= outcounter+ strlen(cmdbuf);
	return 0;  
}

//运行自动测试文件
short scpi_spe_auto_test_file(void)
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
		requ_write_an_reg((u8*)&fnum.reg_int, MAP_AUTO_RUN_FILE);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
	}
	return 0;//先返回成功
}

short scpi_spe_auto_test_file_quest(void)
{
	
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_RUN_FILE,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_auto_test_result_quest(void)
{
	unReg32 mode;
	char cmdbuf[128];

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_RESULT,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_auto_test_result_quest;
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

			if (mode.reg_int== 1)//通过
			{
				strcpy(cmdbuf, "PASS;");
			}
			else if(mode.reg_int== 2)//失败
			{
				strcpy(cmdbuf, "FAIL;");
			}
			else//无结果
			{
				strcpy(cmdbuf, "NULL;");
			}		

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


short scpi_spe_auto_status_step_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_STAT_STEP,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_auto_test_query_step_answer(void)
{
	unReg32 regd, regd2;
	u8 res, check;
	char cmdbuf[128];
	char tmp[10];
	if(can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口		

		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &regd);//检查信息	
			GetModbusRegValue(client_rec_buf+7, &regd2);//测量值

			check= (u8)regd.reg_int&0xff;
			res= (u8)(regd.reg_int>>8&0xff);

			if (res== 1)//通过
			{
				strcpy(cmdbuf, "PASS");
			}
			else if(res== 2)//失败
			{
				strcpy(cmdbuf, "FAIL");
			}
			else//无结果
			{
				strcpy(cmdbuf, "NULL;");
			}

			tmp[0]= 0;
			if ((res==1) || (res== 2))
			{
				switch(check)
				{
				case 1:
					sprintf(tmp, ",%.4fV;", regd2.reg_floa);
					break;

				case 2:
					sprintf(tmp, ",%.4fA;", regd2.reg_floa);
					break;

				case 3:
					sprintf(tmp, ",%.4fW;", regd2.reg_floa);
					break;

				default:
					strcpy(tmp, ";");
					break;
				}               
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
        
        return 0;//返回错误码
}

short scpi_spe_auto_test_query_step_quest(void)
{
	//static u8 res_con_state= 0;	
	Scpipara_type fnum;
	short err_code;	
	u8 *pscpi_pb;
	//int resu;
	//float *pf_rec;
	//char cmdbuf[128];

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &fnum, &err_code);
		if (pscpi_pb== NULL)
			return err_code;

		auto_result.cur_step= fnum.reg_int;
		if (0== auto_result.cur_step)//查询超范围
		{
			//res_con_state= 0;//完成
			return ERR_PARAMETER_ERROR;
		}
	}

	if ((request_comm_service(COMMUN_CONNECT)))//通信接口闲
	{	
		auto_result.cur_page= 1;
		requ_write_an_reg((u8*)&auto_result.cur_step, MAP_AUTO_QUERY_STEP);		
		//Send_event= NULL;
		Recv_event= scpi_spe_auto_test_query_step_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		//res_con_state= 1;//发送要读取的自动测试结果页页面
		return 0;//先返回成功
	}
	else if(can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口		
		
		if (client_rec_buf[1]!= MB_FUNC_WRITE_REGISTER)
		{
			return ERR_EXECUTION_ERROR;
		}	

		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{
			requ_read_regs( MAP_AUTO_DETAIL_STEP, 2 );		
			//Send_event= NULL;
			Recv_event= scpi_spe_auto_test_query_step_answer;
			scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
		}
		
		return 0;//返回错误码
	}

	return 0;
}
short scpi_spe_auto_voltage_connect(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_AUTO_VOLT_CON);			

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;  //先返回成功
}
short scpi_spe_auto_voltage_connect_quest(void)
{
	//float volt;
	//short *perr_code;
	//char cmdbuf[128];
	//unReg32 regd;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_VOLT_CON,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;
}
short scpi_spe_auto_voltage_disconnect(void)
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
		requ_write_an_reg((u8*)&volt.reg_floa, MAP_AUTO_VOLT_CUT);			

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;  //先返回成功
}
short scpi_spe_auto_voltage_disconnect_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_AUTO_VOLT_CUT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestVoltCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;
}
