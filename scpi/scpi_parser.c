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

// 初始化解析器主状态，使下一条命令从预处理阶段开始。
void scpi_init(void)
{
	scpi_parser_primary_status = SCPI_PRETREATMENT;
}

// SCPI 解析总状态机：依次完成预处理、命令头解析、参数解析，并在暂停或结束时返回。
void scpi_parser(void)
{
	while(TRUE)
	{
		switch(scpi_parser_primary_status)
		{
		case SCPI_PRETREATMENT:
			scpi_pretreatment();       // 跳过前导空白，将输入统一转换为小写
			break;
		case SCPI_PARSER_START:
			p_scpi_inside = scpi_inside_buffer;
			p_scpi_level = p_scpi_visible_level = (scpi_special_keyword *)scpi_special_command_table;
			scpi_parser_start();       // 定位本行中下一条有效命令的起点
			break;
		case SCPI_PARSER_HEAD:
			scpi_param_buffer[0] = 0;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_START;
			p_scpi_level = p_scpi_visible_level;
			scpi_parser_head();        // 解析公共命令或设备命令头，并查找处理函数
			break;
		case SCPI_PARSER_PARAM:
			scpi_parser_secondary_status = SCPI_PARAMETER_START;
			p_scpi_param_write = scpi_param_buffer + 1;
			scpi_parser_parameter();   // 按类型解析命令参数并写入参数缓冲区
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

// 输入预处理：跳过前导空白，将命令转为小写并复制到内部解析缓冲区。
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

// 命令起始处理：跳过空白和多余分号，判断本行结束或转入命令头解析。
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

// 命令头二级状态机：调度命令类型判断、关键字提取、查表和命令头收尾。
void scpi_parser_head(void)
{
	while(TRUE)
	{
		switch(scpi_parser_secondary_status)
		{
		case SCPI_COMMAND_HEAD_START:
			scpi_command_head_start(); // 根据首字符判断公共命令、绝对路径或普通设备命令
			break;
		case SCPI_COMMON_HEAD:
			scpi_common_head();        // 提取以 '*' 开头的 IEEE 488.2 公共命令
			break;
		case SCPI_SPECIAL_HEAD:
			scpi_special_head();       // 提取设备命令的当前层级关键字
			break;
		case SCPI_COMMAND_HEAD_END:
			scpi_command_head_end();   // 校验参数需求，并决定进入参数、暂停或结束状态
			return;
		case SCPI_COMMAND_HEAD_NONE:                         //@add luguangping 20110704
			scpi_parser_primary_status = SCPI_PARSER_END;    //@add luguangping 20110704
			return;  
		default:
			return;
		}
	}
}

// 判断命令头类型：'*' 为公共命令，':' 为根路径，字母为设备命令。
void scpi_command_head_start(void)
{
	// 输入已在预处理阶段转为小写，这里根据首字符选择后续命令头解析状态。
	while(TRUE)
	{
		if(*p_scpi_inside == '*')
		{
			// 星号表示公共命令；保留指针在星号处，由 scpi_common_head() 提取完整名称。
			scpi_parser_secondary_status = SCPI_COMMON_HEAD;
			return;
		}
		else if(*p_scpi_inside == ':')
		{
			// 跳过前导冒号，要求其后紧接字母，作为从根路径开始的设备命令。
			if((*(++p_scpi_inside) >= 'a') && (*p_scpi_inside <= 'z'))
			{
				// 前导冒号要求从根节点查找，同时重置当前层和后续命令继承的可见层。
				p_scpi_level = p_scpi_visible_level = (scpi_special_keyword *)scpi_special_command_table;
				scpi_parser_secondary_status = SCPI_SPECIAL_HEAD;
			}
			else
			{
				// 冒号后不是字母，命令头格式错误，交给收尾状态处理。
				scpi_parser_return_code = ERR_COMMAND_HEADER_ERROR;
				scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			}
			return;
		}
		else if((*p_scpi_inside >= 'a') && (*p_scpi_inside <= 'z'))
		{
			// 无前导冒号时沿用当前路径，指针保持在首字母处，交给设备命令头解析。
			scpi_parser_secondary_status = SCPI_SPECIAL_HEAD;
			return;
		}
		else if((*p_scpi_inside == SCPI_END_OF_INPUT))
		{
			// 已到行尾（0x0A），无新命令可解析；外层通过 HEAD_NONE 状态正常结束本行。
			scpi_parser_return_code= ERR_NO_ERROR;//luguangping 20110701

			//scpi_parser_return_code = ERR_SEMICOLON_UNWANTED;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_NONE; //@add luguangping 20110704
			return;
		}
		else if((*p_scpi_inside == ';'))
		{
			// 跳过多余分号，继续寻找有效命令头，保留当前命令路径。
			p_scpi_inside++;//@add luguangping 20110704
			continue;//@add luguangping 20110704
                        //----------------------------------------
		}
		else if(*p_scpi_inside <= ' ')
		{
			// 跳过空格及其余不大于空格的字符；行结束符已由前面的分支处理。
			p_scpi_inside++;
			continue;
		}
		else
		{
			// 其余字符不能作为命令头起始字符，记录格式错误并转入命令头收尾。
			scpi_parser_return_code = ERR_COMMAND_HEADER_ERROR;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			return;
		}
	}
}

// 提取公共命令关键字（如 *IDN?），并检查其长度和结束字符是否合法。
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

// 遍历公共命令表，匹配关键字并将对应处理函数保存到 scpi_func_output。
void scpi_ransack_common_table(void)
{
	p_scpi_common = (scpi_common_keyword *)scpi_common_command_table;

	while(TRUE)
	{
		// strcmp 返回 0 才表示名称相同；非 0 进入当前表项不匹配的分支。
		if(strcmp((char*)scpi_keyword_copy,(char*)p_scpi_common->name))
		{
			// Next 为 -1 表示查找链结束，仍未匹配则报未定义命令头错误（-113）。
			if(p_scpi_common->Next == -1)
			{
				scpi_parser_return_code = ERR_UNDEFINED_HEADER;
				// 转入命令头收尾阶段，由外层状态机根据错误码结束解析。
				scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
				return;
			}
			else
				// Next 是相对于表头的数组下标，指针加法按结构体大小定位下一候选表项。
				p_scpi_common = (scpi_common_keyword *)scpi_common_command_table + p_scpi_common->Next;
		}
		else
		{
			// 名称匹配成功，标记为公共命令，后续从当前表项取得所需参数数量。
			scpi_command_type = SCPI_COMMON;
			// 保存处理函数地址；解析完成后由 scpi_main_proc() 统一调用。
			scpi_func_output = p_scpi_common->p_function;
			// 记录命令头匹配成功，参数是否正确仍需后续流程检查。
			scpi_parser_return_code = ERR_NO_ERROR;
			// 转入命令头收尾阶段，按后续字符决定解析参数、暂停或结束。
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			return;
		}
	}
}

// 提取设备命令当前层关键字，识别查询符 '?' 以及是否还有 ':' 子路径。
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

// 在当前命令树层级查找长/短关键字，并处理子节点、同级节点及默认路径。
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
		// scpi_keyword_copy：从输入提取的当前层关键字，已转为小写，此处已去掉查询符号。
		// p_scpi_keyword->name：命令表当前候选节点的完整名称；shortname 是其短名，随同级遍历切换。
		// 例如解析 :SOUR:SEQ:EDIT:FILE:NUMB? 的第一层时，scpi_keyword_copy 为 "sour"。
		// 从根节点开始查找时，name 最初为 "calibrate"；遍历到 "source" 时，短名 "sour" 与输入匹配。
		// strcmp 返回 0 表示相同；长名和短名都不匹配时，才进入此分支继续查找。
		// 任一名称匹配时条件为假，进入下面的 else 处理已匹配节点。
		if(strcmp((char*)scpi_keyword_copy,(char*)p_scpi_keyword->name)&&strcmp((char*)scpi_keyword_copy,(char*)p_scpi_keyword->shortname))
		{
			// NextBrother 是下一同级节点在命令表中的数组下标，-1 表示没有后续同级节点。
			if(p_scpi_keyword->NextBrother != -1)
				// 以表头加下标定位下一候选节点，下一轮仍用同一个输入关键字进行匹配。
				p_scpi_keyword = (scpi_special_keyword *)scpi_special_command_table + p_scpi_keyword->NextBrother;
			else
				// 退出同级节点查找循环，随后尝试沿默认节点查找当前关键字。
				break;
		}
		else
		{
			// 标志为真表示当前关键字后还有冒号，输入指针已由 scpi_special_head() 移到下一关键字。
			if(scpi_middle_keyword)
			{
				// 当前节点没有子节点时，无法继续匹配冒号后的路径，转入命令头错误收尾。
				if(p_scpi_keyword->FirstChild == -1)
				{
					scpi_parser_return_code = ERR_UNDEFINED_HEADER;
					scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
					return;
				}
				else
				{
					// 记录已进入的路径，供同一行后续相对命令沿用。
					p_scpi_visible_level = p_scpi_keyword;
					// 更新当前查找层级，下一次查表从该节点的 FirstChild 开始匹配子关键字。
					p_scpi_level = p_scpi_keyword;
					scpi_parser_return_code = ERR_NO_ERROR;
					// 设为继续解析状态；逐层循环由外层 scpi_parser_head() 的 while(TRUE) 驱动。
					scpi_parser_secondary_status = SCPI_SPECIAL_HEAD;
					// 先返回 scpi_special_head() 再回到外层；case 的 break 只退出 switch，下一轮继续。
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

// 沿 IsDefault 标记的默认子节点向下查找，补全命令末尾省略的可选路径。
/*
 * 补全命令末尾省略的默认关键字，并保存最终处理函数或错误状态。
 * 入口：p_scpi_keyword 指向有子节点的已匹配节点；递归时指向刚找到的默认节点。
 * 例：:INP ON 与 :INP:STAT ON 等效，省略的 state 节点由 IsDefault=TRUE 标记。
 *     首次进入时 p_scpi_keyword->name 为 "input"，scpi_keyword_copy 为 "inp"，输入指针指向 "on"。
 *     查找到默认叶子 state 后，根据命令类型选择其设置或查询处理函数。
 * while 沿 NextBrother 查找同级默认节点，递归沿 FirstChild 补全更深的默认路径。
 * 输入字符串和 scpi_keyword_copy 内容保持不变，p_scpi_inside 位置不变，参数由外层继续解析。
 * 结果通过全局变量输出：scpi_func_output、scpi_parser_return_code 和二级解析状态。
 * 成功或报错后均进入 SCPI_COMMAND_HEAD_END，由外层完成命令头收尾。
 */
void scpi_exist_default_path(void)
{
	// 将入口节点作为当前父层；上述例子中 p_scpi_level 指向 input。
	p_scpi_level = p_scpi_keyword;
	// FirstChild 是命令表数组下标；进入第一个子节点，例中从 input 移到 state。
	p_scpi_keyword = (scpi_special_keyword *)scpi_special_command_table + p_scpi_keyword->FirstChild;

	while(TRUE)
	{
		// 只有 IsDefault 为真才允许省略；候选节点名称来自命令表，无需再次匹配输入字符串。
		if(p_scpi_keyword->IsDefault)
		{
			// 命令类型已由前面的查表流程确定；带问号的查询命令选择查询回调。
			if(scpi_command_type == SCPI_SPE_QUE)
			{
				// 默认节点还有子节点，继续递归补全下一层默认路径。
				if(p_scpi_keyword->FirstChild != -1)
				{
					scpi_exist_default_path();
					// 递归调用已保存处理函数或错误状态，直接返回并逐层退出。
					return;
				}
				// 到达默认叶子节点，但没有查询回调，记录该命令不可查询的错误。
				else if(p_scpi_keyword->p_q_function == NULL)
				{
					scpi_parser_return_code = ERR_COMMAND_CAN_NOT_QUERY;
					scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
					return;
				}
				else
				{
					// 保存查询函数地址供主处理流程调用；例 :INP? 选择 scpi_spe_input_output_state_quest。
					scpi_func_output = p_scpi_keyword->p_q_function;
					scpi_parser_return_code = ERR_NO_ERROR;
					scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
					return;
				}
			}
			else
			{
				// 默认节点还有子节点，继续递归补全下一层默认路径。
				if(p_scpi_keyword->FirstChild != -1)
				{
					scpi_exist_default_path();
					// 递归调用已保存处理函数或错误状态，直接返回并逐层退出。
					return;
				}
				// 设置命令需要设置回调；默认叶子没有该回调时，记录命令必须用于查询的错误。
				else if(p_scpi_keyword->p_function == NULL)
				{
					scpi_parser_return_code = ERR_COMMAND_MUST_QUERY;
					scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
					return;
				}
				else
				{
					// 例 :INP ON 选择 scpi_spe_input_output_state，返回后由外层解析参数 ON 并调度执行。
					scpi_func_output = p_scpi_keyword->p_function;
					scpi_parser_return_code = ERR_NO_ERROR;
					scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
					return;
				}
			}
		}
		// 当前候选不是默认节点，按 NextBrother 数组下标继续查找下一个同级节点。
		else if(p_scpi_keyword->NextBrother != -1)
		{
			p_scpi_keyword = (scpi_special_keyword *)scpi_special_command_table + p_scpi_keyword->NextBrother;
		}
		else
		{
			// 同级子节点已遍历完仍无默认节点，无法补全省略路径，转入错误收尾。
			scpi_parser_return_code = ERR_UNDEFINED_HEADER;
			scpi_parser_secondary_status = SCPI_COMMAND_HEAD_END;
			return;
		}
	}
}

// 命令头收尾：检查缺失参数，并根据后续字符进入参数解析、分号暂停或整行结束。
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

// 参数解析二级状态机：根据参数类型调度具体解析函数，直至参数段结束。
void scpi_parser_parameter(void)
{
	scpi_param_index = 1;
	while(TRUE)
	{
		switch(scpi_parser_secondary_status)
		{
		case SCPI_PARAMETER_START:
			scpi_parameter_start();       // 根据首字符判定参数类型，并检查参数数量
			break;
		case SCPI_PARAMETER_DECIMAL:
			scpi_parameter_decimal();     // 解析整数、浮点数和科学计数法
			break;
		case SCPI_PARAMETER_KEYWORD:
			scpi_parameter_keyword();     // 解析 ON、OFF、MAX 等裸关键字参数
			break;
		case SCPI_PARAMETER_NONDECIMAL:
			scpi_parameter_nondecimal();  // 处理 #B/#Q/#H 非十进制参数（当前未启用）
			break;
		case SCPI_PARAMETER_STRING:
			scpi_parameter_string();      // 处理带引号字符串参数（当前未启用）
			break;
		case SCPI_PARAMETER_EXPRESSION:
			scpi_parameter_expression();  // 处理括号表达式参数（当前未启用）
			break;
		case SCPI_PARAMETER_BLOCK:
			scpi_parameter_block();       // 处理 SCPI 块数据参数（当前未启用）
			break;
		case SCPI_PARAMETER_UNITS:
			scpi_parameter_units();       // 处理数字后的单位后缀（当前直接报格式错误）
			break;
		case SCPI_PARAMETER_PAUSE:
			scpi_parameter_pause();       // 处理逗号、分号和行尾，决定继续或结束
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

// 根据参数首字符选择数字、关键字、非十进制、字符串、表达式或块数据解析器。
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

// 解析十进制参数：支持符号、小数和指数形式，并按 INT/FLOAT 编码到参数缓冲区。
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

// 解析由字母、数字和下划线组成的裸关键字，并以字符串形式写入参数缓冲区。
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

// 非十进制参数入口：当前未启用 __SUPPORT__NONDECIMAL__，因此返回格式错误。
void scpi_parameter_nondecimal(void)
{
#ifdef __SUPPORT__NONDECIMAL__

#else
	scpi_parser_return_code = ERR_INVALID_FORMAT;
	scpi_parser_secondary_status = SCPI_PARAMETER_END;
	return;
#endif
}

// 带引号字符串入口：当前未启用 __SUPPORT__STRING__，因此返回格式错误。
void scpi_parameter_string(void)
{
#ifdef __SUPPORT__STRING__

#else
	scpi_parser_return_code = ERR_INVALID_FORMAT;
	scpi_parser_secondary_status = SCPI_PARAMETER_END;
	return;
#endif
}

// 表达式参数入口：当前未启用 __SUPPORT__EXPRESSION__，因此返回格式错误。
void scpi_parameter_expression(void)
{
#ifdef __SUPPORT__EXPRESSION__

#else
	scpi_parser_return_code = ERR_INVALID_FORMAT;
	scpi_parser_secondary_status = SCPI_PARAMETER_END;
	return;
#endif
}

// SCPI 块数据入口：当前未启用 __SUPPORT__BLOCK__，因此返回格式错误。
void scpi_parameter_block(void)
{
#ifdef __SUPPORT__BLOCK__

#else
	scpi_parser_return_code = ERR_INVALID_FORMAT;
	scpi_parser_secondary_status = SCPI_PARAMETER_END;
	return;
#endif
}

// 单位后缀入口：当前没有单位解析实现，直接返回 ERR_INVALID_FORMAT。
void scpi_parameter_units(void)
{
	scpi_parser_return_code = ERR_INVALID_FORMAT;
	scpi_parser_secondary_status = SCPI_PARAMETER_END;
	return;
}

// 参数分隔与收尾：处理空白、逗号、分号和换行，并校验实际参数数量。
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

// 宏定义命令入口：当前仅为占位实现。
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