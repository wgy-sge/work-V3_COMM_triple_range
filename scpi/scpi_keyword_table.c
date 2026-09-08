/************************************************************************************************
*																								*
*	File Name	: scpi_keyword_table.h															*
*	Author		: R&D Center,Shenzhen Zidetech Co. Ltd.										*
*	Version		: V1.0																			*
*	Date		: 10/12/2010																	*
*	Description	: This file contains the definition of the array which as the keywords table.	*
*																								*
************************************************************************************************/

#include "include.h"
#include "scpi_parser.h"

/************************************************************************************************
*																								*
*							scpi common command keyword table									*
*																								*
************************************************************************************************/

const scpi_common_keyword scpi_common_command_table[] = 
{
	// Index 0 to 4
	{"*cls",  0,  1,  scpi_com_cls},    
	{"*ese",  1,  2,  scpi_com_ese},
	{"*ese?", 0,  3,  scpi_com_ese_quest},
	{"*esr?", 0,  4,  scpi_com_esr_quest},
	{"*idn?", 0,  5,  scpi_com_idn_quest},
	// Index 5 to 9
	{"*opc",  0,  6,  scpi_com_opc},
	{"*opc?", 0,  7,  scpi_com_opc_quest},
	{"*psc",  1,  8,  scpi_com_psc},
	{"*psc?", 0,  9,  scpi_com_psc_quest},
	{"*rst",  0, 10,  scpi_com_rst},
	// Index 10 to 14
	{"*sre",  1, 11,  scpi_com_sre},
	{"*sre?", 0, 12,  scpi_com_sre_quest}, 
	{"*stb?", 0, 13,  scpi_com_stb_quest},
	{"*sav",  1, 14,  scpi_com_sav},
	// Index 14 to 19
	{"*rcl",  1, 15,  scpi_com_rcl},
	{"*tst?", 0, 16,  scpi_com_tst_quest},
	{"*trg",  0, 17,  scpi_com_trg},
	{"*wai",  0, -1,  scpi_com_wai},	
};



/************************************************************************************************
*																								*
*								scpi special keyword table										*
*																								*
************************************************************************************************/

