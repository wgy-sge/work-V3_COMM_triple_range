/******************** (C) COPYRIGHT 2010 Zidetech *****************************
* File Name          : fmbframe.h
* Author             : xxx
* Version            : V0.1
* Date               : 04/23/2010
* Description        : Define frame format of comunication message
********************************************************************************/

#ifndef __FMBFRAME__H
#define __FMBFRAME__H

/*!
 * Constants which defines the format of a modbus frame. The example is
 * shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
 * dependent on the underlying transport.
 *
 * <code>
 * <----------------------- FMODBUS SERIAL LINE PDU (1) ------------------->
 *              <---------- FMODBUS PDU (1') ---------------->
 *  +-----------+---------------+----------------------------+-------------+
 *  | Address   | Function Code | Data                       |     CRC     |
 *  +-----------+---------------+----------------------------+-------------+
 *  |           |               |                                   |
 * (2)        (3/2')           (3')                                (4)
 *
 * (1)  ... MB_SER_PDU_SIZE_MAX = 512
 * (2)  ... MB_SER_PDU_ADDR_OFF = 0
 * (3)  ... MB_SER_PDU_PDU_OFF  = 1
 * (4)  ... MB_SER_PDU_SIZE_CRC = 2
 *
 * (1') ... MB_PDU_SIZE_MAX     = 509
 * (2') ... MB_PDU_FUNC_OFF     = 0
 * (3') ... MB_PDU_DATA_OFF     = 1
 * </code>
 */

/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN     4       //< Minimum size of a Modbus RTU frame
#define MB_SER_PDU_SIZE_MAX     1024     //< Maximum size of a Modbus RTU frame
#define MB_SER_PDU_SIZE_CRC     2       //< Size of CRC field in PDU
#define MB_SER_PDU_ADDR_OFF     0       //< Offset of source address in Ser-PDU
#define MB_SER_PDU_PDU_OFF      1       //< Offset of Modbus-PDU in Ser-PDU


#define MB_PDU_SIZE_MAX     509 /*!< Maximum size of a PDU. */
#define MB_PDU_SIZE_MIN     1   /*!< Function Code */
#define MB_PDU_FUNC_OFF     0   /*!< Offset of function code in PDU. */
#define MB_PDU_DATA_OFF     1   /*!< Offset for response data in PDU. */


#define MB_ADU_ADDR_OFF         0

#define MB_PDU_FUNC_READ_ADDR_OFF               ( MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_READ_REGCNT_OFF             ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE                   ( 3 )
#define MB_PDU_FUNC_READ_REGCNT_MAX             ( 0x0064 )

#define MB_PDU_FUNC_WRITE_ADDR_OFF              ( MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_WRITE_VALUE_OFF             ( MB_PDU_DATA_OFF + 5 )
#define MB_PDU_FUNC_WRITE_SIZE                  ( 6 )


#define POINT_MESSAGE_DATA                      (client_rec_buf+2)



/* ----------------------- Prototypes  0-------------------------------------*/
typedef void    ( *pvMBFrameStart ) ( void );

typedef void    ( *pvMBFrameStop ) ( void );


typedef void( *pvMBFrameClose ) ( void );



#endif