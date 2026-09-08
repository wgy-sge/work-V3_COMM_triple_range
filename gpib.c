/********************    ELoader  通讯板嵌入式程序    ********************
Copyright (C), 2010, Zidetech. Co., Ltd.
* File Name          : gpib.c
* Author             : lu guangping
* Version            : V0.0.1
* Date               : 2010/9/15
* Description        : GPIB(CPLD)驱动
********************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "include.h"



void CPD_ADR(u8 adr)//5位地址线
{
	if (adr&0x01)
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_11, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_11, Bit_RESET);
	}

	if (adr&0x02)
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_10, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_10, Bit_RESET);
	}

	if (adr&0x04)
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_RESET);
	}

	if (adr&0x08)
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_8, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_8, Bit_RESET);
	}

	if (adr&0x10)
	{
		GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET);
	}

	if (adr&0x20)
	{
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
	}
}

void CPD_DATA_DIR(u8 x)
{
	if(x>0)
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11
			|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//推挽 输出
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11
			|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空 输入
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	}
}


//pb 8~15 脚 输出 
void CPD_DATA_OUT(u8 data)
{
	u16 tem;
	tem = GPIO_ReadOutputData(GPIOE); 
	tem= (tem&0x00ff)+(data<<8);
	GPIO_Write(GPIOE, tem);
}

u8 CPD_DATA_IN(void)
{
	u16 tem;
	tem= GPIO_ReadInputData(GPIOE);
	return((tem&0xff00)>>8);
}

/********************************************************
*                    cpd_write_reg(uint8 addr,uint8 data)
*功能描述:写FT488D内部寄存器
*输入参数:
uint8	addr		//内部寄存器地址
uint8	data		//写入数据
*返回值:无
********************************************************/
void cpd_write_reg(u8 addr,u8 data)
{
	CPD_RW_0;	//write
	CPD_ADR(addr);
	CPD_DATA_DIR(0xff);	//output
	CPD_DATA_OUT(data);
	delay_us(1);
	CPD_EN_0;
	delay_us(1);
	CPD_EN_1;
	delay_us(1);
}


/********************************************************
*                    uint8 cpd_read_reg(uint8 addr)
*功能描述:写FT488D内部寄存器
*输入参数:
uint8	addr		//内部寄存器地址
uint8	data		//写入数据
*返回值:无
********************************************************/
u8 cpd_read_reg(u8 addr)
{
	u8 data;
	CPD_RW_1;	//read
	CPD_ADR(addr);
	CPD_DATA_DIR(0);	//input
	delay_us(1);
	CPD_EN_0;
	delay_us(1);
	data = CPD_DATA_IN();
	CPD_EN_1;
	delay_us(1);

	return data;
}


/********************************************************************
标题: void gpib_init(void)
功能：初始化FT488D
输入：none
输出：none
返回值: none
异常：none
********************************************************************/
void gpib_init(void)
{
	CPD_EN_1;
	CPD_RW_1;	

	delay_ms(20);
	cpd_write_reg(ADDR_INT_EN, 0x27);	//enable DCIE,DTIE,RLIE,TXIE,RXIE
	cpd_write_reg(ADDR_PADDR, 5);		//primary Address   sys_regs[OFFS_REG_GPIB_ADDRESS].reg_int  
	cpd_write_reg(ADDR_CFG0,0x1b);		//PP2  ADCLE  EOSWr  EOSRd
	cpd_write_reg(ADDR_EOS,0x0a);		//PP2  ADCLE  EOSWr  EOSRd
	
	//cpd_write_reg(ADDR_CFG1,0x03);//接收数据伴随EOI或者其值等于EOS时，置位RFDHoldoff状态。
	
	remote_info.gpib.rx_rp=0;
	remote_info.gpib.rx_wp=0;
	remote_info.gpib.tx_rp=0;
	remote_info.gpib.tx_wp=0;
	remote_info.gpib.tx_full=0;        
	cpd_write_reg(ADDR_LOC_MSG, 0x1D);//清空GPIB卡的发送数据
}


