#ifndef __FMB__GLOBAL__H
#define __FMB__GLOBAL__H

#define    RETRY_SEND_TIMES                 5             //重发次数


#define    FRAME_OVER_TIME                  5

///////////////////////////////////////////////////////////////////////////////
//
extern UCHAR  client_snd_buf[MB_SER_PDU_SIZE_MAX];            //客户端发送缓存
extern UCHAR  client_rec_buf[MB_SER_PDU_SIZE_MAX];            //客户端接收缓存


///////////////////////////////////////////////////////////////////////////////
//
extern USHORT ticks_modbus_timeout;                      //Modbus通讯超时计时器


extern u8 gInnerCommFrameTicks;


#endif