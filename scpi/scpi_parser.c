/****************************************************************************************
*                                                                                       *
*	File Name	: scpi_parser.cpp                                                       *
*	Author		: R&D Center,Shenzhen Zidetech Co. Ltd.								*
*	Version		: V2.0																	*
*	Date		: 08/12/2009															*
*	Description	: This file contains all the codes about how the scpi_analyse work.		*
*																						*
****************************************************************************************/


#include "scpi_interface.h"
#include "include.h"

#ifdef __DEBUG__ON__PC__
#include "scpi_keyword_table.h"
#else
#include "scpi_parser.h"
extern const scpi_common_keyword scpi_common_command_table[];
extern const scpi_special_keyword scpi_special_command_table[];
#endif

void scpi_init(void)
{
	scpi_parser_primary_status = SCPI_PRETREATMENT;
}

void scpi_parser(void)
{
	while(TRUE)
	{
		switch(scpi_parser_primary_status)
		{
		case SCPI_PRETREATMENT:
			scpi_pretreatment();
			break;
		case SCPI_PARSER_START:
			p_scpi_inside = scpi_inside_buffer;
			p_scpi_level = p_scpi_visible_level = (scpi_special_keyword *)scpi_special_command_table;
			scpi_parser_start();
			break;
		case SCPI_PARSER_HEAD:
			scpi_param_buffer[0] = 0;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_START;
			p_scpi_level = p_scpi_visible_level;
			scpi_parser_head();
			break;
		case SCPI_PARSER_PARAM:
			scpi_parser_secondary_status = SCPI_PARAMETER_START;
			p_scpi_param_write = scpi_param_buffer + 1;
			scpi_parser_parameter();
			break;
		case SCPI_PARSER_PAUSE:
			scpi_parser_primary_status = SCPI_PARSER_HEAD;
			return;
		case SCPI_PARSER_END:
			if(scpi_parser_return_code)
			{
				scpi_func_output = NULL;
			}
			scpi_parser_is_idle = TRUE;
			p_scpi_input_head = p_scpi_input_tail = scpi_input_buffer;
			p_scpi_inside = scpi_inside_buffer;
			scpi_parser_primary_status = SCPI_PRETREATMENT;
			return;
		default:
			break;
		}
	}
}

void scpi_pretreatment(void)
{
#ifdef __SUPPORT__MACRO__
	unsigned short i;
#endif

	p_scpi_input_head = scpi_input_buffer;
	p_scpi_input_tail = scpi_input_buffer;
	p_scpi_inside = scpi_inside_buffer;

	while((*p_scpi_input_head <= ' ') && (*p_scpi_input_head != SCPI_END_OF_INPUT))
	{
		p_scpi_input_head++;
	}
	if(*p_scpi_input_head == SCPI_END_OF_INPUT)
	{
		scpi_parser_return_code = ERR_NO_ERROR;
		scpi_parser_primary_status = SCPI_PARSER_END;
		return;
	}

#ifdef __SUPPORT__MACRO__
	if(scpi_macro_enable)
	{
	}
	else
#endif
	{
		while(*p_scpi_input_head != SCPI_END_OF_INPUT)
		{
			*p_scpi_inside++ = tolower(*p_scpi_input_head++);
		}
		*p_scpi_inside = *p_scpi_input_head;

		scpi_parser_primary_status = SCPI_PARSER_START;
		return;
	}
}

void scpi_parser_start(void)
{
	while(TRUE)
	{
		if(*p_scpi_inside == SCPI_END_OF_INPUT)
		{
			scpi_parser_is_idle = TRUE;
			scpi_parser_return_code = ERR_NO_ERROR;
			scpi_parser_primary_status = SCPI_PARSER_END;
			return;
		}
		else if(*p_scpi_inside <= ' ')
		{
			p_scpi_inside++;
			continue;
		}
		else if(*p_scpi_inside == ';')
		{
			//----------------------------------------
			p_scpi_inside++;//@add luguangping 20110704
			continue;//@add luguangping 20110704
			//----------------------------------------

			//scpi_parser_return_code = ERR_SEMICOLON_UNWANTED;
			//scpi_parser_primary_status = SCPI_PARSER_END;
			//return;
		}
		else
		{
			scpi_parser_primary_status = SCPI_PARSER_HEAD;
			return;
		}
	}
}

void scpi_parser_head(void)
{
	while(TRUE)
	{
		switch(scpi_parser_secondary_status)
		{
		case SCPI_COMMAND_HEAD_START:
			scpi_command_head_start();
			break;
		case SCPI_COMMON_HEAD:
			scpi_common_head();
			break;
		case SCPI_SPECIAL_HEAD:
			scpi_special_head();
			break;
		case SCPI_COMMAND_HEAD_END:
			scpi_command_head_end();
			return;
		case SCPI_COMMAND_HEAD_NONE:                         //@add luguangping 20110704
			scpi_parser_primary_status = SCPI_PARSER_END;    //@add luguangping 20110704
			return;  
		default:
			return;
		}
	}
}

