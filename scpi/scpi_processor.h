#ifndef _SCPI_PROCESSOR_H_
#define _SCPI_PROCESSOR_H_

#include "include.h"

#define MAXOUTPUTCHARS    1024
#define MAX_ERROR_NUM   32//最大错误数目



enum
{
	root,										//0
	//*********************************************
	//*********************************************
	calibrate,									//1
	calibrate_state,
	calibrate_point,
	calibrate_level,
	calibrate_data,
	calibrate_save,
	calibrate_make,
	calibrate_model,
	calibrate_serial,
	calibrate_date,
	calibrate_alter,
	calibrate_reg,
	calibrate_bound,

   //*********************************************
	//scpi_function,
	//*********************************************
	//*********************************************
	input,										//7	
	output,
	input_state,
	input_function,
	input_short,
	input_von,
	input_von_level,
	input_voff,
	input_voff_level,
	input_timer,
	input_timer_level,
	input_protection,
	input_protection_current,
	input_protection_current_level,
	input_protection_voltage,
	input_protection_voltage_level,
	input_protection_power,		
	input_protection_power_level,
	input_protection_clear,
	//*********************************************
	//*********************************************
	SYSTEM,										//17
	system_sense,
	system_sense_state,	
	system_response,
	system_response_voltage,	
	system_error,
	system_version,
	system_local,	
	system_clear,
	system_clear_charge,
	system_clear_time,
	system_clear_energy,
	system_parallel,
	system_parallel_number,
	system_parallel_slavers,
	system_parallel_state,
	system_rwlock,
	system_battcv,
	system_battcv_state,
	//*********************************************
	//*********************************************
	configure,
	configure_voltage,
	configure_voltage_sense,	
	configure_voltage_response,
	configure_voltage_on,
	configure_voltage_off,
	configure_voltage_upper,
	configure_voltage_lower,
	configure_current,
	configure_current_upper,
	configure_current_lower,
	configure_resistance,
	configure_resistance_upper,
	configure_resistance_lower,
	configure_power,
	configure_power_upper,
	configure_power_lower,
	configure_protect,
	configure_protect_ovp,
	configure_protect_ocp,
	configure_protect_opp,
	configure_protect_lvp,
	configure_protect_time,
	configure_limit,
	configure_limit_current,
	configure_external,
	configure_external_inhibit,
	//*********************************************
	//*********************************************
	measure,									//31
	measure_scalar,
	measure_scalar_current,
	measure_scalar_current_dc,
	measure_scalar_voltage,
	measure_scalar_voltage_dc,
	measure_scalar_power,
	measure_scalar_power_dc,
	measure_scalar_resistance,
	measure_scalar_resistance_dc,
	measure_scalar_temperature,
	measure_charge,
	measure_time,
	measure_energy,

	//*********************************************
	//*********************************************
	fetch,
	fetch_voltage,
	fetch_current,
	fetch_status,
	//*********************************************
	status,										//154
	status_questionable,
	status_questionable_condition,
	status_questionable_enable,
	status_questionable_event,
	status_channel,
	status_channel_condition,
	status_channel_event,
	status_channel_enable,
	status_channel_ptransition,
	status_channel_ntransition,
	//*********************************************
	trigger,									//151
	trigger_immediate,
	trigger_source,
	//*********************************************
	//*********************************************
  	//*********************************************
	source,	
	source_function,
	source_voltage,								
	source_voltage_level,	
	source_voltage_range,
	source_voltage_rise,
	source_voltage_fall,
        source_voltage_dynamic,	
	source_voltage_dynamic_mode,	
	source_voltage_dynamic_l1,
	source_voltage_dynamic_l2,
	source_voltage_dynamic_t1,
	source_voltage_dynamic_t2,
	source_voltage_dynamic_rise,
	source_voltage_dynamic_fall,
	//*********************************************
		
	source_current,
	source_current_level,
	source_current_range,
	source_current_rise,
	source_current_fall,
	source_current_dynamic,	
	source_current_dynamic_mode,	
	source_current_dynamic_l1,
	source_current_dynamic_l2,
	source_current_dynamic_t1,
	source_current_dynamic_t2,
	source_current_dynamic_rise,
	source_current_dynamic_fall,	

