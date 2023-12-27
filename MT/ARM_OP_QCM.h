#pragma once

#include "gfc.h"
#include "Com_Thr_Port.h"
#include "Com_Port.h"
#include "QLib_Defines.h"
#include "QMSL_Core.h"
#include "QMSL_FTM_RF.h"
#include "QMSL_Diag.h"
#include "MS_IMbnDevice.h"

#include "inifile.h"
#include "SSH.h"


#define COMM_SYNC			0//同步I/O方式打开串口
#define COMM_OVERLAPPED		1//重叠I/O方式打开串口

/* Async HDLC achieves data transparency at the byte level by using
   two special values. The first is a flag value which begins and ends
   every packet: */
#define ASYNC_HDLC_FLAG			0x7E
/* The flag value might appear in the data.  If it does, it is sent as
   a two-byte sequence consisting of a special escape value followed by
   the flag value XORed with 0x20.  This gives a special meaning to the
   escape character, so if it appears in the data it is itself escaped
   in the same way. */
#define ASYNC_HDLC_ESC			0x7D
#define ASYNC_HDLC_ESC_MASK_Q	0x20

#define CRC_16_L_SEED			0xFFFF

//DL FLAG
#define DLOAD_FLAG_REQ			0x60
#define DLOAD_FLAG_ACK			0x61

#define  MAX_QD_NUM   8

#define PROVISION_SAMSUNG 0
#define PROVISION_TOSHIBA 1
#define PROVISION_HYNIX 2

//.mbn文件的index
typedef enum tagIDX_MBN_FILE_E
{
	//MDM6200
	//QSC6270
	DBL_MBN_FILE_I				= 0,
	FSBL_MBN_FILE_I				= 1,
	OSBL_MBN_FILE_I				= 2,
	AMSS_MBN_FILE_I				= 3,

	//MDM9215
	SBL1_MBN_FILE_I				= 0,
	SBL2_MBN_FILE_I				= 1,
	RPM_MBN_FILE_I				= 2,
	APPSBL_MBN_FILE_I			= 3,
	DSP1_MBN_FILE_I				= 4,
	DSP2_MBN_FILE_I				= 5,
	DSP3_MBN_FILE_I				= 6,
	//MSM8909 (只显示指令相关和format相关的FW文件)
	FIREHOSE_8909_MBN_FILE_I	= 0,//prog_emmc_firehose_8909_ddr.mbn
	RAWPROG_8909_XML_FILE_I		= 1,//rawprogram_unsparse.xml
	PATCH0_8909_XML_FILE_I		= 2,//patch0.xml
	FS_IMAGE_8909_IMG_FILE_I	= 3,//fs_image.tar.gz.mbn.img (存放射频的配置参数)
	ZERO_8909_BIN_FILE_I		= 4,//zero.bin (会清掉分区里的校准参数)
	//MSM8953 (只显示指令相关和format相关的FW文件)
	FIREHOSE_8953_MBN_FILE_I	= 0,//prog_emmc_firehose_8953_ddr.mbn
	RAWPROG_8953_XML_FILE_I		= 1,//rawprogram_unsparse.xml
	PATCH0_8953_XML_FILE_I		= 2,//patch0.xml
	FS_IMAGE_8953_IMG_FILE_I	= 3,//fs_image.tar.gz.mbn.img (存放射频的配置参数)
	ZERO_8953_BIN_FILE_I		= 4,//zero.bin (会清掉分区里的校准参数)

	//SM6115 (只显示指令相关和format相关的FW文件)
	FIREHOSE_SM6115_MBN_FILE_I	= 0,//prog_firehose_sm6115_ddr.elf
	RAWPROG_SM6115_XML_FILE_I		= 1,//rawprogram_unsparse0.xml
	PATCH0_SM6115_XML_FILE_I		= 2,//patch0.xml
	FS_IMAGE_SM6115_IMG_FILE_I	= 3,//fs_image.tar.gz.mbn.img (存放射频的配置参数)
	ZERO_SM6115_BIN_FILE_I		= 4,//zero.bin (会清掉分区里的校准参数)

	//QCM6125
	FIREHOSE_QCM6125_MBN_FILE_I = 0,//prog_firehose_Qcm6125_ddr.elf
	RAWPROG_QCM6125_XML_FILE_I = 1,//rawprogram_unsparse0.xml
	PATCH0_QCM6125_XML_FILE_I = 2,//patch0.xml
	FS_IMAGE_QCM6125_IMG_FILE_I = 3,//fs_image.tar.gz.mbn.img (存放射频的配置参数)
	ZERO_QCM6125_BIN_FILE_I = 4,//zero.bin (会清掉分区里的校准参数)
	


	//MSM8937 (只显示指令相关和format相关的FW文件)
	FIREHOSE_8937_MBN_FILE_I	= 0,//prog_emmc_firehose_8937_ddr.mbn
	RAWPROG_8937_XML_FILE_I		= 1,//rawprogram_unsparse.xml
	PATCH0_8937_XML_FILE_I		= 2,//patch0.xml
	FS_IMAGE_8937_IMG_FILE_I	= 3,//fs_image.tar.gz.mbn.img (存放射频的配置参数)
	ZERO_8937_BIN_FILE_I		= 4,//zero.bin (会清掉分区里的校准参数)

	//SM4350 (只显示指令相关和format相关的FW文件)
	FIREHOSE_SM4350_ELF_FILE_I = 0,//prog_firehose_SM4350_ddr.elf
	RAWPROG_SM4350_XML_FILE_I = 1,//rawprogram_unsparse.xml
	PATCH0_SM4350_XML_FILE_I = 2,//patch0.xml
	FS_IMAGE_SM4350_IMG_FILE_I = 3,//fs_image.tar.gz.mbn.img (存放射频的配置参数)
	ZERO_SM4350_BIN_FILE_I = 4,//zero.bin (会清掉分区里的校准参数)


	//MSM8917 (只显示指令相关和format相关的FW文件)
	FIREHOSE_8917_MBN_FILE_I	= 0,//prog_emmc_firehose_8917_ddr.mbn
	RAWPROG_8917_XML_FILE_I		= 1,//rawprogram_unsparse.xml
	PATCH0_8917_XML_FILE_I		= 2,//patch0.xml
	FS_IMAGE_8917_IMG_FILE_I	= 3,//fs_image.tar.gz.mbn.img (存放射频的配置参数)
	ZERO_8917_BIN_FILE_I		= 4,//zero.bin (会清掉分区里的校准参数)
	//SDM660 (只显示指令相关和format相关的FW文件)
	FIREHOSE_SDM660_ELF_FILE_I	= 0,//prog_emmc_ufs_firehose_Sdm660_ddr.elf
	RAWPROG_SDM660_XML_FILE_I		= 1,//rawprogram_unsparse.xml
	PATCH0_SDM660_XML_FILE_I		= 2,//patch0.xml
	FS_IMAGE_SDM660_IMG_FILE_I	= 3,//fs_image.tar.gz.mbn.img (存放射频的配置参数)
	ZERO_SDM660_BIN_FILE_I		= 4,//zero.bin (会清掉分区里的校准参数)

	//SC66CENA
	RAWPROG1_SDM660_XML_FILE_I		= 5,//rawprogram1.xml
	RAWPROG2_SDM660_XML_FILE_I		= 6,//rawprogram2.xml
	RAWPROG3_SDM660_XML_FILE_I		= 7,//rawprogram3.xml
	RAWPROG4_SDM660_XML_FILE_I		= 8,//rawprogram4.xml
	RAWPROG5_SDM660_XML_FILE_I		= 9,//rawprogram5.xml

	PATCH1_SDM660_XML_FILE_I		= 10,//patch1.xml
	PATCH2_SDM660_XML_FILE_I		= 11,//patch2.xml
	PATCH3_SDM660_XML_FILE_I		= 12,//patch3.xml
	PATCH4_SDM660_XML_FILE_I		= 13,//patch4.xml
	PATCH5_SDM660_XML_FILE_I		= 14,//patch5.xml

	PROVISION_SDM660_XML_FILE_I     = 15,//provision_samsung.xml 暂且芯片以三星来定

	//SA800U- SDM845 (只显示指令相关和format相关的FW文件)
	FIREHOSE_SDM845_ELF_FILE_I	= 0,//prog_emmc_ufs_firehose_Sdm660_ddr.elf
	RAWPROG_SDM845_XML_FILE_I		= 1,//rawprogram_unsparse.xml
	PATCH0_SDM845_XML_FILE_I		= 2,//patch0.xml
	FS_IMAGE_SDM845_IMG_FILE_I	= 3,//fs_image.tar.gz.mbn.img (存放射频的配置参数)
	ZERO_SDM845_BIN_FILE_I		= 4,//zero.bin (会清掉分区里的校准参数)

	//
	RAWPROG1_SDM845_XML_FILE_I		= 5,//rawprogram1.xml
	RAWPROG2_SDM845_XML_FILE_I		= 6,//rawprogram2.xml
	RAWPROG3_SDM845_XML_FILE_I		= 7,//rawprogram3.xml
	RAWPROG4_SDM845_XML_FILE_I		= 8,//rawprogram4.xml
	RAWPROG5_SDM845_XML_FILE_I		= 9,//rawprogram5.xml

	PATCH1_SDM845_XML_FILE_I		= 10,//patch1.xml
	PATCH2_SDM845_XML_FILE_I		= 11,//patch2.xml
	PATCH3_SDM845_XML_FILE_I		= 12,//patch3.xml
	PATCH4_SDM845_XML_FILE_I		= 13,//patch4.xml
	PATCH5_SDM845_XML_FILE_I		= 14,//patch5.xml

	PROVISION_SDM845_XML_FILE_I     = 15,//provision_samsung.xml 暂且芯片以三星来定


	//MDM9X55 (只显示指令相关和format相关的FW文件)
	FIREHOSE_MDM9X55_MBN_FILE_I	= 0,//prog_nand_firehose_9x55.mbn
	RAWPROG_MDM9X55_XML_FILE_I		= 1,//rawprogram_nand_p2K_b128K_update.xml
	PATCH0_MDM9X55_XML_FILE_I		= 2,//patch_p2K_b128K.xml
	FS_IMAGE_MDM9X55_IMG_FILE_I	= 3,//fs_image.tar.gz.mbn.img (存放射频的配置参数)
	ZERO_MDM9X55_BIN_FILE_I		= 4,//zero.bin (会清掉分区里的校准参数)

	//MDM9x07 (只显示指令相关和format相关的FW文件) 改进版
	FIREHOSE_9X07_MBN_FILE_I	= 0,//prog_nand_firehose_9x07.mbn
	RAWPROG_9X07_XML_FILE_I		= 1,//rawprogram_nand_p4K_b256K_factory.xml、rawprogram_nand_p4K_b256K_update.xml
	PATCH0_9X07_XML_FILE_I		= 2,//patch_p4K_b256K.xml

	//MDM9x07 (加载、显示的文件种类最多，所以定义放在最后)
	TZ_9X07_MBN_FILE_I			= 0,
	RPM_9X07_MBN_FILE_I			= 1,
	ABOOT_9X07_MBN_FILE_I		= 2,
	BOOT_9X07_IMG_FILE_I		= 3,
	MODEM_9X07_UBI_FILE_I		= 4,
	REC_9X07_IMG_FILE_I			= 5,
	RECFS_9X07_UBI_FILE_I		= 6,
	SYS_BACK_9X07_UBI_FILE_I	= 7,
	SYS_9X07_UBI_FILE_I			= 8,
	SBL_9X07_MBN_FILE_I			= 9,
	TZ_B_9X07_MBN_FILE_I		= 10,
	RPM_B_9X07_MBN_FILE_I		= 11,
	ABOOT_B_9X07_MBN_FILE_I		= 12,
	BOOT_B_9X07_IMG_FILE_I		= 13,
	MODEM_B_9X07_UBI_FILE_I		= 14,
   //BG96 MSM9206
	MBA_9X07_MBN_FILE_I        =15,
	QDSP_9X07_MBN_FILE_I       =16,
	APPS_9X07_MBN_FILE_I       =17,
	ACDB_9X07_MBN_FILE_I		=18,
	SCRUB_9X07_MBN_FILE_I		=19,

	MBA_9X07_MBN_FILE_I_EXT		=20,
	TZ_9X07_MBN_FILE_I_EXT		=21,
	RPM_9X07_MBN_FILE_I_EXT		=22,
	APPS_9X07_MBN_FILE_I_EXT	=23,
	ACDB_9X07_MBN_FILE_I_EXT	=24,
	QDSP_REC_9X07_MBN_FILE_I_EXT	=25,
	APPS_REC_9X07_MBN_FILE_I_EXT	=26,
	//MDM9X40 
	TZ_BAK_9X40_MBN_FILE_I_EXT	=27,
	RPM_BAK_9X40_MBN_FILE_I_EXT	=28,
	ABOOT_BAK_9X40_MBN_FILE_I_EXT	=29,
	BOOT_BAK_9X40_MBN_FILE_I_EXT	=30,
	SYS_BAK_9X40_MBN_FILE_I_EXT	=31,

	SYS_BAK_9X06_SEC_DAT = 32,
	EFS2APPS_MBN_FILE_I = 33,
	RECOVERYFS_B_UBI_FILE_I = 34,
	QDSP6SW_B_UBI_FILE_I = 35,
	USR_DATA_UBI_FILE_I = 36,
	MOD_BAK_9X40_MBN_FILE_I_EXT = 37,
	MIBIB_9X07_MBN_FILE_I= 38,

	DEVCFG_UBI_FILE_I=39,

	//9x55--AG15
	EFS2_FILE_I=40,
	APDP_FILE_I=41,
	RAWDATA_FILE_I=42,
	CUST_INFO_FILE_I=43,
	FOTA_FILE_I=44,
	RECOVERYFS_FILE_I=45,
	CACHE_FILE_I=46,
	USRFS_FILE_I=47,
	DEVCFG_9X07_MBN_FILE_I_EXT = 48,

	

	//最多下载的普通.mbn文件
	MAX_MBN_FILE
}IDX_MBN_FILE_E;

