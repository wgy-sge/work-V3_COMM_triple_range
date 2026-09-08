#ifndef _SYS_TIMER_H
#define _SYS_TIMER_H

#include "include.h"


void sys_timer_init(void);
void sys_timer_stop(void);
uint32 sys_timer_Getstamp(void);
void sys_timer_Clrstamp(void);
void sys_timer_usDelay(uint32 ustimedelay);
uint32 sys_timer_getstamp_between(uint32 stamp);
void sys_timer_Irq(void);


#endif