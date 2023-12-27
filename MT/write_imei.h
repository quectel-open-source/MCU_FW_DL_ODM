#pragma once



#ifdef DLL_IMPLEMENT
#define DLL_IMP _declspec(dllexport)
#else
#define DLL_IMP _declspec(dllimport)
#endif



//输出函数接口
DLL_IMP void WRITE_IMEI_INIT_FIRST(void);
DLL_IMP void WRITE_IMEI_LOAD_ARG_QTI(int i_TSCH_Numb, QTI_PROG_ARG * strp_QTI_Arg);
DLL_IMP void WRITE_IMEI_LOAD_ARG_READ_OCP_PARAMS(int i_TSCH_Numb, READ_OCP_PARAMS_ARG * strp_Read_OCP_Params_Arg);
DLL_IMP void WRITE_IMEI_LOAD_PRODU_NAME_AND_TYPE(int i_TSCH_Numb, const wchar_t * wcp_Produ_Name, int i_Produ_Type, int i_Produ_Ver);
DLL_IMP void WRITE_IMEI_LOAD_PARAMS_COM_AT(int i_TSCH_Numb, int i_COM_AT_Port, int i_COM_AT_BR);
DLL_IMP void WRITE_IMEI_LOAD_PARAMS_PATH_INTEL_IMEI_API(int i_TSCH_Numb, const wchar_t * wcp_Intel_Write_Imei_API_Path);
DLL_IMP void WRITE_IMEI_LOAD_PARAMS_FW(int i_TSCH_Numb, const wchar_t * wcp_SW_DL, const wchar_t * wcp_SW_Label);
DLL_IMP void WRITE_IMEI_LOAD_PARAMS_OC(int i_TSCH_Numb, const wchar_t * wcp_OC);
DLL_IMP bool WRITE_IMEI_INIT_ALL(int i_TSCH_Numb);
DLL_IMP void WRITE_IMEI_GET_ERR_INFO(int i_TSCH_Numb, CString * csp_Error_Info);
DLL_IMP void WRITE_IMEI_LOAD_DATA_IMEI_1(int i_TSCH_Numb, bool b_Enable_Write_IMEI_1, const wchar_t * wcp_IMEI_1);
DLL_IMP void WRITE_IMEI_LOAD_DATA_IMEI_2(int i_TSCH_Numb, bool b_Enable_Write_IMEI_2, const wchar_t * wcp_IMEI_2);
DLL_IMP void WRITE_IMEI_LOAD_DATA_SN(int i_TSCH_Numb, bool b_Enable_Write_SN, const wchar_t * wcp_SN);
DLL_IMP void WRITE_IMEI_LOAD_DATA_BT_ADDR(int i_TSCH_Numb, bool b_Enable_Write_BT_Addr, const wchar_t * wcp_BT_Addr);
DLL_IMP void WRITE_IMEI_LOAD_DATA_MAC_ADDR(int i_TSCH_Numb, bool b_Enable_Write_MAC_Addr, const wchar_t * wcp_MAC_Addr);
DLL_IMP void WRITE_IMEI_LOAD_DATA_MEID(int i_TSCH_Numb, bool b_Enable_Write_MEID, const wchar_t * wcp_MEID);
DLL_IMP bool WRITE_IMEI_DO_WRITE_IMEI_V1(int i_TSCH_Numb);
DLL_IMP bool WRITE_IMEI_DO_CHECK_IMEI_V1(int i_TSCH_Numb, bool b_Init_Enable);
DLL_IMP void WRITE_IMEI_CLEAR_ALL_IMPORT_DATA(int i_TSCH_Numb);
DLL_IMP int WRITE_IMEI_DO_WRITE_IMEI_V2(int i_TSCH_Numb, bool b_Init_Enable);

