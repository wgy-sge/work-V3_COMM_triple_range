
#include "include.h"


short scpi_spe_cal_state(void)
{
	//u32 ret_enum;	
	Scpipara_type state, code;
	short err_code;	
	u8 *pscpi_pb;

	code.reg_int= 0;
	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT|SCPI_ON_OFF|SCPI_STR, &state, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 

		if (state.reg_int)//关闭不需要密码
		{
			pscpi_pb= GetParaValue(pscpi_pb, SCPI_INT, &code, &err_code);
			if (pscpi_pb== NULL)
				return err_code; 
		}
		else
		{
			code.reg_int= 0;
		}
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		//code.reg_int= code.reg_int;
		requ_write_an_reg((u8*)&code.reg_int, MAP_CAL_SWITCH);		//0x01405001
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	return 0;
}

short scpi_spe_cal_state_quest(void)
{

	char cmdbuf[128];
	unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CAL_SWITCH,1 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_cal_state_quest;
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
			GetModbusRegValue(client_rec_buf+3, &fun);

			if (fun.reg_int&BIT_INPUT)
			{
				fun.reg_int= 1;
			}
			else
			{
				fun.reg_int= 0;
			}

			strcpy(cmdbuf, pScpi_param[(fun.reg_int+COM_OFF)<<1]);
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

static Scpipara_type cal_point;

short scpi_spe_cal_point(void)
{
	Scpipara_type point;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &point, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&point.reg_int, MAP_CAL_POINT);	//型号为整数	
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;	
}

short scpi_spe_cal_point_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CAL_POINT,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

static Scpipara_type level;
short scpi_spe_cal_level(void)
{	
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &level, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&level.reg_int, MAP_CAL_LEVEL);		
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}

short scpi_spe_cal_level_quest(void)
{


	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CAL_LEVEL,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}
short scpi_spe_cal_data(void)
{
	Scpipara_type cal_data;
	short err_code;	
	u8 *pscpi_pb;	
	enREGMap offs_reg_v;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &cal_data, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		switch(level.reg_int)
		{
		case 1:
			offs_reg_v= MAP_CAL_DATA_IP1;
			break;
		case 2:
			offs_reg_v= MAP_CAL_DATA_IP2;
			break;

		default:			
			offs_reg_v= MAP_CAL_DATA_IP1;	
			break;
		}

		if ((cal_point.reg_int== 0)||(cal_point.reg_int== 7))//校准型号
		{
			if (cal_data.type== SCPI_FLOAT)
			{
				cal_data.reg_int= (int)cal_data.reg_floa;
			}
			requ_write_an_reg((u8*)&cal_data.reg_int, offs_reg_v);	//型号为整数
		}
		else
		{
			if (cal_data.type== SCPI_INT)
			{
				cal_data.reg_floa= (float)cal_data.reg_int;	
			}
			requ_write_an_reg((u8*)&cal_data.reg_floa, offs_reg_v);	
		}		

		//Send_event= NULL;
		Recv_event= scpi_spe_cal_data;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		//client_rec_buf
		err_code= *(short*)client_rec_buf;	
		return err_code;//返回错误码
	}

	return 0;     

}

