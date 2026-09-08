#ifndef _UVMB_RS232_H_
#define _UVMB_RS232_H_




#define    SIZE_OF_OUTER_BUFFER_IN          512            //外部通讯缓存--输入
#define    SIZE_OF_OUTER_BUFFER_OUT         512           //外部通讯缓存--输出




void Rs232MbReceiveFsm( void );
bool Rs232MbCharTimeover( void );
void Rs232MbTransmitFsm( void );


bool Rs232MbIsThereFrame( void );

bool Rs232MbReadFrame( char *pdst, u16 length, u16* pcnt );
void Rs232MbDoPlus( void );
bool Rs232MbIsTransmitFree( void );
bool Rs232MbTransmitFrame( char * pframe, u16 len );







#endif