void scpi_command_head_start(void)
{
	while(TRUE)
	{
		if(*p_scpi_inside == '*')
		{
			scpi_parser_secondary_status = SCPI_COMMON_HEAD;
			return;
		}
		else if(*p_scpi_inside == ':')
		{
			if((*(++p_scpi_inside) >= 'a') && (*p_scpi_inside <= 'z'))
			{
				p_scpi_level = p_scpi_visible_level = (scpi_special_keyword *)scpi_special_command_table;
				scpi_parser_secondary_status = SCPI_SPECIAL_HEAD;
			}
			else
			{
				scpi_parser_return_code = ERR_COMMAND_HEADER_ERROR;
				scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			}
			return;
		}
		else if((*p_scpi_inside >= 'a') && (*p_scpi_inside <= 'z'))
		{
			scpi_parser_secondary_status = SCPI_SPECIAL_HEAD;
			return;
		}
		else if((*p_scpi_inside == SCPI_END_OF_INPUT))
		{
			scpi_parser_return_code= ERR_NO_ERROR;//luguangping 20110701

			//scpi_parser_return_code = ERR_SEMICOLON_UNWANTED;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_NONE; //@add luguangping 20110704
			return;
		}
		else if((*p_scpi_inside == ';'))
		{
			//scpi_parser_return_code= ERR_NO_ERROR;//luguangping 20110701			
			//scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END; //@add luguangping 20110704
			//return;

			//----------------------------------------
			p_scpi_inside++;//@add luguangping 20110704
			continue;//@add luguangping 20110704
                        //----------------------------------------
		}
		/*
		else if((*p_scpi_inside == SCPI_END_OF_INPUT) || (*p_scpi_inside == ';'))
		{	
			scpi_parser_return_code = ERR_SEMICOLON_UNWANTED;
			scpi_parser_secondary_status= SCPI_COMMAND_HEAD_END;
			//scpi_parser_return_code= ERR_NO_ERROR;//luguangping 20110701		
			//scpi_parser_secondary_status = SCPI_COMMAND_HEAD_NONE; //@add luguangping 20110704
			return;
		}*/
		else if(*p_scpi_inside <= ' ')
		{
			p_scpi_inside++;
			continue;
		}
		else
		{
			scpi_parser_return_code = ERR_COMMAND_HEADER_ERROR;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			return;
		}
	}
}

void scpi_common_head(void)
{
	int i;
	memset(scpi_keyword_copy,0,13);
	scpi_keyword_copy[0] = *p_scpi_inside;

	i = 1;
	while((*(++p_scpi_inside) >= 'a') && (*p_scpi_inside <= 'z'))
	{
		if(i <= 3)
		{
			scpi_keyword_copy[i++] = *p_scpi_inside;
		}
		else
		{
			scpi_parser_return_code = ERR_COMMAND_HEADER_ERROR;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			return;
		}
	}
	if( i < 3)
	{
		scpi_parser_return_code = ERR_COMMAND_HEADER_ERROR;
		scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
		return;
	}
	if(*p_scpi_inside == '?')
	{
		scpi_keyword_copy[i] = *p_scpi_inside;
		if((*(++p_scpi_inside) == SCPI_END_OF_INPUT) || (*p_scpi_inside == ';'))
		{
			scpi_ransack_common_table();
			return;
		}
		else if(*p_scpi_inside <= ' ')
		{
			p_scpi_inside++;
			scpi_ransack_common_table();
			return;
		}
		else
		{
			scpi_parser_return_code = ERR_COMMAND_HEADER_ERROR;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			return;
		}
	}
	else if((*p_scpi_inside == SCPI_END_OF_INPUT) || (*p_scpi_inside == ';'))
	{
		scpi_ransack_common_table();
		return;
	}
	else if(*p_scpi_inside <= ' ')
	{
		p_scpi_inside++;
		scpi_ransack_common_table();
		return;
	}
	else
	{
		scpi_parser_return_code = ERR_COMMAND_HEADER_ERROR;
		scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
		return;
	}
}

void scpi_ransack_common_table(void)
{
	p_scpi_common = (scpi_common_keyword *)scpi_common_command_table;

	while(TRUE)
	{
		if(strcmp((char*)scpi_keyword_copy,(char*)p_scpi_common->name))
		{
			if(p_scpi_common->Next == -1)
			{
				scpi_parser_return_code = ERR_UNDEFINED_HEADER;
				scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
				return;
			}
			else
				p_scpi_common = (scpi_common_keyword *)scpi_common_command_table + p_scpi_common->Next;
		}
		else
		{
			scpi_command_type = SCPI_COMMON;
			scpi_func_output = p_scpi_common->p_function;
			scpi_parser_return_code = ERR_NO_ERROR;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			return;
		}
	}
}