	//*********************************************
	source_power,								//55
	source_power_level,
	source_power_dynamic,
	source_power_dynamic_mode,
	source_power_dynamic_l1,
	source_power_dynamic_l2,
	source_power_dynamic_t1,
	source_power_dynamic_t2,
	//*********************************************
	source_resistance,							//63
	source_resistance_level,	
	source_resistance_range,
	source_resistance_rise,
	source_resistance_faLl,

	source_resistance_dynamic,
	source_resistance_dynamic_mode,
	source_resistance_dynamic_l1,
	source_resistance_dynamic_l2,
	source_resistance_dynamic_t1,
	source_resistance_dynamic_t2,	
	source_resistance_dynamic_rise,
	source_resistance_dynamic_fall,	
	
	//*********************************************
	source_transient,							//71
	source_transient_current,
	source_transient_current_fashion,
	source_transient_current_mlevel,
	source_transient_current_mwidth,
	source_transient_current_tlevel,
	source_transient_current_twidth,
	source_transient_current_raise,
	source_transient_current_fall,

	source_transient_power,
	source_transient_power_fashion,
	source_transient_power_mlevel,
	source_transient_power_mwidth,
	source_transient_power_tlevel,
	source_transient_power_twidth,	

	source_transient_resistance,
	source_transient_resistance_fashion,
	source_transient_resistance_mlevel,
	source_transient_resistance_mwidth,
	source_transient_resistance_tlevel,
	source_transient_resistance_twidth,	
	source_transient_resistance_raise,	
	source_transient_resistance_fall,	
	//*********************************************
	//编辑
	source_sequence,							//76
	source_sequence_edit,	
	source_sequence_edit_file,
	source_sequence_edit_file_number,
	source_sequence_edit_file_length,
	source_sequence_edit_file_cycle,
	source_sequence_edit_file_link,
	source_sequence_edit_step,
	//source_sequence_edit_mode,	
	//source_sequence_edit_slew,	
	source_sequence_edit_level,	
	source_sequence_edit_dwell,
	source_sequence_edit_slew,
	source_sequence_edit_mode,
	source_sequence_edit_save,

	//测试
	source_sequence_run,
	source_sequence_run_number,
	source_sequence_status,	
	source_sequence_status_step,
	source_sequence_status_cycle,	
	//*********************************************	
	source_auto,								//110
	source_auto_edit,
	source_auto_edit_file,
	source_auto_edit_file_num,
	source_auto_edit_file_length,
	source_auto_edit_step,
	source_auto_edit_mode,	
	source_auto_edit_level,
	source_auto_edit_short,
	source_auto_edit_rbwhat,
	source_auto_edit_llimit,
	source_auto_edit_ulimit,
	source_auto_edit_delay,
	source_auto_edit_save,
	source_auto_edit_range,

	source_auto_run,
	source_auto_run_file,
	source_auto_run_status,
	source_auto_run_status_step,
	source_auto_run_result,
	source_auto_run_result_all,
	source_auto_run_result_step,

	//source_auto_query,
	//source_auto_query_step,

	source_auto_voltage,
	source_auto_voltage_connect,
	source_auto_voltage_disconnect,	
	//*********************************************
	source_ocp,						//144
	source_ocp_bvoltage,
	source_ocp_bdelay,	
	source_ocp_bcurrent,	
	source_ocp_scurrent,
	source_ocp_delay,	
	source_ocp_ecurrent,
	source_ocp_evoltage,	
	source_ocp_tupper,
	source_ocp_tlower,	
	source_ocp_result,
	source_ocp_range,
	//*********************************************
	source_opp,								//144
	source_opp_bvoltage,
	source_opp_bdelay,	
	source_opp_bpower,	
	source_opp_spower,
	source_opp_sdelay,
	source_opp_epower,
	source_opp_evoltage,
	source_opp_tupper,
	source_opp_tlower,	
	source_opp_result,

	//*********************************************
	source_effect,
	source_effect_current,
	source_effect_current_min,
	source_effect_current_max,
	source_effect_current_normal,
	source_effect_test,
	source_effect_test_dewll,
	source_effect_status,
	source_effect_status_result,

