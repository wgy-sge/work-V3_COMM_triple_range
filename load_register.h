#ifndef __LOAD__REGISTER__H
#define __LOAD__REGISTER__H

//负载功能定义
enum
{
	FUNC_CCH = 0,
	FUNC_CCL,
	FUNC_CVH,
	FUNC_CVL,
	FUNC_CRH,
	FUNC_CRL,
	FUNC_CP,
	FUNC_CCDH,
	FUNC_CCDL,
        FUNC_CVDH,
        FUNC_CVDL,
	FUNC_CRDH,
	FUNC_CRDL,
	FUNC_CPD,
	FUNC_SEQ,
	FUNC_AUTO,
	FUNC_OCP,
	FUNC_OPP,
	FUNC_DISCHARGE,
	FUNC_LOEF,
	FUNC_DCR,
	FUNC_LED,
	FUNC_SWEEP,
	FUNC_CVCC,
	FUNC_CRCC,
	FUNC_CPCC,
        FUNC_CVCR,
	FUNC_WAVE,
        FUNC_CZ,
	AMOUNT_OF_FUNCTION,

};

enum
{
	FT6800_FUNC_CC,
	FT6800_FUNC_CV,
	FT6800_FUNC_CP,
	FT6800_FUNC_CR,
	FT6800_FUNC_TC,
	FT6800_FUNC_TV,
	FT6800_FUNC_NULL,
	FT6800_FUNC_TR,
	FT6800_FUNC_SEQ,
	FT6800_FUNC_AUTO,
	FT6800_FUNC_BRES,
	FT6800_FUNC_BCAP,
	FT6800_FUNC_OCP,
	FT6800_FUNC_CAPA,
};



