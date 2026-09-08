#include "include.h"

//CZ功能
short scpi_spe_cz_parcap(void)
{
	Scpipara_type cap;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &cap, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (cap.type== SCPI_INT)
		{
			cap.reg_floa= (float)cap.reg_int;	
		}
		requ_write_an_reg((u8*)&cap.reg_floa, MAP_CZ_Cl);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}

short scpi_spe_cz_parcap_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CZ_Cl,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestCapCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;
}

short scpi_spe_cz_parres(void)
{
	Scpipara_type res;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &res, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		if (res.type== SCPI_INT)
		{
			res.reg_floa= (float)res.reg_int;	
		}
		requ_write_an_reg((u8*)&res.reg_floa, MAP_CZ_Rl);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}


short scpi_spe_cz_parres_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CZ_Rl,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestResCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;
}


short scpi_spe_cz_serinduct(void)
{
	Scpipara_type induct;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &induct, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
                if (induct.type== SCPI_INT)
		{
			induct.reg_floa= (float)induct.reg_int;	
		}
		requ_write_an_reg((u8*)&induct.reg_floa, MAP_CZ_Ls);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}


short scpi_spe_cz_serinduct_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CZ_Ls,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestInductCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;
}

short scpi_spe_cz_serres(void)
{
	Scpipara_type res;
	short err_code;	
	u8 *pscpi_pb;	

	if (scpi_proc_ava)//无命令待处理
	{
		pscpi_pb= GetParaValue(scpi_param_buffer+1, SCPI_FLOAT|SCPI_INT, &res, &err_code);
		if (pscpi_pb== NULL)
			return err_code; 	
	}

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
                if (res.type== SCPI_INT)
		{
			res.reg_floa= (float)res.reg_int;	
		}
		requ_write_an_reg((u8*)&res.reg_floa, MAP_CZ_Rs);		

		//Send_event= NULL;
		Recv_event= ScpiSetCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令
		return 0;//先返回成功
	}

	return 0; 
}


short scpi_spe_cz_serres_quest(void)
{

	if (request_comm_service(COMMUN_CONNECT))//通信接口闲
	{
		requ_read_regs( MAP_CZ_Rs,1 );		
		//Send_event= NULL;
		Recv_event= ScpiQuestResCmdAnswer;
		scpi_proc_ava= FALSE;//禁止执行下面的SCPI命令		
	}	
	return 0;
}