/********************************************************************
标题: putchar_gpib
功能：向GPIB接口发送一个字节的数据
格式：
void putchar_gpib(uint8 ch)
输入：uint8 ch
输出：none
返回值: none
异常：none
********************************************************************/
void putchar_gpib(u8 ch)
{
	u8 tmp,idle_cnt;


	GPIBIntOFF();                
	idle_cnt=cpd_read_reg(ADDR_STAT1) & 0xf0;
	tmp=remote_info.gpib.tx_wp;

	cpd_write_reg(ADDR_LOC_MSG,0x1c+0x80);	//mav on

	if((tmp==(remote_info.gpib.tx_rp))&&(idle_cnt!=0))
	//if((tmp==(remote_info.gpib.tx_rp))&&(idle_cnt>6))
	{
		// Transmit Holding Register Empty
		cpd_write_reg(ADDR_OUT_Q, ch);
	}
	else
	{
		//当缓冲区满时，停止发送
		//if(((tmp+1)&(COM_BUF_SIZE-1))==remote_info.gpib.tx_rp)
		if(((tmp+1)%COM_BUF_SIZE)==remote_info.gpib.tx_rp)
		{
			if(remote_info.gpib.tx_full!= TRUE)
			{				
				remote_info.gpib.tx_full= TRUE;
				remote_info.gpib.tx_err++;
			}
		}
		else
		{
			remote_info.gpib.tx_buf[tmp] = ch;//移入发送缓冲区
			//remote_info.gpib.tx_wp=(tmp+1)&(COM_BUF_SIZE-1);
			remote_info.gpib.tx_wp=(tmp+1)%COM_BUF_SIZE;
		}
	}
	GPIBIntON(); //启用中断

}


/********************************************************************
标题: getchar_gpib
功能：从GPIB接口接收一个字节的数据
格式：uint16 getchar_gpib()
输入：none
输出：none
返回值: 收到的字节，如果没有新的字节，那么函数返回值为65535
异常：none
********************************************************************/
u16 getchar_gpib(void)
{
	u16 c;
	u8 tmp;

	GPIBIntOFF();
	tmp=remote_info.gpib.rx_rp;
	if(tmp!=(remote_info.gpib.rx_wp))
	{
		c=remote_info.gpib.rx_buf[tmp];//从接收缓冲区读取一个字节

		//remote_info.gpib.rx_rp=(tmp+1)&(COM_BUF_SIZE-1);
		remote_info.gpib.rx_rp=(tmp+1)%COM_BUF_SIZE;
	}
	else 
		c=65535;

	if( remote_info.gpib.rx_wp >= remote_info.gpib.rx_rp ) 
	{
		tmp = remote_info.gpib.rx_wp - remote_info.gpib.rx_rp;
	}
	else
	{
		tmp = remote_info.gpib.rx_wp + COM_BUF_SIZE - remote_info.gpib.rx_rp;
	}


	if( tmp <= (COM_BUF_SIZE/8) ) 
	{
		if( gpib_holdon ) 
		{
			cpd_write_reg(ADDR_LOC_MSG, GPIB_MSG_HOLD);
			gpib_holdon = 0;
			remote_info.gpib.rx_full= FALSE;
		}
	}

	GPIBIntON();    //启用中断
	return c;
}






void GPIBPrintStr(char *pstr)
{	
	u8 count= outcounter;

	GPIBIntOFF();    
	cpd_write_reg(ADDR_LOC_MSG, 0x17);//清空GPIB卡的发送数据
	cpd_write_reg(ADDR_LOC_MSG, 0x1D);//清空GPIB卡的发送数据
	GPIBIntON(); //启用中断
	//delay_ms(50);  2022.1.21 取消延时直接返回数据


	while(count)
	{
		if(remote_info.gpib.tx_full!= TRUE)
		{
			putchar_gpib(*pstr);
			if(remote_info.gpib.tx_full!= TRUE)//发送失败,报输出缓冲溢出
			   pstr++;
			else				
			   break;
			
			count--;
		}
	
	}
}