	//*********************************************
	//LED
	source_led,
	source_led_test,
	source_led_test_voltage,
	source_led_test_current,
	source_led_test_coefficient,
	//*********************************************
	source_sweep,
	source_sweep_current,
	source_sweep_current_l1,
	source_sweep_current_l2,
	source_sweep_current_rise,
	source_sweep_current_fall,
	source_sweep_frequency,
	source_sweep_frequency_start,
	source_sweep_frequency_end,
	source_sweep_frequency_step,
	source_sweep_wave,
	source_sweep_wave_duty,
	source_sweep_wave_dweel,
	source_sweep_status,
	source_sweep_status_result,
	//*********************************************
	source_combine,
	source_combine_mode,
	source_combine_cvcc,
	source_combine_cvcc_voltage,
	source_combine_cvcc_current,
	source_combine_crcc,
	source_combine_crcc_current,
	source_combine_crcc_resistance,
	source_combine_cpcc,
	source_combine_cpcc_current,
	source_combine_cpcc_power,
	//*********************************************
	
	source_bcapacitance,
	source_bcapacitance_test,
	source_bcapacitance_test_mode,
	source_bcapacitance_current,
	source_bcapacitance_dresistance,
	source_bcapacitancet_power,	
	source_bcapacitance_evoltage,
	source_bcapacitance_ecapacity,
	source_bcapacitance_etime,
	source_bcapacitance_result,
	source_bcapacitance_result_query,
	source_bcapacitance_result_capacity,
	source_bcapacitance_result_energy,
	source_bcapacitance_result_time,	
	//*********************************************

	source_bresistance,	
	source_bresistance_range,	
	source_bresistance_current,
	source_bresistance_current_mlevel,
	source_bresistance_current_mwidth,
	source_bresistance_current_tlevel,
	source_bresistance_current_twidth,
	source_bresistance_trip,
	source_bresistance_trip_upper,
	source_bresistance_trip_lower,	
	source_bresistance_result, 
	//*********************************************
	source_wave,
	source_wave_crest,
	source_wave_trough,
	source_wave_frequency,
        
        //*********************************************
	source_cz,
	source_cz_parcap,
	source_cz_parres,
	source_cz_serinduct,
        source_cz_serres,
	//query,
	//query_calibrate,
};



extern int serv_req_enable;

extern u16   cmdcnt; //SCPI解析器收到的命令字符数
extern char OutputBuff[MAXOUTPUTCHARS];
extern u16 outcounter;

extern short scpi_error_fifo[MAX_ERROR_NUM];//错误码缓存
extern u8 curerrornum;//错误计数



//报错函数
short ReportError(void);



extern void scpi_main_proc(void);

//公共指令
extern short scpi_com_cls(void);
extern short scpi_com_ese(void);
extern short scpi_com_ese_quest(void);
extern short scpi_com_esr_quest(void);
extern short scpi_com_idn_quest(void);
extern short scpi_com_opc(void);
extern short scpi_com_opc_quest(void);
extern short scpi_com_psc(void);
extern short scpi_com_psc_quest(void);
extern short scpi_com_rst(void);
extern short scpi_com_sre(void);
extern short scpi_com_sre_quest(void);
extern short scpi_com_stb_quest(void);
extern short scpi_com_trg(void);
extern short scpi_com_sav(void);
extern short scpi_com_rcl(void);
extern short scpi_com_tst_quest(void);
extern short scpi_com_fan(void);
extern short scpi_com_fan_quest(void);
extern short scpi_com_wai(void);

//

//+++++++++++++++++++++++++++++++++++++++++++++++++++
//
//设备私有指令
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++

