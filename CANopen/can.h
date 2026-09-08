#ifndef _CAN_H_
#define _CAN_H_











void can_rx_irq(void);


//开始处理
void CanOpenPollStart();


//释放处理器
void CanOpenPollFree();

#endif