/*------------------------ Registers Map Define ------------------------------*/
#define  REG_ADDRESS_START       0x0001       //首个寄存器的地址 
#define  OFFS_NULL_REG              0         //没有使用的寄存器
typedef enum
{
	MAP_NO_REGISTER= -1,
	MAP_SELF_TEST_FLOW = 0,          //自检流程
	MAP_SELF_TEST_STATE,             //自检状态
	MAP_SELF_TEST_RESULT,            //自检结果
	MAP_CALI_COND_1,                 //校准情况
	MAP_CALI_COND_2,                 //校准情况
	MAP_RB_STATUS_2,                 //状态寄存器2
	MAP_RB_STATUS_1,                 //状态寄存器1
	MAP_RB_VOLTAGE,                  //回显电压 
	MAP_RB_CURRENT,                  //回显电流
	MAP_RB_POWER,                    //回显功率
	MAP_RB_RESISTANCE,               //回显阻值
	MAP_RB_QUANTITY,                 //流经电量
	MAP_RB_ENERGY,                   //流经能量
	MAP_RB_LOADING_TIME,             //持续输出时间
	MAP_RB_TEMPERATURE,              //回显温度传感器温度
	MAP_RB_COUNTDOWN_TIME,            //回显带载倒计时时间

	MAP_TEST_FUNCTION,               //测试功能
	MAP_ADVANCED_FUNCTION,           //高级测试功能
	MAP_RUN_SWITCH,                  //测试开关
	MAP_RUN_INVERSE,                 //反向开关

	MAP_TRIG_SIGNAL,                 //触发信号
	MAP_LOCAL_RMT,                   //远程本地控制
	MAP_CLEAR_FLOW_PAST,             //清除电量、能量、带载时间
	MAP_CLEAR_FAULT,                 //清除故障
	MAP_SAVE_RAPID,                  //保存文件
	MAP_RECALL_RAPID,                //调用文件
	MAP_SAVE_CMD,                    //保存命令
	MAP_RST_FACTORY_SET,             //恢复出厂设定	

	MAP_CAL_SWITCH,                  //校准开关 
	MAP_CAL_POINT,                   //校准项
	MAP_CAL_LEVEL,                   //校准点
	MAP_CAL_ALTER_LEVEL,             //改变校准点
	MAP_CAL_DATA_IP1,                //校准数据输入 
	MAP_CAL_DATA_IP2,	             //校准数据输入
	MAP_CAL_DATA_IP3,                //校准数据输入 
	MAP_CAL_DATA_IP4,	             //校准数据输入
	MAP_CAL_DATA_IP5,                //校准数据输入 
	MAP_CAL_DATA_IP6,	             //校准数据输入
	MAP_CAL_DATA_IP7,                //校准数据输入 
	MAP_CAL_DATA_IP8,	             //校准数据输入
	MAP_CAL_SAVE,                    //校准保存

	MAP_RB_ADC_VOLT,                  //电压采样AD值
	MAP_RB_ADC_CURR,                  //电流采样AD值
	MAP_RB_ADC_TEMP,
	MAP_COMB_CVCR_VOLT,              //组合测试之CV+CR电压
	MAP_COMB_CVCR_RESI,              //组合测试之CV+CR电阻
	MAP_RESERVE_06,                   //预留
	MAP_RESERVE_07,                   //预留

	MAP_CVDH_WAY,                    //CVDH运行方式
	MAP_CVDH_L1,                     //瞬态CV主值电流
	MAP_CVDH_L2,                     //瞬态CV瞬态电流
	MAP_CVDH_T1,                     //瞬态CV主值时间
	MAP_CVDH_T2,                     //瞬态CV瞬态时间
	MAP_CVDH_SLEW_UP,                //CVH上升斜率
	MAP_CVDH_SLEW_DN,                //CVH下降斜率
	MAP_CVDL_WAY,                    //CVDL运行方式
	MAP_CVDL_L1,                     //瞬态CV主值电流
	MAP_CVDL_L2,                     //瞬态CV瞬态电流
	MAP_CVDL_T1,                     //瞬态CV主值时间
	MAP_CVDL_T2,                     //瞬态CV瞬态时间
	MAP_CVDL_SLEW_UP,                //CVL上升斜率
	MAP_CVDL_SLEW_DN,                //CVL下降斜率
        
	MAP_RESERVE_22,                   //预留
	MAP_FUNCTION_MASK,                //功能掩码

	MAP_CCH_CURR_SET,                //CCH电流设定	
	MAP_CCH_SLEW_UP,                 //上升斜率
	MAP_CCH_SLEW_DN,                 //下降斜率
	MAP_CCL_CURR_SET,                //CCL电流设定
	MAP_CCL_SLEW_UP,                 //上升斜率
	MAP_CCL_SLEW_DN,                 //下降斜率
	MAP_CVH_VOLT_SET,                //CVH电压设定
	MAP_CVH_SLEW_UP,                 //上升斜率
	MAP_CVH_SLEW_DN,                 //下降斜率
	MAP_CVL_VOLT_SET,                //CVL电压设定	
	MAP_CVL_SLEW_UP,                 //上升斜率
	MAP_CVL_SLEW_DN,                 //下降斜率
	MAP_CRH_RESI_SET,                 //CR电阻设定
	MAP_CRH_SLEW_UP,                 //上升斜率
	MAP_CRH_SLEW_DN,                 //下降斜率
	MAP_CRL_RESI_SET,                 //CR电阻设定
	MAP_CRL_SLEW_UP,                 //上升斜率
	MAP_CRL_SLEW_DN,                 //下降斜率
	MAP_CP_POW_SET,                  //CP功率设定
	MAP_CP_SLEW_UP,                  //CP上升斜率
	MAP_CP_SLEW_DN,                  //CP下降斜率
	MAP_CCDH_WAY,                    //CCDH运行方式
	MAP_CCDH_L1,                     //瞬态CC主值电流
	MAP_CCDH_L2,                     //瞬态CC瞬态电流
	MAP_CCDH_T1,                     //瞬态CC主值时间
	MAP_CCDH_T2,                     //瞬态CC瞬态时间
	MAP_CCDH_SLEW_UP,                //CCL上升斜率
	MAP_CCDH_SLEW_DN,                //CCL下降斜率
	MAP_CCDL_WAY,                    //CCDL运行方式
	MAP_CCDL_L1,                     //瞬态CC主值电流
	MAP_CCDL_L2,                     //瞬态CC瞬态电流
	MAP_CCDL_T1,                     //瞬态CC主值时间
	MAP_CCDL_T2,                     //瞬态CC瞬态时间
	MAP_CCDL_SLEW_UP,                //CCL上升斜率
	MAP_CCDL_SLEW_DN,                //CCL下降斜率
	MAP_CRDH_WAY,                    //CRDH运行方式
	MAP_CRDH_L1,                     //CRDH主值电阻
	MAP_CRDH_L2,                     //CRDH瞬态电阻
	MAP_CRDH_T1,                     //CRDH主值脉宽
	MAP_CRDH_T2,                     //CRDH瞬态脉宽
	MAP_CRDH_SLEW_UP,                //CRDH上升斜率
	MAP_CRDH_SLEW_DN,                //CRDH下降斜率
	MAP_CRDL_WAY,                    //CRDL运行方式
	MAP_CRDL_L1,                     //CRDL主值电阻
	MAP_CRDL_L2,                     //CRDL瞬态电阻
	MAP_CRDL_T1,                     //CRDL主值脉宽
	MAP_CRDL_T2,                     //CRDL瞬态脉宽
	MAP_CRDL_SLEW_UP,                //CRDL上升斜率
	MAP_CRDL_SLEW_DN,                //CRDL下降斜率
	MAP_CPD_WAY,                     //CPD运行方式
	MAP_CPD_L1,                      //CPD主值电压
	MAP_CPD_L2,                      //CPD瞬态电压
	MAP_CPD_T1,                      //CPD主值时间
	MAP_CPD_T2,                      //CPD瞬态时间
	MAP_CPD_SLEW_UP,                 //CPD上升斜率
	MAP_CPD_SLEW_DN,                 //CPD下降斜率

	MAP_SHORT_CTRL,                  //短路控制
	MAP_VOLT_SENSE,                  //电压采样远近端
	MAP_PROG_ENABLE,                 //编程选择
	MAP_POWER_DOUBLE,                //功率加倍
	MAP_SAMP_FIFO_FILT,              //采样FIFO滤波器
	MAP_MS_SELECT,                   //主机从机选择
	MAP_MS_SLAVE_NUM,                //从机个数
	MAP_MS_CTRL,                     //并机控制
	MAP_RB_CASC_STATUS,              //级联状态
	MAP_CV_RESPONSE,                 //CV响应速度
	MAP_CC_UPPER,                    //CC操作上限
	MAP_CC_LOWER,                    //CC操作下限
	MAP_CV_UPPER,                    //CV操作上限
	MAP_CV_LOWER,                    //CV操作下限
	MAP_CR_UPPER,                    //CR操作上限
	MAP_CR_LOWER,                    //CR操作下限
	MAP_CP_UPPER,                    //CP操作上限
	MAP_CP_LOWER,                    //CP操作下限
	MAP_CURR_LMT,                    //电流限定	
	MAP_VON_POINT,                   //带载电压
	MAP_VON_LATCH,                   //带载锁存
	MAP_VOFF_POINT,                  //卸载电压
	MAP_PROT_OVER_CURR,              //过流保护  
	MAP_PROT_OVER_VOLT,              //过压保护  
	MAP_PROT_OVER_POW,               //过功率保护
	MAP_PROT_UNDER_VOLT,             //低压保护
	MAP_LOAD_TIME,                   //带载时间
	MAP_COMM_DOG_TIME,               //通讯看门狗

	MAP_SPEC_ITEM,                   //容限检查项目
	MAP_SPEC_VOLT_TOP,               //电压上限
	MAP_SPEC_VOLT_FLOOR,             //电压下限
	MAP_SPEC_CURR_TOP,               //电流上限
	MAP_SPEC_CURR_FLOOR,             //电流下限
	MAP_SPEC_POW_TOP,                //功率上限
	MAP_SPEC_POW_FLOOR,              //功率下限

	MAP_TIMING_START,                //时间测量开始
	MAP_TIMING_BEGN_SIGN,            //时间测量起始信号
	MAP_TIMING_BEGN_DIRE,            //时间测量起始信号方向
	MAP_TIMING_BEGN_LEVL,            //时间测量起始信号数值
	MAP_TIMING_END_SIGN,             //时间测量结束信号
	MAP_TIMING_END_DIRE,             //时间测量结束信号方向
	MAP_TIMING_END_LEVL,             //时间测量结束信号数值
	MAP_TIMING_BEGN_TIME,            //时间测量起始点时间
	MAP_TIMING_END_TIME,             //时间测量结束点时间
	MAP_TIMING_WORD,                 //时间测量结果字
	MAP_TIMING_TIME,                 //时间测量结果时间

	MAP_SAVE_POWER_OFF,              //掉电保存
	MAP_INPUT_POWER_ON,              //开机输入	
	MAP_IP_ADDRESS,                  //IP地址
	MAP_SUBNET_MASK,                 //子网掩码
	MAP_GATEWAY,                     //网关
	MAP_COM_BAUD,                    //串口速率
	MAP_COM_PARITY,                  //串口校验方式
	MAP_GPIB_ADDRESS,                //GPIB地址
	MAP_COMM_PROTOCOL,               //通讯协议
	MAP_DEVICE_ADDRESS,              //设备通讯地址
	MAP_KEY_SOUND,                   //键盘声音
	MAP_LANGUAGE,                    //系统语言
	MAP_SHORTCUT,                    //快速调用
	MAP_SHORT_PRESS,                 //短路按键
	MAP_INHIBIT,                     //外部抑制
	MAP_STAT_SLEW_DISP,              //定态斜率显示
	MAP_CV_REGU_AHEA,                //CV超前调节
	MAP_CC_SPEED,                    //CC速率
	MAP_SWITCH_LACK_VOLT,            //缺电压检测开关

	MAP_SCPI_CHAN_COND,              //SCPI通道状态
	MAP_SCPI_CHAN_EVEN,              //SCPI通道事件
	MAP_SCPI_CHAN_EVEN_CSUM,         //SCPI通道事件总结
	MAP_SCPI_STAN_ENAB,              //SCPI标准事件使能  
	MAP_SCPI_SERV_ENAB,              //SCPI服务请求使能
	MAP_SCPI_PSC_SWITCH,             //SCPI--PSC控制开关
	MAP_SCPI_CHAN_PTR,               //SCPI通道状态相关寄存器
	MAP_SCPI_CHAN_NTR,               //SCPI通道状态相关寄存器
	MAP_SCPI_CHAN_ENAB,              //SCPI通道状态相关寄存器

	MAP_MEAS_VOLT_PEAK,              //测量电压峰值
	MAP_MEAS_VOLT_VALL,              //测量电压谷值
	MAP_MEAS_CURR_PEAK,              //测量电流峰值
	MAP_MEAS_CURR_VALL,              //测量电流谷值
	MAP_MEAS_POW_PEAK,               //测量功率峰值
	MAP_MEAS_POW_VALL,               //测量功率谷值
	MAP_RIPP_VOLT_PEAK,              //纹波电压峰值 
	MAP_RIPP_VOLT_VALL,              //纹波电压谷值
	MAP_RIPP_CURR_PEAK,              //纹波电流峰值
	MAP_RIPP_CURR_VALL,              //纹波电流谷值

	MAP_SEQ_EDIT_FILE,               //编辑序列文件
	MAP_SEQ_EDIT_LENGTH,             //编辑序列长度 
	MAP_SEQ_EDIT_CYCLE,              //编辑序列循环次数
	MAP_SEQ_EDIT_LINK,               //编辑序列链接
	MAP_SEQ_EDIT_STEP,               //编辑序列步
	MAP_SEQ_EDIT_MODE,               //编辑模式
	MAP_SEQ_EDIT_LOAD,               //编辑带载主值
	MAP_SEQ_EDIT_SLEW,               //编辑单步斜率                       //2-------------------
	MAP_SEQ_EDIT_SLEW_DN,            //编辑单步下降斜率
	MAP_SEQ_EDIT_DWELL,              //编辑单步时间	                      //2-------------------
	MAP_SEQ_SAVE,                    //保存序列文件
	MAP_SEQ_RUN_FILE,                //序列运行文件
	MAP_SEQ_STAT_STEP,               //序列状态之运行步骤
	MAP_SEQ_STAT_CYCL,               //序列状态之循环次数
	MAP_AUTO_EDIT_FILE,              //编辑自动文件
	MAP_AUTO_EDIT_LENGTH,            //自动文件步数
	MAP_AUTO_EDIT_STEP,              //编辑步
	MAP_AUTO_EDIT_MODE,              //编辑模式
	MAP_AUTO_EDIT_LOAD,              //编辑带载主值
	MAP_AUTO_EDIT_SLEW,              //编辑单步斜率 
	MAP_AUTO_EDIT_SLEW_DN,           //编辑单步下降斜率
	MAP_AUTO_EDIT_SHORT,             //短路使能
	MAP_AUTO_EDIT_CHECK,             //编辑检查项目
	MAP_AUTO_EDIT_LMUP,              //编辑检查上限
	MAP_AUTO_EDIT_LMDN,              //编辑检查下限
	MAP_AUTO_EDIT_DWELL,             //编辑单步时间	
	MAP_AUTO_EDIT_SAVE,              //保存自动文件
	MAP_AUTO_RUN_FILE,               //自动运行文件
	MAP_AUTO_VOLT_CUT,               //断开电压
	MAP_AUTO_VOLT_CON,               //连接电压
	MAP_AUTO_STAT_STEP,              //自动运行步骤
	MAP_AUTO_RESULT,                 //自动运行总结果
	MAP_AUTO_QUERY_STEP,             //查询单步结果
	MAP_AUTO_DETAIL_STEP,            //单步测量细节
	MAP_AUTO_MEASURE_STEP,           //单步测量数值	
	MAP_AUTO_DISPLAY_MANN,           //自动测试结果显示方式
	MAP_OCP_VON_VOLT,                //OCP开启电压
	MAP_OCP_VON_DELAY,               //OCP开启延时
	MAP_OCP_ORIG_CURR,               //OCP起始电流
	MAP_OCP_DELT_CURR,               //OCP步进电流
	MAP_OCP_END_CURR,                //OCP终止电流
	MAP_OCP_DWELL_TIME,              //OCP单步驻留时间
	MAP_OCP_END_VOLT,                //OCP终止电压
	MAP_OCP_MAX_TRIP,                //OCP结果判断上限
	MAP_OCP_MIN_TRIP,                //OCP结果判断下限  
	MAP_OCP_RESULT_WORD,             //OCP结果字信息
	MAP_OCP_RESULT_CURR,             //OCP结果电流
	MAP_OCP_RESU_PMAX_POW,           //OCP最大功率点
	MAP_OCP_RESU_PMAX_VOLT,          //OCP最大功率点之电压
	MAP_OCP_RESU_PMAX_CURR,          //OCP最大功率点之电流
	MAP_OPP_VON_VOLT,                //OPP开启电压
	MAP_OPP_VON_DELAY,               //OPP开启延时
	MAP_OPP_ORIG_POW,                //OPP起始功率
	MAP_OPP_DELT_POW,                //OPP步进功率
	MAP_OPP_END_POW,                 //OPP终止功率
	MAP_OPP_DWELL_TIME,              //OPP单步驻留时间
	MAP_OPP_END_VOLT,                //OPP终止电压
	MAP_OPP_MAX_TRIP,                //OPP结果判断上限
	MAP_OPP_MIN_TRIP,                //OPP结果判断下限  
	MAP_OPP_RESULT_WORD,             //OPP结果字信息
	MAP_OPP_RESULT_POW,              //OPP结果功率
	MAP_OPP_RESU_POW_VOLT,           //OPP结果功率之电压
	MAP_OPP_RESU_POW_CURR,           //OPP结果功率之电流
	MAP_DISC_MODE,                   //放电模式
	MAP_DISC_CURR,                   //放电电流
	MAP_DISC_RESI,                   //放电阻值
	MAP_DISC_POW,                    //放电功率
	MAP_DISC_END_VOLT,               //终止电压
	MAP_DISC_END_CAPA,               //终止电量
	MAP_DISC_END_TIME,               //终止时间
	MAP_DISC_TIME,                   //放电时间
	MAP_LOEF_CURR_MIN,               //负载效应之最小电流  
	MAP_LOEF_CURR_MAX,               //负载效应之最大电流
	MAP_LOEF_CURR_NORM,              //负载效应之正常工作电流
	MAP_LOEF_DELAY,                  //负载效应之持续时间
	MAP_LOEF_REGUL,                  //负载效应之调整率
	MAP_LOEF_DELTV,                  //负载效应之电压变化
	MAP_LOEF_SOUCR,                  //负载效应之电源内阻
	MAP_ESR_MEAS_WAY,                //ESR测量方法
	MAP_ESR_MEAS_RANGE,              //ESR测量量程
	MAP_ESR_WHILE_TIME,              //ESR毫秒测试时间
	MAP_ESR_LOAD_RANGE,              //ESR拉载量程
	MAP_ESR_LOAD_CURR_RAN1,          //ESR拉载电流
	MAP_ESR_LOAD_CURR_RAN2,          //ESR拉载电流
	MAP_ESR_MAX_TRIP,                //ESR结果判断上限
	MAP_ESR_MIN_TRIP,                //ESR结果判断下限
	MAP_ESR_RESULT_WORD,             //ESR结果字
	MAP_ESR_RESULT_RESI,             //ESR测量阻值毫欧
	MAP_LED_VO,                      //顺向工作电压
	MAP_LED_IO,                      //顺向工作电流
	MAP_LED_COEF,                    //Rd系数
	MAP_SWEEP_CURR_A,                //扫频测试之带载电流A 
	MAP_SWEEP_CURR_B,                //扫频测试之带载电流B 
	MAP_SWEEP_SLEW_UP,               //扫频测试之上升斜率
	MAP_SWEEP_SLEW_DN,               //扫频测试之下降斜率
	MAP_SWEEP_DUTY,                  //扫频测试之占空比
	MAP_SWEEP_FSTART,                //扫频测试之起始频率
	MAP_SWEEP_FEND,                  //扫频测试之终止频率
	MAP_SWEEP_FSTEP,                 //扫频测试之步进频率
	MAP_SWEEP_DWELL,                 //扫频测试之单频率点持续时间
	MAP_SWEEP_VPP_VOLT,              //扫描测试之峰值电压
	MAP_SWEEP_VPP_FREQ,              //扫描测试之峰值频率
	MAP_SWEEP_VPV_VOLT,              //扫描测试之谷值电压
	MAP_SWEEP_VPV_FREQ,              //扫描测试之谷值频率
	MAP_COMB_CVCC_CURR,              //组合测试之CV+CC电流
	MAP_COMB_CVCC_VOLT,              //组合测试之CV+CC电压
	MAP_COMB_CRCC_CURR,              //组合测试之CR+CC电流
	MAP_COMB_CRCC_RESI,              //组合测试之CR+CC电阻
	MAP_COMB_CPCC_CURR,              //组合测试之CP+CC电流
	MAP_COMB_CPCC_POW,               //组合测试之CP+CC功率
	MAP_DCR_DISC_CURR_1,             //DCR测试电流1
	MAP_DCR_DISC_TIME_1,             //DCR测试脉宽1
	MAP_DCR_DISC_CURR_2,             //DCR测试电流2
	MAP_DCR_DISC_TIME_2,             //DCR测试脉宽2
	MAP_DCR_MAX_TRIP,                //DCR结果判断上限
	MAP_DCR_MIN_TRIP,                //DCR结果判断下限
	MAP_DCR_RESULT_WORD,             //DCR结果字
	MAP_DCR_RESULT_RESI,             //DCR测量阻值毫欧
	MAP_WAVE_TYPE,                   //波形类型
	MAP_WAVE_SIN_CREST,              //正玄波电流峰值
	MAP_WAVE_SIN_TROUGH,             //正玄波电流谷值
	MAP_WAVE_SIN_FREQUENCY,          //正玄波设置频率
	MAP_WAVE_SIN_PAUSE,              //正玄波暂停继续控制开关
	MAP_WAVE_RB_SIN_FREQ,            //正玄波实际频率
        
        MAP_CZ_Ls=328,                //串联电感
        MAP_CZ_Rs,                      //串联电阻
        MAP_CZ_Cl,                      //并联电容
        MAP_CZ_Rl,                      //并联电阻

	MAP_STEP_RUN_MODE=332,           //STEP运行模式
	MAP_STEP_START,                  //STEP开始
	MAP_STEP_DELTA,                  //STEP DELTA
	MAP_STEP_STOP,                   //STEP停止
	MAP_STEP_TRI_MODE_SELECT,        //STEP触发方式
	MAP_STEP_TIMER,                  //STEP时间

	MAP_MPPT_STEP_SIZE,                      //MPPT步长
	MAP_MPPT_REGU_PERIOD,                    //MPPT调整周期

	/* 三量程扩展寄存器追加在表尾，保持原有寄存器地址不变。 */
	MAP_CCM_CURR_SET,                //CCM curr set
	MAP_CCM_SLEW_UP,                 //CCM slew up
	MAP_CCM_SLEW_DN,                 //CCM slew dn
	MAP_CVM_VOLT_SET,                //CVM volt set
	MAP_CVM_SLEW_UP,                 //CVM slew up
	MAP_CVM_SLEW_DN,                 //CVM slew dn

	MAP_CVDM_WAY,                    //CVDM运行方式
	MAP_CVDM_L1,                     //瞬态CV主值电压
	MAP_CVDM_L2,                     //瞬态CV瞬态电压
	MAP_CVDM_T1,                     //瞬态CV主值时间
	MAP_CVDM_T2,                     //瞬态CV瞬态时间
	MAP_CVDM_SLEW_UP,                //CVDM上升斜率
	MAP_CVDM_SLEW_DN,                //CVDM下降斜率

	MAP_CCDM_WAY,                    //CCDM运行方式
	MAP_CCDM_L1,                     //瞬态CC主值电流
	MAP_CCDM_L2,                     //瞬态CC瞬态电流
	MAP_CCDM_T1,                     //瞬态CC主值时间
	MAP_CCDM_T2,                     //瞬态CC瞬态时间
	MAP_CCDM_SLEW_UP,                //CCDM上升斜率
	MAP_CCDM_SLEW_DN,                //CCDM下降斜率

	MAP_CRDM_WAY,                    //CRDM运行方式
	MAP_CRDM_L1,                     //CRDM主值电阻
	MAP_CRDM_L2,                     //CRDM瞬态电阻
	MAP_CRDM_T1,                     //CRDM主值脉宽
	MAP_CRDM_T2,                     //CRDM瞬态脉宽
	MAP_CRDM_SLEW_UP,                //CRDM上升斜率
	MAP_CRDM_SLEW_DN,                //CRDM下降斜率

	MAP_CRM_RESI_SET,                //CR电阻设定
	MAP_CRM_SLEW_UP,                 //上升斜率
	MAP_CRM_SLEW_DN,                 //下降斜率

	AMOUNT_OF_REGISTER,              //寄存器数目

	//-------------------------------------------------
	
	//MAP_CHANNEL_EVENT,               //通道事件		

	
	MAP_RESERVE_2,                   //预留
	MAP_RESERVE_3,                   //预留

	
	
	
	




}enREGMap;