//校准指令
short scpi_spe_cal_state(void);
short scpi_spe_cal_state_quest(void);
short scpi_spe_cal_point(void);
short scpi_spe_cal_point_quest(void);
short scpi_spe_cal_level(void);
short scpi_spe_cal_level_quest(void);
short scpi_spe_cal_data(void);
short scpi_spe_cal_save(void);
short scpi_spe_cal_make(void);
short scpi_spe_cal_make_quest(void);
short scpi_spe_cal_model(void);
short scpi_spe_cal_model_quest(void);
short scpi_spe_cal_serial(void);
short scpi_spe_cal_serial_quest(void);
short scpi_spe_cal_date(void);
short scpi_spe_cal_date_quest(void);
short scpi_spe_cal_alter(void);
short scpi_spe_cal_alter_quest(void);
short scpi_spe_cal_register(void);
short scpi_spe_cal_register_quest(void);
short scpi_spe_cal_bound_quest(void);
//输入控制指令
short scpi_spe_input_output_state(void);
short scpi_spe_input_output_state_quest(void);
short scpi_spe_input_output_function(void);
short scpi_spe_input_output_function_quest(void);
short scpi_spe_input_output_short(void);
short scpi_spe_input_output_short_quest(void);
short scpi_spe_input_output_von_level(void);
short scpi_spe_input_output_von_level_quest(void);
short scpi_spe_input_output_voff_level(void);
short scpi_spe_input_output_voff_level_quest(void);
short scpi_spe_input_output_timer_level(void);
short scpi_spe_input_output_timer_level_quest(void);
short scpi_spe_input_output_prot_current(void);
short scpi_spe_input_output_prot_current_quest(void);
short scpi_spe_input_output_prot_volt(void);
short scpi_spe_input_output_prot_volt_quest(void);
short scpi_spe_input_output_prot_power(void);
short scpi_spe_input_output_prot_power_quest(void);
short scpi_spe_input_output_prot_clear(void);

short scpi_spe_function(void);
short scpi_spe_function_quest(void);
//系统指令
short scpi_spe_syst_sense_state(void);
short scpi_spe_syst_sense_state_quest(void);
short scpi_spe_syst_response_volt(void);
short scpi_spe_syst_response_volt_quest(void);
short scpi_spe_syst_error_quest(void);
short scpi_spe_syst_version_quest(void);
short scpi_spe_syst_local(void);
short scpi_spe_syst_clear_charge(void);
short scpi_spe_syst_clear_time(void);
short scpi_spe_syst_clear_energy(void);

//并机指令
short scpi_spe_syst_parallel_numb(void);
short scpi_spe_syst_parallel_numb_quest(void);
short scpi_spe_syst_parallel_slavers(void);
short scpi_spe_syst_parallel_slavers_quest(void);
short scpi_spe_syst_parallel_state(void);
short scpi_spe_syst_parallel_state_quest(void);
short scpi_spe_syst_rwlock(void);
short scpi_spe_syst_battcv_state(void);
short scpi_spe_syst_battcv_state_quest(void);

//配置指令
short scpi_spe_conf_volt_sense(void);
short scpi_spe_conf_volt_sense_quest(void);

short scpi_spe_conf_volt_on(void);
short scpi_spe_conf_volt_on_quest(void);
short scpi_spe_conf_volt_off(void);
short scpi_spe_conf_volt_off_quest(void);
short scpi_spe_conf_volt_upper(void);
short scpi_spe_conf_volt_upper_quest(void);
short scpi_spe_conf_volt_lower(void);
short scpi_spe_conf_volt_lower_quest(void);
short scpi_spe_conf_current_upper(void);
short scpi_spe_conf_current_upper_quest(void);
short scpi_spe_conf_current_lower(void);
short scpi_spe_conf_current_lower_quest(void);
short scpi_spe_conf_resistance_upper(void);
short scpi_spe_conf_resistance_upper_quest(void);
short scpi_spe_conf_resistance_lower(void);
short scpi_spe_conf_resistance_lower_quest(void);
short scpi_spe_conf_power_upper(void);
short scpi_spe_conf_power_upper_quest(void);
short scpi_spe_conf_power_lower(void);
short scpi_spe_conf_power_lower_quest(void);

short scpi_spe_conf_protect_ovp(void);
short scpi_spe_conf_protect_ovp_quest(void);
short scpi_spe_conf_protect_ocp(void);
short scpi_spe_conf_protect_ocp_quest(void);
short scpi_spe_conf_protect_opp(void);
short scpi_spe_conf_protect_opp_quest(void);
short scpi_spe_conf_protect_lvp(void);
short scpi_spe_conf_protect_lvp_quest(void);
short scpi_spe_conf_protect_time(void);
short scpi_spe_conf_protect_time_quest(void);
short scpi_spe_conf_limit_curr(void);
short scpi_spe_conf_limit_curr_quest(void);
short scpi_spe_conf_external_inhibit(void);
short scpi_spe_conf_external_inhibit_quest(void);



