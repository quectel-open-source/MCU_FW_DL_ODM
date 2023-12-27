#pragma once

#include "gfc.h"
#include "host_uart.h"

#include "fplink\include\TargetConnection.h"
#include "fplink\include\LodPacket.h"
#include "fplink\include\FlashInfo.h"
#include "fplink\include\ChipInfo.h"
#include "fplink\include\FlashProgram.h"
#include "fplink\include\RamRun.h"
#include <memory.h>

#include "Com_Port.h"


#define ASYNC_HDLC_ESC_X_ON			0x11
#define ASYNC_HDLC_ESC_X_OFF		0x13
#define ASYNC_HDLC_ESC_MASK_R		0x5C

#define ASYNC_HDLC_AD_FLAG			0xAD
#define ASYNC_HDLC_FF_FLAG			0xFF

#define FLASH_PACKET_SIZE_MAX		32768//(32 * 1024)
#define HOST_PACKET_MAX				1024//1024个DWORD ＝ 4096个BYTE = 0x1000

#define F_SPI_16M		0

#define CMD_WR_BYTE     0x80
#define CMD_WR_WORD     0x81
#define CMD_WR_DWORD    0x82
#define CMD_WR_BLOCK    0x83
#define CMD_WR_REG      0x84
#define CMD_RD_BYTE     0x00
#define CMD_RD_WORD     0x01
#define CMD_RD_DWORD    0x02
#define CMD_RD_BLOCK    0x03
#define CMD_RD_REG      0x04

#define CTRL_SET_REG				0x00000000
#define CTRL_CLR_REG				0x00000001
#define CTRL_CFG_REG				0x00000003
#define H2P_STATUS_REG				0x00000005
#define HOST_MONITOR_CTRL			0x81C000A0

#define FLASH_ID_REG				0xA1A04410
#define CHIP_ID_REG					0x01A24000
#define GALLITE_HALFIXED_ADDR		0x81C0027C
#define FPC_ACCESS_OFFSET			0xA4

#define EVENT_CHECK_RFCRC_ERROR		0xB0
#define EVENT_FLASH_PROG_READY		0xF0
#define EVENT_FLASH_PROG_ERROR		0xE0
#define EVENT_FLASH_PROG_UNKNOWN	0xD0
#define EVENT_RESET					0xFFFFFFFF

#define FPC_BUFFER_SIZE				32768//(32 * 1024)

#define  RDA_CONNECTION_COM_FAIL    0x03001001
#define  RDA_HOSTPREPARERAMRUN_FAIL 0x03001002
#define  RDA_LOAD_RAMRUN_FAIL		0x03001003
#define  RDA_RAMRUN_RUN_FAIL		0x03001004
#define  RDA_CHECKSTEART_FAIL		0x03001005
#define  RDA_PROGRAM_DL_FAIL        0x03001006
#define  RDA_VERIFY_FAIL			0x03001007
#define  RDA_ERASE_USER_DATA_FAIL   0x03001008
#define  RDA_ProbeDevice_FAIL       0x03001009



#define MAX_LOD_FILE  8

typedef enum
{
	FPC_NONE          = 0x00000000,
	FPC_PROGRAM       = 0x00000001,
	FPC_ERASE_SECTOR  = 0x00000002,
	FPC_ERASE_CHIP    = 0x00000003,
	FPC_END           = 0x00000004,
	FPC_CHECK_FCS     = 0x00000005,
	FPC_RESTART	      = 0x00000007,
	FPC_CHECK_CALIB   = 0x00000008,
	FPC_DONE          = 0xFFFFFF9C,
	FPC_ERROR         = 0xFFFFFF9D
}FPC_COMMAND_TYPE_T;

typedef struct tagLOD_FILE_S
{
	CString cs_Name;
	CString cs_Path;
	unsigned long ul_File_Size;//LOD文件的BYTE数量
	unsigned long ul_Phase_Quan;//LOD文件中phase的数量
	unsigned long ulr_Start_Index[200];//LOD文件中会有多个FW数据段，该变量代表每个地址数据前（不包括@）所有字符的数量，不包括地址数据和@
	DWORD dwr_Phase_Addr[200];//每个phase的下载地址
	unsigned long ulr_Data_Quan_In_Phase[200];//每个phase中的具体实际dword数据的数量统计
	unsigned long ul_FCS_Quan;//FCS值的数量统计
	DWORD dwr_FCS[300];//每个phase很可能有多个FCS值，因为一个phase会被划分为多个block来发送
	int i_Lod_Type;//“1”代表用“0x0A”结尾换行；“2”代表用“0x0D + 0x0A”结尾换行；
}LOD_FILE_S;

