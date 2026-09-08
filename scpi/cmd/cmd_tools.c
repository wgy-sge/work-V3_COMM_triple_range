#include "include.h"


const char *pScpi_ft6800param[]=
{
	"cc",
	"cc",
	"cv",
	"cv",
	"cp",
	"cp",
	"cr",
	"cr",
	"tc",
	"tc",	
	"tv",
	"tv",
	"NULL",
	"NULL",
	"tr",
	"tr",
	"seq",
	"seq",
	"auto",
	"auto",
	"bres",
	"bres",
	"bcap",
	"bcap",
	"ocp",
	"ocp",
	"capa",
	"capa",
	NULL,
};

const char *pScpi_param[]=
{	
	"off", //0
	"off",
	"on", 
	"on",

	"master",   //4
	"master",

	"slaver",   //6
	"slaver",

	"cch",		//8
	"cch",
	"ccl",		//
	"ccl",
	"cvh",
	"cvh",
	"cvl",
	"cvl",
	"crh",
	"crh",
	"crl",
	"crl",
	"cp",
	"cp",		
	"ccdh",
	"ccdh",
	"ccdl",
	"ccdl",
        
        "cvdh",
	"cvdh",
	"cvdl",
	"cvdl",

	"crdh",
	"crdh",

	"crdl",
	"crdl",

	"cpd",
	"cpd",

	"seq",
	"seq",
	"auto",
	"auto",

	"ocp",
	"ocp",
	"opp",
	"opp",

	"disch",
	"disch",
	"loef",
	"loef",

	"dcr",
	"dcr",
	"led",
	"led",
	"sweep",
	"sweep",

	"cv2cc",
	"cv2cc",
	"cr2cc",
	"cr2cc",
	"cp2cc",
	"cp2cc",
        "cv2cr",
	"cv2cr",
	"wave",
	"wave",
        "cz",
        "cz"
          

	"cont",//34
	"cont",
	"puls",
	"pulse",
	"togg",
	"toggle",

	"curr",//40
	"current",
	"volt", 
	"voltage",
	"pow",
	"power",
	"res",
	"resistance",

	"trig", //48
	"trig",

	"key",	//50
	"key",
	"ext",
	"external",
	"bus",
	"bus",

	"fast", //58
	"fast",

	"slow", //56
	"slow",
	
	"local",
	"local",	

	"remote",
	"remote",

	"cc",
	"cc",	

	"cv",
	"cv",

	

	NULL,
};



void GetModbusRegValue(u8 *pFrameDate, unReg32* pRegDat)
{
	pRegDat->reg_buff[0]= pFrameDate[3];
	pRegDat->reg_buff[1]= pFrameDate[2];
	pRegDat->reg_buff[2]= pFrameDate[1];
	pRegDat->reg_buff[3]= pFrameDate[0];
}




//按位或
u8 ORBits(int inda)
{
	u8 i;
	u8 result= 0;
	for(i= 0; i< 32; i++)
	{
		result= result|(inda&0x01);
		inda= inda>>1;
	}
	return result;

}
//找出指定字符串，在目标字符串中的位置
u32 CmpString(char **pDstr, char *pStr)
{
	u8 i= 0;
	u32 res;

	while (pDstr[i]!= NULL)
	{
		if(strcmp(pDstr[i], pStr)== 0)
		{
			res= i>>1;
			return res;
		}
		i++;
	}

	return 255;
}

//找出指定字符串，在目标字符串中的位置
u32 CmpStringDirec(char **pDstr, char *pStr)
{
	u8 i= 0;
	u32 res;

	while (pDstr[i]!= NULL)
	{
		if(strcmp(pDstr[i], pStr)== 0)
		{
			res= i;
			return res;
		}
		i++;
	}

	return 255;
}

