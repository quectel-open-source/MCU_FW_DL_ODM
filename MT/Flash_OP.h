#pragma once

#include "Gen_Header.h"
//MTK头文件
#include "meta.h"
#include "metaapp.h"
#include "rom_setting.h"
#include "flashtool.h"
#include "SLA_Challenge.h"
#include "gfc.h"
#include "flash_test.h"
#include "write_imei.h"
#include "ocp.h"
#include "com_port.h"

//序号	        FLASH型号			    Flash ID
// 				1	GD25LQ32			0xC8 0x60 0x16
// 				2	W25Q32BV			0xEF 0x40 0x16
// 				3	W25Q32JV			0xEF 0x70 0x16
// 				4	W25Q64DW			0xEF,0x60,0x17
// 				5	W25Q128FW			0xEF,0x60,0x18
// 				6	MX25L3291FWJI_09	0xC2 0x20 0x16
#define  FLASH_GD25LQ32_ID  0x001660C8   
#define  FLASH_W25Q32BV_ID  0x001640EF
#define  FLASH_W25Q32JV_ID  0x001670EF 
#define  FLASH_W25Q64DW_ID  0x001760EF 
#define  FLASH_W25Q128FW_ID  0x001860EF 
#define  FLASH_MX25L3291FWJI_09_ID  0x001620C2

#define  FLASH_GD25LQ128CWIG 0x001860C8

#define  FLASH_W25Q128FW_EXT_ID  0x001880EF 


