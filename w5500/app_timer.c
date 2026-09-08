
#include "app_timer.h"


struct app_timer_it
{
  uint32 time_it;
  uint16 time_period;
  uint8  bit_offset;
  timer_tick_callback callback;
  bool   irq_flag;
};


static struct app_timer_it  app_it_time[TIM_IT_MAX];

/*******************************************************************************
* 函数名     : Uart_Tim_Enable
* 函数描述: 串口定时器打开
* 输入参数: no(串口编号)
* 输出参数: 无
*******************************************************************************/
void Tim_Delay_Init(TIM_TypeDef *timer,uint16 period,uint32 rcc_tim)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  if(rcc_tim  ==  RCC_APB2Periph_TIM1)
  {
    RCC_APB2PeriphClockCmd(rcc_tim,ENABLE);
  }
  else
  {
    RCC_APB1PeriphClockCmd(rcc_tim,ENABLE);
  }
  
  TIM_TimeBaseStructure.TIM_Prescaler = (RCC_Clocks.PCLK2_Frequency/1000000-1);
  TIM_TimeBaseStructure.TIM_Period    = (period-1); /*1us*/
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(timer,&TIM_TimeBaseStructure);
  TIM_ClearFlag(timer,TIM_FLAG_Update);          //清除定时器中断标志  

  TIM_SetCounter(timer,0);   
  TIM_ClearFlag(timer,TIM_FLAG_Update);          //清除定时器中断标志 

  TIM_Cmd(timer,ENABLE);                         //开启时钟
}

/*******************************************************************************
* 函数名     : Uart_Tim_Disable
* 函数描述: 串口定时器关闭
* 输入参数: no(串口编号)
* 输出参数: 无
*******************************************************************************/
void Tim_CounterClr(TIM_TypeDef *timer)
{
  TIM_SetCounter(timer,0);   
}
/*******************************************************************************
* 函数名     : Uart_Tim_Disable
* 函数描述: 串口定时器关闭
* 输入参数: no(串口编号)
* 输出参数: 无
*******************************************************************************/
uint16 Tim_GetCounter(TIM_TypeDef *timer)
{
  return TIM_GetCounter(timer);   
}
/*******************************************************************************
* 函数名     : Uart_Tim_Disable
* 函数描述: 串口定时器关闭
* 输入参数: no(串口编号)
* 输出参数: 无
*******************************************************************************/
void Tim_Delay(TIM_TypeDef *timer,uint32 us)
{
  uint16 i;
  uint16 temp0 = 0;
  uint16 temp1 = 0;
  uint32 time  =   0;
  
  Tim_CounterClr(timer);
  for(i=0;;i++)
  {
    temp0    = Tim_GetCounter(timer);
    if(temp1 > temp0)
    {
      time    +=  (timer->ARR + 1);
    }
    temp1   = temp0;
    if(time + temp0 >= us)
      break;
    
  }
}


/*******************************************************************************
* 函数名     : Uart_Tim_Enable
* 函数描述: 串口定时器打开
* 输入参数: no(串口编号)
* 输出参数: 无
*******************************************************************************/
void Tim_It_Init(TIM_TypeDef *timer,uint16 period,uint32 rcc_tim,uint8 tim_irq,uint8 no,timer_tick_callback callback)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  
  NVIC_InitTypeDef NVIC_InitStructure;
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  if(no >= TIM_IT_MAX)
    return;
  app_it_time[no].time_period = period;
  app_it_time[no].time_it     = 0;
  app_it_time[no].callback    = callback;
  
  RCC_APB1PeriphClockCmd(rcc_tim,ENABLE);
  
  //NVIC_PriorityGroupConfig(SYS_INTERRUTP_PRIOGROUP);
  NVIC_InitStructure.NVIC_IRQChannel = tim_irq;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  
  TIM_TimeBaseStructure.TIM_Prescaler = (RCC_Clocks.PCLK2_Frequency/1000000-1);
  TIM_TimeBaseStructure.TIM_Period    = period - 1; /*1us*/
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(timer,&TIM_TimeBaseStructure);
  TIM_ClearFlag(timer,TIM_FLAG_Update);          //清除定时器中断标志  

  TIM_SetCounter(timer,0);   
  TIM_ClearFlag(timer,TIM_FLAG_Update);          //清除定时器中断标志 

  TIM_ITConfig(timer,TIM_IT_Update,ENABLE);
  
  TIM_Cmd(timer,ENABLE);                         //开启时钟
  

}
/*******************************************************************************
* 函数名     : Uart_Tim_Disable
* 函数描述: 串口定时器关闭
* 输入参数: no(串口编号)
* 输出参数: 无
*******************************************************************************/
void Tim_It_CounterClr(TIM_TypeDef *timer,uint8 no)
{
  TIM_SetCounter(timer,0);  
  app_it_time[no].time_it = 0;
}
/*******************************************************************************
* 函数名     : Uart_Tim_Disable
* 函数描述: 串口定时器关闭
* 输入参数: no(串口编号)
* 输出参数: 无
*******************************************************************************/
uint32 Tim_It_GetCounter(TIM_TypeDef *timer,uint8 no)
{
  uint16 timer_count  = TIM_GetCounter(timer);
  TIM_SetCounter(timer,0);
  app_it_time[no].time_it += timer_count;
  return (app_it_time[no].time_it);
}
/*******************************************************************************
* 函数名     : Uart_Tim_Disable
* 函数描述: 串口定时器关闭
* 输入参数: no(串口编号)
* 输出参数: 无
*******************************************************************************/
void Tim_IT_Irq(TIM_TypeDef *timer,uint8 no)
{
  if(TIM_GetITStatus(timer, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(timer, TIM_IT_Update);
    app_it_time[no].time_it +=  app_it_time[no].time_period;
    if(app_it_time[no].callback !=  NULL)
    {
      app_it_time[no].callback();
    }
    app_it_time[no].irq_flag    = FALSE;
  }
}
/*******************************************************************************
* 函数名     : Tim_IT_Enable
* 函数描述: 定时器关闭
* 输入参数: no(串口编号)
* 输出参数: 无
*******************************************************************************/
void Tim_IT_Disable(TIM_TypeDef *timer)
{
  TIM_ClearITPendingBit(timer, TIM_IT_Update);
  TIM_ITConfig(timer,TIM_IT_Update,DISABLE);
  TIM_Cmd(timer,DISABLE);
}

