/***********************************************************************************************************
************************************************************************************************************
**                                                                                                        **
**                      File name           : sys_timer.c                                                  **
**                      Writer              : YXF                                                         **
**                      Date                : 2014.11.11                                                   **
**                      Version             : V1                                                          **
************************************************************************************************************
***********************************************************************************************************/

#include "sys_timer.h"
#include "app_timer.h"


//系统运行时间撮初始化
void sys_timer_init(void)
{
  Tim_It_Init(TIMER_SYS,TIMER_SYS_PERIOD,RCC_APBPeriph_TIMER_SYS,TIMER_SYS_IRQ,TIMER_SYS_NO,NULL);
}
void sys_timer_stop(void)
{

  Tim_IT_Disable(TIMER_SYS);
}
//获取系统运行时间撮
void sys_timer_Irq(void)
{
  Tim_IT_Irq(TIMER_SYS,TIMER_SYS_NO);
}

//获取系统运行时间撮
uint32 sys_timer_Getstamp(void)
{
  return Tim_It_GetCounter(TIMER_SYS,TIMER_SYS_NO);
}

//获取系统运行时间撮
void sys_timer_Clrstamp(void)
{
  Tim_It_CounterClr(TIMER_SYS,TIMER_SYS_NO);
}
//系统延时函数
void sys_timer_usDelay(uint32 ustimedelay)
{
  uint32 time_stamp0  = 0;
  uint32 time_stamp1  = 0;
  time_stamp0 = Tim_It_GetCounter(TIMER_SYS,TIMER_SYS_NO);
  for(;;)
  {
    time_stamp1 = Tim_It_GetCounter(TIMER_SYS,TIMER_SYS_NO);
    if(time_stamp1 >= time_stamp0)
    {
      if((time_stamp1 - time_stamp0)  >=  ustimedelay)
      {
        break;
      }
    }
    else
    {
      if((time_stamp1 + (0xffffffff - time_stamp0))  >=  ustimedelay)
      {
        break;
      }
    }
  }
}

//获取当前时间与输入时间的时间间隔
uint32 sys_timer_getstamp_between(uint32 stamp)
{
  uint32 time_stamp  = Tim_It_GetCounter(TIMER_SYS,TIMER_SYS_NO);
  if(time_stamp >= stamp)
  {
    return (time_stamp - stamp);
  }
  else
  {
    return (time_stamp + 0xffffffff - stamp);
  }
}