#define   FLASH_ID_NUM  8
class CFlash_OP
{
public:
	CFlash_OP(void);
	~CFlash_OP(void);
	HWND m_hWnd_View;
	int m_iCom_Port;
	UART_BAUDRATE m_eu_BR_UART;
	BBCHIP_TYPE m_eu_BB_Chip;
	EXT_CLOCK m_eu_Ext_Clock;
	bool m_bRuning;
	int m_iClass_Idx;
	CString m_csError_Code;
	int m_iError_Code;
	ROMSetting m_str_RomSetting;
	unsigned short m_usROMcount;
	ROM_INFO m_strr_Rom_Info[MAX_LOAD_SECTIONS];
	ROM_INFO_EX m_strr_Rom_Info_Ex[MAX_LOAD_SECTIONS];
	DL_INFO m_str_DL_Info;
	DL_INFO_EX m_str_DL_Info_Ex;
	bool UN_INIT_DL_HANDLE(void);
	bool LOAD_DA_FILE(const wchar_t * wcp_DA_File_Path);
	bool LOAD_SCAT_FILE(const wchar_t * wcp_Scat_File_Path);
	bool UN_INIT_DA_HANDLE(void);
	void Show_Status(const wchar_t * wcp_Status);
	CString m_csBin_Product;
	int m_iBin_Flash;
	int m_iComm_Method;
	int m_iOP_TYPE;
	int Do_Execute();
	int DL_IN_UART(void);
	int m_iStop_Flag;
	int m_iNV_Retain_File_Quan;
	char m_cr2NV_Retain_File[50][32];
	bool INIT_META_DLL(void);
	int m_iMeta_Handle;
	void UN_INIT_META_DLL(void);
	bool Enter_META_Mode(const BBCHIP_TYPE eu_BB_Chip, const EXT_CLOCK eu_Ext_Clock);
	bool Enter_META_Mode_In_Uart(const BBCHIP_TYPE eu_BB_Chip, const EXT_CLOCK eu_Ext_Clock);
	bool Process_Meta_Result(const META_RESULT eu_Meta_Result, const wchar_t * wcpMETA_FUNC);
	bool Clear_Nv_File(void);
	bool Exit_META_Mode(void);
	bool Process_FlashTool_Result(int i_Status_ID);
	void Arg_Com_Port_Setting(COM_PORT_SETTING * strp_Com_Setting);
	void Arg_Flash_Tool(FLASHTOOL_ARG * strp_FT_Arg, FLASHTOOL_RESULT * strp_FT_Result, bool b_In_USB);
	void Arg_Flash_Tool_Download(FLASHTOOL_DOWNLOAD_ARG * strp_FT_DL_Arg, FLASHTOOL_DOWNLOAD_RESULT * strp_FT_DL_Result);
	void Show_Progress(int i_Pgr_Percent);
	char m_crScat_File_Path[MAX_PATH];
	void Enable_Debug_Log(bool b_Enable);
	int Read_Nv_FAT_File_List(const char * cp_FAT_Base_Dir, char cr2_FAT_File_List[][MAX_PATH], int * ip_FAT_Quan);
	bool LOAD_BIN_FILE(void);
	bool Search_Dynamic_Com_Port(int * ip_New_Com_Port);
	int DL_IN_USB(void);
	unsigned int m_ui_nand_acccon;
	bool Enter_META_Mode_In_USB(const BBCHIP_TYPE eu_BB_Chip, const EXT_CLOCK eu_Ext_Clock);
	void Arg_META_Conn_Req_In_USB(META_ConnectByUSB_Req * strp_META_Conn_Req, META_ConnectByUSB_Report * strp_META_Conn_Report);
	bool Get_Bin_File_Info(wchar_t * wcp_Bin_File_Name, CString * csp_Product_Name, int * ip_Flash);
	bool Get_Bin_File_Name(const wchar_t * wcp_File_Path, wchar_t * wcp_File_Name);
	bool Load_Bin_File_Product_Flash_Name(void);
	bool Load_NV_Retain_File_List(const int i_NV_TYPE, const wchar_t * wcp_Curr_Exe_Path);
	CGFC m_cla_GFC;
	bool Get_Bin_File_Product_Name(const wchar_t * wcp_Bin_File_Name, CString * csp_Product_Name, CString * csp_Useful_Data);
	bool Get_Bin_File_Flash_Index(const wchar_t * wcp_Bin_File_Name, int * ip_Flash_Index);
	bool JUDGE_SCAT_FILE(const wchar_t * wcp_Scat_File_Path, int * ip_Fw_Type);
	int m_iFw_Type;
	bool SETUP_APP_FULL_FW_FILE(const wchar_t * wcp_App_File_Path, const wchar_t * wcp_Model_Name, CString * csp_App_Scat_Cfg_File_Path);
	CString m_csCurr_Exe_Path;
	bool COPY_BIN_FILE(const wchar_t * wcp_Src_Dir, const wchar_t * wcp_Tgt_Dir);
	bool SETUP_APP_SCAT_FILE(const wchar_t * wcp_App_File_Path, const wchar_t * wcp_Src_Dir, const wchar_t * wcp_Tgt_Dir, CString * csp_App_Scat_Cfg_File_Path);
	bool COPY_APP_FILE(const wchar_t * wcp_App_File_Path, const wchar_t * wcp_Tgt_Dir);
	CString m_csScat_Cfg_File_Path;
	QTI_PROG_ARG m_str_QTI_ARG;
	void Load_CALLBACK_ARG(void);
	READ_OCP_PARAMS_ARG m_str_READ_OCP_PAR_ARG;
	CCom_Port m_cla_Com_Port;

	CString m_strNewFlashID;
	CString m_strFLASH_ID[FLASH_ID_NUM];
	int     m_iFLASH_NUM;

	CString m_strFLASH_ID_Current;

	DL_HANDLE * m_DL_HANDLE;
	DA_HANDLE * m_DA_HANDLE;

	BOOL m_bMCXX_Old; //MC20/MC60是否是老版本的标志

	int  m_MC_flag; //如果firmware路径有MC20/MC60,则m_MC_flag=1

	bool Check_APP_VER(void);

	void Show_Dos(const wchar_t * wcp_Status);

	int m_iQFLASH_CMD_MTK;

	bool m_bPLC_Flag;//带PLC控制的夹具
	int m_iPLC_Port;

	void Show_Out(const wchar_t * wcp_Status);

	int Arg_Flash_Tool_FormatEx( FLASHTOOL_FORMAT_ARG_EX       *fmt_arg_ex, FLASHTOOL_FORMAT_RESULT    *fmt_result);
	int Format_Internal(void);

	unsigned int  m_uiFLASH_START_ADDR;//0;  
	unsigned int  m_uiFLASH_ADDR_LENGTH;//0x00300000; 

	

};
