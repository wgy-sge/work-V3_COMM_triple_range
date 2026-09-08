#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#define uint8       u8
#define uint16      u16
#define uint32      u32

#define int8        s8
#define int16       s16
#define int32       s32

#define SOCKET  uint8

#define TCP_MODBUS_PORT  502
#define TCP_SCPI_PORT    5025



#define BROADCAST_ADDRESS    0

struct app_gpio
{
	uint32 periph_rcc;
	GPIO_TypeDef *gpio;
	uint16 gpio_pin;
};


#define COMMUN_CONNECT   68

//通讯协议
enum
{
	PROTOCOL_SCPI = 0,
	PROTOCOL_MODBUS,
};

//通讯信道
enum
{
	CHANNEL_LAN= 0, 
	CHANNEL_RS232,	
	CHANNEL_RS485,
	CHANNEL_TCP,
	CHANNEL_GPIB,
	AMOUNT_OF_CHANNEL,
};

typedef enum
{	
	FT_RS232,
	FT_GPIB,
	FT_USB
}REMOTEPORT;


typedef enum
{	
	BAUD_4800,
	BAUD_9600,
	BAUD_19200,
	BAUD_38400,
	BAUD_115200,
}RS232_BAUD;


typedef enum
{
	NONE_PARITY,	
	ODD_PARITY,
	EVENT_PARITY
}RS232_PARITY;


typedef enum
{
	LOCAL,
	REMOTE,
}LOCAL_REMOTE;

typedef short (*EVENTFUN)(void);

extern EVENTFUN Send_event, Recv_event;//发送和接收，待处理事件
extern bool		scpi_proc_ava;//SCPI解析器是否可用

/* TCP 帧长度*/
typedef union
{	
	u16 valid_len;          //整数寄存器
	u8 len_buff[2];
}unTcpFramelen;


/* 寄存器数据结构定义 */
typedef union
{
	float reg_floa;       //浮点寄存器
	s32 reg_int;          //整数寄存器
	u8 reg_buff[4];
}unReg32;

/* 寄存器数据结构定义 */
typedef struct
{
	u32 type;
	float reg_floa;       //浮点寄存器
	s32 reg_int;          //整数寄存器
	u32 dat32;

}Scpipara_type;

//读取自动测试文件测试结果
typedef struct  
{
	u8    pass;		//单步结果
	u8    fashion;	//单步方式
	float llimit;	//下限值
	float meas_val;	//测量值
	float ulimit;	//上限值
}AUTO_STEP_TYPE;

typedef struct  
{
	u8 pass_all;	//全局结果
	u8 length;		//结果的长度
	u8 pages;		//结果页的数目
	u8 cur_page_num;//当前页结果数目
	int cur_page;	//当前页面
	int cur_step;
	AUTO_STEP_TYPE auto_steps[50];
}AUTO_RESULT_TYPE;

//---------------------系统相关参数-----------------------------

struct net_info
{
	uint8 mac[6];                 //实际应该使用CPU内的唯一码作为mac地址
	uint8 iptab[4];
	uint8 netmask[4];
	uint8 gatewall[4];
	uint16 port;
};

typedef struct
{
	u8 comm_baud;
	u8 comm_parity;
	u8 gpib_addr;
	u8 comm_protocol;
	u8 device_addr;
	u32 comm_dog_time;

	struct net_info *pNetInfo;
}SYST_COMM_INFO_T;


extern SYST_COMM_INFO_T gSystCommInfo;


/*
typedef struct
{
	LOCAL_REMOTE local_remote;  //远程控制还是本地操作

}System_T;

extern System_T sys_info;*/

extern unReg32 sys_regs[];//寄存器顺序参考load_register.h中宏的定义

extern u16 (*GetChar)(void);
extern void (*PrintStr)(char*pstr);







extern AUTO_RESULT_TYPE auto_result;


#endif