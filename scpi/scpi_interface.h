/****************************************************************************************
*                                                                                       *
*	File Name	: scpi_interface.h                                                      *
*	Author		: R&D Center,Shenzhen Zidetech Co. Ltd.								*
*	Version		: V2.0																	*
*	Date		: 08/12/2009															*
*	Description	: This file contains all the variables, macros and functions used in	*
*				  both scpi_analyse and	your embedded code.								*
*																						*
****************************************************************************************/

#ifndef __SCPI__INTERFACE__H__
#define __SCPI__INTERFACE__H__
#include "stm32f10x_type.h"	

//#include "include.h"
//调试环境设置
//#define __DEBUG__ON__PC__

//条件编译宏定义，控制解析器支持的数据类型
//#define __SUPPORT__MACRO__		//支持宏定义
//#define __SUPPORT__NONDECIMAL__	//支持非十进制参数
//#define __SUPPORT__STRING__		//支持字符串参数
//#define __SUPPORT__EXPRESSION__	//支持表达式参数
//#define __SUPPORT__BLOCK__		//支持数据块参数

#ifdef  __SUPPORT__MACRO__
#include "scpi_macro.h"
#endif

//程序重要常量定义
#define SCPI_END_OF_INPUT	0x0A	//解析器输入字符串结束标志
#define SCPI_INPUT_BUF_LEN	1024	//解析器输入缓冲区长度
#define SCPI_PARAM_BUF_LEN	512		//解析器输出参数缓冲区长度

#define SCPI_INPUT_PARAM_LEN    50  //解析器输入参数缓冲区长度

//参数缓冲区中参数类型标识
#define BIT_INDICATES_STRING	0x01	// Is string
#define BIT_INDICATES_INT		0x02	// Is integer
#define BIT_INDICATES_FLOAT		0x04	// Is float

#define    MAXIMUM_BLOCK_FRAME          200


typedef short (*p_scpi_func_type)(void);

//接口变量
extern bool		scpi_parser_is_idle;
extern int	scpi_parser_return_code;

extern unsigned char	scpi_input_buffer[SCPI_INPUT_BUF_LEN];
extern unsigned char	scpi_param_buffer[SCPI_PARAM_BUF_LEN];
extern p_scpi_func_type	scpi_func_output;

#ifdef  __SUPPORT__MACRO__
extern scpi_macro scpi_macro_table[SCPI_MACRO_NUMBER];
#endif

//接口函数
void scpi_init(void);
void scpi_parser(void);

#define    SCPI_ERROR_LIST_LENGTH       16          //SCPI错误队列长度
#endif//#ifndef __SCPI__INTERFACE__H__