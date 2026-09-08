/****************************************************************************************
*                                                                                       *
*	File Name	: scpi_variable.cpp                                                       *
*	Author		: R&D Center,Shenzhen Zidetech Co. Ltd.								*
*	Version		: V2.0																	*
*	Date		: 08/12/2009															*
*	Description	: This file contains all the variables used in both scpi_analyse and	*
*				  your embedded code.													*
*																						*
****************************************************************************************/

#include "scpi_interface.h"
#include "scpi_parser.h"

//接口变量
bool	scpi_parser_is_idle;
int	 scpi_parser_return_code;

unsigned char	scpi_input_buffer[SCPI_INPUT_BUF_LEN];
unsigned char	scpi_param_buffer[SCPI_PARAM_BUF_LEN];
p_scpi_func_type	scpi_func_output;

// scpi_analyse 所用到状态变量
unsigned char	scpi_parser_primary_status = SCPI_PRETREATMENT;
unsigned char	scpi_parser_secondary_status;

// scpi_analyse 所用到的二值逻辑判断标志
bool	scpi_middle_keyword;

#ifdef  __SUPPORT__MACRO__
bool	scpi_macro_enable = FALSE;
#endif

// scpi_analyse 所用到的指针变量
unsigned char	*p_scpi_input_head = scpi_input_buffer;
unsigned char	*p_scpi_input_tail = scpi_input_buffer;
unsigned char	*p_scpi_inside = scpi_inside_buffer;
unsigned char	*p_scpi_param_write;

scpi_common_keyword		*p_scpi_common;
scpi_special_keyword	*p_scpi_keyword;
scpi_special_keyword	*p_scpi_level;
scpi_special_keyword	*p_scpi_visible_level;

// scpi_analyse 所用到的其他变量
unsigned char	scpi_inside_buffer[SCPI_INPUT_BUF_LEN];
unsigned char	scpi_keyword_copy[SCPI_INPUT_PARAM_LEN+1];
unsigned char	scpi_command_type;
unsigned char	scpi_param_index;

unsigned char	scpi_param_base[257];
unsigned char	scpi_param_power[30];

int		scpi_int_value;
int		scpi_power_value;
float	scpi_float_value;

#ifdef  __SUPPORT__MACRO__
scpi_macro scpi_macro_table[SCPI_MACRO_NUMBER] = {{"abcde","auto:test:result:step"},};
#endif