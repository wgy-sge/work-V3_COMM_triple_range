#ifndef    __TRPA__TRSM__H__
#define    __TRPA__TRSM__H__

enum
{
	TRSM_IDLE = 0,
	TRSM_TRANSFER,
	TRSM_WAIT,
};

void TrpaTrsmPoll( void );




#endif