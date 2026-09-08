
#include "include.h"

/* Private variables ---------------------------------------------------------*/
u8 gDevice_CommProtocol = PROTOCOL_MODBUS;//PROTOCOL_SCPI;//

u8 gDeviceAddr= 0;//设备地址
u8 gBroadcastComm= 0;//广播通讯方式


/*******************************************************************************
* Function Name  : EnginePoll
* Description    : 测试引擎函数
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void InitCommunicationProtocol( u8 prot )
{
	if( prot == PROTOCOL_MODBUS )
	{
		gDevice_CommProtocol = PROTOCOL_MODBUS;
	}
	else
	{
		gDevice_CommProtocol = PROTOCOL_SCPI;
	}
}


/*******************************************************************************
* Function Name  : EnginePoll
* Description    : 测试引擎函数
* Input          : None
* Output         : None
* Return         : None
* Note           : None
*******************************************************************************/
void EnginePoll( void )
{
	//AppRuningLedPoll( );
	LanMbDetectFrame( );
	TcpLanMbDetectFrame( );
	if( gDevice_CommProtocol == PROTOCOL_MODBUS )
	{		
		TrpaTrsmPoll( );		
	}
	else
	{	
		scpi_main_proc( );	//SCPI指令处理
	}

	CanOpenPoll();
	Ctx();
}