u8 Hex2Value(char *pHexStr, u32* pVal)
{
	u8 i= 0, j= 0;
	u8 hexv[8];

	u32 val= 0;

	if (strlen(pHexStr)> 8)
	{
		return 1;//数据超出长度
	}

	memset(hexv, 0, 8);//清0

	for (i= 0; i< strlen(pHexStr); i++)
	{
		if (( pHexStr[i]>= '0' )&&( pHexStr[i]<= '9'))
		{
			hexv[i]= pHexStr[i]- '0';
		}   
		else if (( pHexStr[i]>= 'a' )&&( pHexStr[i]<= 'f'))
		{
			hexv[i]= pHexStr[i]- 'a'+10;
		}
		else if (( pHexStr[i]>= 'A' )&&( pHexStr[i]<= 'F'))
		{
			hexv[i]= pHexStr[i]- 'A'+10;
		}
		else	   
		{
			return 2;//数据格式错误
		}
	}

	val= 0;
	for (j= 0; j< i; j++)
	{
		val<<= 4;
		val+=  hexv[j];
	}

	*pVal= val;

	return 0;
}

const char *pExtInhibitStrings[]=
{	
	"trigger",	
	"toggle",
	"hold",
	"halt",
	NULL
};




const char *pSeqModeStrings[]=
{	
	"cch",	
	"ccl",
	"cvh",
	"cvl",
	"crh",
	"crl",
	"cp",
	NULL
};


s32 FT6800_Func2FT68200_Func(s32 ft6800_Func)
{
	s32 ft68200_func= -1;

	switch(ft6800_Func)
	{
	case FT6800_FUNC_CC:
		ft68200_func= FUNC_CCH;
		break;

	case FT6800_FUNC_CV:
		ft68200_func= FUNC_CVH;
		break;

	case FT6800_FUNC_CP:
			ft68200_func= FUNC_CP;
		break;

	case FT6800_FUNC_CR:
		ft68200_func= FUNC_CRH;
		break;

	case FT6800_FUNC_TC:
		ft68200_func= FUNC_CCDH;
		break;

	case FT6800_FUNC_TV:
		ft68200_func= -1;
		break;

	case FT6800_FUNC_TR:
		ft68200_func= FUNC_CRDH;
		break;

	case FT6800_FUNC_SEQ:
		ft68200_func= FUNC_SEQ;
		break;

	case FT6800_FUNC_AUTO:
		ft68200_func= FUNC_AUTO;
		break;

	case FT6800_FUNC_BRES:
		ft68200_func= FUNC_DCR;
		break;

	case FT6800_FUNC_BCAP:
		ft68200_func= FUNC_DISCHARGE;
		break;

	case FT6800_FUNC_OCP:
		ft68200_func= FUNC_OCP;
		break;

	case FT6800_FUNC_CAPA:
		ft68200_func= -1;
		break;
		
	}
		
	return ft68200_func;
}

s32 FT68200_Func2FT6800_Func(s32 ft68200_Func)
{
	s32 ft6800_func= -1;

	switch(ft68200_Func)
	{
	case FUNC_CCH:
		ft6800_func= FT6800_FUNC_CC;
		break;

	case FUNC_CVH:
		ft6800_func= FT6800_FUNC_CV;
		break;
		
	case FUNC_CRH:
	case FUNC_CRL:
		ft6800_func= FT6800_FUNC_CR;
		break;

	case FUNC_CP:
		ft6800_func= FT6800_FUNC_CP;
		break;

	case FUNC_CCDH:
		ft6800_func= FT6800_FUNC_TC;
		break;

	case FUNC_CRDH:
	case FUNC_CRDL:
		ft6800_func= FT6800_FUNC_TR;
		break;

	case FUNC_SEQ:
		ft6800_func= FT6800_FUNC_SEQ;
		break;

	case FUNC_AUTO:
		ft6800_func= FT6800_FUNC_AUTO;
		break;

	case FUNC_OCP:
		ft6800_func= FT6800_FUNC_OCP;
		break;

	case FUNC_DISCHARGE:
		ft6800_func= FT6800_FUNC_BCAP;
		break;

	case FUNC_DCR:
		ft6800_func= FT6800_FUNC_BRES;
		break;


	case FUNC_CCL:
	case FUNC_CVL:
	case FUNC_CCDL:
	case FUNC_CPD:
	case FUNC_OPP:
	case FUNC_LOEF:
	case FUNC_LED:
	case FUNC_SWEEP:
	case FUNC_CVCC:
	case FUNC_CRCC:
	case FUNC_CPCC:
		ft6800_func= ft68200_Func+ FT6800_FUNC_CAPA;
		break;

	default:
		ft6800_func= -1;
		break;
	}

	return ft6800_func;
}