//.yaffs2文件的index
typedef enum tagIDX_YAFFS2_FILE_E
{
	SYS_YAFFS2_FILE_I		= 0,
	USER_YAFFS2_FILE_I		= 1,
	RECOV_YAFFS2_FILE_I		= 2,
	//最多下载的普通.yaffs2文件
	MAX_YAFFS2_FILE
}IDX_YAFFS2_FILE_E;

//.img文件的index
typedef enum tagIDX_IMG_FILE_E
{
	BOOT_IMG_FILE_I		= 0,
	RECOV_IMG_FILE_I	= 1,
	//最多下载的普通.img文件
	MAX_IMG_FILE
}IDX_IMG_FILE_E;

//DMSS下载协议命令集
typedef enum tagDMSS_DL_CMD_ID_E
{
	CMD_WRITE  = 0x01,   /* Write a block of data to memory (received)    */
	CMD_ACK    = 0x02,   /* Acknowledge receiving a packet  (transmitted) */
	CMD_NAK    = 0x03,   /* Acknowledge a bad packet        (transmitted) */
	CMD_ERASE  = 0x04,   /* Erase a block of memory         (received)    */
	CMD_GO     = 0x05,   /* Begin execution at an address   (received)    */
	CMD_NOP    = 0x06,   /* No operation, for debug         (received)    */
	CMD_PREQ   = 0x07,   /* Request implementation info     (received)    */
	CMD_PARAMS = 0x08,   /* Provide implementation info     (transmitted) */
	CMD_DUMP   = 0x09,   /* Debug: dump a block of memory   (received)    */
	CMD_RESET  = 0x0A,   /* Reset the phone                 (received)    */
	CMD_UNLOCK = 0x0B,   /* Unlock access to secured ops    (received)    */
	CMD_VERREQ = 0x0C,   /* Request software version info   (received)    */
	CMD_VERRSP = 0x0D,   /* Provide software version info   (transmitted) */
	CMD_PWROFF = 0x0E,   /* Turn phone power off            (received)    */
	CMD_WRITE_32BIT = 0x0F,  /* Write a block of data to 32-bit memory address (received)*/
	CMD_MEM_DEBUG_QUERY = 0x10, /* Memory debug query       (received)    */
	CMD_MEM_DEBUG_INFO  = 0x11, /* Memory debug info        (transmitted) */
	CMD_MEM_READ_REQ    = 0x12, /* Memory read request      (received)    */
	CMD_MEM_READ_RESP   = 0x13, /* Memory read response     (transmitted) */
	CMD_MEM_UNFRAMED_READ_REQ  = 0x14, /* Memory unframed read request   (received)      */
	CMD_MEM_UNFRAMED_READ_RESP = 0x15, /* Memory unframed read response  (transmitted)   */
	CMD_DLOAD_SWITCH = 0x3A			//Switch to downloader
}DMSS_DL_CMD_ID_E;

//Streaming下载协议命令集
typedef enum tagSTREAM_DL_CMD_ID_E
{
	CMD_HELLO_REQ		=	0x01,	//Hello, Initialize connection
	CMD_HELLO_ACK		=	0x02,	//Hello response, Acknowledge connection, ready to unlock
	CMD_STREAM_WRITE	=	0x07,	//Streaming write command
	CMD_BLOCK_WRITTEN	=	0x08,	//Response to stream write
	CMD_RESET_REQ		=	0x0B,	//Reset target
	CMD_RESET_ACK		=	0x0C,	//Response to reset
	CMD_CLOSE			=	0x15,	//Close and flush last partial write to Flash
	CMD_CLOSED			=	0x16,   //Response to close and flush last partial write to Flash
	CMD_SECURITY_MODE_REQ	=	0x17,   //Send Security mode to use for programming images
	CMD_SECURITY_MODE_RSP	=	0x18,   //Response to Send Security mode
	CMD_PARTITION_TABLE_REQ	=	0x19,   //Send partition table to use for programming images
	CMD_PARTITION_TABLE_RSP	=	0x1A,   //Response to send partition table
	CMD_OPEN_MULTI_IMAGE		=	0x1B,   //Open for writing image (Multi-image mode only)
	CMD_OPENED_MULTI_IMAGE	=	0x1C	//Response to open for writing image
}STREAM_DL_CMD_ID_E;