void scpi_special_head(void)
{
	int i;
	memset(scpi_keyword_copy,0,13);
	scpi_keyword_copy[0] = *p_scpi_inside;

	i = 1;
	while(((*(++p_scpi_inside)>='a')&&(*p_scpi_inside<='z'))||((*p_scpi_inside>='0')&&(*p_scpi_inside<='9'))||(*p_scpi_inside=='_'))
	{
		if(i <= 11)
		{
			scpi_keyword_copy[i++] = *p_scpi_inside;
		}
		else
		{
			scpi_parser_return_code = ERR_PROGRAM_MNEMONIC_TOO_LONG;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			return;
		}
	}
	if(*p_scpi_inside == '?')
	{
		scpi_keyword_copy[i] = *p_scpi_inside;
		if((*(++p_scpi_inside) == SCPI_END_OF_INPUT) || (*p_scpi_inside == ';'))
		{
			scpi_middle_keyword = FALSE;
			scpi_ransack_special_table();
			return;
		}
		else if(*p_scpi_inside <= ' ')
		{
			p_scpi_inside++;
			scpi_middle_keyword = FALSE;
			scpi_ransack_special_table();
			return;
		}
		else
		{
			scpi_parser_return_code = ERR_COMMAND_HEADER_ERROR;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			return;
		}
	}
	else if((*p_scpi_inside == SCPI_END_OF_INPUT) || (*p_scpi_inside == ';'))
	{
		scpi_middle_keyword = FALSE;
		scpi_ransack_special_table();
		return;
	}
	else if(*p_scpi_inside <= ' ')
	{
		p_scpi_inside++;
		scpi_middle_keyword = FALSE;
		scpi_ransack_special_table();
		return;
	}
	else if(*p_scpi_inside == ':')
	{
		if((*(++p_scpi_inside) >= 'a') && (*p_scpi_inside <= 'z'))
		{
			scpi_middle_keyword = TRUE;
			scpi_ransack_special_table();
		}
		else
		{
			scpi_parser_return_code = ERR_COMMAND_HEADER_ERROR;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
		}
		return;
	}
	else
	{
		scpi_parser_return_code = ERR_COMMAND_HEADER_ERROR;
		scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
		return;
	}
}

void scpi_ransack_special_table(void)
{
	char *p_query;
	p_scpi_keyword = (scpi_special_keyword *)scpi_special_command_table + p_scpi_level->FirstChild;
	if(p_query = strchr((char*)scpi_keyword_copy,'?'))
	{
		scpi_command_type = SCPI_SPE_QUE;
		*p_query = 0;
	}
	else
	{
		scpi_command_type = SCPI_SPECIAL;
	}

	while(TRUE)
	{
		if(strcmp((char*)scpi_keyword_copy,(char*)p_scpi_keyword->name)&&strcmp((char*)scpi_keyword_copy,(char*)p_scpi_keyword->shortname))
		{
			if(p_scpi_keyword->NextBrother != -1)
				p_scpi_keyword = (scpi_special_keyword *)scpi_special_command_table + p_scpi_keyword->NextBrother;
			else
				break;
		}
		else
		{
			if(scpi_middle_keyword)
			{
				if(p_scpi_keyword->FirstChild == -1)
				{
					scpi_parser_return_code = ERR_UNDEFINED_HEADER;
					scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
					return;
				}
				else
				{
					p_scpi_visible_level = p_scpi_keyword;
					p_scpi_level = p_scpi_keyword;
					scpi_parser_return_code = ERR_NO_ERROR;
					scpi_parser_secondary_status = SCPI_SPECIAL_HEAD;
					return;
				}
			}
			else
			{
				if(p_scpi_keyword->FirstChild != -1)
				{
					scpi_exist_default_path();
					return;
				}
				else if((p_query?p_scpi_keyword->p_q_function:p_scpi_keyword->p_function) == NULL)
				{
					if(p_query)
					{
						scpi_parser_return_code = ERR_COMMAND_CAN_NOT_QUERY;
						scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
						return;
					}
					else
					{
						scpi_parser_return_code = ERR_COMMAND_MUST_QUERY;
						scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
						return;
					}
				}
				else
				{
					scpi_func_output = p_query?p_scpi_keyword->p_q_function:p_scpi_keyword->p_function;
					scpi_parser_return_code = ERR_NO_ERROR;
					scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
					return;
				}
			}
		}
	}
	p_scpi_keyword = (scpi_special_keyword *)scpi_special_command_table + p_scpi_level->FirstChild;
	while(TRUE)
	{
		if(p_scpi_keyword->IsDefault)
		{
			if(p_scpi_keyword->FirstChild != -1)
			{
				p_scpi_level = p_scpi_keyword;
				if(p_query)
				{
					*p_query = '?';
				}
				scpi_ransack_special_table();
				return;
			}
			else
			{
				scpi_parser_return_code = ERR_UNDEFINED_HEADER;
				scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
				return;
			}
		}
		else if(p_scpi_keyword->NextBrother != -1)
		{
			p_scpi_keyword = (scpi_special_keyword *)scpi_special_command_table + p_scpi_keyword->NextBrother;
		}
		else
		{
			scpi_parser_return_code = ERR_UNDEFINED_HEADER;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			return;
		}
	}
}

