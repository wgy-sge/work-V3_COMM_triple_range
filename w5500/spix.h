#ifndef __SPIX_H
#define __SPIX_H

#include "include.h"

#define SPIx1 0
#define SPIx2 1

#if SPIx1
#define SPIx_CS_PIN     GPIO_Pin_4
#define SPIx_CS_Port   GPIOA
#elif SPIx2
#define SPIx_CS_PIN     GPIO_Pin_12
#define SPIx_CS_Port   GPIOB
#endif


#define SPIx_CS_H()   (SPIx_CS_Port->BSRR = SPIx_CS_PIN)
#define SPIx_CS_L()   (SPIx_CS_Port->BRR = SPIx_CS_PIN)

void SPIx_Init(void);

uint8 SPIx_ReadWrite_Byte(uint8 data);
uint16 SPIx_ReadWrite_nByte(uint8 *data,uint16 readlen);



#endif

