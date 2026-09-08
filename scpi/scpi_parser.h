
#ifndef _SCPI_PARSER_H_
#define _SCPI_PARSER_H_

/****************************************************************************************
*                                                                                       *
*	File Name	: scpi_parser.h		                                                    *
*	Author		: R&D Center,Shenzhen Zidetech Co. Ltd.								*
*	Version		: V2.0																	*
*	Date		: 08/12/2009															*
*	Description	: This file contains all the declare of functions about how the 		*
*                 scpi_parser work.													*
*																						*
****************************************************************************************/


#include "../include.h"

//命令类型代码定义
#define SCPI_COMMON		0x00
#define SCPI_SPECIAL	0x01
#define SCPI_SPE_QUE	0x02

// scpi_analyse 所用到的 6 个主状态及状态变量
#define SCPI_PRETREATMENT	0x00
#define SCPI_PARSER_START	0x01
#define SCPI_PARSER_HEAD	0x02
#define SCPI_PARSER_PARAM	0x03
#define SCPI_PARSER_PAUSE	0x04
#define SCPI_PARSER_END		0x05
extern unsigned char scpi_parser_primary_status;

// scpi_analyse 所用到的二级状态及状态变量
//SCPI_PARSER_HEAD
#define SCPI_COMMAND_HEAD_START		0x00
#define SCPI_COMMON_HEAD			0x01
#define SCPI_SPECIAL_HEAD			0x02
#define SCPI_COMMAND_HEAD_END		0x03
#define SCPI_COMMAND_HEAD_NONE      0x04    //@add luguangping 20110704
//SCPI_PARSER_PARAM
#define SCPI_PARAMETER_START		0x04
#define SCPI_PARAMETER_DECIMAL		0x05
#define SCPI_PARAMETER_KEYWORD		0x06
#define	SCPI_PARAMETER_NONDECIMAL	0x07
#define SCPI_PARAMETER_STRING		0x08
#define SCPI_PARAMETER_EXPRESSION	0x09
#define SCPI_PARAMETER_BLOCK		0x0A
#define SCPI_PARAMETER_UNITS		0x0B
#define SCPI_PARAMETER_PAUSE		0x0C
#define SCPI_PARAMETER_END			0x0D
extern unsigned char scpi_parser_secondary_status;

// scpi_analyse 所用到的二值逻辑判断标志
extern bool scpi_middle_keyword;

// scpi_analyse 所用到的指针变量
extern unsigned char	*p_scpi_input_head;
extern unsigned char	*p_scpi_input_tail;
extern unsigned char	*p_scpi_inside;
extern unsigned char	*p_scpi_param_write;

// scpi_analyse 所用到的其他变量
extern unsigned char	scpi_inside_buffer[SCPI_INPUT_BUF_LEN];
extern unsigned char	scpi_keyword_copy[SCPI_INPUT_PARAM_LEN+1];
extern unsigned char	scpi_command_type;
extern unsigned char	scpi_param_index;

extern unsigned char	scpi_param_base[257];
extern unsigned char	scpi_param_power[30];

extern int		scpi_int_value;
extern int		scpi_power_value;
extern float	scpi_float_value;

/****************************************************************************************
*																						*
*								scpi 命令结构体定义										*
*																						*
****************************************************************************************/

typedef struct SCPI_COMMON_KEYWORD{
	unsigned char name[6];
	short param_num;
	short Next;
	p_scpi_func_type p_function;
}scpi_common_keyword;

typedef struct SCPI_SPECIAL_KEYWORD{
	unsigned char name[13];
	unsigned char shortname[5];
	short FirstChild;
	short NextBrother;
	bool IsDefault;
	short param_num;
	short q_param_num;
	p_scpi_func_type p_function;
	p_scpi_func_type p_q_function;
}scpi_special_keyword;

extern scpi_common_keyword	*p_scpi_common;
extern scpi_special_keyword	*p_scpi_keyword;
extern scpi_special_keyword	*p_scpi_level;
extern scpi_special_keyword	*p_scpi_visible_level;

#define SCPI_PARAM_NUM ((scpi_command_type == SCPI_COMMON)?(p_scpi_common->param_num):((scpi_command_type == SCPI_SPECIAL)?(p_scpi_keyword->param_num):(p_scpi_keyword->q_param_num)))

/****************************************************************************************
*																						*
*								scpi 解析器内部函数声明									*
*																						*
****************************************************************************************/

void scpi_pretreatment(void);
void scpi_parser_start(void);

//命令头处理一级函数，以及被该函数调用的二级函数
void scpi_parser_head(void);
void scpi_command_head_start(void);
void scpi_common_head(void);
void scpi_special_head(void);
void scpi_ransack_common_table(void);
void scpi_ransack_special_table(void);
void scpi_exist_default_path(void);
void scpi_command_head_end(void);

//参数处理一级函数，以及被该函数调用的二级函数
void scpi_parser_parameter(void);
void scpi_parameter_start(void);
void scpi_parameter_decimal(void);
void scpi_parameter_keyword(void);
void scpi_parameter_nondecimal(void);
void scpi_parameter_string(void);
void scpi_parameter_expression(void);
void scpi_parameter_block(void);
void scpi_parameter_units(void);
void scpi_parameter_pause(void);
void scpi_parameter_end(void);

//功能函数
#ifdef  __SUPPORT__MACRO__
short scpi_macro_define(void);
#endif

//测试函数
//short scpi_function_test(void);


#endif