typedef struct tagRDA_FILE_S
{
	CString cs_Name;
	CString cs_Path;
	unsigned long ul_File_Size;
	bool b_Enable_DL;
}RDA_FILE_S;

struct FCSContext
{
	unsigned int remainder;
};

typedef struct
{
	DWORD fpc_access_base;
	DWORD cmd_base[2];
	DWORD ram_buf_base[3];
	DWORD buf_size;	//DWORD size
}FPC_ACCESS_INFO;

typedef struct
{
	FPC_COMMAND_TYPE_T cmd;
	DWORD flash_addr;
	DWORD ram_addr;
	DWORD data_size;
	DWORD fcs;
}FPC_CMD_INFO;

class CARM_OP_RDA
{
public:
	CARM_OP_RDA(void);
	~CARM_OP_RDA(void);
	HWND m_hWnd_View;
	int m_iCom_Port;
	int m_iBaud_Rate;
	bool m_bRuning;
	int m_iStop_Flag;

	CString m_csError_Code;
	int m_iError_Code;

	RDA_FILE_S m_strr_Lod_File_List[MAX_LOD_FILE];

	int m_iClass_Idx;
	void Show_Progress(double d_Pgr_Percent);
	void Show_Status(const wchar_t * wcp_Status);
	int Do_Execute(void);
	CGFC m_cla_GFC;
	CHost_Uart m_cla_Host_Uart;
	void COM_PORT_INIT(void);
	bool Add_Byte_To_Packet(PKT_BUFF_S * strp_Pkt_Cmd, const unsigned char uc_Byte, bool b_Escape_Flag);
	bool PKT_CMD_LOAD_BYTE(unsigned char * ucp_Data, unsigned long ul_Data_Size, unsigned long ul_Addr, unsigned char uc_Cmd, PKT_BUFF_S * strp_Pkt_Cmd);
	bool PKT_CMD_LOAD_DWORD(DWORD * dwp_Data, unsigned long ul_Data_Size, unsigned long ul_Addr, unsigned char uc_Cmd, PKT_BUFF_S * strp_Pkt_Cmd);
	PKT_BUFF_S m_str_Pkt_Send;
	PKT_BUFF_S m_str_Com_Rx_Buff;
	bool Read_DWORD(DWORD dw_Read_Addr, DWORD * dwp_Read_Data, int i_TimeOut);
	bool Write_DWORD(DWORD dw_Write_Addr, DWORD dw_Write_Data);
	void Clear_Com_Rx_Buffer(void);
	bool COM_PORT_OPEN(void);
	void COM_PORT_CLOSE(void);
	bool WRITE_PKT(const PKT_BUFF_S str_Tx_Pkt_Buff);
	bool READ_PKT(PKT_BUFF_S * strp_Rx_Pkt_Buff);
	bool QUERY_PKT(PKT_BUFF_S str_Send_Pkt, PKT_BUFF_S * strp_Read_Pkt, int i_TimeOut);
	bool GET_DATA_FROM_READ_PKT(PKT_BUFF_S * strp_Read_Pkt, unsigned char * ucp_Rd_ID, unsigned char * ucp_Read_Data, unsigned long * ulp_Read_Quan);
	bool Probe_Port(void);
	bool Write_REG(DWORD dw_Write_Addr, unsigned char uc_Reg_Data);
	bool Read_REG(DWORD dw_Read_Addr, unsigned char * ucp_Read_Reg_Data, int i_TimeOut);
	bool Lock_XCPU(void);
	bool Read_Spi_Flash(int * ip_Flash_Idx);
	bool Get_Spi_Flash_Name(const int i_Flash_Idx, CString * csp_Flash_Name);
	bool Enter_Host_Mode(void);
	bool READ_RSP_PKT(PKT_BUFF_S * strp_Read_Pkt, int i_TimeOut);
	bool Config_Ebc_Ram(void);
	bool Read_Chip_ID(DWORD * dwp_Chip_ID);
	LOD_FILE_S m_str_Lod_RamRun;
	LOD_FILE_S m_str_Lod_FW;
	CString m_csCurr_Exe_Path;
	bool LOAD_LOD_FILE(LOD_FILE_S * strp_Lod_File);
	bool Calcu_FCS(unsigned char * ucp_Data_Buff, unsigned long ul_Data_Quan, LOD_FILE_S * strp_Lod_File);
	void FCSInit(struct FCSContext * context);
	void FCSUpdate(struct FCSContext * context, unsigned char * buffer, int len);
	void FCSFinal(struct FCSContext * context, unsigned char * fcs);
	bool Write_DWORD_BLOCK(DWORD dw_Write_Addr, DWORD * dwp_Write_Data, unsigned long ul_Write_Quan);
	bool DL_RAMRUN_LOD_FILE(LOD_FILE_S str_Ramrun_Lod);
	void TRANS_DWORD_DATA(unsigned char * ucp_Hex_Data, DWORD * dwp_Buff, DWORD * dwp_Quan);
	bool WAIT_DWORD(unsigned char uc_Tgt_ID, DWORD dw_Tgt_Data, int i_TimeOut);
	int READ_FILE(HANDLE h_File, unsigned char * ucp_Read_Buff, unsigned long ul_Bytes_To_Read, unsigned long * ulp_Bytes_Real_Rec, bool b_Show_Progress);
	unsigned long m_ulRead_Bytes;
	unsigned long m_ulAll_Bytes;
	bool DL_FW_LOD_FILE(LOD_FILE_S str_FW_Lod);
	bool Read_FPC_Access_Info(FPC_ACCESS_INFO * strp_Fpc_Access_Info);
	FPC_ACCESS_INFO m_str_Fpc_Access_Info;
	void Get_Lod_File_Name(const wchar_t * wcp_File_Path, wchar_t * wcp_File_Name);
	CString m_csLod_Product;
	CString m_csLod_XCV_MODEL;
	bool Get_Lod_File_Info(wchar_t * wcp_Lod_File_Name, CString * csp_Product_Name);
	bool Load_Lod_File_Info(void);
	bool Load_Lod_File_Params(const wchar_t * wcp_File_Path);
	bool Get_Lod_File_XCV_MODEL_Params(const wchar_t * wcp_File_Path);