//测量指令
short scpi_spe_measure_current_quest(void);
short scpi_spe_measure_volt_quest(void);
short scpi_spe_measure_power_quest(void);
short scpi_spe_measure_resis_quest(void);
short scpi_spe_measure_temperature_quest(void);
short scpi_spe_measure_state_registers_quest(void);
short scpi_spe_measure_charge_quest(void);
short scpi_spe_measure_energy_quest(void);
short scpi_spe_measure_time_quest(void);
short scpi_spe_fetch_status_quest(void);

//定态电流
short scpi_spe_static_current_level(void);
short scpi_spe_static_current_level_quest(void);
short scpi_spe_static_current_range(void);
short scpi_spe_static_current_range_quest(void);

short scpi_spe_static_current_rise(void);
short scpi_spe_static_current_rise_quest(void);
short scpi_spe_static_current_fall(void);
short scpi_spe_static_current_fall_quest(void);

//定态电压
short scpi_spe_static_volt_level(void);
short scpi_spe_static_volt_level_quest(void);
short scpi_spe_static_volt_range(void);
short scpi_spe_static_volt_range_quest(void);
short scpi_spe_static_volt_rise(void);
short scpi_spe_static_volt_rise_quest(void);
short scpi_spe_static_volt_fall(void);
short scpi_spe_static_volt_fall_quest(void);


//定态功率
short scpi_spe_static_power_level(void);
short scpi_spe_static_power_level_quest(void);

//定态阻值
short scpi_spe_static_resistance_level(void);
short scpi_spe_static_resistance_level_quest(void);
short scpi_spe_static_resistance_range(void);
short scpi_spe_static_resistance_range_quest(void);
short scpi_spe_static_resistance_rise(void);
short scpi_spe_static_resistance_rise_quest(void);
short scpi_spe_static_resistance_fall(void);
short scpi_spe_static_resistance_fall_quest(void);

//瞬态电压
short scpi_spe_transient_voltage_fashion(void);
short scpi_spe_transient_voltage_fashion_quest(void);
short scpi_spe_transient_voltage_mlevel(void);
short scpi_spe_transient_voltage_mlevel_quest(void);
short scpi_spe_transient_voltage_mwidth(void);
short scpi_spe_transient_voltage_mwidth_quest(void);
short scpi_spe_transient_voltage_tlevel(void);
short scpi_spe_transient_voltage_tlevel_quest(void);
short scpi_spe_transient_voltage_twidth(void);
short scpi_spe_transient_voltage_twidth_quest(void);
short scpi_spe_transient_voltage_negative(void);
short scpi_spe_transient_voltage_negative_quest(void);
short scpi_spe_transient_voltage_positive(void);
short scpi_spe_transient_voltage_positive_quest(void);

//瞬态电流
short scpi_spe_transient_current_fashion(void);
short scpi_spe_transient_current_fashion_quest(void);
short scpi_spe_transient_current_mlevel(void);
short scpi_spe_transient_current_mlevel_quest(void);
short scpi_spe_transient_current_mwidth(void);
short scpi_spe_transient_current_mwidth_quest(void);
short scpi_spe_transient_current_tlevel(void);
short scpi_spe_transient_current_tlevel_quest(void);
short scpi_spe_transient_current_twidth(void);
short scpi_spe_transient_current_twidth_quest(void);
short scpi_spe_transient_current_negative(void);
short scpi_spe_transient_current_negative_quest(void);
short scpi_spe_transient_current_positive(void);
short scpi_spe_transient_current_positive_quest(void);



//瞬态功率
short scpi_spe_transient_power_fashion(void);
short scpi_spe_transient_power_fashion_quest(void);
short scpi_spe_transient_power_mlevel(void);
short scpi_spe_transient_power_mlevel_quest(void);
short scpi_spe_transient_power_mwidth(void);
short scpi_spe_transient_power_mwidth_quest(void);
short scpi_spe_transient_power_tlevel(void);
short scpi_spe_transient_power_tlevel_quest(void);
short scpi_spe_transient_power_twidth(void);
short scpi_spe_transient_power_twidth_quest(void);

//瞬态阻值
short scpi_spe_transient_resistance_fashion(void);
short scpi_spe_transient_resistance_fashion_quest(void);
short scpi_spe_transient_resistance_mlevel(void);
short scpi_spe_transient_resistance_mlevel_quest(void);
short scpi_spe_transient_resistance_mwidth(void);
short scpi_spe_transient_resistance_mwidth_quest(void);
short scpi_spe_transient_resistance_tlevel(void);
short scpi_spe_transient_resistance_tlevel_quest(void);
short scpi_spe_transient_resistance_twidth(void);
short scpi_spe_transient_resistance_twidth_quest(void);
short scpi_spe_transient_resistance_positive(void);
short scpi_spe_transient_resistance_positive_quest(void);
short scpi_spe_transient_resistance_negative(void);
short scpi_spe_transient_resistance_negative_quest(void);


