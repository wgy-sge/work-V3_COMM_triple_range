#ifndef _GPIB_H_
#define _GPIB_H_





#define		CPD_RST_0			GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET)
#define		CPD_RST_1			GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET)



#define		CPD_EN_0			GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET)
#define		CPD_EN_1			GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET)
#define		CPD_RW_0			GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET)
#define		CPD_RW_1			GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET)
#define		CPD_OE_0			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET)
#define		CPD_OE_1			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET)



//---------------FT488D¼Ä´æÆ÷µØÖ·¶¨Òå--------------
typedef enum{
	ADDR_IN_Q,
	ADDR_OUT_Q,
	ADDR_INT_VEC,
	ADDR_INT_EN,
	ADDR_STAT0,
	ADDR_STAT1,
	ADDR_STAT2,
	ADDR_STAT3,
	ADDR_LOC_MSG,
	ADDR_STB,
	ADDR_SRE,
	ADDR_EOS,
	ADDR_GLINE,
	ADDR_GDATA,
	ADDR_CFG0,
	ADDR_CFG1,
	ADDR_PADDR,
	ADDR_SADDR,
	ADDR_PPEL,
	ADDR_PPEH,
	ADDR_DDC
} FT488D_REG;

typedef enum{
    GPIB_MSG_GO,
    GPIB_MSG_GTS,
	GPIB_MSG_HOLD
} GPIB_MSG;

typedef enum{
	VEC_NONE,
	VEC_IFC,
	VEC_DCLR,
	VEC_DTRIG,
	VEC_REM,
	VEC_TXC,
	VEC_RDY
} INT_VECTOR;

void	CPD_ADR(u8 adr);
void CPD_DATA_DIR(u8 x);

void cpd_write_reg(u8 addr,u8 data);
u8 cpd_read_reg(u8 addr);
void gpib_init(void);
u16 getchar_gpib(void);
void putchar_gpib(u8 ch);
void GPIBPrintStr(char *pstr);


































#endif