u8* GetParaValue(u8 *pdata, u32 para_type, Scpipara_type* param, short *pscpi_err)
{
	u8 *pVal;	
	SCPI_STR_PARAMETER scpi_para;
	s32 tmp_func;

	pVal= (pdata+1);
	switch(pdata[0])
	{
	case BIT_INDICATES_INT:	
		if ((para_type&SCPI_INT)== 0)
		{
			*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
			return NULL;
		}
		(*param).reg_int= *(int*)pVal;
		if (para_type&SCPI_FT6800_FUNCTION)//与ELoader兼容参数解析
		{
			(*param).reg_int= FT6800_Func2FT68200_Func((*param).reg_int);
			if((*param).reg_int== -1)
				return (pdata+1+sizeof(int));//不支持的功能			
		}
		(*param).type= SCPI_INT;
		return (pdata+1+sizeof(int));	

	case BIT_INDICATES_FLOAT:	
		if ((para_type&SCPI_FLOAT)== 0)
		{
			*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
			return NULL;
		}
		(*param).reg_floa= *(float*)pVal;
		(*param).type= SCPI_FLOAT;
		return (pdata+1+sizeof(float));	

	case BIT_INDICATES_STRING:
		if ((para_type&SCPI_STR)== 0)
		{
			*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
			return NULL;
		}

		if (para_type&SCPI_HEX)//可见字符表示十六进制数
		{	
			if ((pVal[0]== 'x')||(pVal[0]== 'X'))
			{				
				if (Hex2Value(pVal+1, &(*param).dat32)== 0)
				{
					return (pdata+1+strlen((char *)pVal)+1);
				}
				else
				{
					*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
					return NULL;
				}				
			}
			else
			{
				*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
				return NULL;
			}				
		}		

		if (para_type&SCPI_EXT_INHIBIT)
		{
			scpi_para= (SCPI_STR_PARAMETER)CmpStringDirec((char **)pExtInhibitStrings, (char*)pVal);

			if(scpi_para> 3)
			{
				*pscpi_err= ERR_INVALID_FORMAT;//参数格式错误
				return NULL;
			}

			(*param).reg_int= scpi_para;
			return (pdata+1+strlen((char *)pVal)+1); 
		}	

		if (para_type&SCPI_SEQ_MODE)
		{
			scpi_para= (SCPI_STR_PARAMETER)CmpStringDirec((char **)pSeqModeStrings, (char*)pVal);

			if(scpi_para> 2)
			{
				*pscpi_err= ERR_INVALID_FORMAT;//参数格式错误
				return NULL;
			}

			(*param).reg_int= scpi_para;
			return (pdata+1+strlen((char *)pVal)+1); 
		}
		

	
		if (para_type&SCPI_FT6800_FUNCTION)//与ELoader兼容参数解析
		{
				scpi_para= CmpString((char **)pScpi_ft6800param, (char*)pVal);

				
				tmp_func= FT6800_Func2FT68200_Func((s32)scpi_para);

				if (tmp_func <0)
				{
					(*param).reg_int= -1;//清空
					return (pdata+1+strlen((char *)pVal)+1);;//不支持的功能
				}
				else
				{
					(*param).reg_int= tmp_func;
					return (pdata+1+strlen((char *)pVal)+1);
				}				

		}

		scpi_para= (SCPI_STR_PARAMETER)CmpString((char **)pScpi_param, (char*)pVal);
		switch(scpi_para)
		{	
		case COM_ON:
		case COM_OFF:
			if ((para_type&SCPI_ON_OFF)== 0)
			{
				*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
				return NULL;
			}	
			(*param).reg_int= scpi_para- COM_OFF;
			break;

		case COM_MASTER:	
		case COM_SLAVER:

			if ((para_type&SCPI_SLAVER_MASTER)== 0)
			{
				*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
				return NULL;
			}	
			(*param).reg_int= scpi_para- COM_MASTER;
			break;	

		case COM_MODE_CC:
			if (para_type&SCPI_DISCH_MODE)//放电模式
			{				
				(*param).reg_int= 0;
				return (pdata+1+strlen((char *)pVal)+1); 
			}
		case COM_MODE_CV:
			if ((para_type&SCPI_MODE)== 0)
			{
				*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
				return NULL;
			}	
			(*param).reg_int= scpi_para- COM_MODE_CC;
			break;

		case COM_CRH:	
		case COM_CRL:	
		case COM_CP:
			if (para_type&SCPI_MODE)//模式
			{
				if ((scpi_para== COM_CRH)||(scpi_para== COM_CRL))
				{
					(*param).reg_int= 2;
				}
				else
				{
					(*param).reg_int= 3;
				}
				
				return (pdata+1+strlen((char *)pVal)+1); 
			}
			else if (para_type&SCPI_DISCH_MODE)//放电模式
			{
				if ((scpi_para== COM_CRH)||(scpi_para== COM_CRL))
				{
					(*param).reg_int= 1;
				}
				else
				{
					(*param).reg_int= 2;
				}
				return (pdata+1+strlen((char *)pVal)+1); 
			}

		case COM_CV2CC:
		case COM_CR2CC:
		case COM_CP2CC:	
                case COM_CV2CR:
			if (para_type&SCPI_COMB_MODE)//复合测试模式
			{
				if (scpi_para== COM_CV2CC)
				{
					(*param).reg_int= 0;
				}
				else if (scpi_para== COM_CR2CC)
				{
					(*param).reg_int= 1;
				}
				else if (scpi_para== COM_CP2CC)
				{
					(*param).reg_int= 2;
				}
                                else if (scpi_para== COM_CV2CR)
				{
					(*param).reg_int= 3;
				}
				return (pdata+1+strlen((char *)pVal)+1); 
			}
			

		case COM_CCH:
		case COM_CVH:	
		case COM_CCL:
		case COM_CVL:
		case COM_CCDH:	
		case COM_CCDL:
                case COM_CVDH:	
		case COM_CVDL:
		case COM_CRDH:
		case COM_CRDL:
		case COM_CPD:
		case COM_SEQ:
		case COM_AUTO:
		case COM_OCP:
		case COM_OPP:
		case COM_DISCH:
		case COM_LOEF:
		case COM_ESR:
		case COM_LED:
		case COM_SWEEP:	
		case COM_WAVE:
                case COM_CZ:
			if ((para_type&SCPI_FUNCTION)== 0)
			{
				*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
				return NULL;
			}			
			(*param).reg_int= scpi_para- COM_CCH;
			break;		

		case COM_CONT:
		case COM_PULSE:
		case COM_TOGGLE:
			if ((para_type&(SCPI_CONT|SCPI_PULSE_TOGG))== 0)
			{
				*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
				return NULL;
			}			
			(*param).reg_int= scpi_para- COM_CONT;
			break;

		case COM_CURR:
		case COM_VOLT:		
		case COM_POWER:		
		//case COM_RES:
			if ((para_type&SCPI_VOLT_CURR_POW_RES )== 0)
			{
				*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
				return NULL;
			}			
			(*param).reg_int= scpi_para- COM_CURR;
			break;

		case COM_TRIG:
			if ((para_type&SCPI_TRIG)== 0)
			{
				*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
				return NULL;
			}			
			(*param).reg_int= 1;
			break;

		case COM_KEY:
		case COM_EXT:
		case COM_BUS:
			if ((para_type&SCPI_EXT_KEY_BUS )== 0)
			{
				*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
				return NULL;
			}			
			(*param).reg_int= scpi_para- COM_KEY;
			break;

		case COM_LOCAL:			
		case COM_REMOTE:
			if ((para_type&SCPI_LOCAL_REMOTE  )== 0)
			{
				*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
				return NULL;
			}			
			(*param).reg_int= scpi_para- COM_LOCAL;
			break;				

		case COM_FAST:
		case COM_SLOW:
			if ((para_type&SCPI_FAST_SLOW  )== 0)
			{
				*pscpi_err= ERR_ILLEGAL_PARAMETER_VALUE;//非法参数值
				return NULL;
			}			
			(*param).reg_int= scpi_para- COM_FAST;
			break;
		default:
			*pscpi_err= ERR_PARAMETER_ERROR;//未定义的参数错误
			return NULL;
		}
		return (pdata+1+strlen((char *)pVal)+1);

	default:
		*pscpi_err= ERR_INVALID_FORMAT;//参数格式错误
		return NULL;

	}

}