	//20180929 MC25 --8955
	int  m_chipe_flag; //1:MC25 --8955
	bool LOAD_LOD_FILE_Ext(LOD_FILE_S * strp_Lod_File);
	bool Load_Com_Conn(void);
	int  Do_Execute_8955(void);

	LodData m_Lod;
	LodData m_Lod_Sub[MAX_LOD_FILE];
	LodHeader* m_Lodheader;
	TargetConnection* m_conn;

	BOOL m_bRDA_download;//TRUE:格式化后下载 FALSE:不格式化直接升级

	void Show_Out(const wchar_t * wcp_Status);
	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);

	bool m_bPLC_Flag;//带PLC控制的夹具
	int m_iPLC_Port;

	UINT32 m_iFS_ROOT_BASE;
	UINT32 m_iFS_ROOT_SIZE;

	UINT32 m_iBOOT_SECTOR;//主lod中一般是 0，APP中一般是非0

	bool Get_Lod_File_FS_ROOT_BASE_Params(const wchar_t * wcp_File_Path,int* p_iFS_ROOT_BASE);
	bool Get_Lod_File_FS_ROOT_SIZE_Params(const wchar_t * wcp_File_Path);
	bool Get_Lod_File_BOOT_SECTOR_Params(const wchar_t * wcp_File_Path,int* p_iBOOT_SECTOR);

	bool MergeLod(const CString lodName1,const CString lodName2,const CString DestlodName);

	bool LOAD_LOD_FILE_Ext_Sub(LOD_FILE_S * strp_Lod_File);
	int Do_Execute_8955_Sub(void);

	void Find_lod_File(const wchar_t * wcp_FW_File_Path);

	int  m_Lod_Num;
	int  m_iProdu_Type;//通过OCP获取的机型，主要是工厂用来判断是升级还是工厂格式化下载

	int   m_iCfgFileNum;

	BOOL m_bRdaReboot;

};
