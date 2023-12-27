
#if !defined(_QPWCLIENT_C_DLL_H_)
#define _QPWCLIENT_C_DLL_H_


#ifdef QPWCLIENT_C_DLL_EXPORTS
#define QPWCLIENT_C_DLL_API __declspec(dllexport)
#else
#define QPWCLIENT_C_DLL_API __declspec(dllimport)
#endif


extern "C"
{

	/***********************************************************************************************
	*函数名       ： Query_Connect_Keyword
	*函数功能描述 ： 输入指定参数查询对应IP,PORT,USER,PASSWORD
	*函数参数     ： i_Input_Query_Type			:<input> 查询类型< 0 =  MAIN_SQL,1 = LOG_SQL ,2 = LOG_FTP>
	*                cp_Input_IP_Addr		    :<input> 被访问的IP地址(e.g.192.168.10.38)
	*                i_Input_Port			    :<input> 被访问的Port(e.g.8015)
	*                i_Input_Send_TimeOut_mS    :<input> 发送超时设置(e.g.1000)
	*                i_Input_Recv_TimeOut_mS    :<input> 接收超时设置(e.g.1000)
	*		         cp_Output_IP_Addr          :<output> Target IP
	*                i_Output_Port              :<output> Target PORT
	*                cp_Output_User             :<output> Target USER
	*                cp_Output_Password         :<output> Target PASSWORD
	*函数返回值   ： TRUE = 成功;FALSE = 失败
	***********************************************************************************************/
	QPWCLIENT_C_DLL_API bool Query_Connect_Keyword(int i_Input_Query_Type,const char * cp_Input_IP_Addr, int i_Input_Port,int i_Input_Send_TimeOut_mS,int i_Input_Recv_TimeOut_mS,char * cp_Output_IP_Addr,int * i_Output_Port,char * cp_Output_User,char * cp_Output_Password);

	/***********************************************************************************************
	*函数名       ： Query_Connect_Keyword_Ex
	*函数功能描述 ： 输入指定参数查询IP,PORT,USER,PASSWORD对应字符串
	*函数参数     ： i_Input_Query_Type			:<input> 查询类型< 0 =  MAIN_SQL,1 = LOG_SQL ,2 = LOG_FTP>
	*                cp_Input_IP_Addr		    :<input> 被访问的IP地址(e.g.192.168.10.38)
	*                i_Input_Port			    :<input> 被访问的Port(e.g.8015)
	*                i_Input_Send_TimeOut_mS    :<input> 发送超时设置(e.g.1000)
	*                i_Input_Recv_TimeOut_mS    :<input> 接收超时设置(e.g.1000)
	*		         cp_Output_Target_Keyword   :<output> Target Keyword(e.g. "192.168.10.38,1433,ate,a@psd2*7e")
	*函数返回值   ： TRUE = 成功;FALSE = 失败
	***********************************************************************************************/
	QPWCLIENT_C_DLL_API bool Query_Connect_Keyword_Ex(int i_Input_Query_Type,const char * cp_Input_IP_Addr, int i_Input_Port,int i_Input_Send_TimeOut_mS,int i_Input_Recv_TimeOut_mS,char * cp_Output_Target_Keyword);

	/***********************************************************************************************
	*函数名       ： Get_Last_Error_Info
	*函数功能描述 ： 用于当调用Query_Connect_Keyword返回FALSE时获取失败信息
	*函数参数     ： cpErrorInfo               :<output> 具体失败log
	*函数返回值   ： 无
	***********************************************************************************************/
	QPWCLIENT_C_DLL_API void Get_Last_Error_Info(char* cpErrorInfo);//获取失败信息
}



#endif // #if !defined(_QPWCLIENT_C_DLL_H_)