typedef enum tagHEX_REC_FORMAT_E
{
	HEX_MARK	= 0,
	HEX_LEN		= 1,
	HEX_OFFSET	= 3,
	HEX_TYPE	= 7,
	HEX_DATA	= 9   
}HEX_REC_FORMAT_E;

typedef enum tagHEX_EXTRACT_RTN_E
{
	HEX_ETR_SUCCESS		= 0,
	HEX_ETR_DATA_WRONG	= 1,
	HEX_ETR_BUFF_FULL	= 2,
	HEX_ETR_CSUM_ERR	= 3,
	HEX_ETR_END_FLAG	= 4
}HEX_EXTRACT_RTN_E;

typedef struct tagMBN_FILE_S
{
	CString cs_Name;
	CString cs_Path;
	unsigned long ul_File_Size;
	bool b_Enable_DL;
}MBN_FILE_S;

typedef struct tagHEX_FILE_S
{
	CString cs_Name;
	CString cs_Path;
	unsigned long ul_File_Size;
}HEX_FILE_S;

typedef struct tagPKT_BUFF_V2_S
{
	unsigned char ucr_Buff[32];
	unsigned long ul_Data_Length;
}PKT_BUFF_V2_S;

typedef struct tagPKT_DATA_ADDR_BUFF_S
{
	tagPKT_BUFF_S str_Data;
	DWORD dw_Pkt_Addr;
	bool b_Read_Finish;
}PKT_DATA_ADDR_BUFF_S;

typedef struct tagHEX_REAL_DATA_S
{
	tagPKT_DATA_ADDR_BUFF_S str_Actual_Data;
	WORD w_Base_Addr;
	WORD w_Offset_Addr;
	WORD w_Run_Segment_Addr;
	WORD w_Run_Offset_Addr;
	bool b_Base_Addr;
	bool b_Offset_Addr;
	bool b_Run_Flag;
	bool b_End_Flag;
}HEX_REAL_DATA_S;

typedef struct tagRSP_INFO_S
{
	unsigned int ui_Rsp_ID;
	unsigned int ui_Pkt_Valid_Len;//正确数据包中有用数据（去头尾标识）可能的最小数量（因为可能有“转意运算”）
	unsigned int ui_Pkt_Quan;
}RSP_INFO_S;

typedef struct tagRSP_INFO_V2_S
{
	unsigned char ucr_Expect_Rsp[32];
	unsigned int ui_Rsp_Data_Length;
	unsigned int ui_Rsp_Pkt_Quan;
}RSP_INFO_V2_S;

typedef enum tagBB_CHIP_QCM_TYPE
{
	MDM6X00 = 0,
	QSC6270 = 1,
	MDM9X15 = 2,
	MDM9X07 = 3,
	MSM8909 = 4,
	MSM8937 = 5,
	MSM8953 = 6,
	MDM9X40 = 7,
	MDM9X06 = 8,
	MSM8917 = 9,
	SDX20 = 10, //sdx20\sdx24
	SDM660 = 11,
	MDM9X55 = 12,
	MDM9X05 = 13,
	SDX55 = 14,
	SDM845 =15,
	HONEYWELL=16,
	SM6115=17,
	QCM6125 = 18,
	QDM002=19,
	AG215S=20,
	SM4350 = 21,
	SDX62 =22,
	QCM_JLink = 23,
	Chip_MCU = 24,
	Chip_ESP32 = 25
}BB_CHIP_QCM_TYPE;

typedef enum tagMEMORY_NAME_TYPE
{
	EMMC = 0,
	NAND = 1,
	UFS = 2
}MEMORY_NAME_TYPE;



// sbl1.mbn
// tz.mbn
// rpm.mbn
// appsboot.mbn
// mdm9607-perf-boot.img
// mdm-perf-recovery-image-mdm9607-perf.ubi
// NON-HLOS.ubi
// usrdata.ubi
// mdm9607-perf-sysfs.ubi
typedef struct tag9X07_Firehose_File
{
	CString Firehose_File;
	int dis_flag;
}E9X07_Firehose_File;

#define Firehose_File_NUM 10

#define  QCM_ERR_COM_PORT_OPEN_FAIL 1001
#define  QCM_ERR_CTRL_DL_TYPE_FAIL 1002
#define  QCM_ERR_PING_DLOAD_PORT_FAIL 1003
#define  QCM_ERR_CTRL_DLOAD_PORT_FAIL 1004
#define  QCM_ERR_DIAG_Switching_FAIL  1005
#define  QCM_ERR_Switch_To_DL_Mode_FAIL 1006
#define  QCM_ERR_Dload_port_use_uart_port_FAIL 1007
#define  QCM_ERR_Dload_port_use_FAIL 1008
#define  QCM_ERR_Ping_Downloader_FAIL 1009
#define  QCM_ERR_DL_type_FAIL  1010
#define  QCM_ERR_Send_Dmss_Cmd_No_OP_FAIL 1011
#define  QCM_ERR_READ_DMSS_HW_INFO_FAIL 1012
#define  QCM_ERR_Flash_block_size_FAIL 1013
#define  QCM_ERR_DL_Hex_N_File_FAIL 1014
#define  QCM_ERR_DL_Hex_E_File_FAIL 1015
#define  QCM_ERR_COM_PORT_RE_DETECT_FAIL 1016
#define  QCM_ERR_Send_Stream_Cmd_Hello_FAIL 1017
#define  QCM_ERR_Send_Stream_Cmd_Security_Mode_FAIL 1018
#define  QCM_ERR_DL_Mbn_Partition_Table_FAIL 1019
#define  QCM_ERR_DL_Mbn_All_File_V3_FAIL 1020
#define  QCM_ERR_Send_Stream_Cmd_Reset_FAIL 1021
#define  Ping_DLoad_Port_Fail    1022
#define  Switch_To_DL_Mode_In_AT_Fail1  1023
#define  Switch_To_DL_Mode_In_AT_Fail2  1024
#define  Send_Write_DLoad_Flag_Fail1   1025
#define  READ_DMSS_HW_INFO_Fail1   1026
#define  Get_Streaming_Info_Fail1  1027
#define  Read_Nv_Max_Item_Max_Len_In_AT_Fail11  1028
#define  Read_Nv_Max_Item_Max_Len_In_AT_Fail12   1029
#define  Read_Nv_Max_Item_Max_Len_In_AT_Fail13  1030
#define  Read_Nv_Max_Item_Max_Len_In_AT_Fail14  1031
#define  BACKUP_QCN_Fail11  1032
#define  BACKUP_QCN_Fail12  1033
#define  BACKUP_QCN_Fail13   1034
#define  BACKUP_QCN_IN_AT_Fail11  1035
#define  BACKUP_QCN_IN_AT_Fail12  1036
#define  BACKUP_QCN_IN_AT_Fail13  1037
#define  BACKUP_QCN_IN_AT_Fail14   1038
#define  BACKUP_QCN_IN_AT_Fail15   1039
#define  BACKUP_QCN_IN_AT_Fail16   1040
#define  BACKUP_QCN_IN_AT_Fail17   1041 
#define  BACKUP_QCN_IN_AT_Fail18   1042
#define  BACKUP_QCN_IN_AT_Fail19   1043
#define  BACKUP_QCN_IN_AT_Fail110  1044
#define  BACKUP_QCN_IN_AT_Fail111  1045
#define  BACKUP_QCN_IN_AT_Fail112  1046
#define  BACKUP_QCN_IN_AT_Fail113  1047
#define  BACKUP_QCN_IN_AT_Fail114  1048
#define  Send_AT_In_AT_Fail11      1049
#define  Send_AT_In_AT_Fail12      1050
#define  Read_Nv_Item_Data_In_AT_Fail11    1051 
#define  Read_Nv_Item_Data_In_AT_Fail12    1052
#define  Read_Nv_Item_Data_In_AT_Fail13    1053
#define Read_Nv_Item_Params_In_AT_Fail11    1054
#define Read_Nv_Item_Params_In_AT_Fail12    1055
#define Read_Nv_Item_Params_In_AT_Fail13    1056
#define Read_Nv_Item_Params_In_AT_Fail14    1057
#define Get_AT_Nv_Item_Params_Fail1     1058
#define RESTORE_QCN_Fail1   1059
#define RESTORE_QCN_Fail2    1060
#define RESTORE_QCN_Fail3    1061
#define RESTORE_QCN_IN_AT_Fail1  1062
#define RESTORE_QCN_IN_AT_Fail2   1063
#define RESTORE_QCN_IN_AT_Fail3    1064
#define RESTORE_QCN_IN_AT_Fail4     1065
#define RESTORE_QCN_IN_AT_Fail5     1066
#define RESTORE_QCN_IN_AT_Fail6     1067
#define RESTORE_QCN_IN_AT_Fail7     1068
#define Write_Nv_Item_Data_In_AT_Fail1  1069
#define Write_Nv_Item_Data_In_AT_Fail2   1070
#define SCAN_NEW_COM_PORT_Fail1  1071
#define SCAN_NEW_COM_PORT_Fail2  1072
#define SCAN_NEW_COM_PORT_Fail3   1073
#define SCAN_NEW_COM_PORT_Fail4   1074
#define LOAD_BB_INFO_Fail1        1075
#define LOAD_BB_INFO_Fail2        1076
#define Flash_block_size_Fail       1077
#define  FB_DEVICE_V1_Fail1         1078
#define  FB_DEVICE_V1_Fail2         1079
#define  DL_Rawprogram_fail1       1080
#define  DL_Rawprogram_fail2       1081
#define  DL_Rawprogram_fail3		1082
#define DL_Firehose_fail1   1083
#define DL_Firehose_fail2   1084
#define DL_Firehose_fail3   1085
#define DL_Patch_fail1    1086
#define DL_Patch_fail2    1087
#define DL_Patch_fail3    1088
#define DL_Patch_fail4    1089
#define DL_Reset_fail1    1090
#define DL_Reset_fail2    1091
#define DL_Reset_fail3    1092
#define DL_Reset_fail4    1093
#define Confirm_Dynamic_Com_Port_fail1   1094
#define Confirm_Dynamic_Com_Port_fail2    1095

