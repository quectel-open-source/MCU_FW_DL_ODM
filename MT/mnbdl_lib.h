#pragma once
//#include "Run.h"

//
#ifdef DLL_IMPLEMENT
#define DLL_IMP _declspec(dllexport)
#else
#define DLL_IMP _declspec(dllimport)
#endif


//输出函数接口
DLL_IMP void MNB_DL_INIT_FIRST( void );
DLL_IMP void MNB_DL_LOAD_ARG_QTI( int i_TSCH_Numb , QTI_PROG_ARG * strp_QTI_Arg );
DLL_IMP void MNB_DL_LOAD_CTR_PROGRESS( int i_TSCH_Numb , CProgressCtrl* pCtrProgress );
DLL_IMP void MNB_DL_LOAD_DA_FILE_PATH( int i_TSCH_Numb , const CString csDA_File_Path );
DLL_IMP void MNB_DL_LOAD_CFG_FILE_PATH( int i_TSCH_Numb, const CString csCFG_File_Path );
// 加载Log File 路径信息
DLL_IMP void MNB_DL_LOAD_LOG_FILE_PATH(int i_TSCH_Numb, const CString csLog_File_Path);

DLL_IMP void MNB_DL_LOAD_COM_CONFIG( int i_TSCH_Numb , int i_COM_Port , int i_COM_BR );
DLL_IMP bool MNB_DL_DO_MNB_FW_DOWNLOAD(int i_TSCH_Numb, int iComm_Method, bool b_RebootAfterDL, bool b_Format_All_Before_DL, bool b_Total_Format_Except_Retained_Region, bool b_save_log);
DLL_IMP void MNB_DL_GET_ERROR_ALL( int i_TSCH_Numb , int i_Station_Type , int * ip_Error_Code , CString * csp_Error_Info );
//加载 NV BIN File 路径信息
DLL_IMP void MNB_DL_LOAD_NV_FILE_PATH(int i_TSCH_Numb, const CString csNV_File_Path);

//MTK原始log的保存
DLL_IMP void MNB_DL_Enable_MTK_LOG(void);
DLL_IMP void MNB_DL_Disable_MTK_LOG(void);
