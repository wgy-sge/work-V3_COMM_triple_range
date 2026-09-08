#ifndef _SOCKET_APP_H
#define _SOCKET_APP_H


#include "include.h"



void socket_app_init(void);
void socket_app_pool(void);
void socket_timetick_pool(void);
void SocketPoll(void);
u16 SocketUdpReadData( u8 *pdst, u16 length );
u16 SocketUdpWriteData( u8 *psrc, u16 length );

u16 SocketTcpReadData( u8 *pdst, u16 length );
u16 SocketTcpWriteData( u8 *psrc, u16 length );



u16 getchar_LAN(void);
void LAN_PrintStr(char *pstr);

u16 getchar_TcpLAN(void);
void TcpLAN_PrintStr(char *pstr);

#endif
