#ifndef _UART_H_
#define _UART_H_


#define	TIME_OUT_ONE_PARAM    50	// 50ms
#define	TIME_OUT_BLK_PARAM    100	// 100ms


#define	COM_BUF_SIZE	250

typedef struct {
  u8 rx_buf[COM_BUF_SIZE], tx_buf[COM_BUF_SIZE];
  u16 tx_rp, tx_wp;
  u16 rx_rp, rx_wp;	//RxdCount--从RxBuffer读数据时的读指针， TxdCount--从TxBuffer读数据时的读指针
  u8 tx_full;	    //发送缓冲区满
  u8 rx_full;		//接收缓冲区满
  u8 rx_err;		//接收缓冲区满错误标志
  u8 tx_err;		//发送缓冲区满错误标志
} COM_INFO_T;

//-------------远程通信接口----------------
typedef struct {
 COM_INFO_T  uart1;
 COM_INFO_T  gpib;
 COM_INFO_T  usb;
 COM_INFO_T  lan;
 COM_INFO_T  tcplan;
 bool change;
} REMOTE_INFO_T;

//-------------CTRL<-->COMM通信接口------------
#define  MAX_CMD_LENGTH		128
typedef struct
{
	COM_INFO_T  uart2;
	u8  cmd[MAX_CMD_LENGTH];
	u8  cmd_index;
} COMM_CTRL_PORT;



//---------------------------------------------------------------------------

extern COMM_CTRL_PORT	comm_ctrl;
extern REMOTE_INFO_T	remote_info;


void putchar_uart1(u8 data );
u16 getchar_uart1(void);

void COMPrintStr(char *pstr);
void putchar_uart2(u8 data );
u16 getchar_uart2(void);

void putchar_usb(u8 data );
u16 getchar_usb(void);
void USBPrintStr(char *pstr);

#endif