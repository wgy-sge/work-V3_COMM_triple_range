#ifndef _SCPI_ERRORS_H_
#define _SCPI_ERRORS_H_


#include "include.h"

#define ERROR_TOOMANY           350
#define COMMAND_ERRORS			100
#define COMMAND_HEADER_ERRORS   110

#define NUMERIC_DATA_ERRORS     120
#define SUFFIXERRORS			130
#define CHARACTER_DATA_ERRORS   140
#define STRING_DATA_ERRORS		150
#define BLOCK_DATA_ERRORS		160

#define EXPRESSION_ERRORS1		170
#define MACRO_ERRORS1			180
#define EXECUTION_ERRORS		200
#define TRIGGER_ERRORS			210
#define PARAMETER_ERRORS		220
#define HARDWARE_ERRORS			240
#define MASS_STORAGE_ERRORS		250
#define EXPRESSION_ERRORS2		261
#define MACRO_ERRORS2			271

#define PROGRAM_ERRORS          280
#define MEMORY_USE_ERRORS       290

//#define QUERY_OVERFLOW			350
#define QUERY_ERRORS            400

typedef struct
{
	bool  err_busy;
	u8    err_num;       //错误序号
	short err_code;      //错误码
	u16    flash_cyc;     //一个字符停留时间
	u8    err_str_start; //第一个字符位置
	u8    err_str_end;   //最后一个字符位置
	u8    err_str_len;   //错误信息字符串长度
	char  dispbuf[13];   //当前显示的内容
	const char  *p_err_str;    //错误信息字符串
}ERR_FLASH_INFO_T;

extern const char too_many_errors[];

extern const char Command_errors[][25];
extern const char Command_header_errors[][30];

extern const char Numeric_data_errors[][30];
extern const char Suffix_errors[][20];
extern const char Character_data_errors[][30];
extern const char String_data_errors[][25]
;
extern const char Block_data_errors[][25];
extern const char Expression_errors1[][30]
;
extern const char Macro_errors1[][35];
extern const char Execution_errors[][25];
extern const char Trigger_errors[][20];
extern const char Parameter_errors[][25];
extern const char Hardware_errors[][18];
extern const char Mass_storage_errors[][23];
extern const char Expression_errors2[][23];
extern const char Macro_errors2[][32];
extern const char Program_errors[][28];
extern const char Memory_use_errors[][32];
extern const char Query_errors[][20];


extern ERR_FLASH_INFO_T err_flash_info;//滚动显示错误信息用
extern void Panel_ReadError(short err_code);

//extern bool err_flash_procesor(void);


#endif