//编辑序列
short scpi_spe_seq_edit_file_number(void);
short scpi_spe_seq_edit_file_number_quest(void);
short scpi_spe_seq_edit_file_length(void);
short scpi_spe_seq_edit_file_length_quest(void);
short scpi_spe_seq_edit_cycle(void);
short scpi_spe_seq_edit_cycle_quest(void);
short scpi_spe_seq_edit_link(void);
short scpi_spe_seq_edit_link_quest(void);
short scpi_spe_seq_edit_step(void);
short scpi_spe_seq_edit_step_quest(void);
short scpi_spe_seq_edit_level(void);
short scpi_spe_seq_edit_level_quest(void);
short scpi_spe_seq_edit_width(void);
short scpi_spe_seq_edit_width_quest(void);
	
short scpi_spe_seq_edit_slew(void);
short scpi_spe_seq_edit_slew_quest(void);
short scpi_spe_seq_edit_mode(void);
short scpi_spe_seq_edit_mode_quest(void);

short scpi_spe_seq_edit_save(void);


//运行序列
short scpi_spe_seq_test_file(void);
short scpi_spe_seq_test_file_quest(void);
short scpi_spe_seq_status_step_quest(void);
short scpi_spe_seq_status_circle_quest(void);


//编辑自动测试文件
short scpi_spe_auto_edit_file_number(void);
short scpi_spe_auto_edit_file_number_quest(void);
short scpi_spe_auto_edit_file_length(void);
short scpi_spe_auto_edit_file_length_quest(void);
short scpi_spe_auto_edit_step(void);
short scpi_spe_auto_edit_step_quest(void);
short scpi_spe_auto_edit_mode(void);
short scpi_spe_auto_edit_mode_quest(void);
short scpi_spe_auto_edit_level(void);
short scpi_spe_auto_edit_level_quest(void);
short scpi_spe_auto_edit_short(void);
short scpi_spe_auto_edit_short_quest(void);
short scpi_spe_auto_edit_rbwhat(void);
short scpi_spe_auto_edit_rbwhat_quest(void);
short scpi_spe_auto_edit_llimit(void);
short scpi_spe_auto_edit_llimit_quest(void);
short scpi_spe_auto_edit_ulimit(void);
short scpi_spe_auto_edit_ulimit_quest(void);
short scpi_spe_auto_edit_delay(void);
short scpi_spe_auto_edit_delay_quest(void);
short scpi_spe_auto_edit_save(void);
short scpi_spe_auto_edit_range(void);
short scpi_spe_auto_edit_range_quest(void);

//运行自动测试文件
short scpi_spe_auto_test_file(void);
short scpi_spe_auto_test_file_quest(void);
short scpi_spe_auto_test_result_quest(void);
short scpi_spe_auto_status_step_quest(void);
short scpi_spe_auto_test_query_step_quest(void);
short scpi_spe_auto_voltage_connect(void);
short scpi_spe_auto_voltage_connect_quest(void);
short scpi_spe_auto_voltage_disconnect(void);
short scpi_spe_auto_voltage_disconnect_quest(void);

//电池内阻测试
short scpi_spe_bresistance_current(void);
short scpi_spe_bresistance_current_quest(void);
short scpi_spe_bresistance_range(void);
short scpi_spe_bresistance_range_quest(void);
short scpi_spe_bresistance_delay(void);
short scpi_spe_bresistance_delay_quest(void);
short scpi_spe_bresistance_llimit(void);
short scpi_spe_bresistance_llimit_quest(void);
short scpi_spe_bresistance_ulimit(void);
short scpi_spe_bresistance_ulimit_quest(void);
short scpi_spe_bresistance_result_quest(void);

