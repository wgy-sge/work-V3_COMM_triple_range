#ifndef _SCPI_MACRO_H_
#define _SCPI_MACRO_H_


/****************************************************************************************
*                                                                                       *
*	File Name	: scpi_macro.h		                                                    *
*	Author		: R&D Center,Shenzhen Zidetech Co. Ltd.								*
*	Version		: V2.0																	*
*	Date		: 08/12/2009															*
*	Description	: This file contains define of macro structure type.					*
*																						*
****************************************************************************************/

#define SCPI_MACRO_NUMBER			10
#define SCPI_MACRO_LABEL_LENGTH		12
#define SCPI_MACRO_CONTENT_LENGTH	242

typedef struct SCPI_MACRO{
	unsigned char name[SCPI_MACRO_LABEL_LENGTH + 1];
	unsigned char content[SCPI_MACRO_CONTENT_LENGTH + 1];
}scpi_macro;

extern bool	scpi_macro_enable;














#endif