#include "net_config.h"
//#include "sys_timer.h"
#include "socket.h"
#include "w5500.h"
#include "spix.h"


const uint8 net_txsize[MAX_SOCK_NUM] = {8,8};
const uint8 net_rxsize[MAX_SOCK_NUM] = {8,8};

const struct app_gpio w5500_reset_gpio  = 
{
	RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_1,  //RESET
};

const struct app_gpio w5500_int_gpio  = 
{
	RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_0,  //IRQ
};

#define W5500_RESET_TIME        200
#define W5500_RESET_TIMEDELY    1000

void delay_ms(vu32  ms);

void Reset_W5500(void)
{
	GPIO_ResetBits(w5500_reset_gpio.gpio, w5500_reset_gpio.gpio_pin);

	delay_ms(W5500_RESET_TIME);
	//sys_timer_usDelay(W5500_RESET_TIME);  

	GPIO_SetBits(w5500_reset_gpio.gpio, w5500_reset_gpio.gpio_pin);

	delay_ms(W5500_RESET_TIMEDELY);
	//sys_timer_usDelay(W5500_RESET_TIMEDELY); 

}


void net_port_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

	SPIx_Init();

	RCC_APB2PeriphClockCmd(w5500_reset_gpio.periph_rcc, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = w5500_reset_gpio.gpio_pin;
	GPIO_Init(w5500_reset_gpio.gpio, &GPIO_InitStructure);
	GPIO_ResetBits(w5500_reset_gpio.gpio, w5500_reset_gpio.gpio_pin);

#if 0

	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(w5500_int_gpio.periph_rcc, ENABLE );	
	/* configure PB0 as external interrupt */
	GPIO_InitStructure.GPIO_Pin = w5500_int_gpio.gpio_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(w5500_int_gpio.gpio, &GPIO_InitStructure);

	/* Connect ENC28J60 EXTI Line to GPIOB Pin 0 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	/* Configure ENC28J60 EXTI Line to generate an interrupt on falling edge */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	/* Clear the Key Button EXTI line pending bit */
	EXTI_ClearITPendingBit(EXTI_Line0);
	/* Enable the EXTI0 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	Reset_W5500();

}



bool set_network(uint8 *mac,uint8 *ip,uint8 *sub,uint8 *gatewall)
{
	uint8 temp[8];
	memcpy(temp,mac,6);
	setSHAR(temp);/*配置Mac地址*/
	memcpy(temp,ip,4);
	setSIPR(temp);/*配置Ip地址*/
	memcpy(temp,sub,4);
	setSUBR(temp);/*配置子网掩码*/
	memcpy(temp,gatewall,4);
	setGAR(temp);/*配置默认网关*/

	//Init. TX & RX Memory size of w5500
	wiz_mem_init((uint8 *)net_txsize, (uint8 *)net_rxsize); /*初始化8个socket*/

	setRTR(2000);/*设置溢出时间值,2000*100 = 200MS*/
	setRCR(3);/*设置最大重新发送次数*/

	getSIPR (temp);

	if(memcmp(temp,ip,4)  !=  0)
	{
		return FALSE;
	}
	getSUBR(temp);
	if(memcmp(temp,sub,4)  !=  0)
	{
		return FALSE;
	}
	getGAR(temp);
	if(memcmp(temp,gatewall,4)  !=  0)
	{
		return FALSE;
	}

	temp[0] = get_VERSIONR();
	if(temp[0]  !=  0x04)
	{
		return FALSE;
	}

	return TRUE;
}

void net_phyreset(void)
{
	uint8 phyconfig = get_PHYCFGR();
	set_PHYCFGR((phyconfig&0x7F));
	set_PHYCFGR((phyconfig|0x80));
}

bool net_linkconfig(void)
{
	static uint8 state_phyconfig = 0;
	static uint32 time_link      = 0;
	static bool   link_state    =  TRUE;

#define LINK_TIMEOUT        2000000

	state_phyconfig = get_PHYCFGR();
	if((state_phyconfig&0x01) ==  FALSE)
	{
		if(link_state == TRUE)
		{
			time_link  = sys_timer_Getstamp();
			link_state = FALSE;
		}
		else if(link_state == FALSE && sys_timer_getstamp_between(time_link) >= LINK_TIMEOUT)
		{
			net_phyreset();
			link_state    =  TRUE;
			return FALSE;
		}
		return FALSE;
	}
	link_state    =  TRUE;
	return TRUE;
}