short ScpiSetCmdAnswer(void)
{
	if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		//client_rec_buf
		if (client_rec_buf[1]!= MB_FUNC_WRITE_REGISTER)
		{
			return ERR_EXECUTION_ERROR;
		}	
	}

	return 0;
}

short ScpiQuestCmdAnswer(u32 ParaType, u8 UnitType)
{
	unReg32 regd;
	char cmdbuf[128];
	if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		//
		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &regd);	

			if (ParaType== SCPI_FLOAT)
			{
				sprintf(cmdbuf, "%.4f", regd.reg_floa);
			} 
			else
			{
				sprintf(cmdbuf, "%d", regd.reg_int);
			}

			switch(UnitType)
			{
			case UNIT_V:
				strcat(cmdbuf, "V;");
				break;

			case UNIT_A:
				strcat(cmdbuf, "A;");
				break;

			case UNIT_W:
				strcat(cmdbuf, "W;");
				break;

			case UNIT_S:
				strcat(cmdbuf, "S;");
				break;

			case UNIT_HZ:
				strcat(cmdbuf, "Hz;");
				break;

			case UNIT_MS:
				strcat(cmdbuf, "ms;");
				break;

			case UNIT_V_MS:
				strcat(cmdbuf, "V/ms;");
				break;

			case UNIT_A_US:
				strcat(cmdbuf, "A/us;");
				break;
                                
                        case UNIT_UF:
				strcat(cmdbuf, "uF;");
				break;
                                
                        case UNIT_UH:
				strcat(cmdbuf, "uH;");
				break;
				
			case UNIT_OHM:
				strcat(cmdbuf, "OHM;");
				break;
                                
			default:
				strcat(cmdbuf, ";");
				break;

			}			

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