short scpi_spe_cal_save(void)
{
	int save= 1;
	

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{			
		requ_write_an_reg((u8*)&save, MAP_CAL_SAVE);
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;
}

short HaveDoneWriteBlock(void)
{
	if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		//client_rec_buf
		if (client_rec_buf[1]!= MB_FUNC_WRITE_BLOCK_DATA)
		{
			return ERR_EXECUTION_ERROR;
		}	
	}

	return 0;
}
/*******************************************************************************
* Function Name  : prvucMBCHAR2BIN
* Description    : 字符转二级制数值
* Input          : ucCharacter--待转换字符
* Output         : None
* Return         : 二级制数值
*******************************************************************************/
static UCHAR TwoLoaweCombineOne( UCHAR ch1,UCHAR ch2 )
{
	UCHAR hi4 = 0;
	UCHAR lo4 = 0;

	if( ( ch1 >= '0' ) && ( ch1 <= '9' ) )
	{
		hi4 = (ch1 - '0') <<4;
	}
	else if( ( ch1 >= 'a' ) && ( ch1 <= 'f' ) )
	{
		hi4 =  (ch1 - 'a' + 0x0A) << 4;
	}

	if( ( ch2 >= '0' ) && ( ch2 <= '9' ) )
	{
		lo4 = ch2 - '0';
	}
	else if( ( ch2 >= 'a' ) && ( ch2 <= 'f' ) )
	{
		lo4 =  ch2 - 'a' + 0x0A;
	}

	return (hi4 + lo4);
}
short scpi_spe_cal_make(void)
{
	u8 bytes, i;
	char cmdbuf[128];

	u8 *pchar = scpi_param_buffer + 2;

	if( scpi_param_buffer[0] > 1 )
	{ 
		return ERR_PARAMETER_NOT_ALLOWED; 
	}
	else if( scpi_param_buffer[0] < 1 )
	{
		return ERR_MISSING_PARAMETER;
	}
	else if( *pchar != 'x' )
	{
		return ERR_STRING_DATA_NOT_ALLOWED;
	}
	else
	{
		pchar ++;
	}

	bytes = strlen((char*)pchar);

	if( bytes <= MAXIMUM_BLOCK_FRAME )
	{
		u8 len = bytes / 2;
		u8 i;
		for( i=0; i<len; i++ )
		{
			cmdbuf[i] = TwoLoaweCombineOne( pchar[i<<1],pchar[(i<<1)+1] );
		}
		cmdbuf[i]= 0;
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{	
			RequestWriteBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_MANUFACTURER, (u8*)cmdbuf, strlen( cmdbuf )); 
			Recv_event= HaveDoneWriteBlock;
			scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
		}	
	}
	else
	{
		return ERR_CHARACTER_DATA_TOO_LONG;		
	}		

	return 0;
}
short scpi_spe_cal_make_quest(void)
{
	u8 i;
	char cmdbuf[128];
	//unReg32 fun;


	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		RequestReadBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_MANUFACTURER );		
		//Send_event= NULL;
		Recv_event= scpi_spe_cal_make_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_BLOCK_DATA)
		{
			for (i= 0; i< client_rec_buf[2]; i++)
			{
				cmdbuf[i]= client_rec_buf[3+ i];
			}

			cmdbuf[i]= ';';	
                        cmdbuf[i+1]= 0;
			//sprintf(cmdbuf, "%d;", fun.reg_int);
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
short scpi_spe_cal_model(void)
{
	u8 bytes, i;
	char cmdbuf[128];

	u8 *pchar = scpi_param_buffer + 2;

	if( scpi_param_buffer[0] > 1 )
	{ 
		return ERR_PARAMETER_NOT_ALLOWED; 
	}
	else if( scpi_param_buffer[0] < 1 )
	{
		return ERR_MISSING_PARAMETER;
	}
	else if( *pchar != 'x' )
	{
		return ERR_STRING_DATA_NOT_ALLOWED;
	}
	else
	{
		pchar ++;
	}

	bytes = strlen((char*)pchar);

	if( bytes <= MAXIMUM_BLOCK_FRAME )
	{
		u8 len = bytes / 2;
		u8 i;
		for( i=0; i<len; i++ )
		{
			cmdbuf[i] = TwoLoaweCombineOne( pchar[i<<1],pchar[(i<<1)+1] );
		}
		cmdbuf[i]= 0;
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{	
			RequestWriteBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_MODEL, (u8*)cmdbuf, strlen(cmdbuf)); 
			Recv_event= HaveDoneWriteBlock;
			scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
		}	
	}
	else
	{
		return ERR_CHARACTER_DATA_TOO_LONG;		
	}
	return 0;
}
short scpi_spe_cal_model_quest(void)
{
	u8 i;
	char cmdbuf[128];
	//unReg32 fun;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		RequestReadBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_MODEL );		
		//Send_event= NULL;
		Recv_event= scpi_spe_cal_model_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_BLOCK_DATA)
		{
			for (i= 0; i< client_rec_buf[2]; i++)
			{
				cmdbuf[i]= client_rec_buf[3+ i];
			}

			cmdbuf[i]= ';';	
			cmdbuf[i+1]= 0;
			//sprintf(cmdbuf, "%d;", fun.reg_int);
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
short scpi_spe_cal_serial(void)
{
	u8 bytes, i;
	char cmdbuf[128];

	u8 *pchar = scpi_param_buffer + 2;

	if( scpi_param_buffer[0] > 1 )
	{ 
		return ERR_PARAMETER_NOT_ALLOWED; 
	}
	else if( scpi_param_buffer[0] < 1 )
	{
		return ERR_MISSING_PARAMETER;
	}
	else if( *pchar != 'x' )
	{
		return ERR_STRING_DATA_NOT_ALLOWED;
	}
	else
	{
		pchar ++;
	}

	bytes = strlen((char*)pchar);

	if( bytes <= MAXIMUM_BLOCK_FRAME )
	{
		u8 len = bytes / 2;
		u8 i;
		for( i=0; i<len; i++ )
		{
			cmdbuf[i] = TwoLoaweCombineOne( pchar[i<<1],pchar[(i<<1)+1] );
		}
		cmdbuf[i]= 0;
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{	
			RequestWriteBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_SERIAL, (u8*)cmdbuf, strlen( cmdbuf )); 
			Recv_event= HaveDoneWriteBlock;
			scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
		}	
	}
	else
	{
		return ERR_CHARACTER_DATA_TOO_LONG;		
	}
	return 0;
}
short scpi_spe_cal_serial_quest(void)
{
	u8 i;
	char cmdbuf[128];
	//unReg32 fun;


	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		RequestReadBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_SERIAL );		
		//Send_event= NULL;
		Recv_event= scpi_spe_cal_serial_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_BLOCK_DATA)
		{
			for (i= 0; i< client_rec_buf[2]; i++)
			{
				cmdbuf[i]= client_rec_buf[3+ i];
			}

			cmdbuf[i]= ';';	
			cmdbuf[i+1]= 0;
			//sprintf(cmdbuf, "%d;", fun.reg_int);
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
short scpi_spe_cal_date(void)
{
	u8 bytes, i;
	char cmdbuf[128];

	u8 *pchar = scpi_param_buffer + 2;

	if( scpi_param_buffer[0] > 1 )
	{ 
		return ERR_PARAMETER_NOT_ALLOWED; 
	}
	else if( scpi_param_buffer[0] < 1 )
	{
		return ERR_MISSING_PARAMETER;
	}
	else if( *pchar != 'x' )
	{
		return ERR_STRING_DATA_NOT_ALLOWED;
	}
	else
	{
		pchar ++;
	}

	bytes = strlen((char*)pchar);

	if( bytes <= MAXIMUM_BLOCK_FRAME )
	{
		u8 len = bytes / 2;
		u8 i;
		for( i=0; i<len; i++ )
		{
			cmdbuf[i] = TwoLoaweCombineOne( pchar[i<<1],pchar[(i<<1)+1] );
		}
		cmdbuf[i]= 0;
		if (request_comm_service(COMMUN_CONNECT))//通信接口闲
		{	
			RequestWriteBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_DATE, (u8*)cmdbuf, strlen( cmdbuf )); 
			Recv_event= HaveDoneWriteBlock;
			scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令	
		}	
	}
	else
	{
		return ERR_CHARACTER_DATA_TOO_LONG;		
	}
	return 0;
}
short scpi_spe_cal_date_quest(void)
{
	u8 i;
	char cmdbuf[128];
	//unReg32 fun;
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		RequestReadBlockData( MAIN_BOARD_DEVICE_ADDRESS, MAP_BLOCK_DATE );		
		//Send_event= NULL;
		Recv_event= scpi_spe_cal_date_quest;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		if (client_rec_buf[1]== MB_FUNC_READ_BLOCK_DATA)
		{
			for (i= 0; i< client_rec_buf[2]; i++)
			{
				cmdbuf[i]= client_rec_buf[3+ i];
			}

			cmdbuf[i]= ';';	
			cmdbuf[i+1]= 0;
			//sprintf(cmdbuf, "%d;", fun.reg_int);
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



short scpi_spe_cal_alter(void)
{
	Scpipara_type point;
	short err_code;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_INT, &point, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 		
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&point.reg_int, MAP_CAL_ALTER_LEVEL);	//型号为整数	
		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}

	return 0;	
}