const scpi_special_keyword scpi_special_command_table[] =
{
	//ROOT
	{"root","root",calibrate,-1,TRUE,0,0,NULL,NULL},
	//CALibrate
	{"calibrate","cal",calibrate_state,input,FALSE,0,0,NULL,NULL},
	//CALibrate[:STATe]
	{"state","stat",-1,calibrate_point,TRUE,-1,0,scpi_spe_cal_state,scpi_spe_cal_state_quest},
	//CALibrate:POINt
	{"point","poin",-1,calibrate_level,FALSE,1,0,scpi_spe_cal_point,scpi_spe_cal_point_quest},
	//CALibrate:LEVel
	{"level","lev",-1,calibrate_data,FALSE,1,0,scpi_spe_cal_level,scpi_spe_cal_level_quest},
	//CALibrate:DATA
	{"data","data",-1,calibrate_save,FALSE,1,0,scpi_spe_cal_data,NULL},
	//CALibrate:SAVE
	{"save","save",-1,calibrate_make,FALSE,0,0,scpi_spe_cal_save,NULL},	

	//CALibrate:MAKE
	{"make","make",-1,calibrate_model,FALSE,1,0,scpi_spe_cal_make,scpi_spe_cal_make_quest},
	//CALibrate:MODel
	{"model","mod",-1,calibrate_serial,FALSE,1,0,scpi_spe_cal_model,scpi_spe_cal_model_quest},
	//CALibrate:SERial
	{"serial","ser",-1,calibrate_date,FALSE,1,0,scpi_spe_cal_serial,scpi_spe_cal_serial_quest},
	//CALibrate:DATE
	{"date","date",-1,calibrate_alter,FALSE,1,0,scpi_spe_cal_date,scpi_spe_cal_date_quest},
	//CALibrate:ALTer
	{"alter","alt",-1,calibrate_reg,FALSE,1,0,scpi_spe_cal_alter,scpi_spe_cal_alter_quest},

	//CALibrate:REGister
	{"register","reg",-1,calibrate_bound,FALSE,2,2,scpi_spe_cal_register,scpi_spe_cal_register_quest},

	//CALibrate:BOUNd
	{"bound","boun",-1,-1,FALSE,0,1,NULL,scpi_spe_cal_bound_quest},
	
	//scpi_function
	//FUNCtion
	//{"function","func", -1, input, FALSE, 1, 0,scpi_spe_input_output_function,scpi_spe_input_output_function_quest},

	//INPut
	{"input","inp",input_state,output,FALSE,0,0,NULL,NULL},
	//OUTPut
	{"output","outp",input_state,SYSTEM,FALSE,0,0,NULL,NULL},
	//INPut|OUTPut[:STATe]
	{"state","stat",-1,input_function,TRUE,1,0, scpi_spe_input_output_state, scpi_spe_input_output_state_quest},
	//INPut|OUTPut:FUNCtion
	{"function","func",-1,input_short,FALSE,1,0,scpi_spe_input_output_function,scpi_spe_input_output_function_quest},
	//INPut|OUTPut:SHORt
	{"short","shor",-1,input_von,FALSE,1,0,scpi_spe_input_output_short,scpi_spe_input_output_short_quest},
	//INPut|OUTPut:VON
	{"von","von",input_von_level,input_voff,FALSE,0,0,NULL,NULL},  
	//INPut|OUTPut:VON[:LEVel]
	{"level","lev",-1,-1,TRUE,1,0,scpi_spe_input_output_von_level, scpi_spe_input_output_von_level_quest},
	//INPut|OUTPut:VOFF
	{"voff","voff",input_voff_level,input_timer,FALSE,0,0,NULL,NULL},
	//INPut|OUTPut:VOFF[:LEVel]
	{"level","lev",-1,-1,TRUE,1,0,scpi_spe_input_output_voff_level, scpi_spe_input_output_voff_level_quest},
	//INPut|OUTPut:TIMer
	{"timer","tim",input_timer_level,input_protection,FALSE,0,0,NULL,NULL},
	//INPut|OUTPut:TIMer:LEVel
	{"level","lev",-1,-1,TRUE,1,0,scpi_spe_input_output_timer_level, scpi_spe_input_output_timer_level_quest},

	//INPut|OUTPut:PROTection
	{"protection","prot",input_protection_current,-1,FALSE,0,0,NULL,NULL},
	//INPut|OUTPut:PROTection:CURRent
	{"current","curr",input_protection_current_level,input_protection_voltage,FALSE,0,0,NULL, NULL},
	//INPut|OUTPut:PROTection:CURRent[:LEVel]
	{"level","lev",-1,-1,TRUE,1,0,scpi_spe_input_output_prot_current, scpi_spe_input_output_prot_current_quest},
	//INPut|OUTPut:PROTection:VOLTage
	{"voltage","volt",input_protection_voltage_level,input_protection_power,FALSE,0,0,NULL, NULL},
	//INPut|OUTPut:PROTection:VOLTage[:LEVel]
	{"level","lev",-1,-1,TRUE,1,0,scpi_spe_input_output_prot_volt, scpi_spe_input_output_prot_volt_quest},
	//INPut|OUTPut:PROTection:POWer
	{"power","pow",input_protection_power_level,input_protection_clear,FALSE,0,0,NULL, NULL},
	//INPut|OUTPut:PROTection:POWer[:LEVel]
	{"level","lev",-1,-1,TRUE,1,0,scpi_spe_input_output_prot_power, scpi_spe_input_output_prot_power_quest},
	
	//INPut|OUTPut:PROTection:CLEar
	{"clear","cle",-1,-1,TRUE,0,0,scpi_spe_input_output_prot_clear, NULL},


	//SYSTem
	{"system","syst",system_sense,configure,FALSE,0,0,NULL,NULL},
	//SYSTem:SENSe
	{"sense","sens",system_sense_state,system_response,FALSE,0,0,NULL,NULL},
	//SYSTem:SENSe[:STATe]
	{"state","stat",-1,-1,TRUE,1,0,scpi_spe_syst_sense_state,scpi_spe_syst_sense_state_quest},
	//SYSTem:RESPonse
	{"response","resp",system_response_voltage,system_error,FALSE,0,0,NULL,NULL},
	//SYSTem:RESPonse:VOLTage
	{"voltage","volt",-1,-1,FALSE,1,0,scpi_spe_syst_response_volt,scpi_spe_syst_response_volt_quest},	
	//SYSTem:ERRor
	{"error","err",-1,system_version,FALSE,0,0,NULL,scpi_spe_syst_error_quest},
	//SYSTem:VERSion
	{"version","vers",-1,system_local,FALSE,0,0,NULL,scpi_spe_syst_version_quest},
	//SYSTem:LOCal
	{"local","loc",-1,system_clear,FALSE,0,0,scpi_spe_syst_local,NULL},
	//SYSTem:CLEar
	{"clear","cle",system_clear_charge,system_parallel,FALSE,0,0,NULL,NULL},
	//SYSTem:CLEar:CHARge
	{"charge","char",-1,system_clear_time,FALSE,0,0,scpi_spe_syst_clear_charge,NULL},
	//SYSTem:CLEar:TIME
	{"time","time",-1,system_clear_energy,FALSE,0,0,scpi_spe_syst_clear_time,NULL},
	//SYSTem:CLEar:ENERgy
	{"energy","ener",-1,-1,FALSE,0,0,scpi_spe_syst_clear_energy,NULL},

	//SYSTem:PARallel,
	{"parallel","par",system_parallel_number,system_rwlock,FALSE,0,0,NULL,NULL},
	//SYSTem:PARallel:NUMBer,
	{"number","numb",-1,system_parallel_slavers,TRUE,1,0,scpi_spe_syst_parallel_numb,scpi_spe_syst_parallel_numb_quest},
	//SYSTem:PARallel:SLAVers,
	{"slavers","slav",-1,system_parallel_state,FALSE,1,0,scpi_spe_syst_parallel_slavers,scpi_spe_syst_parallel_slavers_quest},
	//SYSTem:PARallel:STATe,
	{"state","stat",-1,-1,FALSE,1,0,scpi_spe_syst_parallel_state,scpi_spe_syst_parallel_state_quest},
	//SYSTem:RWLock,
	{"rwlock","rwl",-1,system_battcv,TRUE,1,0,scpi_spe_syst_rwlock,NULL},		
	//SYSTem:BATTcv,
	{"battcv","batt",system_battcv_state,-1,FALSE,0,0,NULL,NULL},
	//SYSTem:BATTcv[:STATe],
	{"state","stat",-1,-1,TRUE,1,0,scpi_spe_syst_battcv_state,scpi_spe_syst_battcv_state_quest},


	//CONFigure
	{"configure","conf",configure_voltage,measure,FALSE,0,0,NULL,NULL},
	//CONFigure:VOLTage
	{"voltage","volt",configure_voltage_sense,configure_current,FALSE,0,0,NULL,NULL},	
	//CONFigure:VOLTage:SENSe
	{"sense","sens",-1,configure_voltage_response,FALSE,1,0,scpi_spe_conf_volt_sense,scpi_spe_conf_volt_sense_quest},	
	//CONFigure:VOLTage:RESPonse
	{"response","resp",-1,configure_voltage_on,FALSE,1,0,scpi_spe_syst_response_volt,scpi_spe_syst_response_volt_quest},
	//CONFigure:VOLTage:ON
	{"on","on",-1,configure_voltage_off,FALSE,1,0,scpi_spe_conf_volt_on,scpi_spe_conf_volt_on_quest},
	//CONFigure:VOLTage:OFF
	{"off","off",-1,configure_voltage_upper,FALSE,1,0,scpi_spe_conf_volt_off,scpi_spe_conf_volt_off_quest},
	//CONFigure:VOLTage:UPPer
	{"upper","upp",-1,configure_voltage_lower,FALSE,1,0,scpi_spe_conf_volt_upper,scpi_spe_conf_volt_upper_quest},
	//CONFigure:VOLTage:LOWer
	{"lower","low",-1,-1,FALSE,1,0,scpi_spe_conf_volt_lower,scpi_spe_conf_volt_lower_quest},
	//CONFigure:CURRent
	{"current","curr",configure_current_upper,configure_resistance,FALSE,0,0,NULL,NULL},
	//CONFigure:CURRent:UPPer
	{"upper","upp",-1,configure_current_lower,FALSE,1,0,scpi_spe_conf_current_upper,scpi_spe_conf_current_upper_quest},
	//CONFigure:CURRent:LOWer
	{"lower","low",-1,-1,FALSE,1,0,scpi_spe_conf_current_lower,scpi_spe_conf_current_lower_quest},
	//CONFigure:RESistance
	{"resistance","res",configure_resistance_upper,configure_power,FALSE,0,0,NULL,NULL},
	//CONFigure:RESistance:UPPer
	{"upper","upp",-1,configure_resistance_lower,FALSE,1,0,scpi_spe_conf_resistance_upper,scpi_spe_conf_resistance_upper_quest},
	//CONFigure:RESistance:LOWer
	{"lower","low",-1,-1,FALSE,1,0,scpi_spe_conf_resistance_lower,scpi_spe_conf_resistance_lower_quest},
	//CONFigure:POWer
	{"power","pow",configure_power_upper,configure_protect,FALSE,0,0,NULL,NULL},
	//CONFigure:POWer:UPPer
	{"upper","upp",-1,configure_power_lower,FALSE,1,0,scpi_spe_conf_power_upper,scpi_spe_conf_power_upper_quest},
	//CONFigure:POWer:LOWer
	{"lower","low",-1,-1,FALSE,1,0,scpi_spe_conf_power_lower,scpi_spe_conf_power_lower_quest},	
	//CONFigure:PROTect
	{"protect","prot",configure_protect_ovp,configure_limit,FALSE,0,0,NULL,NULL},
	//CONFigure:PROTect:OVP
	{"ovp","ovp",-1,configure_protect_ocp,FALSE,1,0,scpi_spe_conf_protect_ovp,scpi_spe_conf_protect_ovp_quest},
	//CONFigure:PROTect:OCP
	{"ocp","ocp",-1,configure_protect_opp,FALSE,1,0,scpi_spe_conf_protect_ocp,scpi_spe_conf_protect_ocp_quest},
	//CONFigure:PROTect:OPP
	{"opp","opp",-1,configure_protect_lvp,FALSE,1,0,scpi_spe_conf_protect_opp,scpi_spe_conf_protect_opp_quest},
	//CONFigure:PROTect:LVP
	{"lvp","lvp",-1,configure_protect_time,FALSE,1,0,scpi_spe_conf_protect_lvp,scpi_spe_conf_protect_lvp_quest},
	//CONFigure:PROTect:TIME
	{"time","time",-1,-1,FALSE,1,0,scpi_spe_conf_protect_time,scpi_spe_conf_protect_time_quest},
	//CONFigure:LIMit
	{"limit","lim",configure_limit_current,configure_external,FALSE,0,0,NULL,NULL},
	//CONFigure:LIMit:CURRent
	{"current","curr",-1,-1,FALSE,1,0,scpi_spe_conf_limit_curr,scpi_spe_conf_limit_curr_quest},
	//CONFigure:EXTernal
	{"external","ext",configure_external_inhibit,-1,FALSE,0,0,NULL,NULL},
	//CONFigure:EXTernal:INHibit
	{"inhibit","inh",-1,-1,FALSE,1,0,scpi_spe_conf_external_inhibit,scpi_spe_conf_external_inhibit_quest},


	//MEASure
	{"measure","meas",measure_scalar,fetch,FALSE,0,0,NULL,NULL},
	//MEASure[:SCAlar]
	{"scalar","scal",measure_scalar_current,measure_charge,TRUE,0,0,NULL,NULL},
	//MEASure[:SCAlar]:CURRent
	{"current","curr",measure_scalar_current_dc,measure_scalar_voltage,FALSE,0,0,NULL,NULL},
	//MEASure[:SCAlar]:CURRent[:DC]
	{"dc","dc",-1,-1,TRUE,0,0,NULL,scpi_spe_measure_current_quest},
	//MEASure[:SCAlar]:VOLTage
	{"voltage","volt",measure_scalar_voltage_dc,measure_scalar_power,FALSE,0,0,NULL,NULL},
	//MEASure[:SCAlar]:VOLTage[:DC]
	{"dc","dc",-1,-1,TRUE,0,0,NULL,scpi_spe_measure_volt_quest},
	//MEASure[:SCAlar]:POWer
	{"power","pow",measure_scalar_power_dc,measure_scalar_resistance,FALSE,0,0,NULL,NULL},
	//MEASure[:SCAlar]:POWer[:DC]
	{"dc","dc",-1,-1,TRUE,0,0,NULL,scpi_spe_measure_power_quest},
	//MEASure[:SCAlar]:RESistance
	{"resistance","res",measure_scalar_resistance_dc,measure_scalar_temperature,FALSE,0,0,NULL,NULL},
	//MEASure[:SCAlar]:RESistance[:DC]
	{"dc","dc",-1,-1,TRUE,0,0,NULL,scpi_spe_measure_resis_quest},
	//MEASure[:SCAlar]:TEMPerature
	{"temperature","temp",-1,-1,FALSE,0,0,NULL,scpi_spe_measure_temperature_quest},

	//MEASure:CHARge
	{"charge","char",-1,measure_time,FALSE,0,0,NULL,scpi_spe_measure_charge_quest},
	//MEASure:TIME
	{"time","time",-1,measure_energy,FALSE,0,0,NULL,scpi_spe_measure_time_quest},	
	//MEASure:ENERgy
	{"energy","ener",-1,-1,FALSE,0,0,NULL,scpi_spe_measure_energy_quest},	

	//FETCh
	{"fetch","fetc",fetch_voltage,status,FALSE,0,0,NULL,NULL},
	//FETCh:VOLTage
	{"voltage","volt",-1,fetch_current,FALSE,0,0,NULL,scpi_spe_measure_volt_quest},
	//FETCh:CURRent
	{"current","curr",-1,fetch_status,FALSE,0,0,NULL,scpi_spe_measure_current_quest},
	//FETCh:STATus
	{"status","stat",-1,-1,FALSE,0,0,NULL,scpi_spe_fetch_status_quest},


	//STATus
	{"status","stat",status_questionable,trigger,FALSE,0,0,NULL,NULL},
	//STATus:QUEStionable
	{"questionable","ques",status_questionable_condition,status_channel,FALSE,0,0,NULL,NULL},
	//STATus:QUEStionable:CONDdition
	{"condition","cond",-1,status_questionable_enable,FALSE,0,0,NULL,scpi_spe_status_questionable_condition_quest},
	//STATus:QUEStionable:ENABle
	{"enable","enab",-1,status_questionable_event,FALSE,1,0,scpi_spe_status_questionable_enable, scpi_spe_status_questionable_enable_quest},
	//STATus:QUEStionable:EVENT
	{"event","even",-1,-1,TRUE,0,0,NULL, scpi_spe_status_questionable_event_quest},

	//STATus:CHANnel
	{"channel","chan",status_channel_condition,-1,FALSE,0,0,NULL,NULL},
	//STATus:CHANnel:CONDition
	{"condition","cond",-1,status_channel_event,FALSE,0,0,NULL,scpi_spe_status_channel_condition_quest},
	//STATus:CHANnel[:EVENt]
	{"event","even",-1,status_channel_enable,TRUE,0,0,NULL,scpi_spe_status_channel_event_quest},
	//STATus:CHANnel:ENAble
	{"enable","enab",-1,status_channel_ptransition,FALSE,1,0,scpi_spe_status_channel_enable,scpi_spe_status_channel_enable_quest},
	//SCKE_STAT_CHAN_PTR
	{ "ptransition","ptr",-1,status_channel_ntransition,FALSE,1,0,StatusChannelPtransition, StatusChannelPtransitionAsk },
	//SCKE_STAT_CHAN_NTR
	{ "ntransition","ntr",-1,-1,FALSE,1,0,StatusChannelNtransition, StatusChannelNtransitionAsk },

	//TRIGger
	{"trigger","trig",trigger_immediate,source,FALSE,0,0,NULL,NULL},
	//TRIGger[:IMMediate]
	{"immediate","imm",-1,trigger_source,TRUE,0,0,scpi_spe_trigger_imm,NULL},
	//TRIGger:SOURce
	{"source","sour",-1,-1,FALSE,1,0,scpi_spe_trigger_source,scpi_spe_trigger_source_quest},

	//[SOURce:]
	{"source","sour",source_function,source_sequence,TRUE,0,0,NULL,NULL},
	//[SOURce:]FUNCtion
	{"function","func",-1,source_voltage,FALSE,1,0,scpi_spe_function,scpi_spe_function_quest},
        
	//[SOURce:]VOLTage
	{"voltage","volt",source_voltage_level,source_current,FALSE,0,0,NULL,NULL},
	//[SOURce:]VOLTage[:LEVel]
	{"level","lev",-1,source_voltage_range,TRUE,1,0,scpi_spe_static_volt_level,scpi_spe_static_volt_level_quest},
	//[SOURce:]VOLTage:RANGe
	{"range","rang",-1,source_voltage_rise,FALSE,1,0,scpi_spe_static_volt_range,scpi_spe_static_volt_range_quest},
	//[SOURce:]VOLTage:RISE
	{"rise","rise",-1,source_voltage_fall,FALSE,1,0,scpi_spe_static_volt_rise,scpi_spe_static_volt_rise_quest},
	//[SOURce:]VOLTage:FALL
	{"fall","fall",-1,source_voltage_dynamic,FALSE,1,0,scpi_spe_static_volt_fall,scpi_spe_static_volt_fall_quest},
        
        //[SOURce:]VOLTage:DYNamic             CVD                                                                                 /////////////////////////////////////////////
	{"dynamic","dyn",source_voltage_dynamic_mode,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]VOLTage:DYNamic:MODE
	{"mode","mode",-1,source_voltage_dynamic_l1,FALSE,1,0,scpi_spe_transient_voltage_fashion, scpi_spe_transient_voltage_fashion_quest},
	//[SOURce:]VOLTage:DYNamic:L1
	{"l1","l1",-1,source_voltage_dynamic_l2,FALSE,1,0,scpi_spe_transient_voltage_mlevel,scpi_spe_transient_voltage_mlevel_quest},
	//[SOURce:]VOLTage:DYNamic:L2
	{"l2","l2",-1,source_voltage_dynamic_t1,FALSE,1,0,scpi_spe_transient_voltage_tlevel,scpi_spe_transient_voltage_tlevel_quest},
	//[SOURce:]VOLTage:DYNamic:T1
	{"t1","t1",-1,source_voltage_dynamic_t2,FALSE,1,0,scpi_spe_transient_voltage_mwidth,scpi_spe_transient_voltage_mwidth_quest},
	//[SOURce:]VOLTage:DYNamic:T2
	{"t2","t2",-1,source_voltage_dynamic_rise,FALSE,1,0,scpi_spe_transient_voltage_twidth,scpi_spe_transient_voltage_twidth_quest},
	//[SOURce:]VOLTage:DYNamic:RISE
	{"rise","rise",-1,source_voltage_dynamic_fall,FALSE,1,0,scpi_spe_transient_voltage_positive,scpi_spe_transient_voltage_positive_quest},
	//[SOURce:]VOLTage:DYNamic:FALL
	{"fall","fall",-1,-1,FALSE,1,0,scpi_spe_transient_voltage_negative,scpi_spe_transient_voltage_negative_quest},          /////////////////////////////////////////////

	//[SOURce:]CURRent
	{"current","curr",source_current_level,source_power,FALSE,0,0,NULL,NULL},
	//[SOURce:]CURRent[:LEVel]
	{"level","lev",-1,source_current_range,TRUE,1,0,scpi_spe_static_current_level,scpi_spe_static_current_level_quest},
	//[SOURce:]CURRent:RANGe
	{"range","rang",-1,source_current_rise,FALSE,1,0,scpi_spe_static_current_range,scpi_spe_static_current_range_quest},
	//[SOURce:]CURRent:RISE
	{"rise","rise",-1,source_current_fall,FALSE,1,0,scpi_spe_static_current_rise,scpi_spe_static_current_rise_quest},
	//[SOURce:]CURRent:FALL
	{"fall","fall",-1,source_current_dynamic,FALSE,1,0,scpi_spe_static_current_fall,scpi_spe_static_current_fall_quest},
        
	//[SOURce:]CURRent:DYNamic      CCD
	{"dynamic","dyn",source_current_dynamic_mode,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]CURRent:DYNamic:MODE
	{"mode","mode",-1,source_current_dynamic_l1,FALSE,1,0,scpi_spe_transient_current_fashion, scpi_spe_transient_current_fashion_quest},
	//[SOURce:]CURRent:DYNamic:L1
	{"l1","l1",-1,source_current_dynamic_l2,FALSE,1,0,scpi_spe_transient_current_mlevel,scpi_spe_transient_current_mlevel_quest},
	//[SOURce:]CURRent:DYNamic:L2
	{"l2","l2",-1,source_current_dynamic_t1,FALSE,1,0,scpi_spe_transient_current_tlevel,scpi_spe_transient_current_tlevel_quest},
	//[SOURce:]CURRent:DYNamic:T1
	{"t1","t1",-1,source_current_dynamic_t2,FALSE,1,0,scpi_spe_transient_current_mwidth,scpi_spe_transient_current_mwidth_quest},
	//[SOURce:]CURRent:DYNamic:T2
	{"t2","t2",-1,source_current_dynamic_rise,FALSE,1,0,scpi_spe_transient_current_twidth,scpi_spe_transient_current_twidth_quest},
	//[SOURce:]CURRent:DYNamic:RISE
	{"rise","rise",-1,source_current_dynamic_fall,FALSE,1,0,scpi_spe_transient_current_positive,scpi_spe_transient_current_positive_quest},
	//[SOURce:]CURRent:DYNamic:FALL
	{"fall","fall",-1,-1,FALSE,1,0,scpi_spe_transient_current_negative,scpi_spe_transient_current_negative_quest},
	
	//[SOURce:]POWer
	{"power","pow",source_power_level,source_resistance,FALSE,0,0,NULL,NULL},
	//[SOURce:]POWer[:LEVel]
	{"level","lev",-1,source_power_dynamic,TRUE,1,0,scpi_spe_static_power_level,scpi_spe_static_power_level_quest},
        
	//[SOURce:]POWer:DYNamic        CPD
	{"dynamic","dyn",source_power_dynamic_mode,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]POWer:DYNamic:MODE
	{"mode","mode",-1,source_power_dynamic_l1,FALSE,1,0,scpi_spe_transient_power_fashion,scpi_spe_transient_power_fashion_quest},
	//[SOURce:]POWer:DYNamic:L1
	{"l1","l1",-1,source_power_dynamic_l2,FALSE,1,0,scpi_spe_transient_power_mlevel,scpi_spe_transient_power_mlevel_quest},
	//[SOURce:]POWer:DYNamic:L2
	{"l2","l2",-1,source_power_dynamic_t1,FALSE,1,0,scpi_spe_transient_power_tlevel,scpi_spe_transient_power_tlevel_quest},
	//[SOURce:]POWer:DYNamic:T1
	{"t1","t1",-1,source_power_dynamic_t2,FALSE,1,0,scpi_spe_transient_power_mwidth,scpi_spe_transient_power_mwidth_quest},
	//[SOURce:]POWer:DYNamic:T2
	{"t2","t2",-1,-1,FALSE,1,0,scpi_spe_transient_power_twidth,scpi_spe_transient_power_twidth_quest},
	
	
	//[SOURce:]RESistance
	{"resistance","res",source_resistance_level,source_transient,FALSE,0,0,NULL,NULL},
	//[SOURce:]RESistance[:LEVel]
	{"level","lev",-1,source_resistance_range,TRUE,1,0,scpi_spe_static_resistance_level,scpi_spe_static_resistance_level_quest},
	//[SOURce:]RESistance:RANGe
	{"range","rang",-1,source_resistance_rise,FALSE,1,0,scpi_spe_static_resistance_range,scpi_spe_static_resistance_range_quest},
	//[SOURce:]RESistance:RISE
	{"rise","rise",-1,source_resistance_faLl,FALSE,1,0,scpi_spe_static_resistance_rise,scpi_spe_static_resistance_rise_quest},
	//[SOURce:]RESistance:FALL
	{"fall","fall",-1,source_resistance_dynamic,FALSE,1,0,scpi_spe_static_resistance_fall,scpi_spe_static_resistance_fall_quest},

	//[SOURce:]RESistance:DYNamic   CCR
	{"dynamic","dyn",source_resistance_dynamic_mode,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]RESistance:DYNamic:MODE
	{"mode","mode",-1,source_resistance_dynamic_l1,FALSE,1,0,scpi_spe_transient_resistance_fashion,scpi_spe_transient_resistance_fashion_quest},
	//[SOURce:]RESistance:DYNamic:L1
	{"l1","l1",-1,source_resistance_dynamic_l2,FALSE,1,0,scpi_spe_transient_resistance_mlevel,scpi_spe_transient_resistance_mlevel_quest},
	//[SOURce:]RESistance:DYNamic:L2
	{"l2","l2",-1,source_resistance_dynamic_t1,FALSE,1,0,scpi_spe_transient_resistance_tlevel,scpi_spe_transient_resistance_tlevel_quest},
	//[SOURce:]RESistance:DYNamic:T1
	{"t1","t1",-1,source_resistance_dynamic_t2,FALSE,1,0,scpi_spe_transient_resistance_mwidth,scpi_spe_transient_resistance_mwidth_quest},
	//[SOURce:]RESistance:DYNamic:T2
	{"t2","t2",-1,source_resistance_dynamic_rise,FALSE,1,0,scpi_spe_transient_resistance_twidth,scpi_spe_transient_resistance_twidth_quest},
	//[SOURce:]RESistance:DYNamic:RISE
	{"rise","rise",-1,source_resistance_dynamic_fall,FALSE,1,0,scpi_spe_transient_resistance_positive,scpi_spe_transient_resistance_positive_quest},
	//[SOURce:]RESistance:DYNamic:FALL
	{"fall","fall",-1,-1,FALSE,1,0,scpi_spe_transient_resistance_negative,scpi_spe_transient_resistance_negative_quest},


	//[SOURce:]TRANsient
	{"transient","tran",source_transient_current,source_sequence,FALSE,0,0,NULL,NULL},
	//[SOURce:]TRANsient:CURRent
	{"current","curr",source_transient_current_fashion,source_transient_power,FALSE,0,0,NULL,NULL},//----------------------------
	//[SOURce:]TRANsient:CURRent:MODE
	{"mode","mode",-1,source_transient_current_mlevel,FALSE,1,0,scpi_spe_transient_current_fashion,scpi_spe_transient_current_fashion_quest},
	//[SOURce:]TRANsient:CURRent:MLEVel
	{"mlevel","mlev",-1,source_transient_current_mwidth,FALSE,1,0,scpi_spe_transient_current_mlevel,scpi_spe_transient_current_mlevel_quest},
	//[SOURce:]TRANsient:CURRent:MWIDth
	{"mwidth","mwid",-1,source_transient_current_tlevel,FALSE,1,0,scpi_spe_transient_current_mwidth,scpi_spe_transient_current_mwidth_quest},
	//[SOURce:]TRANsient:CURRent:TLEVel
	{"tlevel","tlev",-1,source_transient_current_twidth,FALSE,1,0,scpi_spe_transient_current_tlevel,scpi_spe_transient_current_tlevel_quest},
	//[SOURce:]TRANsient:CURRent:TWIDth
	{"twidth","twid",-1,source_transient_current_raise,FALSE,1,0,scpi_spe_transient_current_twidth,scpi_spe_transient_current_twidth_quest},
	//[SOURce:]TRANsient:CURRent:RAISe
	{"raise","rais",-1,source_transient_current_fall,FALSE,1,0,scpi_spe_transient_current_positive,scpi_spe_transient_current_positive_quest},
	//[SOURce:]TRANsient:CURRent:FALL
	{"fall","fall",-1,-1,FALSE,1,0,scpi_spe_transient_current_negative,scpi_spe_transient_current_negative_quest},


	//[SOURce:]TRANsient:POWer
	{"power","pow",source_transient_power_fashion,source_transient_resistance,FALSE,0,0,NULL,NULL},//----------------------------
	//[SOURce:]TRANsient:POWer:MODE
	{"mode","mode",-1,source_transient_power_mlevel,FALSE,1,0,scpi_spe_transient_power_fashion,scpi_spe_transient_power_fashion_quest},
	//[SOURce:]TRANsient:POWer:MLEVel
	{"mlevel","mlev",-1,source_transient_power_mwidth,FALSE,1,0,scpi_spe_transient_power_mlevel,scpi_spe_transient_power_mlevel_quest},
	//[SOURce:]TRANsient:POWer:MWIDth
	{"mwidth","mwid",-1,source_transient_power_tlevel,FALSE,1,0,scpi_spe_transient_power_mwidth,scpi_spe_transient_power_mwidth_quest},
	//[SOURce:]TRANsient:POWer:TLEVel
	{"tlevel","tlev",-1,source_transient_power_twidth,FALSE,1,0,scpi_spe_transient_power_tlevel,scpi_spe_transient_power_tlevel_quest},
	//[SOURce:]TRANsient:POWer:TWIDth
	{"twidth","twid",-1,-1,FALSE,1,0,scpi_spe_transient_power_twidth,scpi_spe_transient_power_twidth_quest},

	//[SOURce:]TRANsient:RESistance
	{"resistance","res",source_transient_resistance_fashion,-1,FALSE,0,0,NULL,NULL},//----------------------------
	//[SOURce:]TRANsient:RESistance:MODE
	{"mode","mode",-1,source_transient_resistance_mlevel,FALSE,1,0,scpi_spe_transient_resistance_fashion,scpi_spe_transient_resistance_fashion_quest},
	//[SOURce:]TRANsient:RESistance:MLEVel
	{"mlevel","mlev",-1,source_transient_resistance_mwidth,FALSE,1,0,scpi_spe_transient_resistance_mlevel,scpi_spe_transient_resistance_mlevel_quest},
	//[SOURce:]TRANsient:RESistance:MWIDth
	{"mwidth","mwid",-1,source_transient_resistance_tlevel,FALSE,1,0,scpi_spe_transient_resistance_mwidth,scpi_spe_transient_resistance_mwidth_quest},
	//[SOURce:]TRANsient:RESistance:TLEVel
	{"tlevel","tlev",-1,source_transient_resistance_twidth,FALSE,1,0,scpi_spe_transient_resistance_tlevel,scpi_spe_transient_resistance_tlevel_quest},
	//[SOURce:]TRANsient:RESistance:TWIDth
	{"twidth","twid",-1,source_transient_resistance_raise,FALSE,1,0,scpi_spe_transient_resistance_twidth,scpi_spe_transient_resistance_twidth_quest},
	//[SOURce:]TRANsient:RESistance:RAISe
	{"raise","rais",-1,source_transient_resistance_fall,FALSE,1,0,scpi_spe_transient_resistance_positive,scpi_spe_transient_resistance_positive_quest},
	//[SOURce:]TRANsient:RESistance:FALL
	{"fall","fall",-1,-1,FALSE,1,0,scpi_spe_transient_resistance_negative,scpi_spe_transient_resistance_negative_quest},

	

	//[SOURce:]SEQuence//±‡º≠	
	{"sequence","seq",source_sequence_edit,source_auto,FALSE,0,0,NULL,NULL},
	//[SOURce:]SEQuence:[EDIT]
	{"edit","edit",source_sequence_edit_file,source_sequence_run, TRUE,0,0,NULL,NULL},
	//[SOURce:]SEQuence:[EDIT]:FILE
	{"file","file",source_sequence_edit_file_number,source_sequence_edit_step, FALSE,0,0,NULL,NULL},
	//[SOURce:]SEQuence:[EDIT]:FILE:NUMBer
	{"number","numb",-1,source_sequence_edit_file_length,FALSE,1,0,scpi_spe_seq_edit_file_number,scpi_spe_seq_edit_file_number_quest},
	//[SOURce:]SEQuence:[EDIT]:FILE:LENGth
	{"length","leng",-1,source_sequence_edit_file_cycle,FALSE,1,0,scpi_spe_seq_edit_file_length,scpi_spe_seq_edit_file_length_quest},
	//[SOURce:]SEQuence:[EDIT]:FILE:CYCLe
	{"cycle","cycl",-1,source_sequence_edit_file_link,FALSE,1,0,scpi_spe_seq_edit_cycle,scpi_spe_seq_edit_cycle_quest},
	//[SOURce:]SEQuence:[EDIT]:FILE:LINK
	{"link","link",-1,-1,FALSE,1,0,scpi_spe_seq_edit_link,scpi_spe_seq_edit_link_quest},
	//[SOURce:]SEQuence:[EDIT]:STEP
	{"step","step",-1,source_sequence_edit_level,FALSE,1,0,scpi_spe_seq_edit_step,scpi_spe_seq_edit_step_quest},	
	//[SOURce:]SEQuence:[EDIT]:MODE
	//{"mode","mode",-1,source_sequence_edit_slew,FALSE,1,0,scpi_spe_seq_edit_mode,scpi_spe_seq_edit_mode_quest},
	//[SOURce:]SEQuence:[EDIT]:SLEW
	//{"slew","slew",-1,source_sequence_edit_slew,FALSE,1,0,scpi_spe_seq_edit_slew,scpi_spe_seq_edit_slew_quest},

	//[SOURce:]SEQuence:[EDIT]:LEVel
	{"level","lev",-1,source_sequence_edit_dwell,FALSE,1,0,scpi_spe_seq_edit_level,scpi_spe_seq_edit_level_quest},	
	//[SOURce:]SEQuence:[EDIT]:DWELl
	{"delay","del",-1,source_sequence_edit_slew,FALSE,1,0,scpi_spe_seq_edit_width,scpi_spe_seq_edit_width_quest},

	//[SOURce:]SEQuence:[EDIT]:SLEW
	{"slew","slew",-1,source_sequence_edit_mode,FALSE,1,0,scpi_spe_seq_edit_slew,scpi_spe_seq_edit_slew_quest},
	//[SOURce:]SEQuence:[EDIT]:MODE
	{"mode","mode",-1,source_sequence_edit_save,FALSE,1,0,scpi_spe_seq_edit_mode,scpi_spe_seq_edit_mode_quest},
	
	//[SOURce:]SEQuence:[EDIT]:SAVE
	{"save","save",-1,-1,FALSE,0,0,scpi_spe_seq_edit_save,NULL},

	//[SOURce:]SEQuence:RUN//≤‚ ‘
	{"run","run",source_sequence_run_number, source_sequence_status,FALSE,0,0,NULL,NULL},
	//[SOURce:]SEQuence:RUN:NUMBer
	{"file","file",-1,-1,FALSE,1,0,scpi_spe_seq_test_file,scpi_spe_seq_test_file_quest},
	//[SOURce:]SEQuence:STATus
	{"status","stat",source_sequence_status_step,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]SEQuence:STATus:STEP
	{"step","step",-1,source_sequence_status_cycle,FALSE,0,0,NULL,scpi_spe_seq_status_step_quest},
	//[SOURce:]SEQuence:STATus:CYCLe
	{"cycle","cycl",-1,-1,FALSE,0,0,NULL,scpi_spe_seq_status_circle_quest},

	//[SOURce:]AUTO//±‡º≠
	{"auto","auto",source_auto_edit,source_ocp,FALSE,0,0,NULL,NULL},
	//[SOURce:]AUTo:[EDIT]
	{"edit","edit",source_auto_edit_file,source_auto_run,TRUE,0,0,NULL,NULL},
	//[SOURce:]AUTo:[EDIT]:FILE
	{"file","file",source_auto_edit_file_num,source_auto_edit_step,FALSE,0,0,NULL,NULL},
	//[SOURce:]AUTo:[EDIT]:FILE:NUMBer
	{"number","numb",-1,source_auto_edit_file_length,FALSE,1,0,scpi_spe_auto_edit_file_number,scpi_spe_auto_edit_file_number_quest},
	//[SOURce:]AUTo:[EDIT]:FILE:LENGth
	{"length","leng",-1,-1,FALSE,1,0,scpi_spe_auto_edit_file_length,scpi_spe_auto_edit_file_length_quest},
	//[SOURce:]AUTo:[EDIT]:STEP
	{"step","step",-1,source_auto_edit_mode,FALSE,1,0,scpi_spe_auto_edit_step,scpi_spe_auto_edit_step_quest},
	//[SOURce:]AUTo:EDIT:MODE
	{"mode","mode",-1,source_auto_edit_level,FALSE,1,0,scpi_spe_auto_edit_mode,scpi_spe_auto_edit_mode_quest},	
	//[SOURce:]AUTo:[EDIT]:LEVel
	{"level","lev",-1,source_auto_edit_short,FALSE,1,0,scpi_spe_auto_edit_level,scpi_spe_auto_edit_level_quest},
	//[SOURce:]AUTo:[EDIT]:SHORt
	{"short","shor",-1,source_auto_edit_rbwhat,FALSE,1,0,scpi_spe_auto_edit_short,scpi_spe_auto_edit_short_quest},
	//[SOURce:]AUTo:[EDIT]:CHECk
	{"rbwhat","rbwh",-1,source_auto_edit_llimit,FALSE,1,0,scpi_spe_auto_edit_rbwhat,scpi_spe_auto_edit_rbwhat_quest},
	//[SOURce:]AUTo:[EDIT]:LLIMit
	{"llimit","llim",-1,source_auto_edit_ulimit,FALSE,1,0,scpi_spe_auto_edit_llimit,scpi_spe_auto_edit_llimit_quest},
	//[SOURce:]AUTo:[EDIT]:ULIMit
	{"ulimit","ulim",-1,source_auto_edit_delay,FALSE,1,0,scpi_spe_auto_edit_ulimit,scpi_spe_auto_edit_ulimit_quest},
	//[SOURce:]AUTo:[EDIT]:DWELl
	{"delay","del",-1,source_auto_edit_save,FALSE,1,0,scpi_spe_auto_edit_delay,scpi_spe_auto_edit_delay_quest},
	//[SOURce:]AUTo:[EDIT]:SAVE
	{"save","save",-1,source_auto_edit_range,FALSE,0,0,scpi_spe_auto_edit_save,NULL},
	//[SOURce:]AUTo:[EDIT]:RANGe
	{"range","rang",-1,-1,FALSE,1,0,scpi_spe_auto_edit_range,scpi_spe_auto_edit_range_quest},

	//[SOURce:]AUTo:RUN//≤‚ ‘
	{"run","run",source_auto_run_file,source_auto_voltage,FALSE,0,0,NULL,NULL},
	//[SOURce:]AUTo:RUN:FILE
	{"file","file",-1,source_auto_run_status,FALSE,1,0,scpi_spe_auto_test_file,scpi_spe_auto_test_file_quest},
	//[SOURce:]AUTo:RUN:STATus
	{"status","stat",source_auto_run_status_step,source_auto_run_result,FALSE,0,0,NULL,NULL},
	//[SOURce:]AUTo:RUN:STATus[:STEP]
	{"step","step",-1,-1,TRUE,0,0,NULL,scpi_spe_auto_status_step_quest},
	//[SOURce:]AUTo:RUN:RESult
	{"result","res",source_auto_run_result_all,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]AUTo:RUN:RESult[:ALL]
	{"all","all",-1,source_auto_run_result_step,TRUE,0,0,NULL,scpi_spe_auto_test_result_quest},
	//[SOURce:]AUTo:RUN:RESult:STEP
	{"step","step",-1,-1,FALSE,0,1,NULL,scpi_spe_auto_test_query_step_quest},
	
	//[SOURce:]AUTo:VOLTage
	{"voltage","volt",source_auto_voltage_connect,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]AUTo:VOLTage:CONNect
	{"connect","conn",-1,source_auto_voltage_disconnect,FALSE,1,0,scpi_spe_auto_voltage_connect,scpi_spe_auto_voltage_connect_quest},
	//[SOURce:]AUTo:VOLTage:DISConnect
	{"disconnect","disc",-1,-1,FALSE,1,0,scpi_spe_auto_voltage_disconnect,scpi_spe_auto_voltage_disconnect_quest},


	//[SOURce:]OCP
	{"ocp","ocp",source_ocp_bvoltage,source_opp,FALSE,0,0,NULL,NULL},
	//[SOURce:]OCP:VOLTage
    {"bvoltage","bvol",-1,source_ocp_bdelay,FALSE,1,0,scpi_spe_ocp_start_voltage,scpi_spe_ocp_start_voltage_quest},
	//[SOURce:]OCP:BDELay
	{"bdelay","bdel",-1,source_ocp_bcurrent,FALSE,1,0,scpi_spe_ocp_start_delay,scpi_spe_ocp_start_delay_quest},
	//[SOURce:]OCP:BCURrent
	{"bcurrent","bcur",-1,source_ocp_scurrent,FALSE,1,0,scpi_spe_ocp_original_current,scpi_spe_ocp_original_current_quest},
	//[SOURce:]OCP:SCURrent
	{"scurrent","scur",-1,source_ocp_delay,FALSE,1,0,scpi_spe_ocp_step_current,scpi_spe_ocp_step_current_quest},
	//[SOURce:]OCP:STEP:DELay
	{"delay","del",-1,source_ocp_ecurrent,FALSE,1,0,scpi_spe_ocp_step_dwell,scpi_spe_ocp_step_dwell_quest},
	//[SOURce:]OCP:ECURRent
	{"ecurrent","ecur",-1,source_ocp_evoltage,FALSE,1,0,scpi_spe_ocp_end_current,scpi_spe_ocp_end_current_quest},
	//[SOURce:]OCP:EVOLTage
	{"evoltage","evol",-1,source_ocp_tupper,FALSE,1,0,scpi_spe_ocp_end_voltage,scpi_spe_ocp_end_voltage_quest},
		
	//[SOURce:]OCP:TUPPer
	{"tupper","tupp",-1,source_ocp_tlower,FALSE,1,0,scpi_spe_ocp_trip_upper,scpi_spe_ocp_trip_upper_quest},
	//[SOURce:]OCP:TLOWer
	{"tlower","tlow",-1,source_ocp_result,FALSE,1,0,scpi_spe_ocp_trip_lower,scpi_spe_ocp_trip_lower_quest},
	//[SOURce:]OCP:RESult
	{"result","res",-1,source_ocp_range,FALSE,1,0,NULL,scpi_spe_ocp_status_result_quest},
	//[SOURce:]OCP:RANGe
	{"range","rang",-1,-1,FALSE,1,0,scpi_spe_ocp_range,scpi_spe_ocp_range_quest},



	//[SOURce:]OPP
	{"opp","opp",source_opp_bvoltage,source_effect,FALSE,0,0,NULL,NULL},
	//[SOURce:]OPP:BVOLtage
	{"bvoltage","bvol",-1,source_opp_bdelay,FALSE,1,0,scpi_spe_opp_start_voltage,scpi_spe_opp_start_voltage_quest},
	//[SOURce:]OPP:BDELay
	{"bdelay","bdel",-1,source_opp_bpower,FALSE,1,0,scpi_spe_opp_start_delay,scpi_spe_opp_start_delay_quest},
	//[SOURce:]OPP:BPOWer
	{"bpower","bpow",-1,source_opp_spower,FALSE,1,0,scpi_spe_opp_original_power,scpi_spe_opp_original_power_quest},
	//[SOURce:]OPP:SPOWer
	{"spower","spow",-1,source_opp_sdelay,FALSE,1,0,scpi_spe_opp_step_power,scpi_spe_opp_step_power_quest},
	//[SOURce:]OPP:DELay
	{"delay","del",-1,source_opp_epower,FALSE,1,0,scpi_spe_opp_step_dwell,scpi_spe_opp_step_dwell_quest},
	//[SOURce:]OPP:EPOWer
	{"epower","epow",-1,source_opp_evoltage,FALSE,1,0,scpi_spe_opp_end_power,scpi_spe_opp_end_power_quest},
	//[SOURce:]OPP:END:EVOLTage
	{"evoltage","evol",-1,source_opp_tupper,FALSE,1,0,scpi_spe_opp_end_voltage,scpi_spe_opp_end_voltage_quest},
	//[SOURce:]OPP:TRIP:UPPer
	{"tupper","tupp",-1,source_opp_tlower,FALSE,1,0,scpi_spe_opp_trip_upper,scpi_spe_opp_trip_upper_quest},
	//[SOURce:]OPP:TRIP:LOWer
	{"tlower","tlow",-1,source_opp_result,FALSE,1,0,scpi_spe_opp_trip_lower,scpi_spe_opp_trip_lower_quest},	
	//[SOURce:]OPP:RESult
	{"result","res",-1,-1,FALSE,0,0,NULL,scpi_spe_opp_status_result_quest},	
	
	//[SOURce:]EFFect
	{"effect","eff",source_effect_current,source_led,FALSE,0,0,NULL,NULL},
	//[SOURce:]EFFect:CURRent
	{"current","curr",source_effect_current_min,source_effect_test,FALSE,0,0,NULL,NULL},
	//[SOURce:]EFFect:CURRent:MIN
	{"min","min",-1,source_effect_current_max,FALSE,1,0,scpi_spe_effect_current_min,scpi_spe_effect_current_min_quest},
	//[SOURce:]EFFect:CURRent:MAX
	{"max","max",-1,source_effect_current_normal,FALSE,1,0,scpi_spe_effect_current_max,scpi_spe_effect_current_max_quest},
	//[SOURce:]EFFect:CURRent:NORMal
	{"normal","norm",-1,-1,FALSE,1,0,scpi_spe_effect_current_normal,scpi_spe_effect_current_normal_quest},
	//EFFect:TEST
	{"test","test",source_effect_test_dewll,source_effect_status,FALSE,0,0,NULL,NULL},
	//[SOURce:]EFFect:TEST:DWELl
	{"dwell","dwel",-1,-1,FALSE,1,0,scpi_spe_effect_test_dewll,scpi_spe_effect_test_dewll_quest},
	//[SOURce:]EFFect:STATus
	{"status","stat",source_effect_status_result,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]EFFect:STATus:RESult
	{"result","res",-1,-1,FALSE,0,0,NULL,scpi_spe_effect_status_result_quest},	


	//[SOURce:]LED
	{"led","led",source_led_test,source_sweep,FALSE,0,0,NULL,NULL},
	//[SOURce:]LED:TEST
	{"test","test",source_led_test_voltage,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]LED:TEST:VOLTage
	{"voltage","volt",-1,source_led_test_current,FALSE,1,0,scpi_spe_led_test_voltage,scpi_spe_led_test_voltage_quest},
	//[SOURce:]LED:TEST:CURRent
	{"current","curr",-1,source_led_test_coefficient,FALSE,1,0,scpi_spe_led_test_current,scpi_spe_led_test_current_quest},
	//LED:TEST:COEFficient
	{"coefficient","coef",-1,-1,FALSE,1,0,scpi_spe_led_test_coefficient,scpi_spe_led_test_coefficient_quest},


	//[SOURce:]SWEEP
	{"sweep","swee",source_sweep_current,source_combine,FALSE,0,0,NULL,NULL},
	//[SOURce:]SWEEP:CURRent
	{"current","curr",source_sweep_current_l1,source_sweep_frequency,FALSE,0,0,NULL,NULL},
	//[SOURce:]SWEEP:CURRent:L1
	{"l1","l1",-1,source_sweep_current_l2,FALSE,1,0,scpi_spe_sweep_current_l1,scpi_spe_sweep_current_l1_quest},
	//[SOURce:]SWEEP:CURRent:L2
	{"l2","l2",-1,source_sweep_current_rise,FALSE,1,0,scpi_spe_sweep_current_l2,scpi_spe_sweep_current_l2_quest},
	//[SOURce:]SWEEP:CURRent:RISE
	{"rise","rise",-1,source_sweep_current_fall,FALSE,1,0,scpi_spe_sweep_current_rise,scpi_spe_sweep_current_rise_quest},
	//[SOURce:]SWEEP:CURRent:FALL
	{"fall","fall",-1,-1,FALSE,1,0,scpi_spe_sweep_current_fall,scpi_spe_sweep_current_fall_quest},
	//[SOURce:]SWEEP:FREQuency
	{"frequency","freq",source_sweep_frequency_start,source_sweep_wave,FALSE,0,0,NULL,NULL},
	//[SOURce:]SWEEP:FREQuency:STARt
	{"start","star",-1,source_sweep_frequency_end,FALSE,1,0,scpi_spe_sweep_frequency_start,scpi_spe_sweep_frequency_start_quest},
	//[SOURce:]SWEEP:FREQuency:END
	{"end","end",-1,source_sweep_frequency_step,FALSE,1,0,scpi_spe_sweep_frequency_end,scpi_spe_sweep_frequency_end_quest},
	//[SOURce:]SWEEP:FREQuency:STEP
	{"step","step",-1,-1,FALSE,1,0,scpi_spe_sweep_frequency_step,scpi_spe_sweep_frequency_step_quest},
	//[SOURce:]SWEEP:WAVE
	{"wave","wave",source_sweep_wave_duty,source_sweep_status,FALSE,0,0,NULL,NULL},
	//[SOURce:]SWEEP:WAVE:DUTY
	{"duty","duty",-1,source_sweep_wave_dweel,FALSE,1,0,scpi_spe_sweep_wave_duty,scpi_spe_sweep_wave_duty_quest},
	//[SOURce:]SWEEP:WAVE:DWEEl
	{"dweel","dwee",-1,-1,FALSE,1,0,scpi_spe_sweep_wave_dweel,scpi_spe_sweep_wave_dweel_quest},
	//[SOURce:]SWEEP:STATus
	{"status","stat",source_sweep_status_result,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]SWEEP:STATus:RESult
	{"result","res",-1,-1,FALSE,0,0,NULL,scpi_spe_sweep_status_result_quest},


	//[SOURce:]COMBine
	{"combine","comb",source_combine_mode,source_bcapacitance,FALSE,0,0,NULL,NULL},
	//[SOURce:]COMBine:MODE
	{"mode","mode",-1,source_combine_cvcc,FALSE,1,0,scpi_spe_combine_mode,scpi_spe_combine_mode_quest},
	//[SOURce:]COMBine:CVCC
	{"cvcc","cvcc",source_combine_cvcc_voltage,source_combine_crcc,FALSE,0,0,NULL,NULL},
	//[SOURce:]COMBine:CVCC:VOLTage
	{"voltage","volt",-1,source_combine_cvcc_current,FALSE,1,0,scpi_spe_combine_cvcc_voltage,scpi_spe_combine_cvcc_voltage_quest},
	//[SOURce:]COMBine:CVCC:CURRent
	{"current","curr",-1,-1,FALSE,1,0,scpi_spe_combine_cvcc_current,scpi_spe_combine_cvcc_current_quest},
	//[SOURce:]COMBine:CRCC
	{"crcc","crcc",source_combine_crcc_current,source_combine_cpcc,FALSE,0,0,NULL,NULL},
	//[SOURce:]COMBine:CRCC:CURRent
	{"current","curr",-1,source_combine_crcc_resistance,FALSE,1,0,scpi_spe_combine_crcc_current,scpi_spe_combine_crcc_current_quest},
	//[SOURce:]COMBine:CRCC:RESistance
	{"resistance","res",-1,-1,FALSE,1,0,scpi_spe_combine_crcc_resistance,scpi_spe_combine_crcc_resistance_quest},
	//[SOURce:]COMBine:CPCC
	{"cpcc","cpcc",source_combine_cpcc_current,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]COMBine:CPCC:CURRent
	{"current","curr",-1,source_combine_cpcc_power,FALSE,1,0,scpi_spe_combine_cpcc_current,scpi_spe_combine_cpcc_current_quest},
	//[SOURce:]COMBine:CPCC:POWer
	{"power","pow",-1,-1,FALSE,1,0,scpi_spe_combine_cpcc_power,scpi_spe_combine_cpcc_power_quest},



	//[SOURce:]BCAPacitance
	{"bcapacitance","bcap",source_bcapacitance_test,source_bresistance,FALSE,0,0,NULL,NULL},
	//[SOURce:]BCAPacitance:TEST
	{"test","test",source_bcapacitance_test_mode,source_bcapacitance_current,FALSE,0,0,NULL,NULL},
	//[SOURce:]BCAPacitance:TEST:MODE
	{"mode","mode",-1,-1,FALSE,1,0,scpi_spe_discharge_test_mode,scpi_spe_discharge_test_mode_quest},
	//[SOURce:]BCAPacitance:CURRent
	{"current","curr",-1,source_bcapacitance_dresistance,FALSE,1,0,scpi_spe_discharge_test_current,scpi_spe_discharge_test_current_quest},
	//[SOURce:]BCAPacitance:DRESistance
	{"dresistance","dres",-1,source_bcapacitancet_power,FALSE,1,0,scpi_spe_discharge_test_resistance,scpi_spe_discharge_test_resistance_quest},
	//[SOURce:]BCAPacitance:POWer
	{"power","pow",-1,source_bcapacitance_evoltage,FALSE,1,0,scpi_spe_discharge_test_power,scpi_spe_discharge_test_power_quest},
	//[SOURce:]BCAPacitance:EVOLtage
	{"evoltage","evol",-1,source_bcapacitance_ecapacity,FALSE,1,0,scpi_spe_discharge_end_voltage,scpi_spe_discharge_end_voltage_quest},
	//[SOURce:]BCAPacitance:ECAPacity
	{"ecapacity","ecap",-1,source_bcapacitance_etime,FALSE,1,0,scpi_spe_discharge_end_capacity,scpi_spe_discharge_end_capacity_quest},
	//[SOURce:]BCAPacitance:ETIMe
	{"etime","etim",-1,source_bcapacitance_result,FALSE,1,0,scpi_spe_discharge_end_time,scpi_spe_discharge_end_time_quest},
	//[SOURce:]BCAPacitance:RESult
	{"result","res",source_bcapacitance_result_query,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]BCAPacitance:RESult[:QUERy]
	{"query","quer",-1,source_bcapacitance_result_capacity,TRUE,0,0,NULL,scpi_spe_discharge_result_quest},
	//[SOURce:]BCAPacitance:RESult:CAPAcity
	{"capacity","cap",-1,source_bcapacitance_result_energy,FALSE,0,0,NULL,scpi_spe_discharge_status_capacity_quest},
	//[SOURce:]BCAPacitance:RESult:ENERgy
	{"energy","ener",-1,source_bcapacitance_result_time,FALSE,0,0,NULL,scpi_spe_discharge_status_energy_quest},
	//[SOURce:]BCAPacitance:RESult:TIME
	{"time","time",-1,-1,FALSE,0,0,NULL,scpi_spe_discharge_status_time_quest},
	
	//[SOURce:]BRESistance
	{"bresistance","bres",source_bresistance_range,source_wave,FALSE,0,0,NULL,NULL},
	//[SOURce:]BRESistance:RANGe
	{"range","rang",-1,source_bresistance_current,FALSE,1,0,scpi_spe_esr_range,scpi_spe_esr_range_quest},
	//[SOURce:]BRESistance:CURRent
	{"current","curr",source_bresistance_current_mlevel,source_bresistance_trip,FALSE,0,0,NULL,NULL},
	//[SOURce:]BRESistance:CURRent:MLEVel
	{"mlevel","mlev",-1,source_bresistance_current_mwidth,FALSE,1,0,scpi_spe_esr_test_current_mlevel,scpi_spe_esr_test_current_mlevel_quest},
	//[SOURce:]BRESistance:CURRent:MWIDth
	{"mwidth","mwid",-1,source_bresistance_current_tlevel,FALSE,1,0,scpi_spe_esr_test_current_mwidth,scpi_spe_esr_test_current_mwidth_quest},
	//[SOURce:]BRESistance:CURRent:TLEVel
	{"tlevel","tlev",-1,source_bresistance_current_twidth,FALSE,1,0,scpi_spe_esr_test_current_tlevel,scpi_spe_esr_test_current_tlevel_quest},
	//[SOURce:]BRESistance:CURRent:TWIDth
	{"twidth","twid",-1,-1,FALSE,1,0,scpi_spe_esr_test_current_twidth,scpi_spe_esr_test_current_twidth_quest},

	
	//[SOURce:]BRESistance:TRIP
	{"trip","trip",source_bresistance_trip_upper,source_bresistance_result,FALSE,0,0,NULL,NULL},	
	//[SOURce:]BRESistance:TRIP:UPPer
	{"upper","upp",-1,source_bresistance_trip_lower,FALSE,1,0,scpi_spe_esr_trip_upper,scpi_spe_esr_trip_upper_quest},
	//[SOURce:]BRESistance:TRIP:LOWer
	{"lower","low",-1,-1,FALSE,1,0,scpi_spe_esr_trip_lower,scpi_spe_esr_trip_lower_quest},	
	//[SOURce:]BRESistance:RESult
	{"result","res",-1,-1,FALSE,0,0,NULL,scpi_spe_esr_status_result_quest},


	//[SOURce:]WAVE
	{"wave","wave",source_wave_crest, source_cz,FALSE,0,0,NULL,NULL},
	//[SOURce:]WAVE:CRESt
	{"crest","cres",-1,source_wave_trough,FALSE,1,0,scpi_spe_wave_crest, scpi_spe_wave_crest_quest},
	//[SOURce:]WAVE:TROUgh
	{"trough","trou",-1,source_wave_frequency,FALSE,1,0, scpi_spe_wave_trough, scpi_spe_wave_trough_quest},
	//[SOURce:]WAVE:FREQuency
	{"frequency","freq",-1,-1,FALSE,1,0,scpi_spe_wave_frequency, scpi_spe_wave_frequency_quest},

        //[SOURce:]CZ
	{"cz","cz",source_cz_parcap, -1,FALSE,0,0,NULL,NULL},
        //[SOURce:]CZ:PARCap
	{"parcap","parc",-1,source_cz_parres,FALSE,1,0,scpi_spe_cz_parcap, scpi_spe_cz_parcap_quest},
	//[SOURce:]CZ:PARRes
	{"parres","parr",-1,source_cz_serinduct,FALSE,1,0,scpi_spe_cz_parres, scpi_spe_cz_parres_quest},
	//[SOURce:]CZ:SERInduct
	{"serinduct","seri",-1,source_cz_serres,FALSE,1,0, scpi_spe_cz_serinduct, scpi_spe_cz_serinduct_quest},
	//[SOURce:]CZ:SERRes
	{"serres","serr",-1,-1,FALSE,1,0,scpi_spe_cz_serres, scpi_spe_cz_serres_quest},

	/*	
	//[SOURce:]QUERy
	{"query","quer",query_calibrate,-1,FALSE,0,0,NULL,NULL},
	//[SOURce:]QUERy:CALibrate
	{"calibrate","cal",-1,-1,FALSE,0,1,NULL,scpi_spe_query_calibrate_quest},
	*/


};