//电池容量测试
short scpi_spe_bcapacitance_current(void);
short scpi_spe_bcapacitance_current_quest(void);
short scpi_spe_bcapacitance_range(void);
short scpi_spe_bcapacitance_range_quest(void);
short scpi_spe_bcapacitance_ovoltage(void);
short scpi_spe_bcapacitance_ovoltage_quest(void);
short scpi_spe_bcapacitance_result_quest(void);
short scpi_spe_bcapacitance_result_clear(void);


//OCP测试
short scpi_spe_ocp_start_voltage(void);
short scpi_spe_ocp_start_voltage_quest(void);
short scpi_spe_ocp_start_delay(void);
short scpi_spe_ocp_start_delay_quest(void);
short scpi_spe_ocp_original_current(void);
short scpi_spe_ocp_original_current_quest(void);
short scpi_spe_ocp_step_current(void);
short scpi_spe_ocp_step_current_quest(void);
short scpi_spe_ocp_step_dwell(void);
short scpi_spe_ocp_step_dwell_quest(void);
short scpi_spe_ocp_end_current(void);
short scpi_spe_ocp_end_current_quest(void);
short scpi_spe_ocp_end_voltage(void);
short scpi_spe_ocp_end_voltage_quest(void);
short scpi_spe_ocp_trip_upper(void);
short scpi_spe_ocp_trip_upper_quest(void);
short scpi_spe_ocp_trip_lower(void);
short scpi_spe_ocp_trip_lower_quest(void);
short scpi_spe_ocp_status_result_quest(void);
short scpi_spe_ocp_range(void);
short scpi_spe_ocp_range_quest(void);



//OPP测试	
short scpi_spe_opp_start_voltage(void);
short scpi_spe_opp_start_voltage_quest(void);
short scpi_spe_opp_start_delay(void);
short scpi_spe_opp_start_delay_quest(void);
short scpi_spe_opp_original_power(void);
short scpi_spe_opp_original_power_quest(void);
short scpi_spe_opp_step_power(void);
short scpi_spe_opp_step_power_quest(void);
short scpi_spe_opp_step_dwell(void);
short scpi_spe_opp_step_dwell_quest(void);
short scpi_spe_opp_end_power(void);
short scpi_spe_opp_end_power_quest(void);
short scpi_spe_opp_end_voltage(void);
short scpi_spe_opp_end_voltage_quest(void);
short scpi_spe_opp_trip_upper(void);
short scpi_spe_opp_trip_upper_quest(void);
short scpi_spe_opp_trip_lower(void);
short scpi_spe_opp_trip_lower_quest(void);
short scpi_spe_opp_status_result_quest(void);	
	
//	DISCharge
short scpi_spe_discharge_test_mode(void);
short scpi_spe_discharge_test_mode_quest(void);
short scpi_spe_discharge_test_current(void);
short scpi_spe_discharge_test_current_quest(void);
short scpi_spe_discharge_test_resistance(void);
short scpi_spe_discharge_test_resistance_quest(void);
short scpi_spe_discharge_test_power(void);
short scpi_spe_discharge_test_power_quest(void);
short scpi_spe_discharge_end_voltage(void);
short scpi_spe_discharge_end_voltage_quest(void);
short scpi_spe_discharge_end_capacity(void);
short scpi_spe_discharge_end_capacity_quest(void);
short scpi_spe_discharge_end_time(void);
short scpi_spe_discharge_end_time_quest(void);
short scpi_spe_discharge_result_quest(void);
short scpi_spe_discharge_status_capacity_quest(void);
short scpi_spe_discharge_status_energy_quest(void);
short scpi_spe_discharge_status_time_quest(void);	



//EFFECT
short scpi_spe_effect_current_min(void);
short scpi_spe_effect_current_min_quest(void);
short scpi_spe_effect_current_max(void);
short scpi_spe_effect_current_max_quest(void);
short scpi_spe_effect_current_normal(void);
short scpi_spe_effect_current_normal_quest(void);
short scpi_spe_effect_test_dewll(void);
short scpi_spe_effect_test_dewll_quest(void);
short scpi_spe_effect_status_result_quest(void);


