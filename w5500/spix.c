#include "spix.h"



#define SPI_GPIO_MAX      4

enum
{
  SPI_GPIO_CS     = 0,
  SPI_GPIO_CLK    = 1,
  SPI_GPIO_MISO   = 2,
  SPI_GPIO_MOSI   = 3,
};

#if SPIx1

#define SPIx    SPI1
#define SPI_NET_DR_Base           ((u32)0x4001300C)

const struct app_gpio spi_gpio[SPI_GPIO_MAX]  = 
{
  RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_4,  //CS
  RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_5,  //CLK
  RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_6,  //MISO
  RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_7,  //MOSI
};

#define DMA_SPIx_RxCHN    DMA1_Channel2
#define DMA_SPIx_TxCHN    DMA1_Channel3

#define DMA_SPIx_RxCHN_FLAG    DMA1_FLAG_TC2
#define DMA_SPIx_TxCHN_FLAG    DMA1_FLAG_TC3


#elif SPIx2
#define SPIx    SPI2

#define SPI_NET_DR_Base           ((u32)0x4000380C)
const struct app_gpio spi_gpio[SPI_GPIO_MAX]  = 
{
  RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_12,  //CS
  RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_13,  //CLK
  RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_14,  //MISO
  RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_15,  //MOSI
};

#define DMA_SPIx_RxCHN          DMA1_Channel4
#define DMA_SPIx_TxCHN          DMA1_Channel5
#define DMA_SPIx_RxCHN_FLAG     DMA1_FLAG_TC4
#define DMA_SPIx_TxCHN_FLAG     DMA1_FLAG_TC5
#endif

void SPIx_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  SPI_InitTypeDef   SPI_InitStructure;
#if SPIx1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_SPI1, ENABLE);
#elif SPIx2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
#endif
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  RCC_APB2PeriphClockCmd(spi_gpio[SPI_GPIO_CLK].periph_rcc|spi_gpio[SPI_GPIO_MISO].periph_rcc|spi_gpio[SPI_GPIO_MOSI].periph_rcc|spi_gpio[SPI_GPIO_CS].periph_rcc, ENABLE);

  GPIO_InitStructure.GPIO_Pin = spi_gpio[SPI_GPIO_CLK].gpio_pin;
  GPIO_Init(spi_gpio[SPI_GPIO_CLK].gpio, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = spi_gpio[SPI_GPIO_MISO].gpio_pin;
  GPIO_Init(spi_gpio[SPI_GPIO_MISO].gpio, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = spi_gpio[SPI_GPIO_MOSI].gpio_pin;
  GPIO_Init(spi_gpio[SPI_GPIO_MOSI].gpio, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = spi_gpio[SPI_GPIO_CS].gpio_pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(spi_gpio[SPI_GPIO_CS].gpio, &GPIO_InitStructure);
  
  SPI_I2S_DeInit(SPIx);
  /* SPI Config -------------------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;

  SPI_Init(SPIx, &SPI_InitStructure);

  DMA_InitTypeDef  DMA_InitStructure;
  DMA_DeInit(DMA_SPIx_RxCHN);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32)SPI_NET_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32)NULL;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA_SPIx_RxCHN, &DMA_InitStructure);

  DMA_DeInit(DMA_SPIx_TxCHN);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32)SPI_NET_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32)NULL;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA_SPIx_TxCHN, &DMA_InitStructure);
  
  SPI_Cmd(SPIx, DISABLE); 

  /* Enable SPI */
  SPI_Cmd(SPIx, ENABLE);

}


#if 0
uint16 SPIx_ReadWrite_nByte(uint8 *data,uint16 readlen)
{
  uint16 len  = 0;
  DMA1_Channel2->CMAR = (uint32)data;
  DMA1_Channel2->CNDTR = readlen;
  
  DMA1_Channel3->CMAR = (uint32)data;//spi_rtx_buf;
  DMA1_Channel3->CNDTR = readlen;
  

  DMA_Cmd(DMA1_Channel2, ENABLE);
  DMA_Cmd(DMA1_Channel3, ENABLE);
  
  
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
  
  SPI_Cmd(SPIx, ENABLE);
  
  
  while (!DMA_GetFlagStatus(DMA1_FLAG_TC2));
  while (!DMA_GetFlagStatus(DMA1_FLAG_TC3));
  
  DMA_ClearFlag(DMA1_FLAG_TC2);
  DMA_ClearFlag(DMA1_FLAG_TC3);
  
  DMA_Cmd(DMA1_Channel2, DISABLE);
  DMA_Cmd(DMA1_Channel3, DISABLE);
  
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
  
  SPI_Cmd(SPIx, DISABLE);
  
  len = readlen;
  return len;
}
#else
uint16 SPIx_ReadWrite_nByte(uint8 *data,uint16 readlen)
{
  uint32 timecout = 0;
#define TIMEOUT     1000000
  uint16 len  = 0;
  

  DMA_SPIx_RxCHN->CMAR = (uint32)data;
  DMA_SPIx_RxCHN->CNDTR = readlen;
  
  DMA_SPIx_TxCHN->CMAR = (uint32)data;//spi_rtx_buf;
  DMA_SPIx_TxCHN->CNDTR = readlen;
  

  DMA_Cmd(DMA_SPIx_RxCHN, ENABLE);
  DMA_Cmd(DMA_SPIx_TxCHN, ENABLE);
  
  
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
  
  SPI_Cmd(SPIx, ENABLE);
  
  
  while ((!DMA_GetFlagStatus(DMA_SPIx_RxCHN_FLAG))  && timecout < TIMEOUT)
  {
    timecout++;
  }
  timecout  = 0;
  while ((!DMA_GetFlagStatus(DMA_SPIx_TxCHN_FLAG))  && timecout < TIMEOUT)
  {
    timecout++;
  }
  
  DMA_ClearFlag(DMA_SPIx_RxCHN_FLAG);
  DMA_ClearFlag(DMA_SPIx_TxCHN_FLAG);
  
  DMA_Cmd(DMA_SPIx_RxCHN, DISABLE);
  DMA_Cmd(DMA_SPIx_TxCHN, DISABLE);
  
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
  
  SPI_Cmd(SPIx, DISABLE);
  
  len = readlen;
  return len;
}
#endif

/**************************************************************************
*功能   ：SPI1发送数据和接受函数
*函数名 : SPIx_ReadWriteByte
*输入   ：tdata
*返回   ：rdata
**************************************************************************/
uint8 SPIx_ReadWrite_Byte(uint8 data)
{
/*  spi_write(TxData);
  return spi_read(); */
  SPI_Cmd(SPIx, ENABLE);
  while((SPIx->SR & SPI_I2S_FLAG_TXE) == (u16)RESET);
  SPIx->DR = data;
  while((SPIx->SR & SPI_I2S_FLAG_RXNE) == (u16)RESET);
  data = SPIx->DR;
  SPI_Cmd(SPIx, DISABLE); 
  return data;
 
}