#define DL_FW_FAIL1  1096
#define DL_FW_FAIL2  1097
#define DL_FW_FAIL3  1098
#define DL_FW_FAIL4  1099

#define DL_QBhiServer_fail1  1096
#define DL_QBhiServer_fail2  1097
#define DL_QBhiServer_fail3  1098
#define DL_QBhiServer_Error   1099
#define DL_QBhiServer_TimeOut  1100

#define  DL_Efuse_fail1  1101
#define  DL_Efuse_fail2  1102
#define  DL_SPC_FW_fail1  1103
#define  DL_SPC_FW_fail2  1104
#define  DL_SPC_FW_fail4  1105
#define  DL_SPC_FW_fail3  1106
#define  DL_SPC_CRK_fail1  1107
#define  DL_SPC_CRK_fail2  1108
#define  DL_SPC_CRK_fail4  1109
#define  DL_SPC_CRK_fail3  1110

#define  DL_SSH_LINK_fail  1111
#define  DL_SSH_CmdReturnData_fail  1112
#define  DL_SSH_CheckSN_fail  1113
#define  DL_SSH_EEPROMVerification_fail  1114
#define  DL_SSH_VerifyMAC_fail   1115
#define  DL_SSH_FirmwareVersionCheck_fail   1116
#define  DL_SSH_CheckBeforeUpgrade_HLOS_fail  1117
#define  DL_SSH_CheckBeforeUpgrade_rootfs_fail  1118
#define  DL_SSH_UpdateFromUSDCard_fail   1119
#define  DL_SSH_BaseTestModeDisable_fail   1120
#define  DL_SSH_CheckAfterUpgrade_HLOS_fail  1121
#define  DL_SSH_CheckAfterUpgrade_rootfs_fail  1122
#define  DL_SSH_CheckPartition_fail 1123


#define Write_SerialNumber_Fail					1124
#define Check_SerialNumber_Fail					1125
#define File_Not_Found					1126
#define QCX_Aware_Service_Provisioning_Fail  1127
#define Del_ProvisionLog_Fail  1128

#define QCX_FlashtoolBat_Not_Found  1129
#define Del_QCX_FlashtoolBat_Log_Fail  1130
#define Open_QCX_FlashtoolBat_Log_Fail  1131
#define Burn_QCX_FlashtoolBat_Fail  1132
#define QCX_Aware_Service_ProvisioningBat_Not_Found  1132
#define QCX_Aware_Service_Provisioning_Fail   1133
#define QCX_Aware_Service_Provisioning_TxtLogNotFound   1134
#define QCX_Aware_Service_Provisioning_TxtLogOpenFail   1135

#define QCX_SetAutoReset_Fail   1136
#define QCX_CheckShutdown_Fail   1137
#define QCX_Shutdown_Fail   1138


class CARM_OP_QCM
{
public:
	CARM_OP_QCM(void);
	~CARM_OP_QCM(void);
	HWND m_hWnd_View;
	HANDLE m_hQLib_Handle;

	struct struct_ESP32_FileAddress
	{
		int iFileNums = 0;
		CString str_ESP32_File1 = _T("");
		CString str_ESP32_File2 = _T("");
		CString str_ESP32_File3 = _T("");
		CString str_ESP32_File4 = _T("");
		CString str_ESP32_File5 = _T("");

		CString str_ESP32_Address1 = _T("");
		CString str_ESP32_Address2 = _T("");
		CString str_ESP32_Address3 = _T("");
		CString str_ESP32_Address4 = _T("");
		CString str_ESP32_Address5 = _T("");
	}Mystruct_ESP32_FileAddress;


	//ssh
	char hostname[30];
	char command[100];
	char username[20];
	char password[20];
	//// ssh配置
	ssh_config m_sshconfig;
	//// ssh类
	CSSH m_ssh;


	int m_iCom_Port;
	int m_iCom_Port_Auto_Sec;//一拖四的自动化夹具使用
	int m_iQDPort;
	int m_iSdx55_Port;
	int m_iBG95_Ctrl_Port;//QDM017GLBL-D08-C5M3A,QDM018GLBL-D08-C5M2C 控制口
	int m_iCom_Old_Port;
	int m_iBaud_Rate;
	int  m_iError_Code;
	int m_iClass_Idx;
	int m_iStop_Flag;
	int m_iComm_Type;
	int m_irCom_List_MSM8953[64];
	int m_iCom_Quan_MSM8953;
	int m_iAutoSaveLog;
	int m_irCom_List_old[64], m_iCom_Quan_old;
	int m_iMDM9X07_NEW_FLAG; //1：新的9X07的支持Firehose下载
	int m_iAutoDL_Flag; //自动载入Firmware开关
	int m_iOP_Robot;
	int m_Ts_Num;
	int m_DM_Port;
	int m_iFirhose_DB;//默认状态:0, 在线情况:1 数据库设置   2数据库中无设置
	int m_iFirhose_DB_Path;//路径是否有Firehose
	int m_iQDloader_S[MAX_QD_NUM];//记录EDL之前的QDloader端口
	int m_iQDloader_E[MAX_QD_NUM];//记录EDL之后的QDloader端口
	int m_iCsrListNum;
	int mMbn_File_NUM;
	int m_iATCom_Port;
	int m_iAT_Port;
	int m_iCTRL_DL_TYPE;//强制控制下载类型
	int m_iDelay_Time;//找口延时
	int m_iDL_Emerg_Norm_TYPE;
	int m_iProvision_Type;
	int m_iDM_Port;
	int m_iCTRL_DLOAD_PORT;//强制控制下载口类型
	int m_iDLOAD_PORT;
	int m_iProdu_Type;//通过OCP获取的机型，主要是工厂用来判断是升级还是工厂格式化下载

	unsigned long m_ul_Transmit_Bytes;
	unsigned long m_ul_DL_All_Bytes;

	bool m_bQDM01_Pow_Ctrl;//QDM017GLBL-D08-C5M3A,QDM018GLBL-D08-C5M2C 控制口
	bool m_bRuning;
	bool m_bLow_Speed_CPU;
	bool m_bEDL_Now;
	bool mQCM_SC662NCE_Flag; //SC662NCE 机型特殊下载
	bool m_bDL_Provision; //SC662NCE 机型 UFS Provision下载
	bool m_bPCIE_DL;//PCIE 下载
	bool m_bSending_File;
	bool m_bSendFileRes;
	bool m_bHoneywell_A4_flag;
	bool m_bQDM002_flag;
	bool m_bAT_UseFlag;//根据OC来判断是否需要
	bool m_bAT_UseFlag_QDM;
	bool m_bOldFlag; //沿用老的方式
	bool m_bCEFS_DL;//控制下载CEFS文件, 控制“是否格式化”
	bool m_bQCN_DL;//控制是否导入FW中的QCN文件，一些关键NV恢复Quectel默认的配置，涉及到Quectel默认配置的修改
	bool m_bFW_DL;//控制是否下载FW
	bool m_bQCN_BACKUP;//控制是否做“备份模块中QCN”
	bool m_bQCN_RESTORE;//控制是否导入QCN
	bool m_bQDloader_Get_Success;
	bool m_bAG_Flag;//AG机型识别标志

	BOOL m_bFirehose_Reset_after_DL;//1:Firehose下载后自动重启
	BOOL m_bAUTOSEL_MBN;
	BOOL  m_bSaharaOnly;//20200114 界面菜单上设置SaharaOnly

	

	CString m_csError_Code;
	CString m_csPartition_XML_File;
	CString m_csFW_File_Path;
	CString m_csrPN[16];
	CString m_strOpName;
	CString m_csMbnFilePath;
	CString m_strQW_MAC;
	CString m_strEID;
	CString m_strFirehoseFullPath;//用来保存载入的全路径

