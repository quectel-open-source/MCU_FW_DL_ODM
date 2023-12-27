#pragma once



#ifdef DLL_IMPLEMENT
#define DLL_IMP _declspec(dllexport)
#else
#define DLL_IMP _declspec(dllimport)
#endif



//输出函数接口
DLL_IMP void FLASH_TEST_INIT_FIRST(void);
DLL_IMP void FLASH_TEST_LOAD_ARG_QTI(int i_TSCH_Numb, QTI_PROG_ARG * strp_QTI_Arg);
DLL_IMP void FLASH_TEST_LOAD_PRODU_NAME_AND_TYPE(const wchar_t * wcp_Produ_Name, int i_Produ_Type, int i_Produ_Ver);
DLL_IMP void FLASH_TEST_LOAD_PARAMS_COM_AT(int i_TSCH_Numb, int i_COM_AT_Port, int i_COM_AT_BR);
DLL_IMP void FLASH_TEST_LOAD_PARAMS_FW(const wchar_t * wcp_SW_DL, const wchar_t * wcp_SW_Label);
DLL_IMP bool FLASH_TEST_READ_PC_ID(char * cp_PC_ID);
DLL_IMP bool FLASH_TEST_INIT_ALL(char * cp_PC_ID, const wchar_t * wcp_Key_File_1_Path, const wchar_t * wcp_Key_File_2_Path, const wchar_t * wcp_Key_File_3_Path);
DLL_IMP int FLASH_TEST_DO_EXECUTE(int i_TSCH_Numb);
DLL_IMP int  FLASH_TEST_Run_Model(bool bInit, int iIndex);