void scpi_exist_default_path(void)
{
	p_scpi_level = p_scpi_keyword;
	p_scpi_keyword = (scpi_special_keyword *)scpi_special_command_table + p_scpi_keyword->FirstChild;

	while(TRUE)
	{
		if(p_scpi_keyword->IsDefault)
		{
			if(scpi_command_type == SCPI_SPE_QUE)
			{
				if(p_scpi_keyword->FirstChild != -1)
				{
					scpi_exist_default_path();
					return;
				}
				else if(p_scpi_keyword->p_q_function == NULL)
				{
					scpi_parser_return_code = ERR_COMMAND_CAN_NOT_QUERY;
					scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
					return;
				}
				else
				{
					scpi_func_output = p_scpi_keyword->p_q_function;
					scpi_parser_return_code = ERR_NO_ERROR;
					scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
					return;
				}
			}
			else
			{
				if(p_scpi_keyword->FirstChild != -1)
				{
					scpi_exist_default_path();
					return;
				}
				else if(p_scpi_keyword->p_function == NULL)
				{
					scpi_parser_return_code = ERR_COMMAND_MUST_QUERY;
					scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
					return;
				}
				else
				{
					scpi_func_output = p_scpi_keyword->p_function;
					scpi_parser_return_code = ERR_NO_ERROR;
					scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
					return;
				}
			}
		}
		else if(p_scpi_keyword->NextBrother != -1)
		{
			p_scpi_keyword = (scpi_special_keyword *)scpi_special_command_table + p_scpi_keyword->NextBrother;
		}
		else
		{
			scpi_parser_return_code = ERR_UNDEFINED_HEADER;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			return;
		}
	}
}

void scpi_command_head_end(void)
{
	if(scpi_parser_return_code)
	{
		scpi_parser_primary_status = SCPI_PARSER_END;
		return;
	}

	while(TRUE)
	{
		if(*p_scpi_inside == SCPI_END_OF_INPUT)
		{
			if(SCPI_PARAM_NUM > 0)
			{
				scpi_func_output = NULL;
				scpi_parser_return_code = ERR_MISSING_PARAMETER;
				scpi_parser_primary_status = SCPI_PARSER_END;
				return;
			}
			else
			{
				scpi_parser_return_code = ERR_NO_ERROR;
				scpi_parser_primary_status = SCPI_PARSER_END;
				return;
			}
		}
		else if(*p_scpi_inside <= ' ')
		{
			p_scpi_inside++;
			continue;
		}
		else if(*p_scpi_inside == ';')
		{
			if(SCPI_PARAM_NUM > 0)
			{
				scpi_parser_return_code = ERR_MISSING_PARAMETER;
				scpi_parser_primary_status = SCPI_PARSER_END;
				return;
			}
			else
			{
				p_scpi_inside++;
				scpi_parser_return_code = ERR_NO_ERROR;
				scpi_parser_primary_status = SCPI_PARSER_PAUSE;
				return;
			}
		}
		else
		{
			scpi_parser_primary_status = SCPI_PARSER_PARAM;
			return;
		}
	}
}

void scpi_parser_parameter(void)
{
	scpi_param_index = 1;
	while(TRUE)
	{
		switch(scpi_parser_secondary_status)
		{
		case SCPI_PARAMETER_START:
			scpi_parameter_start();
			break;
		case SCPI_PARAMETER_DECIMAL:
			scpi_parameter_decimal();
			break;
		case SCPI_PARAMETER_KEYWORD:
			scpi_parameter_keyword();
			break;
		case SCPI_PARAMETER_NONDECIMAL:
			scpi_parameter_nondecimal();
			break;
		case SCPI_PARAMETER_STRING:
			scpi_parameter_string();
			break;
		case SCPI_PARAMETER_EXPRESSION:
			scpi_parameter_expression();
			break;
		case SCPI_PARAMETER_BLOCK:
			scpi_parameter_block();
			break;
		case SCPI_PARAMETER_UNITS:
			scpi_parameter_units();
			break;
		case SCPI_PARAMETER_PAUSE:
			scpi_parameter_pause();
			break;
		case SCPI_PARAMETER_END:
			if(scpi_parser_return_code)
			{
				scpi_func_output = NULL;
				scpi_parser_primary_status = SCPI_PARSER_END;
			}
			return;
		default:
			return;
		}
	}
}

