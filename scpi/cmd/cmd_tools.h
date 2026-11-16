#ifndef _CMD_TOOLS_H_
#define _CMD_TOOLS_H_

#include "scpi_interface.h"

//SCPI参数定义
typedef enum
{
	SCPI_ON_OFF = 0x01,
	SCPI_SLAVER_MASTER = 0x02,
	SCPI_FUNCTION =0x04,
	SCPI_TCC_TCV_TCP_TCR = 0x08,
	SCPI_SEQ_AUTO_BRES_CAP_OCP = 0x10,
	SCPI_CONT = 0x20,
	SCPI_PULSE_TOGG =0x40,
	SCPI_TRIG =0x80,
	SCPI_VOLT_CURR_POW_RES = 0x100,
	SCPI_EXT_KEY_BUS =0x200,
	SCPI_FAST_SLOW = 0x400,

	SCPI_HEX= 0x800,// 可见字符表示十六进制
	SCPI_LOCAL_REMOTE = 0x1000,

	SCPI_MODE= 0x2000,//模式
	SCPI_DISCH_MODE= 0x4000,//放电模式
	SCPI_COMB_MODE=  0x8000,//复合测试模式

	SCPI_EXT_INHIBIT=  0x10000,//外部信号控制方式
	SCPI_FT6800_FUNCTION= 0x20000,//ELoader功能码
	SCPI_SEQ_MODE= 0x40000,//序列模式
	SCPI_INT  = 0x20000000,
	SCPI_FLOAT= 0x40000000,
	SCPI_STR  = 0x80000000,
}SCPI_STR_PARAMETER_TYPES;


//SCPI查询单位定义
typedef enum
{
	UNIT_NONE,
	UNIT_V,
	UNIT_A,
	UNIT_W,
	UNIT_S,
	UNIT_MS,
	UNIT_V_MS,
	UNIT_A_US,
	UNIT_HZ,
        UNIT_UF,
        UNIT_OHM,
        UNIT_UH,
}SCPI_UNIT_TYPE;

typedef enum
{
	COM_OFF,
	COM_ON,	

	COM_MASTER,
	COM_SLAVER,

	COM_CCH,
	COM_CCM,
	COM_CCL,
	COM_CVH,
	COM_CVM,
	COM_CVL,
	COM_CRH,	
	COM_CRM,
	COM_CRL,	
	COM_CP,		
	COM_CCDH,	
	COM_CCDM,
	COM_CCDL,
        COM_CVDH,	
	COM_CVDM,
	COM_CVDL,
	COM_CRDH,
	COM_CRDM,
	COM_CRDL,
	COM_CPD,
	COM_SEQ,
	COM_AUTO,
	COM_OCP,
	COM_OPP,
	COM_DISCH,
	COM_LOEF,
	COM_ESR,
	COM_LED,
	COM_SWEEP,
	COM_CV2CC,
	COM_CR2CC,
	COM_CP2CC,
        COM_CV2CR,
	COM_WAVE,
        COM_CZ,

	COM_CONT,
	COM_PULSE,
	COM_TOGGLE,

	COM_CURR,
	COM_VOLT,
	COM_POWER,
	COM_RES,

	COM_TRIG,

	COM_KEY,
	COM_EXT,
	COM_BUS,

	COM_FAST,
        COM_SLOW,

	COM_LOCAL,
	COM_REMOTE,
	
	COM_MODE_CC,
	COM_MODE_CV,

	

}SCPI_STR_PARAMETER;



extern const char *pExtInhibitStrings[];
extern const char *pSeqModeStrings[];


extern const char *pScpi_param[];

void GetModbusRegValue(u8 *pFrameDate, unReg32* pRegDat);
u8 ORBits(int inda);
u32 CmpString(char **pDstr, char *pStr);
u8 Hex2Value(char *pHexStr, u32* pVal);
u8* GetParaValue(u8 *pdata, u32 para_type, Scpipara_type* param, short *pscpi_err);

short ScpiSetCmdAnswer(void);

//电压查询
short ScpiQuestVoltCmdAnswer(void);

//电压斜率查询
short ScpiQuestVoltSlewCmdAnswer(void);

//电流查询
short ScpiQuestCurrCmdAnswer(void);

//电流斜率查询
short ScpiQuestCurrSlewCmdAnswer(void);


//功率查询
short ScpiQuestPowerCmdAnswer(void);

//毫秒参数显示查询
short ScpiQuestMSCmdAnswer(void);

//秒参数显示查询
short ScpiQuestSCmdAnswer(void);
//整数秒参数显示查询
short ScpiQuestIntSCmdAnswer(void);

//浮点数查询
short ScpiQuestFloatCmdAnswer(void);
//整数查询
short ScpiQuestIntCmdAnswer(void);

//电阻查询
short ScpiQuestResCmdAnswer(void);

//电容查询
short ScpiQuestCapCmdAnswer(void);

//电感查询
short ScpiQuestInductCmdAnswer(void);

enum 
{
	SEQ_MODE_STRING,
	
};
extern u16 EnumStringType;//枚举字符串类型
//枚举字符串查询
short ScpiQuestEnumStringAnswer(void);

//整数Hz参数显示查询
short ScpiQuestIntHzCmdAnswer(void);

void ClearErrorQueue( void );
short PullOutError( void );
bool PushInError( short err );




unReg32 GetPreRegDat(void);

short PreLoadRegAsk(u16 regaddr, p_scpi_func_type pAnswerProc);


#endif