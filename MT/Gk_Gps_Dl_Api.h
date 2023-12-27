#pragma once
//#include "Tools.h"

//
#ifdef DLL_IMPLEMENT
#define DLL_IMP _declspec(dllexport)
#else
#define DLL_IMP _declspec(dllimport)
#endif

DLL_IMP void GK_DL_LOAD_COM_CONFIG( int i_TSCH_Numb , int i_COM_Port , int i_COM_BR );
DLL_IMP void GK_DL_LOAD_CTR_PROGRESS( int i_TSCH_Numb , CProgressCtrl* pCtrProgress );
DLL_IMP void GK_DL_LOAD_ARG_QTI( int i_TSCH_Numb , QTI_PROG_ARG * strp_QTI_Arg );
DLL_IMP void GK_DL_INIT_FIRST( void );
DLL_IMP bool GK_DL_LOAD_IMGFile_PATH( int i_TSCH_Numb , const wchar_t *  csIMGFile_Path );
DLL_IMP void GK_DL_ProgramClose( int i_TSCH_Numb);
DLL_IMP bool GK_DL_OpenSerialPort ( int i_TSCH_Numb, int i_COM_Port);
DLL_IMP void GK_DL_CloseSerialPort ( int i_TSCH_Numb);
DLL_IMP void GK_DL_GET_ERROR_ALL( int i_TSCH_Numb,int * ip_Error_Code, CString* csp_Error_Info );
DLL_IMP int GK_DL_DO_FW_DOWNLOAD ( int i_TSCH_Numb);