//电压查询
short ScpiQuestVoltCmdAnswer(void)
{
   return ScpiQuestCmdAnswer(SCPI_FLOAT, UNIT_V);
}

//电压斜率查询
short ScpiQuestVoltSlewCmdAnswer(void)
{
	return ScpiQuestCmdAnswer(SCPI_FLOAT, UNIT_V_MS);
}

//电流查询
short ScpiQuestCurrCmdAnswer(void)
{
	 return ScpiQuestCmdAnswer(SCPI_FLOAT, UNIT_A);
}

//电流斜率查询
short ScpiQuestCurrSlewCmdAnswer(void)
{
	return ScpiQuestCmdAnswer(SCPI_FLOAT, UNIT_A_US);
}


//功率查询
short ScpiQuestPowerCmdAnswer(void)
{
	return ScpiQuestCmdAnswer(SCPI_FLOAT, UNIT_W);
}

//毫秒参数显示查询
short ScpiQuestMSCmdAnswer(void)
{
	return ScpiQuestCmdAnswer(SCPI_FLOAT, UNIT_MS);
}
//秒参数显示查询
short ScpiQuestSCmdAnswer(void)
{
return ScpiQuestCmdAnswer(SCPI_FLOAT, UNIT_S);
}

//整数秒参数显示查询
short ScpiQuestIntSCmdAnswer(void)
{
	return ScpiQuestCmdAnswer(SCPI_INT, UNIT_S);
}

//浮点数查询
short ScpiQuestFloatCmdAnswer(void)
{
	return ScpiQuestCmdAnswer(SCPI_FLOAT, UNIT_NONE);
}