void scpi_parameter_start(void)
{
	if(((*p_scpi_inside >= '0') && (*p_scpi_inside <= '9')) || (*p_scpi_inside == '+') || (*p_scpi_inside == '-') || (*p_scpi_inside == '.'))
	{
		scpi_parser_secondary_status = SCPI_PARAMETER_DECIMAL;
	}
	else if((*p_scpi_inside >= 'a') && (*p_scpi_inside <= 'z'))
	{
		scpi_parser_secondary_status = SCPI_PARAMETER_KEYWORD;
	}
	else if(*p_scpi_inside == '#')
	{
		if((*(++p_scpi_inside) == 'b') || (*p_scpi_inside == 'q') || (*p_scpi_inside == 'h'))
		{
			scpi_parser_secondary_status = SCPI_PARAMETER_NONDECIMAL;
		}
		else if((*p_scpi_inside >= '0') && (*p_scpi_inside <= '9'))
		{
			scpi_parser_secondary_status = SCPI_PARAMETER_BLOCK;
		}
		else
		{
			scpi_parser_return_code = ERR_PARAMETER_ERROR;
			scpi_parser_secondary_status = SCPI_PARAMETER_END;
			return;
		}
	}
	else if((*p_scpi_inside == '\'') || (*p_scpi_inside == '\"'))
	{
		scpi_parser_secondary_status = SCPI_PARAMETER_STRING;
	}
	else if(*p_scpi_inside == '(')
	{
		scpi_parser_secondary_status = SCPI_PARAMETER_EXPRESSION;
	}
	else if(*p_scpi_inside == ',')
	{
		scpi_parser_return_code = ERR_COMMA_UNWANTED;
		scpi_parser_secondary_status = SCPI_PARAMETER_END;
		return;
	}
	else
	{
		scpi_parser_return_code = ERR_PARAMETER_ERROR;
		scpi_parser_secondary_status = SCPI_PARAMETER_END;
		return;
	}

	if((SCPI_PARAM_NUM != -1) && (scpi_param_index > SCPI_PARAM_NUM))
	{
		scpi_parser_return_code = ERR_PARAMETER_NOT_ALLOWED;
		scpi_parser_secondary_status = SCPI_PARAMETER_END;
	}
	return;
}