	CString m_csFW_File_Path_efuse;
	CString m_csOC;
	CString m_csNv_Definition_File;
	bool m_rep_flag; //REP工单 设置为TRUE
	CString m_csMCU_Name;


	HEX_FILE_S m_str_Hex_E;
	HEX_FILE_S m_str_Hex_N;
	MBN_FILE_S m_str_Mbn_Partition_Table;
	MBN_FILE_S m_str_Mbn_PT_2;
	MBN_FILE_S m_str_Mbn_CEFS;
	MBN_FILE_S m_str_Mbn_QCN_DL;
	MBN_FILE_S m_str_Mbn_QCN_BU_RS;
	MBN_FILE_S m_str_partition_complete;
	MBN_FILE_S m_strr_Mbn_File_List[MAX_MBN_FILE];
	MBN_FILE_S m_strr_Mbn_File_List_efuse[MAX_MBN_FILE];
	MBN_FILE_S m_strr_Yaffs2_File_List[MAX_YAFFS2_FILE];
	MBN_FILE_S m_strr_Img_File_List[MAX_IMG_FILE];
	MBN_FILE_S m_str_Mbn_Emerg;
	MBN_FILE_S m_str_Mbn_Norm;
	MBN_FILE_S m_str_Mbn_SBL2_TMP;
	MBN_FILE_S m_str_Mbn_APPSBL_TMP;
	MBN_FILE_S m_str_Yaffs_Head_aa;
	MBN_FILE_S m_str_Yaffs_Head_ab;
	MBN_FILE_S m_str_QDE007_HEX;

	PKT_BUFF_S m_str_Pkt_Send;
	PKT_BUFF_S m_str_Pkt_Read;


		
	CGFC m_cla_GFC;
	BB_CHIP_QCM_TYPE m_eu_BB_Chip;
	MEMORY_NAME_TYPE   m_eu_Memory_Type;
	E9X07_Firehose_File Firehose_File[Firehose_File_NUM];

	CIniFile m_cla_iniQD_Params;//用来读写10个通道的9008 QDloader口
	MBN_FILE_S m_strr_Mbn_File_List_Ext[MAX_MBN_FILE];

	CCom_Port m_cla_AT_Com_Port;	
	CCom_Port m_cla_Sdx55_Com_Port;
	CCom_Port m_cla_BG95_Com_Port;
	CCom_Thr_Port m_cla_Com_Thr_Port;
	CCom_Port m_cla_Com_Port;

	HEX_REAL_DATA_S m_str_Real_Hex_Send;
	PKT_DATA_ADDR_BUFF_S m_str_Mbn_Buff;
	CMS_IMbnDevice m_cls_IMbnDevice;


	//-----------------------------------------------------------