short scpi_spe_cal_alter_quest(void)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CAL_ALTER_LEVEL,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestIntCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}
	return 0;
}





void BigPrintf(u8 *pLdat, u8 printlen, char* pStr)
{
	char tmpstr[5];
	u8 i, j;
	strcpy(pStr, "0x");
	for (i= printlen; i> 0; i--)
	{
		sprintf(tmpstr, "%2x", *(pLdat+i-1));

		for (j= 0; j< strlen(tmpstr); j++)
		{
			if (tmpstr[j]== ' ')
			{
				tmpstr[j]= '0';
			}        
		}

		strcat(pStr, tmpstr);
	}

	strcat(pStr, ",");
}


Scpipara_type regaddr, regval;
short scpi_spe_cal_register(void)
{
	char cmdbuf[128];
	short err_code, i;	
	u8 *pscpi_pb;

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_STR|SCPI_HEX, &regaddr, &err_code);//寄存器地址
		if (pscpi_pb== NULL)
		{			
			scpi_parser_return_code= err_code+ERR_SCPI_MODBUS+ERR_SCPI_MODBUS_OFFSET;
			ReportError();		

			return ERR_NO_ERROR;//返回错误码
		}	

		//寄存器数值
		pscpi_pb= GetParaValue(pscpi_pb, SCPI_STR|SCPI_HEX, &regval, &err_code);
		if (pscpi_pb== NULL)
		{			
			scpi_parser_return_code= err_code+ERR_SCPI_MODBUS+ERR_SCPI_MODBUS_OFFSET;
			ReportError();		

			return ERR_NO_ERROR;//返回错误码
		}	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{		
		requ_write_an_reg((u8*)&regval.dat32, (regaddr.dat32/2));		
		//Send_event= NULL;
		Recv_event= scpi_spe_cal_register;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}
	else if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口


		err_code= *(short*)client_rec_buf;	
		if(err_code== ERR_NO_ERROR)
		{
			sprintf(cmdbuf, "0x%4x;", regaddr.dat32);			
			for (i= 0; i< strlen(cmdbuf); i++)
			{
				if (cmdbuf[i]== ' ')
				{
					cmdbuf[i]= '0';
				}        
			}

			if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
			{
				return ERR_OUTBUF_OVERFLOWER;
			}
			memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
			outcounter= outcounter+ strlen(cmdbuf);		

		}
		else
		{
			scpi_parser_return_code= err_code+ERR_SCPI_MODBUS+ERR_SCPI_MODBUS_OFFSET;

			ReportError();
		}

		return ERR_NO_ERROR;//返回错误码
	}

	return 0;
}

