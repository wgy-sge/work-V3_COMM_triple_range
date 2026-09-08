/******************** (C) COPYRIGHT 2010 Zidetech *****************************
* File Name          : port_timer.c
* Author             : xxx
* Version            : V1.0
* Date               : 2010.06.21
* Description        : 定时器操作函数
********************************************************************************
* Log :
2009.06.21 xxx, initial version
*******************************************************************************/
#include "fmb.h"

u8 gInnerCommFrameTicks= 0;


/*---------------------------- Functions -------------------------------------*/

/*******************************************************************************
* Function Name  : xMBPortTimersInit
* Description    : 初始化定时器
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
BOOL xMBPortTimersInit( void )
{
    /*
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	

    TIM_TimeBaseStructure.TIM_Period = 30000;
    TIM_TimeBaseStructure.TIM_Prescaler = 8;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    //启用ARR的影子寄存器
    TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清除更新中断请求位
	TIM_SetCounter( TIM3,0 );
        TIM_Cmd(TIM3, ENABLE); 
    //开中断
    TIM_ITConfig( TIM3,TIM_IT_Update,ENABLE );      
    

	TIM_Cmd(TIM3, DISABLE); 
	*/
	
    return TRUE;
}

/*******************************************************************************
* Function Name  : vMBPortTimersEnable
* Description    : 使能定时器
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void vMBPortTimersEnable( void )
{
	gInnerCommFrameTicks= FRAME_OVER_TIME;
    /* Reset timer counter and set compare interrupt. */	
   // TIM_SetCounter( TIM3,0 );
   // TIM_Cmd(TIM3, ENABLE);  
	
}

/*******************************************************************************
* Function Name  : vMBPortTimersDisable
* Description    : 失能定时器
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void vMBPortTimersDisable( void )
{
	gInnerCommFrameTicks= 0;
   // TIM_Cmd(TIM3,DISABLE );
}