//整数查询
short ScpiQuestIntCmdAnswer(void)
{
	return ScpiQuestCmdAnswer(SCPI_INT, UNIT_NONE);
}

//电阻查询
short ScpiQuestResCmdAnswer(void)
{
	return ScpiQuestCmdAnswer(SCPI_FLOAT, UNIT_OHM);
}

//电容查询
short ScpiQuestCapCmdAnswer(void)
{
	return ScpiQuestCmdAnswer(SCPI_FLOAT, UNIT_UF);
}

//电感查询
short ScpiQuestInductCmdAnswer(void)
{
	return ScpiQuestCmdAnswer(SCPI_FLOAT, UNIT_UH);
}

u16 EnumStringType= 0;
void *pEumStrings[2]=
{
	(void*)pSeqModeStrings,
	(void*)NULL
};

//枚举字符串查询
short ScpiQuestEnumStringAnswer(void)
{
	unReg32 regd;
	char cmdbuf[128];
	if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		//Send_event= NULL;
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口
		//
		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &regd);		
			sprintf(cmdbuf, "%s;", ((char**)pEumStrings[EnumStringType])[regd.reg_int]);
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

//整数Hz参数显示查询
short ScpiQuestIntHzCmdAnswer(void)
{
	return ScpiQuestCmdAnswer(SCPI_INT, UNIT_HZ);
}

//错误系统
static short err_list[SCPI_ERROR_LIST_LENGTH];
static unsigned char  err_cnt;
static unsigned char  err_wi;
static unsigned char  err_ri;
/*******************************************************************************
* Function Name  : ScpiDisposePoll
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
bool PushInError( short err )
{
	bool status = TRUE;

	//PutBitStatusByte( BIT_SCPI_MAV );

	//enter critical
	if( err_cnt < (SCPI_ERROR_LIST_LENGTH - 1) )
	{
		err_list[err_wi] = err;

		err_wi = (err_wi + 1) % SCPI_ERROR_LIST_LENGTH;   
		err_cnt ++;		
	}
	else if( err_cnt == (SCPI_ERROR_LIST_LENGTH - 1) )
	{
		err_list[err_wi] = ERR_QUERY_OVERFLOW;

		err_wi = (err_wi + 1) % SCPI_ERROR_LIST_LENGTH;   
		err_cnt ++;		
	}
	else
	{
		status = FALSE;
	}
	//exit critical

	return status;
}

/*******************************************************************************
* Function Name  : PullOutError
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
short PullOutError( void )
{
	short err = ERR_NO_ERROR;

	if( err_cnt > 0 )
	{
		err = err_list[err_ri];

		err_ri = (err_ri + 1) % SCPI_ERROR_LIST_LENGTH;   
		err_cnt --;		
	}

	return err;
}

/*******************************************************************************
* Function Name  : ClearErrorQueue
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void ClearErrorQueue( void )
{
	err_cnt = 0;
	err_ri = 0;
	err_wi = 0;
}



static unReg32 sPreRegDat;
static p_scpi_func_type spAnswerProc;

short PreLoadRegAnswer(void)
{
	if (can_use(COMMUN_CONNECT))//已经收到数据
	{
		Recv_event= NULL;
		scpi_proc_ava= TRUE;//继续执行下面的SCPI命令
		back_idle();//释放通信接口

		if (client_rec_buf[1]== MB_FUNC_READ_REGISTERS)
		{
			GetModbusRegValue(client_rec_buf+3, &sPreRegDat);			
			spAnswerProc( );			
			return 0;
		}
		else
			return ERR_QUERY_ERROR;//返回错误码
	}

	return 0;
}
short PreLoadRegAsk(u16 regaddr, p_scpi_func_type pAnswerProc)
{
	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( regaddr, 1);		
		//Send_event= NULL;
		Recv_event= PreLoadRegAnswer;
		spAnswerProc= pAnswerProc;//保留调用指针
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令

	}

	return 0;//先返回成功
}

unReg32 GetPreRegDat(void)
{
	return sPreRegDat;
}