//ESR
short scpi_spe_esr_range(void);
short scpi_spe_esr_range_quest(void);
short scpi_spe_esr_test_current_mlevel(void);	
short scpi_spe_esr_test_current_mlevel_quest(void);	
short scpi_spe_esr_test_current_mwidth(void);	
short scpi_spe_esr_test_current_mwidth_quest(void);
short scpi_spe_esr_test_current_tlevel(void);	
short scpi_spe_esr_test_current_tlevel_quest(void);	
short scpi_spe_esr_test_current_twidth(void);	
short scpi_spe_esr_test_current_twidth_quest(void);	
short scpi_spe_esr_trip_upper(void);
short scpi_spe_esr_trip_upper_quest(void);
short scpi_spe_esr_trip_lower(void);
short scpi_spe_esr_trip_lower_quest(void);
short scpi_spe_esr_status_result_quest(void);


//LED
short scpi_spe_led_test_voltage(void);	
short scpi_spe_led_test_voltage_quest(void);
short scpi_spe_led_test_current(void);
short scpi_spe_led_test_current_quest(void);
short scpi_spe_led_test_coefficient(void);
short scpi_spe_led_test_coefficient_quest(void);

//WAVE
short scpi_spe_wave_crest(void);
short scpi_spe_wave_crest_quest(void);
short scpi_spe_wave_trough(void);
short scpi_spe_wave_trough_quest(void);
short scpi_spe_wave_frequency(void);
short scpi_spe_wave_frequency_quest(void);

//CZ
short scpi_spe_cz_parcap(void);
short scpi_spe_cz_parcap_quest(void);
short scpi_spe_cz_parres(void);
short scpi_spe_cz_parres_quest(void);
short scpi_spe_cz_serinduct(void);
short scpi_spe_cz_serinduct_quest(void);
short scpi_spe_cz_serres(void);
short scpi_spe_cz_serres_quest(void);

//SWEEP
short scpi_spe_sweep_current_l1(void);	
short scpi_spe_sweep_current_l1_quest(void);
short scpi_spe_sweep_current_l2(void);
short scpi_spe_sweep_current_l2_quest(void);
short scpi_spe_sweep_current_rise(void);
short scpi_spe_sweep_current_rise_quest(void);
short scpi_spe_sweep_current_fall(void);	
short scpi_spe_sweep_current_fall_quest(void);
short scpi_spe_sweep_frequency_start(void);
short scpi_spe_sweep_frequency_start_quest(void);
short scpi_spe_sweep_frequency_end(void);
short scpi_spe_sweep_frequency_end_quest(void);
short scpi_spe_sweep_frequency_step(void);	
short scpi_spe_sweep_frequency_step_quest(void);
short scpi_spe_sweep_wave_duty(void);
short scpi_spe_sweep_wave_duty_quest(void);
short scpi_spe_sweep_wave_dweel(void);
short scpi_spe_sweep_wave_dweel_quest(void);               
short scpi_spe_sweep_status_result_quest(void);



//COMBINE
short scpi_spe_combine_mode(void);
short scpi_spe_combine_mode_quest(void);
short scpi_spe_combine_cvcc_voltage(void);
short scpi_spe_combine_cvcc_voltage_quest(void);
short scpi_spe_combine_cvcc_current(void);	
short scpi_spe_combine_cvcc_current_quest(void);
short scpi_spe_combine_crcc_current(void);
short scpi_spe_combine_crcc_current_quest(void);
short scpi_spe_combine_crcc_resistance(void);
short scpi_spe_combine_crcc_resistance_quest(void);               
short scpi_spe_combine_cpcc_current(void);
short scpi_spe_combine_cpcc_current_quest(void);
short scpi_spe_combine_cpcc_power(void);               
short scpi_spe_combine_cpcc_power_quest(void);



//SCPI状态寄存器

void InitStatusVariable(void);	
short scpi_spe_status_questionable_condition_quest(void);	
short scpi_spe_status_questionable_enable(void);
short scpi_spe_status_questionable_enable_quest(void);
short scpi_spe_status_questionable_event_quest(void);	
short scpi_spe_status_channel_condition_quest(void);
short scpi_spe_status_channel_event_quest(void);
short scpi_spe_status_channel_enable(void);
short scpi_spe_status_channel_enable_quest(void);
short StatusChannelPtransition(void);               
short StatusChannelPtransitionAsk(void);
short StatusChannelNtransition(void);
short StatusChannelNtransitionAsk(void);               

//触发
short scpi_spe_trigger_imm(void);
short scpi_spe_trigger_source(void);
short scpi_spe_trigger_source_quest(void);

short scpi_spe_query_calibrate_quest(void);

#endif