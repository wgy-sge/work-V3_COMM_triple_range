#ifndef    __UVMB__TCP__LAN__H__
#define    __UVMB__TCP__LAN__H__




bool TcpLanMbIsThereFrame( void );
bool TcpLanMbReadFrame( char *pdst, u16 length, u16* pcnt );
void TcpLanMbDoPlus( void );
bool TcpLanMbIsTransmitFree( void );
bool TcpLanMbTransmitFrame( char * pframe, u16 len );




#endif