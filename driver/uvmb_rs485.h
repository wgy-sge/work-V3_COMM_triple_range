#ifndef _UVMB_RS485_H_
#define _UVMB_RS485_H_




#define    SIZE_OF_OUTER_BUFFER_IN          512            //外部通讯缓存--输入
#define    SIZE_OF_OUTER_BUFFER_OUT         512           //外部通讯缓存--输出




void Rs485MbReceiveFsm( void );
bool Rs485MbCharTimeover( void );
void Rs485MbTransmitFsm( void );


bool Rs485MbIsThereFrame( void );

bool Rs485MbReadFrame( char *pdst, u16 length, u16* pcnt );
void Rs485MbDoPlus( void );
bool Rs485MbIsTransmitFree( void );
bool Rs485MbTransmitFrame( char * pframe, u16 len );







#endif