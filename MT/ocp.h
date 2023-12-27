#pragma once
#include "Global_Header.h"


//
#ifdef DLL_IMPLEMENT
#define DLL_IMP _declspec(dllexport)
#else
#define DLL_IMP _declspec(dllimport)
#endif




//ocp输出函数接口
DLL_IMP bool OCP_IDENTIFY_OC( const wchar_t * wcp_OC, CString * csp_Produ_Name, int * ip_Produ_Type, int * ip_Produ_Ver );
DLL_IMP void OCP_READ_PRODU_NAME_LIST( CString * csp_Produ_Name_List, int * ip_Produ_Name_Quan );
DLL_IMP bool OCP_LOAD_PRODU_PARAMS_GEN( const wchar_t * wcp_Produ_Name, int i_Produ_Type, int i_Produ_Ver );
DLL_IMP void OCP_LOAD_PRODU_FW_VER( const wchar_t * wcp_FW_VER );
DLL_IMP void OCP_GET_ERROR_INFO( CString * csp_Error_Info );
DLL_IMP bool OCP_READ_PRODU_GSM_BAND_LIST( int * ip_GSM_Band_List, int * ip_GSM_Band_Quan );
DLL_IMP bool OCP_READ_PRODU_WCDMA_BAND_LIST( int * ip_WCDMA_Band_List, int * ip_WCDMA_Band_Quan );
DLL_IMP bool OCP_READ_PRODU_C2K_BAND_LIST( int * ip_C2K_Band_List, int * ip_C2K_Band_Quan );
DLL_IMP bool OCP_READ_PRODU_TD_BAND_LIST( int * ip_TD_Band_List, int * ip_TD_Band_Quan );
DLL_IMP bool OCP_READ_PRODU_LTE_BAND_LIST( int * ip_LTE_Band_List, int * ip_LTE_Band_Quan );
DLL_IMP bool OCP_READ_PRODU_NBIOT_BAND_LIST( int * ip_NBIoT_Band_List, int * ip_NBIoT_Band_Quan );
DLL_IMP bool OCP_READ_PRODU_ANT_LIST( ANT_PROP_S * strp_ANT_Prop_List, int * ip_ANT_Quan );
DLL_IMP bool OCP_READ_PRODU_PF_AND_BB_CHIP( int * ip_Platform_Solution, int * ip_BB_Chip );
DLL_IMP bool OCP_READ_PRODU_TS_TYPE_SEQ( int * ip_TS_Type_Seq );
DLL_IMP bool OCP_READ_PRODU_INIT_TIME( int * ip_Init_Time );
DLL_IMP bool OCP_READ_PRODU_CAL_TREE_NAME( CString * csp_Cal_Tree_Name );
DLL_IMP bool OCP_READ_PRODU_CARTON_LABEL_PRINT_NAME( CString * csp_Carton_Label_Print_Name );
DLL_IMP bool OCP_READ_PRODU_VOLT_AND_CURR( double * dp_Voltage, double * dp_Current );
DLL_IMP bool OCP_READ_PRODU_PS_CH2_VOLT_AND_CURR( double * dp_Voltage_CH2, double * dp_Current_CH2 );
DLL_IMP bool OCP_READ_PRODU_LTE_CAT( int * ip_LTE_CAT );
DLL_IMP bool OCP_READ_PRODU_LANDI_ENABLE( bool * bp_Landi_Enable );
DLL_IMP bool OCP_READ_PRODU_WIFI_ENABLE( bool * bp_WiFi_Enable );
DLL_IMP bool OCP_READ_PRODU_NW_SCAN_MODE( int * ip_Nw_Scan_Mode );
DLL_IMP void OCP_READ_PRODU_HW_SIM_ENABLE( const wchar_t * wcp_OC, bool * bp_HW_SIM_Enable );
DLL_IMP void OCP_LOAD_PRODU_PN( const wchar_t * wcp_PN );
DLL_IMP bool OCP_READ_PRODU_NO_PMI_ENABLE( bool & oj_b_NO_PMI );
DLL_IMP bool OCP_READ_PRODU_RF_KEY_TYPE_INDEX( int & oj_i_RF_Key_Type_Index );
DLL_IMP bool OCP_READ_PRODU_NETWORK_FORMAT_BAND_LIST( int i_Network_Format, int * ip_Band_List, int * ip_Band_Quan );
