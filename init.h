#ifndef _INIT_H_
#define _INIT_H_





extern EXTI_InitTypeDef 	 	EXTI_InitStructure;
extern GPIO_InitTypeDef 		GPIO_InitStructure;
extern NVIC_InitTypeDef 	 	NVIC_InitStructure;
extern USART_InitTypeDef 	    USART_InitStructure;
//extern I2C_InitTypeDef	 	    I2C_InitStructure;







void GPIBIntOFF(void);
void GPIBIntON(void);

void delay_us(vu32 us);
void delay_ms(vu32  ms);

void ResetInit(void);

void RCC_Configuration(void);
void Init_Device(void);


void ChangeGPIBAddr(void);

void Init_SystVariables(void);

void uart1Init(void);


void InitScpiREG(void);





#endif