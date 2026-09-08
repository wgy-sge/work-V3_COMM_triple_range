#ifndef _APP_TIMER_H
#define _APP_TIMER_H

#include "include.h"

typedef void (*timer_tick_callback)(void); 

#define TIM_IT_MAX            2

void Tim_Delay_Init(TIM_TypeDef *timer,uint16 period,uint32 rcc_tim);
void Tim_CounterClr(TIM_TypeDef *timer);
uint16 Tim_GetCounter(TIM_TypeDef *timer);
void Tim_Delay(TIM_TypeDef *timer,uint32 us);

void Tim_It_Init(TIM_TypeDef *timer,uint16 period,uint32 rcc_tim,uint8 tim_irq,uint8 no,timer_tick_callback callback);
void Tim_It_CounterClr(TIM_TypeDef *timer,uint8 no);
uint32 Tim_It_GetCounter(TIM_TypeDef *timer,uint8 no);
void Tim_IT_Irq(TIM_TypeDef *timer,uint8 no);
void Tim_IT_Disable(TIM_TypeDef *timer);



#endif
