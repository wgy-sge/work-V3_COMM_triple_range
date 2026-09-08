#ifndef _CANOPENPOLL_H_
#define _CANOPENPOLL_H_



//事件定义
#define EVENT_CANOPEN_NONE                0
#define EVENT_CANOPEN_READ_ONE_REG        1
#define EVENT_CANOPEN_WRITE_ONE_REG       2


void PostEventVirtulOIDReg(u8 EName, u16 index, u8 *pEParam, u16 len);

void CanOpenPoll(void);

#endif