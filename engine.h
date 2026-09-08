#ifndef  __ENGINE__H__
#define  __ENGINE__H__

void InitCommunicationProtocol( u8 prot );
void EnginePoll( void );

extern u8 gDeviceAddr;
extern u8 gDevice_CommProtocol;

extern u8 gBroadcastComm;//广播通讯方式

#endif