Scpipara_type regnum;
short scpi_spe_cal_register_quest(void)
{

	//short *perr_code;
	char cmdbuf[128];

	char tmpbuf[10];

	u8 *pscpi_pb, i= 0, j= 0;
	short err_code;	
	//u32 val;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_STR|SCPI_HEX, &regaddr, &err_code);//寄存器地址
		if (pscpi_pb== NULL)
		{			
			scpi_parser_return_code= err_code+ERR_SCPI_MODBUS+ERR_SCPI_MODBUS_OFFSET;
			ReportError();		

			return ERR_NO_ERROR;//返回错误码
		}	

		//寄存器数值
		pscpi_pb= GetParaValue(pscpi_pb, SCPI_STR|SCPI_HEX, &regnum, &err_code);
		if (pscpi_pb== NULL)
		{			
			scpi_parser_return_code= err_code+ERR_SCPI_MODBUS+ERR_SCPI_MODBUS_OFFSET;
			ReportError();		

			return ERR_NO_ERROR;//返回错误码
		}	

		regnum.dat32/= 2;//2个地址表示一个寄存器
		requ_read_regs(  (regaddr.dat32/2), regnum.dat32 );		
		//Send_event= NULL;
		Recv_event= scpi_spe_cal_register_quest;
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
		err_code= *(short*)client_rec_buf;

		if (err_code== ERR_NO_ERROR)
		{
			memset(cmdbuf, 0, 128);
			for (i= 0; i< regnum.dat32; i++)
			{				
				BigPrintf(client_rec_buf+4*i+2, 4, tmpbuf);
				strcat(cmdbuf, tmpbuf);
			}

			cmdbuf[strlen(cmdbuf)-1]= ';';			

			if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
			{
				return ERR_OUTBUF_OVERFLOWER;
			}
			memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
			outcounter= outcounter+ strlen(cmdbuf);
		}
		else
		{
			scpi_parser_return_code= err_code+ERR_SCPI_MODBUS+ERR_SCPI_MODBUS_OFFSET;

			ReportError();
		}

		return ERR_NO_ERROR;//返回错误码
	}

	return 0;
}

short scpi_spe_cal_bound_quest(void)
{

	short *perr_code;
	char cmdbuf[128];

	char tmpbuf[10];
	u8 *pscpi_pb, i= 0, j= 0;
	short err_code;	
	u32 val;

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_STR|SCPI_HEX, &regaddr, &err_code);//寄存器地址
		if (pscpi_pb== NULL)
		{			
			scpi_parser_return_code= err_code+ERR_SCPI_MODBUS+ERR_SCPI_MODBUS_OFFSET;
			ReportError();		

			return ERR_NO_ERROR;//返回错误码
		}		

		requ_read_bound_regs(regaddr.dat32/2);

		//Send_event= NULL;
		Recv_event= scpi_spe_cal_bound_quest;
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
		err_code= *(short*)client_rec_buf;
		if (err_code== ERR_NO_ERROR)
		{
			memset(cmdbuf, 0, 128);
			for (i= 0; i< 2; i++)
			{				
				BigPrintf(client_rec_buf+4*i+2, 4, tmpbuf);	
				strcat(cmdbuf, tmpbuf);
			}

			cmdbuf[strlen(cmdbuf)-1]= ';';

			if (outcounter+ strlen(cmdbuf)> MAXOUTPUTCHARS)
			{
				return ERR_OUTBUF_OVERFLOWER;
			}
			memcpy(OutputBuff+outcounter, cmdbuf, strlen(cmdbuf));
			outcounter= outcounter+ strlen(cmdbuf);
		}		
		else
		{
			scpi_parser_return_code= err_code+ERR_SCPI_MODBUS+ERR_SCPI_MODBUS_OFFSET;

			ReportError();
		}

		return ERR_NO_ERROR;//返回错误码
	}

	return 0;
}