	void Load_Com_Port_Params(void);
	int Do_Execute(void);
	bool LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path);
	int DL_FW_V1(void);
	void Show_Status(const wchar_t * wcp_Status);
	void Show_Out(const wchar_t * wcp_Status);
	void Show_Status_In_Each(const wchar_t * wcp_Status, const wchar_t * wcp_Title_In_Each);
	bool Send_Dmss_Cmd_No_OP(int i_TimeOut);
	void PKT_DMSS_CMD_NO_OP(PKT_BUFF_S * strp_Pkt_Cmd);
	unsigned short Get_CRC_16(const unsigned char * ucp_Data, const int i_Quan, bool b_Escape_Flag);

	void Add_Byte_To_Packet(PKT_BUFF_S * strp_Pkt_Cmd, const unsigned char uc_Byte, bool b_Escape_Flag);
	void Load_Packet_FCS_Data(unsigned char * ucp_Data, const int i_Byte_Quan, PKT_BUFF_S * strp_Pkt_Buff);
	unsigned short Del_FCS_From_Packet(PKT_BUFF_S * strp_Pkt_Buff);
	void PKT_CMD_SWITCH_TO_DLOAD(PKT_BUFF_S * strp_Pkt_Cmd);
	bool Switch_To_DL_Mode(int i_TimeOut);
	bool DL_Hex_File(const HEX_FILE_S str_Hex_File, unsigned long ul_Pkt_Data_Len);
	DWORD HexChar_To_Int(const unsigned char * ucp_Hex_Char, int i_Byte_Quan);
	int Extract_Hex_Data(PKT_BUFF_S * strp_Input_Data, PKT_BUFF_S * strp_Left_Data, HEX_REAL_DATA_S * strp_Hex_Real_Data);
	int Extract_Hex_Record_Data(const unsigned char * ucp_Hex_Record, int i_Rec_Byte_Quan, HEX_REAL_DATA_S * strp_Real_Data);
	bool Send_Hex_Pkt_Data_V1(HEX_REAL_DATA_S * strp_Hex_Real_Data, const int i_Pkt_Length);
	bool Send_Dmss_Cmd_Go(const WORD w_Segment_Addr, const WORD w_Offset_Addr);
	void PKT_DMSS_CMD_GO(PKT_BUFF_S * strp_Pkt_Cmd, const WORD w_Segment_Addr, const WORD w_Offset_Addr);
	bool PKT_DMSS_CMD_WRITE_32BIT(PKT_BUFF_S * strp_Pkt_Cmd, HEX_REAL_DATA_S * strp_Hex_Real_Data, const int i_Pkt_Length);
	void Debug_Show_Byte_Data(const unsigned char * ucp_Data, const int i_Quan);
	void Debug_Show_DWord_Data(DWORD dw_Data);
	void PKT_STREAM_CMD_HELLO(PKT_BUFF_S * strp_Pkt_Cmd, unsigned int ui_Version);
	void PKT_STREAM_CMD_CLOSE(PKT_BUFF_S * strp_Pkt_Cmd);
	void PKT_STREAM_CMD_SECURITY_MODE(PKT_BUFF_S * strp_Pkt_Cmd, const unsigned int ui_Security_Mode);
	bool PKT_STREAM_CMD_PARTITION_TABLE(PKT_BUFF_S * strp_Pkt_Cmd, const PKT_BUFF_S str_Partition_Table, unsigned int ui_Override);
	void PKT_STREAM_CMD_OPEN_MULTI_IMAGE(PKT_BUFF_S * strp_Pkt_Cmd, const unsigned int ui_Open_Image_Type);
	bool PKT_STREAM_CMD_STREAM_WRITE_V1(PKT_BUFF_S * strp_Pkt_Cmd, PKT_DATA_ADDR_BUFF_S * strp_Data_Addr, const int i_Pkt_Length);
	bool PKT_STREAM_CMD_STREAM_WRITE_V2(PKT_BUFF_S * strp_Pkt_Cmd, PKT_DATA_ADDR_BUFF_S * strp_Data_Addr);
	void PKT_STREAM_CMD_RESET(PKT_BUFF_S * strp_Pkt_Cmd);
	bool Send_Stream_Cmd_Hello(int i_TimeOut, unsigned int ui_Version, unsigned long * ulp_Max_Block_Size);
	bool Send_Stream_Cmd_Close(int i_TimeOut);
	bool Send_Stream_Cmd_Open_Multi_Image(unsigned int ui_Open_Image_Type);
	bool Send_Stream_Cmd_Security_Mode(unsigned int ui_Security_Mode);
	bool Send_Stream_Cmd_Partition_Table(const PKT_BUFF_S str_Partition_Table, unsigned int ui_Override);
	bool Send_Stream_Cmd_Reset(void);
	bool DL_Partition_Table_File(const MBN_FILE_S str_Partition_Table_File);
	bool DL_Mbn_File_V1(const MBN_FILE_S str_Mbn_File, unsigned long ul_Pkt_Data_Len);
	bool Send_Mbn_Pkt_Data_V1(PKT_DATA_ADDR_BUFF_S * strp_Mbn_Data_Addr, const int i_Pkt_Data_Len);
	bool DL_Mbn_All_File(unsigned long ul_Pkt_Data_Len);
	bool Get_Mbn_Open_Multi_Image_Type(const wchar_t * wcp_Mbn_Name, unsigned int * uip_Multi_Image_Type);
	void Show_Progress(double d_Pgr_Percent);
	bool GET_FW_SIZE_QCM();

	bool COM_PORT_OPEN(void);
	void COM_PORT_CLOSE(void);
	CString GET_COM_PORT_ERROR_INFO(void);
	bool WRITE_BYTE(char * cp_Send_Data, unsigned long ul_Data_Quan);
	bool READ_PKT_RSP(RSP_INFO_S str_Rsp_Info, PKT_BUFF_S * strp_Pkt_Receive, int i_TimeOut);
	bool READ_PKT_RSP_OVERLAPPED(RSP_INFO_S str_Rsp_Info, PKT_BUFF_S * strp_Pkt_Receive, int i_TimeOut);
	bool READ_PKT_RSP_SYNC(RSP_INFO_S str_Rsp_Info, PKT_BUFF_S * strp_Pkt_Receive, int i_TimeOut);
	bool SEND_PKT_V1(PKT_BUFF_S str_Pkt_Write, RSP_INFO_S str_Rsp_Info, PKT_BUFF_S * strp_Pkt_Read, int i_TimeOut);
	void Clear_Com_Rx_Buffer(void);

	bool Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size);
	bool Ping_Downloader(int * ip_DL_TYPE);
	bool DL_Cefs_File(int i_Pkt_Data_Len);
	void Show_DL_Info(void);
	bool Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut);



	bool QUERY_PKT(PKT_BUFF_V2_S str_Pkt_V2_Write, RSP_INFO_V2_S str_Rsp_Info_V2, int i_TimeOut);
	bool READ_PKT_RSP_V2(RSP_INFO_V2_S str_Rsp_Info_V2, int i_TimeOut);
	bool READ_PKT_RSP_SYNC_V2(RSP_INFO_V2_S str_Rsp_Info_V2, int i_TimeOut);
	bool SEND_PKT_V2(PKT_BUFF_S str_Pkt_Write, RSP_INFO_V2_S str_Rsp_Info_V2, int i_TimeOut);
	bool COM_PORT_RE_DETECT(const wchar_t * wcp_Info, int i_Sleep_Time, bool b_Ctrl_Com_Port);
	bool READ_PKT_RSP_OVERLAPPED_V2(RSP_INFO_V2_S str_Rsp_Info_V2, int i_TimeOut);
	bool Get_Mbn_File_Index(CString cs_Mbn_Name, int * ip_Index);
	bool Judge_Ctrl_OP_Option(void);
	bool QLIB_INIT(int i_Com_Port);
	
	void QLIB_FREE(void);
	bool IS_PHONE_CONNECTED(void);
	bool CONFIG_TIME_OUT(unsigned long ul_TimeOut_ID, unsigned long ul_New_Value_mS);
	bool GET_PHONE_OP_MODE(unsigned long * ulp_Phone_Mode);
	bool IS_FTM_MODE(unsigned char * ucp_Is_FTM_Mode);
	bool DIAG_CONTROL_F(int i_Mode);
	bool FTM_SET_COMMAND_CODE(short s_Command_Code);
	bool Enter_FTM_Mode(void);
	bool Enter_Online_Mode(void);
	bool BACKUP_QCN_QLIB(MBN_FILE_S str_QCN_FILE);
	bool RESTORE_QCN_QLIB(MBN_FILE_S str_QCN_B_FILE, MBN_FILE_S str_QCN_A_FILE);
	bool WRITE_QCN_TO_MODULE(MBN_FILE_S str_QCN_FILE);
	
	bool Load_Nv_Definition(const wchar_t * wcp_Nv_Definition_File);
	bool Ping_DLoad_Port(int * ip_DLoad_Port, int * ip_DL_TYPE);
	void PKT_DMSS_CMD_VER(PKT_BUFF_S * strp_Pkt_Cmd);
	bool READ_BYTE(unsigned char ** ucp2_Buff_Pointer, unsigned long * ulp_Data_Length);
	bool QUERY_BYTE(unsigned char * ucp_Send_Data, unsigned long ul_Send_Quan, unsigned char ** ucp2_Read_Data, unsigned long * ulp_Read_Quan, int i_TimeOut);
	bool Switch_To_DL_Mode_In_AT(int i_TimeOut);
	void Get_Char_String(unsigned char * ucp_Buff_Data, unsigned long ul_Data_Quan, CString * csp_String_Data);
	void PKT_WRITE_DLOAD_FLAG_CMD(unsigned char uc_Flag, PKT_BUFF_S * strp_Pkt_Cmd);
	bool Send_Write_DLoad_Flag(unsigned char uc_Flag, int i_TimeOut);
	void COM_PORT_CHANGE_BAUDRATE(int i_Baud_Rate);
	bool READ_DMSS_HW_INFO(unsigned long * ulp_Max_Write_Size);
	bool Get_Streaming_Info(PKT_BUFF_S str_Pkt_Rec_Data, unsigned long * ulp_Max_Block_Size);
	
	bool Read_Nv_Max_Item_Max_Len_In_AT(int * ip_Max_Item, int * ip_Max_Len);
	bool BACKUP_QCN(MBN_FILE_S str_QCN_FILE);
	bool BACKUP_QCN_IN_AT(MBN_FILE_S str_QCN_FILE);
	bool Send_AT_In_AT(int i_TimeOut);
	bool Read_Nv_Item_Data_In_AT(int i_Item, int i_Index, char * cp_Item_Data, int * ip_Data_Quan);
	bool Read_Nv_Item_Params_In_AT(int i_Item, int * ip_Array_Size, int * ip_Item_Size);
	bool Get_AT_Nv_Item_Data(int * ip_Item, int * ip_Index, char * cp_Item_Data, int * ip_Data_Quan);
	void DEL_BYTE(unsigned long ul_Del_Quan);
	bool Get_AT_Nv_Item_Params(int * ip_Item, int * ip_P3, int * ip_P4);
	bool RESTORE_QCN(MBN_FILE_S str_QCN_FILE_1, MBN_FILE_S str_QCN_FILE_2);
	bool RESTORE_QCN_IN_AT(MBN_FILE_S str_QCN_FILE);
	bool Write_Nv_Item_Data_In_AT(int i_Item, int i_Index, const wchar_t * wcp_Item_Data);
	bool SCAN_NEW_COM_PORT(const int i_Tgt_Com_Port, int * ip_New_Com_Port, int i_TimeOut);


	bool LOAD_BB_INFO(const wchar_t * wcp_FW_File_Path);
	bool Send_Stream_Cmd_Open_Multi_Image_Cus_V1(const wchar_t * wcp_Mbn_Name);
	bool Send_Stream_Cmd_Open_Multi_Image_Cus_V2(const wchar_t * wcp_Mbn_Name);
	void PKT_STREAM_CMD_OPEN_MULTI_IMAGE_CUS_V1(PKT_BUFF_S * strp_Pkt_Cmd, const wchar_t * wcp_Mbn_Name);
	void PKT_STREAM_CMD_OPEN_MULTI_IMAGE_CUS_V2(PKT_BUFF_S * strp_Pkt_Cmd, const wchar_t * wcp_Mbn_Name);
	int DL_FW_V2(void);
	bool FB_DEVICE_V1();
	CString m_csCurr_Exe_Path;
	bool Switch_To_FB_Mode_In_AT();
	bool DL_FB_V1(void);
	bool COM_PORT_OPEN_CUS(int i_Com_Port, int i_BR);
	bool FB_DL_FILE(const MBN_FILE_S str_FB_File);
	bool FB_CONTINUE(void);
	bool Read_Reg_USB_AT_Port(int * ip_AT_Port_List, int * ip_Port_Quan);
	bool m_bFastboot_DL;
	bool Get_Yaffs2_File_Index(CString cs_Yaffs2_Name, int * ip_Index);
	bool Get_Img_File_Index(CString cs_Img_Name, int * ip_Index);
	bool Get_New_Dynamic_Com_Port(int * ip_New_Port_List, int * ip_New_Port_Quan, int i_TimeOut);
	bool Read_Reg_USB_DM_Port(int * ip_DM_Port_List, int * ip_Port_Quan);

	int DL_FW_V3(bool b_Reset_DUT);
	bool DL_Mbn_All_File_V3(unsigned long ul_Pkt_Data_Len);
	bool DL_FB_V3(bool b_Detect_ADB_DUT, bool b_Reboot_DUT);
	bool DL_FW_SBL2(void);
	bool FB_REBOOT(void);
	bool Update_USB_DL_Port(const int i_Old_Port, int * ip_New_Port);
	bool Send_Hex_Pkt_Data_V2(HEX_REAL_DATA_S * strp_Hex_Real_Data, const int i_Pkt_Length);
	bool SEND_PKT_V3(PKT_BUFF_S str_Pkt_Write, RSP_INFO_V2_S str_Rsp_Info_V2, int i_TimeOut);
	bool Send_Mbn_Pkt_Data_V2(PKT_DATA_ADDR_BUFF_S * strp_Mbn_Data_Addr, const int i_Pkt_Data_Len);
	int Execute_MDM6X00(void);
	int Execute_MDM9X15_V1(void);
	void LOAD_TEMP_FW_FILE(const wchar_t * wcp_Cur_Exe_File_Path);
	bool DL_Mbn_Partition_Table(void);

	bool LOAD_FW_FILE_V1(const wchar_t * wcp_FW_File_Path);
	bool LOAD_FW_FILE_V2(const wchar_t * wcp_FW_File_Path);
	bool LOAD_FW_FILE_V6(const wchar_t * wcp_FW_File_Path);
	bool LOAD_FW_FILE_V6_EXT(const wchar_t * wcp_FW_File_Path);
	
	bool Load_Partition_Xml_Data(const wchar_t * wcp_Xml_File, const wchar_t * wcp_Dir_Path);
	int Execute_MDM9X07(void);
	bool DL_FW_V4_MDM9x07(void);
	bool Ping_DLoad_Port_MDM9x07(int * ip_Cur_Status, int i_TimeOut);

	int Execute_MDM9X05(void);
	int DL_FW_V4_MDM9x05_Firehose(void);

	int Execute_MDM9X06(void);
	bool DL_FW_V4_MDM9x06(void);
	bool Ping_DLoad_Port_MDM9x06(int * ip_Cur_Status, int i_TimeOut);

	bool DL_FW_SAHARA_ALL(void);
	bool DL_FW_SAHARA_Mbn_File(int i_DL_Emerg_Norm_Type, unsigned long ul_Image_ID_First, unsigned long ul_Data_Offset_First, unsigned long ul_Data_Length_First);
	bool Query_SA_Read_Data_Packet(unsigned char * ucp_Cmd_Data, unsigned long ul_Cmd_Quan, int i_TimeOut, int * ip_Rsp_ID, unsigned long * ulp_Rsp_Image_ID, unsigned long * ulp_Rsp_Data_Offset, unsigned long * ulp_Rsp_Data_Length);
	bool Send_Mbn_Pkt_Data_V3(PKT_DATA_ADDR_BUFF_S * strp_Mbn_Data_Addr);
	bool DL_Mbn_File_V2(const MBN_FILE_S str_Mbn_File, unsigned long ul_Pkt_Data_Len);
	bool DL_FW_SAHARA_EMERG(void);


	bool FB_ERASE(const wchar_t * wcp_Erase_Cmd);
	int Execute_MDM9X15_V2(void);
	bool FB_DL_IN_LOCK_V1(bool b_Reset_DUT);
	bool FB_DL_IN_LOCK_V2(void);
	bool Reboot_ADB_And_Enter_DM(void);
	bool FB_DEVICE_V2(int * ip_Adb_Quan);
	void Count_Key_Word_Quan(const wchar_t * wcp_Src_Data, const wchar_t * wcp_Key_Word, int * ip_Key_Quan);
	bool Switch_Fastboot_Mode_In_Reset_Cmd(void);
	bool LOAD_FW_FILE_V3(const wchar_t * wcp_FW_File_Path);
	bool Switch_EDL(int * ip_New_QDLoader_Port);
	bool Update_USB_QDLoader_Port(int i_Old_Port, int * ip_New_QDLoader_Port);
	bool Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(int * ip_QDLoader_Port_List, int * ip_Port_Quan);
	
	int Execute_MSM8909(void);
	bool Send_EDL_Mode_Cmd(void);
	bool DL_FW_V5_MSM8909(int i_QDLoader_Port);
	bool QFIL_DL_Firehose_Programmer(int i_QDLoader_Port, const MBN_FILE_S str_Firehose_File);
	bool QFIL_DL_Rawprogram(int i_QDLoader_Port, const MBN_FILE_S str_Rawprogram_File);
	bool QFIL_DL_Patch(int i_QDLoader_Port, const MBN_FILE_S str_Patch_File);
	bool QFIL_DL_Reset(int i_QDLoader_Port);
	bool LOAD_FW_FILE_V4(const wchar_t * wcp_FW_File_Path);
	bool LOAD_FW_FILE_V5(const wchar_t * wcp_FW_File_Path);
	int Execute_MSM8937(void);
	int Execute_MSM8953(void);
	int Execute_SM6115(void);


	bool DL_FW_V5_MSM8937(int i_QDLoader_Port);
	bool DL_FW_V5_MSM8953(int i_QDLoader_Port);
	bool DL_FW_V5_SM6115(int i_QDLoader_Port);
	
	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	bool LOAD_FW_FILE_V2_EXT(const wchar_t * wcp_FW_File_Path);//MDM 9X40



	bool LOAD_FW_FILE_V7(const wchar_t * wcp_FW_File_Path);
	int Execute_MSM8917(void);
	bool DL_FW_V7_MSM8917(int i_QDLoader_Port);

	int Execute_MDM9X07_EXT(void);
	bool DL_FW_V2_MSM9X07(int i_QDLoader_Port);

	void Show_COMPORT(const wchar_t * wcp_Status);
	void Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN);
	bool QFIL_DL_Reset_Prepare(int i_QDLoader_Port);
	int DL_FW_V4_MDM9x06_Firehose(void);
	bool LOAD_FW_FILE_Sdm660(const wchar_t * wcp_FW_File_Path);
	int Execute_SDM660(void);
	bool DL_FW_SDX660(int i_QDLoader_Port);
	bool LOAD_FW_FILE_9X55(const wchar_t * wcp_FW_File_Path);
	bool DL_FW_MDM9X55_SAHARA(void);
	int Execute_MDM9X55_FIREHOSE(void);
	bool DL_FW_MDM9X55_FIREHOSE(int i_QDLoader_Port);
	int Execute_MDM9X55(void);
	bool Switch_EDL_Sub(void);
	//如果口消失认为是正常的
	bool Confirm_Dynamic_Com_Port_N(const int i_Tgt_Com_Port, int i_TimeOut);
	//SC66 在裸片下载FW前第一步需下载provision （SDM660 导入 UFS 存储）
	bool QFIL_DL_UFS_Provision(int i_QDLoader_Port, const MBN_FILE_S str_provision_File);
	//预留
	bool QFIL_DL_UFS_Provision_Check(int i_QDLoader_Port);

	bool QFIL_DL_Rawprogram_Ext(int i_QDLoader_Port, 
		const MBN_FILE_S str_Rawprogram_File,
		const MBN_FILE_S str_Rawprogram_File1,
		const MBN_FILE_S str_Rawprogram_File2,
		const MBN_FILE_S str_Rawprogram_File3,
		const MBN_FILE_S str_Rawprogram_File4,
		const MBN_FILE_S str_Rawprogram_File5);

	bool QFIL_DL_Patch_Ext(int i_QDLoader_Port, 
		const MBN_FILE_S str_Patch_File,
		const MBN_FILE_S str_Patch_File1,
		const MBN_FILE_S str_Patch_File2,
		const MBN_FILE_S str_Patch_File3,
		const MBN_FILE_S str_Patch_File4,
		const MBN_FILE_S str_Patch_File5);

	//兼容有设置第二端口的功能
	//1：找到第一个端口   2：成功找到第二个端口   0：标识没找到
	int Confirm_Dynamic_Com_Port_Ext(const int i_Tgt_Com_Port, const int i_Second_Tgt_Com_Port, int i_TimeOut);
	bool QFIL_DL_FHLoaderErase_CMD(int i_QDLoader_Port);

	//全擦
	bool QFIL_DL_FHLoaderErase(int i_QDLoader_Port);
	bool QFIL_DL_FHLoaderErase_Proc(int i_QDLoader_Port);
	void Detect_QDloader_Port();


	bool LOAD_FW_FILE_V2_MDM9X06(const wchar_t * wcp_FW_File_Path);
	bool LOAD_FW_FILE_Sdm845(const wchar_t * wcp_FW_File_Path);
	int Execute_SDM845(void);
	bool DL_FW_SDM845(int i_QDLoader_Port);
	bool QFIL_DL_SetactivePartition(int i_QDLoader_Port);
	bool Deactivate_MBN_In_AT(int i_TimeOut,CString strATCommand);
	bool List_Op_In_AT_Ext(int i_TimeOut,CString strATCommand);
	bool QMBN_Del_In_AT(int i_TimeOut,CString strATCommand);

	bool FindMbnFile(CString lpPath);
	bool Set_MBN_Sent_In_AT(int i_TimeOut,CString strATCommand);
	bool Qfdel_In_AT(int i_TimeOut,CString strATCommand);
	void OnSend_MBNFile();
	bool Send_File(CString csPath, BOOL bHEX, BOOL bEnter);
	bool Add_MBN_In_AT(int i_TimeOut,CString strATCommand);
	bool List_Op_In_AT(int i_TimeOut,CString strATCommand);
	bool Select_Op_In_AT(int i_TimeOut,CString strATCommand);

	int View_HEX_Char_To_Signed_Char(char *cpHEX, char *cpData, int &iBytes);
	bool Autosel_In_AT(int i_TimeOut, CString strATCommand);
	bool LOAD_FW_FILE_HONEYWELL(const wchar_t * wcp_FW_File_Path);
	int Execute_HONEYWELL(void);

	bool LOAD_FW_FILE_SM6115(const wchar_t * wcp_FW_File_Path);

	bool QFIL_DL_FHLoaderErase_Proc_1(int i_QDLoader_Port);
	bool QFIL_DL_FHLoaderErase_Proc_2(int i_QDLoader_Port);
	bool QFIL_DL_FHLoaderErase_Proc_3(int i_QDLoader_Port);
	bool QFIL_DL_FHLoaderErase_Proc_4(int i_QDLoader_Port);
	bool QFIL_DL_FHLoaderErase_Proc_5(int i_QDLoader_Port);

	bool Get_EID(void);
	//at+quimslot=2 OK
	//at+quimslot=1 OK
	bool Set_Quimslot(int iTag);
	//AT+QUIMSLOT? QUIMSLOT: 2
	//AT+QUIMSLOT? QUIMSLOT: 1
	bool Get_Quimslot(int iTag);

	//AT+CSIM=26,"00A4040008A000000151000000"
	//+CSIM: 4,"6184"
	bool Check_ESIM_1(void);

	//AT+CSIM=10,"80CA004200"
	//+CSIM: 48,"4214574C43535036382D65554943432D4D464FFFFFFF9000"
	bool Check_ESIM_2(void);

	bool Q_DL_QBhiServer();
	//检测统计存在UDE AT口
	bool Detect_PCIE_UDE_Port(int iTimeOut);
	bool _W_PCIE_Rescan(int i_TimeOut);
	bool Confirm_NO_PCIE_Port(int i_TimeOut);
	bool Confirm_PCIE_Port(int i_TimeOut);

	//检测是否存在PCIE DM口
	bool Detect_UDE_DM_Port();

	//检测统计存在UDE EDL口
	bool Confirm_PCIE_UDE_EDL_Port(int i_TimeOut);
	bool Detect_PCIE_UDE_EDL_Port(int iTimeOut);

	//检测统计存在UDE AT口
	bool Confirm_PCIE_UDE_AT_Port(int i_TimeOut);
	bool Detect_PCIE_UDE_AT_Port(int iTimeOut);

	bool Check_PN(void);
	bool Check_OS(void);

	bool LOAD_FW_FILE_QDM002(const wchar_t * wcp_FW_File_Path);
	int Execute_QDM002(void);
	bool LOAD_FW_FILE_V3_Ext(const wchar_t * wcp_FW_File_Path);

	int QUALCOMM_PCIE_EFUSE(void);
	bool QFIL_DL_Rawprogram_Efuse(int i_QDLoader_Port, const MBN_FILE_S str_Rawprogram_File);
	bool QFIL_DL_Patch_Efuse(int i_QDLoader_Port, const MBN_FILE_S str_Patch_File);

	int Execute_QDM002_bootloader(void);
	int Execute_QDM002_app_bin(void);

	int Do_Execute_SPC(void);
	int Do_Execute_SPC_CRK(void);
	int Do_Execute_SPC_FW(void);

	bool QFIL_DL_Getstorageinfo(int i_QDLoader_Port);

	bool LOAD_FW_FILE_QCM6125(const wchar_t * wcp_FW_File_Path);
	int Execute_QCM6125(void);
	bool DL_FW_V5_QCM6125(int i_QDLoader_Port);

	//检测是否存在UDE DM口
	bool Detect_UDE_DM_Port_Ext();
	//AT+QWSETMAC="xxxxxx"
	bool Set_QWSETMAC(void);
	bool Get_QWSETMAC(void);
	bool CreatQDE007_Jlink_CMD(void);
	int Execute_QDE007_Danlow_Jlink(void);

	int Execute_QDM002_DL_Jlink(void);

	BOOL EnumAllDevice();

	// check port 是否存在
	bool Check_Port(void);
	int Execute_SM4350(void);
	bool DL_FW_V5_SM4350(int i_QDLoader_Port);
	bool LOAD_FW_FILE_SM4350(const wchar_t * wcp_FW_File_Path);

	//AT+QCFG="usbnet",0  返回的是：OK
	bool Set_RMNET(void);
	//AT+QCFG="usbnet"   返回的是：+QCFG:"usbnet",0 OK
	bool Get_RMNET(void);

	bool LOAD_FW_FILE_QDE006(const wchar_t * wcp_FW_File_Path);
	int Execute_QDE006_MCU2(void);

	int DL_FW_ESP32_MCU(void);

	CString  m_strESP32_main_path;
	CString  m_straudio_esp_path;
	CString m_strESP32_Bootloader_Path;
	CString m_strESP32_PartitionTable_Path;
	CString m_strESP32_OtaDataInitial_Path;
	CString m_strESP32_NetworkAdapter_Path;
	CString m_strESP32_HaraldUpgrader_Path;
	int Execute_DL_Jlink(CString str_JlinkFolderName);
	bool Creat_DL_Jlink_CMD(CString str_JLinkFolderName, CString str_DeviceName, CString str_FlashAddress_01, CString str_FlashAddress_02, CString str_FWPath_01, CString str_FWPath_02);
	void TraversFolderFile(CString str_TargetPath, CString str_FileMark, CString & strFilePath);
	CString m_csScat_Cfg_File_Path;

	bool Load_MCU_File();
	bool Creat_DL_Jflash_CMD(CString str_JlinkFolderName, CString str_DeviceName, CString str_FlasgAddress, CString str_FWPath);
	int Execute_DL_Jflash(CString str_JflashFolderName, CString str_JflashName);
	int DL_FW_ESP32_MCU_Smart(struct struct_ESP32_FileAddress * esp32_Struct);
	CString str_ESP32_File_All;
	bool ESP32_Setting();
	int Erase_FW_ESP32_MCU();
	int Execute_DL_JTAG(CString str_JTAG_CMD_FolderName);
	int Modify_FileContent(CString str_TargetFile, CString str_NeedMidfyLocationMark, CString str_RepaceText);
	int Execute_DL_FLHubTool(CString str_FLHubTool_FolderName);
	int DL_FW_MCU_All();
	void TraversFolderFile_ESP32(CString str_TargetPath, CString str_FileMark, CString & strFilePath);
	int Execute_DL_CCSLOAD(CString str_ToolFolderName, CString str_FW_FullPathName);
	int Only_Run_Cmd_NoReturnData(CString str_Tool_FullPathName, CString str_Command);
	CString GetExeFilePath();
	int Flash_SunmiCfg(CString str_ToolFullPathName, CString str_SunmiCfgFullPathName);
	int Sunmi_Install_Crp(CString str_ToolFullPathName, CString str_SigCrpFile_FullPathName);
	int UpdateRomSetting();
	bool Call_QDC519_SunmiDownloader(CString str_key, CString str_boot, CString str_cfg, CString str_os);
	int iQDC519_Port1;
	int iQDC519_Port2;
	int iQDC519_Port3;
	int iQDC519_Port4;
	int GetSunmiDLResult();
	int Execute_DL_PICkit(CString str_PICkitFolderName, CString str_ToolShortName, CString str_DeviceName, CString & strBackData);
	int m_iBBchip_Curr;
	int QDE017_PushBinFile();
	CString m_str_QDE017_Binfile;
	int Xmodem_transferfiles(CString str_ToolFolderName);
	int QWinExplorer_Run_Result(CString str_Action_Title, CString str_ToolPathName, CString str_Command, CString str_ReturnOK_Mark1, CString str_ReturnOK_Mark2);
	int FileAppendStr(CString strFileFullPathName, CString str_AppendStr);
	CString str_InputSN;
	int ComPort_SendCmd(CString str_Show, int iport, CString str_cmd, CString str_returnData);
	int COM_Modem_PushBin(CString str_ShowLog, int iPort, CString str_BaudRate, CString str_TransportProtocol);
	int CheckPushBinVer(int iFirst);
	CString COM_Query_Result(CString str_ShowLog, int iport, int iBaudRate, int iDTSRTS_Mode, CString str_Cmd);
	int bk_loader_Image(CString str_FW_PathFullName);
	int Run_QDM025_FastbootCMD();
	int FRDownload_Image(int iComNum, int iBaud_Rate, CString str_FW_PathFullName, CString str_chip_type);
	int Only_Run_Cmd_NoReturnData_QDM548(CString str_Tool_FullPathName, CString str_Command);
	int SendComCmd(CString strTitel, int iPort, int iBaudRate, bool IsEnDTR, bool IsEnRTS, CString str_cmd, CString strExpected);
	int Check_SSH(CString str_Titel, CString str_sshCmd, CString str_sshHostname, CString str_sshUsername, CString str_sshPassword, CString& str_sshReturnData, bool bNeedReturnData = true);
	int QDM046_CustomerFlowTest();
	int Check_SSH_NOReturnData(CString str_Titel, CString str_sshCmd, CString str_sshHostname, CString str_sshUsername, CString str_sshPassword);
	CString Get_Md5_String(CString str);
	CString CStringHexSum(CString strHex1, CString strHex2, int iByteNum);
	CString str_DB_R3_Mac;
	CString m_csMCU_Name2;
	int QDM055_CustomerFlowTest();
	int QDM055_QCX_Flashtool(CString str_Dev_Type);
	int Write_Check_Lock_SerialNumber();
	bool RunComCmd(CString strTitel, int iPort, int iBaudRate, bool IsEnDTR, bool IsEnRTS, CString str_cmd, CString strExpected, int iTimeout);
	CString m_DB_IMEI;
	CString m_DB_Barcode;
	int QCX_Aware_Service_Provisioning(CString str_Environment);
	bool QCX_Aware_Service_Provisioning_RunScript(CString str_ScriptFullPath, CString Aware_Configs_Folder, CString Registration_CSV_File, CString Environment_String, CString str_ProvisioningCsvLog_NameFullPath, CString strCom);
	bool DeleteDirectory(CString DirName);
	int QCX_216_ShutDown();
};
