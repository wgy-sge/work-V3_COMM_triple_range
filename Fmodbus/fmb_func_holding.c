/******************** (C) COPYRIGHT 2010 Zidetech *****************************
* File Name          : fmb_func_holding.c
* Author             : xxx
* Version            : V1.0
* Date               : 2010.06.21
* Description        : 操作保持寄存器函数
********************************************************************************
* Log :
2009.06.21 xxx, initial version
*******************************************************************************/

#include "stdlib.h"
#include "string.h"

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "fmb.h"
#include "fmb_frame.h"
#include "fmb_proto.h"


/* ----------------------- Defines ------------------------------------------*/



/* ----------------------- Start implementation -----------------------------*/

/*******************************************************************************
* Function Name  : eMBFuncWriteHoldingRegister
* Description    : 写单个的寄存器
* Input          : None
* Output         : None
* Return         : errcode
*******************************************************************************/
smbError eMBFuncWriteHoldingRegister( UCHAR * pucFrame, USHORT * usLen )
{
    USHORT          usRegAddress;
    
    smbError    err_code = ERR_NO_ERROR;
    
    
    
    if( *usLen == ( MB_PDU_FUNC_WRITE_SIZE + MB_PDU_SIZE_MIN ) )
    {
        /* 从消息中解析地址 */
        usRegAddress = ( USHORT )( pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8 );
        usRegAddress |= ( USHORT )( pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF + 1] );
        
        /* 调用函数更新寄存器 */
        err_code = write_load_register( &pucFrame[MB_PDU_FUNC_WRITE_VALUE_OFF],
                                      usRegAddress, 1 );

        /* 对于写操作，只返回其错误码 */
        * usLen = 2;
    }
    else
    {
        /* Can't be a valid request because the length is incorrect. */
        err_code = ERR_NUMERIC_DATA_ERROR;
    }
    return err_code;
}


/*******************************************************************************
* Function Name  : eMBFuncReadHoldingRegister
* Description    : 读寄存器
* Input          : None
* Output         : None
* Return         : errcode
*******************************************************************************/
smbError eMBFuncReadHoldingRegister( UCHAR * pucFrame, USHORT * usLen )
{
    USHORT          usRegAddress;
    USHORT          usRegCount;
    UCHAR          *pucFrameCur;

    
    smbError    err_code = ERR_NO_ERROR;    

    if( *usLen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN ) )
    {
        usRegAddress = ( USHORT )( pucFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
        usRegAddress |= ( USHORT )( pucFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1] );
        

        usRegCount = ( USHORT )( pucFrame[MB_PDU_FUNC_READ_REGCNT_OFF]  );
        
        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
        if( ( usRegCount >= 1 ) && ( usRegCount <= MB_PDU_FUNC_READ_REGCNT_MAX ) )
        {
            /* Set the current PDU data pointer to the beginning. */
            pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
            
            pucFrameCur += 2;
            
            *usLen = usRegCount * 4 + 2;
            
            /* Make callback to fill the buffer. */
            err_code = read_load_register( pucFrameCur, usRegAddress, usRegCount );
        }
        else
        {
            err_code = ERR_NUMERIC_DATA_ERROR;
        }
    }
    else
    {
        /* Can't be a valid request because the length is incorrect. */
        err_code = ERR_NUMERIC_DATA_ERROR;
    }
    
    /* 查询出错 */
    if( err_code != ERR_NO_ERROR )
    {
        *usLen = 2;    
    }
    
    return err_code;
}



#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0

eMBException
eMBFuncReadWriteMultipleHoldingRegister( UCHAR * pucFrame, USHORT * usLen )
{
    USHORT          usRegReadAddress;
    USHORT          usRegReadCount;
    USHORT          usRegWriteAddress;
    USHORT          usRegWriteCount;
    UCHAR           ucRegWriteByteCount;
    UCHAR          *pucFrameCur;

    eMBException    eStatus = MB_EX_NONE;
    eMBErrorCode    eRegStatus;

    if( *usLen >= ( MB_PDU_FUNC_READWRITE_SIZE_MIN + MB_PDU_SIZE_MIN ) )
    {
        usRegReadAddress = ( USHORT )( pucFrame[MB_PDU_FUNC_READWRITE_READ_ADDR_OFF] << 8U );
        usRegReadAddress |= ( USHORT )( pucFrame[MB_PDU_FUNC_READWRITE_READ_ADDR_OFF + 1] );
        usRegReadAddress++;

        usRegReadCount = ( USHORT )( pucFrame[MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF] << 8U );
        usRegReadCount |= ( USHORT )( pucFrame[MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF + 1] );

        usRegWriteAddress = ( USHORT )( pucFrame[MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF] << 8U );
        usRegWriteAddress |= ( USHORT )( pucFrame[MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF + 1] );
        usRegWriteAddress++;

        usRegWriteCount = ( USHORT )( pucFrame[MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF] << 8U );
        usRegWriteCount |= ( USHORT )( pucFrame[MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF + 1] );

        ucRegWriteByteCount = pucFrame[MB_PDU_FUNC_READWRITE_BYTECNT_OFF];

        if( ( usRegReadCount >= 1 ) && ( usRegReadCount <= 0x7D ) &&
            ( usRegWriteCount >= 1 ) && ( usRegWriteCount <= 0x79 ) &&
            ( ( 2 * usRegWriteCount ) == ucRegWriteByteCount ) )
        {
            /* Make callback to update the register values. */
            eRegStatus = eMBRegHoldingCB( &pucFrame[MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF],
                                          usRegWriteAddress, usRegWriteCount, MB_REG_WRITE );

            if( eRegStatus == MB_ENOERR )
            {
                /* Set the current PDU data pointer to the beginning. */
                pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
                *usLen = MB_PDU_FUNC_OFF;

                /* First byte contains the function code. */
                *pucFrameCur++ = MB_FUNC_READWRITE_MULTIPLE_REGISTERS;
                *usLen += 1;

                /* Second byte in the response contain the number of bytes. */
                *pucFrameCur++ = ( UCHAR ) ( usRegReadCount * 2 );
                *usLen += 1;

                /* Make the read callback. */
                eRegStatus =
                    eMBRegHoldingCB( pucFrameCur, usRegReadAddress, usRegReadCount, MB_REG_READ );
                if( eRegStatus == MB_ENOERR )
                {
                    *usLen += 2 * usRegReadCount;
                }
            }
            if( eRegStatus != MB_ENOERR )
            {
                eStatus = prveMBError2Exception( eRegStatus );
            }
        }
        else
        {
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    return eStatus;
}

#endif
