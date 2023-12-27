#pragma once


//
#ifdef DLL_IMPLEMENT
#define DLL_IMP _declspec(dllexport)
#else
#define DLL_IMP _declspec(dllimport)
#endif



//输出函数接口
DLL_IMP void TS_LAN_INIT_FIRST( HWND h_Wnd );
DLL_IMP void TS_LAN_LOAD_QTI_ARG( QTI_PROG_ARG * strp_Qti_Arg );
DLL_IMP void TS_LAN_LOAD_TS_PC_LAN_IP_AND_PORT( const wchar_t * wcp_IP, int i_Port );
DLL_IMP void TS_LAN_LOAD_MAIN_CTRL_SERVER_LAN_IP_AND_PORT( const wchar_t * wcp_IP, int i_Port );
DLL_IMP bool TS_LAN_START_TCP_SERVER( void );
DLL_IMP void TS_LAN_STOP_TCP_SERVER( void );
DLL_IMP bool TS_LAN_PROCESS_TCP_ACCEPT( void );
DLL_IMP bool TS_LAN_PROCESS_TCP_OP( WPARAM w_Param, LPARAM l_Param );
DLL_IMP bool TS_LAN_SEND_RESULT_LAN_CMD_TO_MC_PC( const wchar_t * wcp_TS_Name, const wchar_t * wcp_Cmd_Type, int i_TS_Channel, bool b_Result );
DLL_IMP bool TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( int i_TS_Number, const wchar_t * wcp_Cmd_Type );
DLL_IMP bool TS_LAN_SEND_PFQUAN_LAN_CMD_TO_MC_PC( const wchar_t * wcp_TS_Name, int i_TS_Number, int i_Quan_Pass, int i_Quan_Fail );
DLL_IMP bool TS_LAN_SEND_TRD_LAN_CMD_TO_MC_PC( int i_TS_Number, const wchar_t * wcp_TRD );
DLL_IMP bool TS_LAN_SEND_RELOAD_FIXTURE_LAN_CMD_TO_MC_PC( int i_TS_Number );
DLL_IMP void TS_LAN_GET_MO_AND_TEST_INFO( CString * csp_MO_Name, CString * csp_Test_Line_Numb, CString * csp_Operator_ID );
DLL_IMP bool TS_LAN_SEND_QUERY_MO_NAME_LAN_CMD_TO_MC_PC( int i_TS_Number );
DLL_IMP bool TS_LAN_SEND_QUERY_FIXTURE_CODE_LAN_CMD_TO_MC_PC( int i_TS_Number );
