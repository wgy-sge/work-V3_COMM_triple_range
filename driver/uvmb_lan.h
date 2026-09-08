#ifndef    __UVMB__LAN__H__
#define    __UVMB__LAN__H__



void LanMbDetectFrame( void );
bool LanMbIsThereFrame( void );
bool LanMbReadFrame( char *pdst, u16 length, u16* pcnt );
void LanMbDoPlus( void );
bool LanMbIsTransmitFree( void );
bool LanMbTransmitFrame( char * pframe, u16 len );




#endif