//状态寄存器1位定义
#define    BIT_INPUT             0x00000001     //输入指示位[0]
#define    BIT_RV                0x00000002     //反接指示位[1]
#define    BIT_SHORT             0x00000004     //短路指示位[2]
#define    BIT_SENSE             0x00000008     //电压SENSE位[3]
#define    BITS_MODE             0x000000F0     //模式位域[4-7]
#define    BITS_FUNCTION         0x00001F00     //功能位域[8-12]
#define    BIT_SAMP_VOLT_OL      0x00002000     //电压采样超量程[13]
#define    BIT_SAMP_CURR_OL      0x00004000     //电流采样超量程[14]
#define    BIT_RMT               0x00008000     //远程控制位域[15]
#define    BIT_FINISH            0x00010000     //测试完成位[16]
#define    BIT_RUNNING           0x00020000     //测试进行位[17] 
#define    BITS_PROTECT          0x00FC0000     //保护位域[18-23]
#define    BITS_RINGING          0x07000000     //铃声位域[24-26]
#define    BITS_EXAMINE_RESULT   0x18000000     //检查结果[27-28]
#define    BIT_RMT_LOCK          0x20000000     //远程锁定[29]
#define    BIT_CASCADE_ENABLE    0x40000000     //级联使能[30]

#define    BIT_START_APP         0x80000000            //应用程序启动[31]

#define    SHIFT_MODE             4
#define    SHIFT_FUNCTION         8
#define    SHIFT_APG              11
#define    SHIFT_RINGING          24
#define    SHIFT_PROTECT          18

#define  REG_COUNT_NREGS         (OFFS_REG_NUMBER-1)          //寄存器的个数



//块数据地址
enum
{
	//MAP_BLOCK_ATTRIBUTE = 0,
	MAP_BLOCK_SPECIFICATION= 1 ,
	MAP_BLOCK_MAIN_SOFT,
	MAP_BLOCK_COMM_SOFT,
	MAP_BLOCK_MANUFACTURER=8,
	MAP_BLOCK_MODEL,
	MAP_BLOCK_SERIAL,
	MAP_BLOCK_DATE,
	//MAP_BLOCK_ATTRIBUTE,
	MAP_BLOCK_CASC_KIBI,
	MAP_BLOCK_MAIN_BOOT_VERS,
	MAP_BLOCK_COMM_BOOT_VERS,

	AMOUNT_OF_BLOCK,
};


#endif