void scpi_parameter_decimal(void)
{
	int i = 0;
	bool scpi_point_exist = FALSE;
	bool scpi_leading_zero_exist = FALSE;
	memset(scpi_param_base,0,257);
	memset(scpi_param_power,0,7);

	if((*p_scpi_inside == '+') || (*p_scpi_inside == '-'))
	{
		scpi_param_base[i++] = *p_scpi_inside;
		p_scpi_inside++;
	}
	else
	{
		scpi_param_base[i++] = '+';
	}

	while(*p_scpi_inside == '0')
	{
		scpi_leading_zero_exist = TRUE;
		p_scpi_inside++;
	}

	while(((*p_scpi_inside >= '0') && (*p_scpi_inside <= '9')) || (*p_scpi_inside == '.'))
	{
		if(i < 256)
		{
			if((*p_scpi_inside >= '0') && (*p_scpi_inside <= '9'))
			{
				scpi_param_base[i++] = *p_scpi_inside++;
			}
			else if((*p_scpi_inside == '.') && (!scpi_point_exist))
			{
				scpi_param_base[i++] = *p_scpi_inside++;
				scpi_point_exist = TRUE;
				if((i == 2) && (!scpi_leading_zero_exist) && ((*p_scpi_inside < '0') || (*p_scpi_inside > '9')))
				{
					scpi_parser_return_code = ERR_NUMERIC_DATA_ERROR;
					scpi_parser_secondary_status = SCPI_PARAMETER_END;
					return;
				}
			}
			else
			{
				scpi_parser_return_code = ERR_NUMERIC_DATA_ERROR;
				scpi_parser_secondary_status = SCPI_PARAMETER_END;
				return;
			}
		}
		else
		{
			scpi_parser_return_code = ERR_TOO_MANY_DIGITS;
			scpi_parser_secondary_status = SCPI_PARAMETER_END;
			return;
		}
	}

	if(i == 1)
	{
		if(scpi_leading_zero_exist)
		{
			scpi_param_base[i] = '0';
		}
		else
		{
			scpi_parser_return_code = ERR_NUMERIC_DATA_ERROR;
			scpi_parser_secondary_status = SCPI_PARAMETER_END;
			return;
		}
	}

	if(scpi_point_exist)
	{
		sscanf((char*)scpi_param_base,"%f",&scpi_float_value);
	}
	else
	{
		sscanf((char*)scpi_param_base,"%d",&scpi_int_value);
	}

	while((*p_scpi_inside <= ' ') && (*p_scpi_inside != SCPI_END_OF_INPUT))
	{
		p_scpi_inside++;
	}
	if((*p_scpi_inside == ',') || (*p_scpi_inside == ';') || (*p_scpi_inside == SCPI_END_OF_INPUT))
	{
		if(scpi_point_exist)
		{
			if(p_scpi_param_write < scpi_param_buffer + SCPI_PARAM_BUF_LEN - 3 - sizeof(float))
			{
				scpi_param_buffer[0]++;
				*p_scpi_param_write++ = BIT_INDICATES_FLOAT;
				memcpy(p_scpi_param_write,&scpi_float_value,sizeof(float));
				p_scpi_param_write += sizeof(float);
			}
			else
			{
				scpi_parser_return_code = ERR_OUT_OF_MEMORY;
				scpi_parser_secondary_status = SCPI_PARAMETER_END;
				return;
			}
		}
		else
		{
			if(p_scpi_param_write < scpi_param_buffer + SCPI_PARAM_BUF_LEN - 3 - sizeof(int))
			{
				scpi_param_buffer[0]++;
				*p_scpi_param_write++ = BIT_INDICATES_INT;
				memcpy(p_scpi_param_write,&scpi_int_value,sizeof(int));
				p_scpi_param_write += sizeof(int);
			}
			else
			{
				scpi_parser_return_code = ERR_OUT_OF_MEMORY;
				scpi_parser_secondary_status = SCPI_PARAMETER_END;
				return;
			}
		}
		scpi_parser_secondary_status = SCPI_PARAMETER_PAUSE;
		return;
	}
	else if(((*p_scpi_inside >= 'a') && (*p_scpi_inside <= 'z')) || (*p_scpi_inside == '/'))
	{
		if(*p_scpi_inside == 'e')	//下段代码判断 e 是不是指数的一部分
		{
			p_scpi_input_tail = p_scpi_inside++;
			if((*p_scpi_inside == SCPI_END_OF_INPUT) || ((*p_scpi_inside >= 'a') && (*p_scpi_inside <= 'z')) || (*p_scpi_inside == '/'))
			{
				p_scpi_inside = p_scpi_input_tail;
				scpi_parser_secondary_status = SCPI_PARAMETER_UNITS;
				return;
			}
			else
			{
				while(*p_scpi_inside <= ' ')
				{
					p_scpi_inside++;
				}
				if((*p_scpi_inside == ',') || (*p_scpi_inside == ';') || (*p_scpi_inside == SCPI_END_OF_INPUT))
				{
					p_scpi_inside = p_scpi_input_tail;
					scpi_parser_secondary_status = SCPI_PARAMETER_UNITS;
					return;
				}
				else if(((*p_scpi_inside >= '0') && (*p_scpi_inside <= '9')) || (*p_scpi_inside == '+') || (*p_scpi_inside == '-'))	// 指数参数形式
				{
					i = 0;
					scpi_leading_zero_exist = FALSE;

					if(!scpi_point_exist)
					{
						scpi_float_value = (float)scpi_int_value;
					}

					if((*p_scpi_inside == '+') || (*p_scpi_inside == '-'))
					{
						scpi_param_power[i++] = *p_scpi_inside++;
					}
					else
					{
						scpi_param_power[i++] = '+';
					}
					while(*p_scpi_inside == '0')
					{
						scpi_leading_zero_exist = TRUE;
						p_scpi_inside++;
					}
					while((*p_scpi_inside >= '0') && (*p_scpi_inside <= '9'))
					{
						if(i < 6)
						{
							scpi_param_power[i++] = *p_scpi_inside++;
						}
						else
						{
							scpi_parser_return_code = ERR_EXPONENT_TOO_LARGE;
							scpi_parser_secondary_status = SCPI_PARAMETER_END;
							return;
						}
					}
					if(i == 1)
					{
						if(scpi_leading_zero_exist)
						{
							scpi_param_power[i] = '0';
						}
						else
						{
							scpi_parser_return_code = ERR_NUMERIC_DATA_ERROR;
							scpi_parser_secondary_status = SCPI_PARAMETER_END;
							return;
						}
					}
					sscanf((char*)scpi_param_power,"%d",&scpi_power_value);
					if((scpi_power_value > 32000) || (scpi_power_value < -32000))
					{
						scpi_parser_return_code = ERR_EXPONENT_TOO_LARGE;
						scpi_parser_secondary_status = SCPI_PARAMETER_END;
						return;
					}
					if(scpi_power_value > 0)
					{
						for(i = 1;i <= scpi_power_value;i++)
						{
							scpi_float_value *= 10;
						}
					}
					else
					{
						for(i = -1;i >= scpi_power_value;i--)
						{
							scpi_float_value /= 10;
						}
					}

					while((*p_scpi_inside <= ' ') && (*p_scpi_inside != SCPI_END_OF_INPUT))
					{
						p_scpi_inside++;
					}
					if((*p_scpi_inside == ',') || (*p_scpi_inside == ';') || (*p_scpi_inside == SCPI_END_OF_INPUT))
					{
						if(p_scpi_param_write < scpi_param_buffer + SCPI_PARAM_BUF_LEN - 3 - sizeof(float))
						{
							scpi_param_buffer[0]++;
							*p_scpi_param_write++ = BIT_INDICATES_FLOAT;
							memcpy(p_scpi_param_write,&scpi_float_value,sizeof(float));
							p_scpi_param_write += sizeof(float);
						}
						else
						{
							scpi_parser_return_code = ERR_OUT_OF_MEMORY;
							scpi_parser_secondary_status = SCPI_PARAMETER_END;
							return;
						}
						scpi_parser_secondary_status = SCPI_PARAMETER_PAUSE;
						return;
					}
					else if(((*p_scpi_inside >= 'a') && (*p_scpi_inside <= 'z')) || (*p_scpi_inside == '/'))
					{
						scpi_parser_secondary_status = SCPI_PARAMETER_UNITS;
						return;
					}
					else if((*p_scpi_inside >= '0') && (*p_scpi_inside <= '9'))
					{
						scpi_parser_return_code = ERR_INVALID_SEPARATOR;
						scpi_parser_secondary_status = SCPI_PARAMETER_END;
						return;
					}
					else
					{
						scpi_parser_return_code = ERR_INVALID_CHARACTER;
						scpi_parser_secondary_status = SCPI_PARAMETER_END;
						return;
					}
				}
				else
				{
					scpi_parser_return_code = ERR_NUMERIC_DATA_ERROR;
					scpi_parser_secondary_status = SCPI_PARAMETER_END;
					return;
				}
			}
		}
		else
		{
			scpi_parser_secondary_status = SCPI_PARAMETER_UNITS;
			return;
		}
	}
	else if((*p_scpi_inside >= '0') && (*p_scpi_inside <= '9'))
	{
		scpi_parser_return_code = ERR_INVALID_SEPARATOR;
		scpi_parser_secondary_status = SCPI_PARAMETER_END;
		return;
	}
	else
	{
		scpi_parser_return_code = ERR_INVALID_CHARACTER;
		scpi_parser_secondary_status = SCPI_PARAMETER_END;
		return;
	}
}

void scpi_parameter_keyword(void)
{
	int i;
	memset(scpi_keyword_copy,0,SCPI_INPUT_PARAM_LEN+1);

	i = 0;
	while(((*p_scpi_inside >= 'a') && (*p_scpi_inside <= 'z')) || ((*p_scpi_inside >= '0') && (*p_scpi_inside <= '9')) || (*p_scpi_inside == '_'))
	{
		if(i < SCPI_INPUT_PARAM_LEN)
		{
			scpi_keyword_copy[i++] = *p_scpi_inside++;
		}
		else
		{
			scpi_parser_return_code = ERR_CHARACTER_DATA_TOO_LONG;
			scpi_parser_secondary_status = SCPI_PARAMETER_END;
			return;
		}
	}
	if(p_scpi_param_write < scpi_param_buffer + SCPI_PARAM_BUF_LEN - 3 - i)
	{
		scpi_param_buffer[0]++;
		*p_scpi_param_write++ = BIT_INDICATES_STRING;
		memcpy(p_scpi_param_write,scpi_keyword_copy,i);
		p_scpi_param_write += i;
		*p_scpi_param_write++ = 0;
	}
	else
	{
		scpi_parser_return_code = ERR_OUT_OF_MEMORY;
		scpi_parser_secondary_status = SCPI_PARAMETER_END;
		return;
	}
	scpi_parser_secondary_status = SCPI_PARAMETER_PAUSE;
	return;
}

void scpi_parameter_nondecimal(void)
{
#ifdef __SUPPORT__NONDECIMAL__

#else
	scpi_parser_return_code = ERR_INVALID_FORMAT;
	scpi_parser_secondary_status = SCPI_PARAMETER_END;
	return;
#endif
}

void scpi_parameter_string(void)
{
#ifdef __SUPPORT__STRING__

#else
	scpi_parser_return_code = ERR_INVALID_FORMAT;
	scpi_parser_secondary_status = SCPI_PARAMETER_END;
	return;
#endif
}

void scpi_parameter_expression(void)
{
#ifdef __SUPPORT__EXPRESSION__

#else
	scpi_parser_return_code = ERR_INVALID_FORMAT;
	scpi_parser_secondary_status = SCPI_PARAMETER_END;
	return;
#endif
}

void scpi_parameter_block(void)
{
#ifdef __SUPPORT__BLOCK__

#else
	scpi_parser_return_code = ERR_INVALID_FORMAT;
	scpi_parser_secondary_status = SCPI_PARAMETER_END;
	return;
#endif
}

void scpi_parameter_units(void)
{
	scpi_parser_return_code = ERR_INVALID_FORMAT;
	scpi_parser_secondary_status = SCPI_PARAMETER_END;
	return;
}

void scpi_parameter_pause(void)
{
	while(TRUE)
	{
		if(*p_scpi_inside == SCPI_END_OF_INPUT)
		{
			if(SCPI_PARAM_NUM > scpi_param_index)
			{
				scpi_parser_return_code = ERR_MISSING_PARAMETER;
			}
			else
			{
				scpi_parser_return_code = ERR_NO_ERROR;
				scpi_parser_primary_status = SCPI_PARSER_END;
			}
			scpi_parser_secondary_status = SCPI_PARAMETER_END;
			return;
		}
		else if(*p_scpi_inside == ';')
		{
			if(SCPI_PARAM_NUM > scpi_param_index)
			{
				scpi_parser_return_code = ERR_MISSING_PARAMETER;
			}
			else
			{
				p_scpi_inside++;
				scpi_parser_return_code = ERR_NO_ERROR;
				scpi_parser_primary_status = SCPI_PARSER_PAUSE;
			}
			scpi_parser_secondary_status = SCPI_PARAMETER_END;
			return;
		}
		else if(*p_scpi_inside <= ' ')
		{
			p_scpi_inside++;
			continue;
		}
		else if(*p_scpi_inside == ',')
		{
			while(TRUE)
			{
				if((*(++p_scpi_inside) == SCPI_END_OF_INPUT) || (*p_scpi_inside == ';'))
				{
					scpi_parser_return_code = ERR_COMMA_UNWANTED;
					scpi_parser_secondary_status = SCPI_PARAMETER_END;
					return;
				}
				else if(*p_scpi_inside <= ' ')
				{
					continue;
				}
				else
				{
					scpi_param_index++;
					scpi_parser_secondary_status = SCPI_PARAMETER_START;
					return;
				}
			}
		}
		else if(((*p_scpi_inside >= 'a') && (*p_scpi_inside <= 'z')) || ((*p_scpi_inside >= '0') && (*p_scpi_inside <= '9')))
		{
			scpi_parser_return_code = ERR_INVALID_SEPARATOR;
			scpi_parser_secondary_status = SCPI_PARAMETER_END;
			return;
		}
		else
		{
			scpi_parser_return_code = ERR_PARAMETER_ERROR;
			scpi_parser_secondary_status = SCPI_PARAMETER_END;
			return;
		}
	}
}

short scpi_macro_define(void)
{
	return 0;
}

//测试函数
/*
short scpi_function_test(void)
{
int i;
int j = 1;
float x;
int y;

if(scpi_command_type == SCPI_COMMON)
{
//printf("Common Command:%s.\nParameter buffer:\t",p_scpi_common->name);
}
else if(scpi_command_type == SCPI_SPECIAL)
{
//printf("Special Command: %d.\nParameter buffer:\t",p_scpi_keyword - scpi_special_command_table);
}
else if(scpi_command_type == SCPI_SPE_QUE)
{
//printf("Special Query: %d.\nParameter buffer:\t",p_scpi_keyword - scpi_special_command_table);
}
else
{
//printf("Some error in parser we don't know.\n");
return 0;
}

for(i = 0;i < scpi_param_buffer[0];i++)
{
if(scpi_param_buffer[j] == BIT_INDICATES_STRING)
{
while(scpi_param_buffer[++j] != 0)
{
//printf("%c",scpi_param_buffer[j]);
}
//printf("\t");
j++;
}
else if(scpi_param_buffer[j] == BIT_INDICATES_INT)
{
j++;
memcpy(&y,scpi_param_buffer+j,sizeof(int));
//printf("%d\t",y);
j += sizeof(int);
}
else if(scpi_param_buffer[j] == BIT_INDICATES_FLOAT)
{
j++;
memcpy(&x,scpi_param_buffer+j,sizeof(float));
printf("%f\t",x);
j += sizeof(float);
}
else
{
//printf("Some error happend in parameter buffer!\n");
}
}
//printf("\n");
scpi_param_buffer[0] = 0;

return 1;
}*/