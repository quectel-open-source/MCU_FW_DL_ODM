/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001
*
*******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *     download.h
 *
 * Project:
 * --------
 *   MTK6208 DOWNLOAD AGENT
 *
 * Description:
 * ------------
 *   This file is intends for download agent specific definition
 *
 * Author:
 * -------
 *  Jensen Hu
 *
 *******************************************************************************/
#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#include "mtk_status.h"

/* DA Version */
#define DA_MAJOR_VER            0x03
#define DA_MINOR_VER            0x02

/* Max Number of Load Sections */
#define MAX_LOAD_SECTIONS		35

// Max Die Number of MCP
#define MAX_DIE_IN_MCP          2

/* RETURN VALUE */
#define SOC_OK                  0xC1
#define SOC_FAIL                0xCF
#define SYNC_CHAR               0xC0
#define CONT_CHAR               0x69
#define STOP_CHAR               0x96
#define ACK                     0x5A
#define NACK                    0xA5
#define UNKNOWN_CMD             0xBB
#define FLUSH_CONT              0xE1
#define FLUSH_DONE              0xE2

//
// For any extented DA COMMANDs, please use "0x60" as the first command byte
//
#define DA_EXTEND_CMD       0x60

//
//  SV5 only DA COMMANDs
//
#define DA_DOWNLOAD_BLOADER_CMD     0x51
#define DA_GET_CBR_SETTTING_CMD         0x52
#define DA_CREATE_CBR_CMD               0x53
#define DA_CHK_PC_SECURITY_VER_CMD      0x54
#define DA_FORMAT_HB                             0x55
#define DA_FORMAT_CBR                           0x56      // not support via DA anymore!
#define DA_FORMAT_CBR_RECORD            0x57
#define DA_CALCULATE_CBR_BLOCK_INFO         0x58
#define DA_SWITCH_USB_2_0               0x59
#define DA_MOUNT_SF_TABLE               0x5A
#define DA_ERASE_MAUI_INFO           0x5B
#define DA_WRITE_PARTITION_TBL_CMD   0x5C
#define DA_READ_PARTITION_TBL_CMD    0x5D
#define DA_READ_BMT                  0x5E
#define DA_DL_LINUX_RAW_DATA       0x5F

#define DA_DL_LINUX_YAFFS_DATA     0x70
#define DA_BACKUP_AND_ERASE_HB     0x71
#define DA_RESTORE_HB              0x72
#define DA_DL_AND_ENCRYPT_LINUX_RAW_DATA 0x73


#define DA_GET_EFUSE_BACKUP_DATA    0X75
#define DA_ENTER_GPS_RELAY_MODE_CMD     0x76


//
//  SV3/SV5 share DA COMMANDs
//
#define DA_NOR_BLOCK_INDEX_TO_ADDRESS   0xB0
#define DA_NOR_ADDRESS_TO_BLOCK_INDEX   0xB1
#define DA_NOR_WRITE_DATA               0xB2
#define DA_NAND_WRITE_DATA              0xB3
#define DA_SECURE_USB_RECHECK_CMD       0xB4
#define DA_SECURE_USB_DECRYPT_CMD       0xB5
#define DA_BL_FEATURE_CHECK_CMD         0xB6
// 0xB7 is empty now
#define DA_FORMAT_FAT_CMD               0xB8
#define DA_CLEAR_POWERKEY_IN_META_MODE_CMD  0xB9
#define DA_RESET_TARGET_TO_NORMAL_MODE_FLAG_CMD 0xBA
#define DA_SPK_CMD                      0xBC
#define DA_NAND_WRITE_BLOCK             0xBD
#define DA_READ_BL_SHARED_DATA_CMD      0xBE
#define DA_SET_NORMAL_RESET_FLAG_CMD    0xBF
#define DA_NAND_WRITE_PAGES_WITHIN_BLOCK    0xC0
#define DA_ENABLE_BOOT_CMD              0xC1
#define DA_USB_SPEED_CMD                       0xC2
#define DA_GET_DIDR                          0xC3
#define DA_GET_SDS_INFO_CMD             0xC4
#define DA_EMMC_PARTITION_CMD                0xC5
#define DA_ERASE_FIRST_BLOCK            0xC6
#define DA_NOR_ERASE_SET_SAFE_MODE      0xC7
#define DA_NOR_FAT_RAW_WRITE            0xC8
#define DA_CLEAR_LONG_PRESS_PWKEY_CMD   0XC9
#define DA_ENABLE_LOGGING_CHANNEL       0xCA




#define DA_CONFIG_EMI                   0xD0
#define DA_POST_PROCESS                 0xD1
#define DA_SPEED_CMD                    0xD2
#define DA_MEM_CMD                      0xD3
#define DA_FORMAT_CMD                   0xD4
#define DA_WRITE_CMD                    0xD5
#define DA_READ_CMD                     0xD6
//#define DA_WRITE_REG16_CMD            0xD7
//#define DA_READ_REG16_CMD             0xD8
#define DA_FINISH_CMD                   0xD9
#define DA_GET_DSP_VER_CMD              0xDA
#define DA_ENABLE_WATCHDOG_CMD          0xDB
#define DA_NFB_WRITE_BLOADER_CMD        0xDC
#define DA_NAND_IMAGE_LIST_CMD          0xDD
#define DA_NFB_WRITE_IMAGE_CMD          0xDE
#define DA_NAND_READPAGE_CMD            0xDF
#define DA_CHK_PC_SEC_INFO_CMD          0xE0
#define DA_UPDATE_FLASHTOOL_CFG_CMD    0xE1
#define DA_CUST_PARA_GET_INFO_CMD       0xE2
#define DA_CUST_PARA_READ_CMD           0xE3
#define DA_CUST_PARA_WRITE_CMD          0xE4
//[SV3]
#define DA_SEC_RO_GET_INFO_CMD          0xE5
#define DA_SEC_RO_READ_CMD              0xE6
#define DA_SEC_RO_WRITE_CMD             0xE7
//[SV5]
#define DA_CBR_RECORD_GET_LENGTH_CMD    0xE5
#define DA_CBR_RECORD_READ_CMD          0xE6
#define DA_CBR_RECORD_WRITE_CMD         0xE7

#define DA_ENABLE_DRAM                  0xE8
#define DA_OTP_CHECKDEVICE_CMD          0xE9
#define DA_OTP_GETSIZE_CMD              0xEA
#define DA_OTP_READ_CMD                 0xEB
#define DA_OTP_PROGRAM_CMD              0xEC
#define DA_OTP_LOCK_CMD                 0xED
#define DA_OTP_LOCK_CHECKSTATUS_CMD   0xEE
#define DA_GET_PROJECT_ID_CMD           0xEF
#define DA_GET_FAT_INFO_CMD             0xF0
#define DA_FDM_MOUNTDEVICE_CMD          0xF1
#define DA_FDM_SHUTDOWN_CMD         0xF2
#define DA_FDM_READSECTORS_CMD          0xF3
#define DA_FDM_WRITESECTORS_CMD     0xF4
#define DA_FDM_MEDIACHANGED_CMD     0xF5
#define DA_FDM_DISCARDSECTORS_CMD       0xF6
#define DA_FDM_GETDISKGEOMETRY_CMD      0xF7
#define DA_FDM_LOWLEVELFORMAT_CMD       0xF8
#define DA_FDM_NONBLOCKWRITESECTORS_CMD     0xF9
#define DA_FDM_RECOVERABLEWRITESECTORS_CMD    0xFA
#define DA_FDM_RESUMESECTORSTATES               0xFB
#define DA_NAND_EXTRACT_NFB_CMD             0xFC
#define DA_NAND_INJECT_NFB_CMD                  0xFD
#define DA_MEMORY_TEST_CMD                  0xFE
#define DA_ENTER_RELAY_MODE_CMD             0xFF


#define DA_PRE_DL_CMD             0x90
#define DA_BR_DL_CMD             0x91
#define DA_MR_DL_CMD             0x92
#define DA_POST_DL_CMD             0x93
#define DA_GET_FLASHLAYOUT_INFO_CMD    0x94
#define DA_LOAD_FLASHLAYOUT_INFO_CMD    0x95

typedef enum {
    GPS_OPERATION_DOWNLOAD,
    GPS_OPERATION_FORMAT,
    GPS_OPERATION_READBACK
}GPS_OPERATION;

/* SPEED_PARA */
typedef enum {
    UART_BAUD_921600    = 0x01,
    UART_BAUD_460800    = 0x02,
    UART_BAUD_230400    = 0x03,
    UART_BAUD_115200    = 0x04,
    UART_BAUD_57600     = 0x05,
    UART_BAUD_38400     = 0x06,
    UART_BAUD_19200     = 0x07,
    UART_BAUD_9600      = 0x08,
    UART_BAUD_4800      = 0x09,
    UART_BAUD_2400      = 0x0a,
    UART_BAUD_1200      = 0x0b,
    UART_BAUD_300       = 0x0c,
    UART_BAUD_110       = 0x0d
}UART_BAUDRATE;

typedef enum {
     USB_COM_PORT_CONTROL_NO_OPERATION                   = 0x00000000
    ,USB_COM_PORT_CONTROL_REMOVE_PORT_UPON_DISCONNECTION = 0x00000001
    ,USB_COM_PORT_CONTROL_END                            = (int)0xFFFFFFFF
} USB_COM_PORT_CONTROL;

typedef enum {
     CS0 = 0
    ,CS1
    ,CS2
    ,CS3
    ,CS4
    ,CS5
    ,CS6
    ,CS7
    ,CS_WITH_DECODER
    ,MAX_CS = CS_WITH_DECODER
    ,HW_CHIP_SELECT_END
} HW_ChipSelect_E;

typedef enum {
     HW_STORAGE_NOR = 0
    ,HW_STORAGE_NAND
    ,HW_STORAGE_EMMC
    ,HW_STORAGE_TYPE_END
} HW_StorageType_E;

typedef enum {
     HW_RAM_UNKNOWN = 0
    ,HW_RAM_SRAM
    ,HW_RAM_DRAM
    ,HW_RAM_TYPE_END
} HW_RAMType_E;

typedef enum {
     HW_MEM_NOR = 0         // NOR Flash
    ,HW_MEM_NAND            // NAND Flash
    ,HW_MEM_EXT_SRAM        // External SRAM
    ,HW_MEM_EXT_DRAM        // External DRAM
    ,HW_MEM_TYPE_END
} HW_MemoryType_E;

typedef enum {
     HW_MEM_IO_8BIT = 0     //  8-Bits Memory I/O
    ,HW_MEM_IO_16BIT        // 16-Bits Memory I/O
    ,HW_MEM_IO_32BIT        // 32-Bits Memory I/O
    ,HW_MEM_IO_TYPE_END
} HW_MemoryIO_E;

typedef enum {
     HW_MEM_DUMP = 0        // Memory Dump (Warning: it's not a test scenario!)
    ,HW_MEM_PATTERN_TEST    // Pattern Test Scenario
    ,HW_MEM_INC_DEC_TEST    // Increment/Decrement Test Scenario
    ,HW_MEM_ADDR_BUS_TEST   // EMI Address Bus Test Scenario
    ,HW_MEM_DATA_BUS_TEST   // EMI Data Bus Test Scenario
    ,HW_MEM_IO_BUS_TEST     // NFI I/O Bus Test Scenario
    ,HW_MEM_TEST_TYPE_END
} HW_MemoryTestMethod_E;

typedef enum {
    NUTL_READ_PAGE_SPARE = 0          /* Read page and spare with ECC disabled, do not skip bad block */
    ,NUTL_READ_PAGE_ONLY              /* Read page only with ECC enabled, do not skip bad block */
    ,NUTL_READ_SPARE_ONLY             /* Read spare only with ECC enabled, do not skip bad block */
    ,NUTL_READ_PAGE_WITH_ECC          /* Read page only with ECC enabled, skip bad bock */
    ,NUTL_VERIFY_AFTER_PROGRAM        /* Read page only with ECC enabled, do not skip bad block */
    ,NUTL_LOGICAL_BMT_READ_PAGE_SPARE /* Logical BMT Read page + spare */
    ,NUTL_LOGICAL_BMT_READ_PAGE_ONLY  /* Locgical BMT Read page */
    ,NUTL_READ_PAGE_SPARE_WITH_ECC_BYPASS_BAD_BLOCK_CHECK /* Read page and spare with ECC enabled, do not skip bad block */
	,NUTL_READ_FLAG_END
} NUTL_ReadFlag_E;

typedef enum {
     NUTL_ERASE = 0
    ,NUTL_FORCE_ERASE
    ,NUTL_MARK_BAD_BLOCK
    ,NUTL_ERASE_FLAG_END
} NUTL_EraseFlag_E;

typedef enum {
     NUTL_PROGRAM_PAGE_SPARE = 0
    ,NUTL_PROGRAM_PAGE_ONLY
    ,NUTL_PROGRAM_SPARE_ONLY
    ,NUTL_PROGRAM_PAGE_ONLY_PARTIAL_SPARE
    ,NUTL_PROGRAM_PAGE_FDM
    ,NUTL_PROGRAM_FLAG_END
} NUTL_ProgramFlag_E;

//----------------------------------------

// NOR Flash Callback Function group id
typedef enum {
     ID_DUMMY_CMD = 0
    ,ID_AMD_CMD_CB_UNLOCK_BYPASS_PGM
    ,ID_AMD_CMD_CB_MIRRORBIT_BUF_PGM
    ,ID_AMD_CMD_CB_MIRRORBIT_V2_BUF_PGM
    ,ID_AMD_CMD_CB_S29WS_MIRRORBIT_BUF_PGM
    ,ID_AMD_CMD_CB_S29N_MIRRORBIT_BUF_PGM
    ,ID_AMD_CMD_CB_S29GL_MIRRORBIT_BUF_PGM
    ,ID_AMD_CMD_CB_WORD_PGM
    ,ID_AMD_AM29PDL128G_CMD_CB_UNLOCK_BYPASS_PGM
    ,ID_TOSHIBA_CMD_CB_BUF_PGM
    ,ID_INTEL_CMD_CB_WORD_PGM
    ,ID_INTEL_CMD_CB_32WORD_BUF_PGM
    ,ID_INTEL_CMD_CB_SIBLEY_BUF_PGM
    ,ID_RENESAS_CMD_CB_WORD_PGM_64Mb
    ,ID_RENESAS_CMD_CB_128WORD_PAGE_PGM_64Mb
    ,ID_RENESAS_CMD_CB_WORD_PGM
    ,ID_RENESAS_CMD_CB_128WORD_PAGE_PGM
    ,ID_SHARP_CMD_CB_WORD_PGM
    ,ID_SHARP_CMD_CB_16WORD_BUF_PGM
    ,ID_SHARP_CMD_CB_WORD_PGM_NO_UNLOCK
    ,ID_SST_CMD_CB_WORD_PGM
    ,ID_SAMSUNG_CMD_CB_UNLOCK_BYPASS_PGM
    ,ID_TOSHIBA_CMD_CB_PAGE_8WORDS_PGM
    ,ID_SAMSUNG_CMD_CB_32WORD_BUF_PGM
        // Serial Flash
    ,ID_SST_CMD_CB_BUF_PGM
    ,ID_MXIC_CMD_CB_BUF_PGM
    ,ID_EON_CMD_CB_BUF_PGM
    ,ID_WINBOND_CMD_CB_BUF_PGM
    ,ID_FIDELIX_CMD_CB_BUF_PGM
    ,ID_MICRON_CMD_CB_BUF_PGM

    ,ID_NOR_CALLBACK_UNKNOWN = 0xFFFF
} NOR_CMD_Callback_ID_E;

// NAND Flash Callback Function group id
typedef enum {
     ID_COMMON_CB_FUNC_SET = 0
    ,ID_COMMON_CB_FUNC_SET_WITHOUT_COPYBACK
    ,ID_ST_CB_FUNC_SET
    ,ID_ST_CB_FUNC_SET_WITHOUT_COPYBACK
    ,ID_superAND_CB_FUNC_SET

    ,ID_NAND_CALLBACK_UNKNOWN = 0xFFFF
} NAND_CMD_Callback_ID_E;

//----------------------------------------

// NOR flash device id
typedef enum {
     NOR_AM29DL323D = 0
    ,NOR_AM29DL640D
    ,NOR_ST_M29DW640D
    ,NOR_AM29DL322GT
    ,NOR_MB84VD23280FA
    ,NOR_AM29PDL128G
    ,NOR_MB84VD22280FE
    ,NOR_MB84VP24491HK
    ,NOR_AM50DL128CG
    ,NOR_AM49DL3208GB
    ,NOR_ST_M74DW66500B
    ,NOR_INTEL_28F128L30_B
    ,NOR_K5A3280YT
    ,NOR_TH50VPF6782AASB
    ,NOR_TH50VPF6783AASB
    ,NOR_TV00578002AABD
    ,NOR_TV00578003AABD
    ,NOR_MB84VP24581HK
    ,NOR_INTEL_28F640W30_B
    ,NOR_AM49PDL127BH
    ,NOR_AM49PDL129BH
    ,NOR_M6MGD13BW66CDG
    ,NOR_W19B322TM
    ,NOR_W19B323TM
    ,NOR_KAD060300B
    ,NOR_K5J6316CTM
    ,NOR_TH50VPF5682CDSB
    ,NOR_TH50VPF5683CDSB
    ,NOR_IS75V16F128GS32
    ,NOR_M6MGT64BM34CDG
    ,NOR_ST_M30L0T7000T0
    ,NOR_ST_M30L0T7000B0
    ,NOR_SHARP_LRS1862
    ,NOR_SHARP_LRS1806A
    ,NOR_AM49DL3208GT
    ,NOR_S29PL032J
    ,NOR_SHARP_LRS1828C
    ,NOR_M6MGB64BM34CDG
    ,NOR_SHARP_LRS18B0
    ,NOR_SHARP_LRS1863
    ,NOR_S71AL016D_T
    ,NOR_S71AL016D_B
    ,NOR_SHARP_LRS18C8A
    ,NOR_INTEL_28F640L18_T
    ,NOR_INTEL_28F128L18_T
    ,NOR_INTEL_28F256L18_T
    ,NOR_INTEL_28F640L18_B
    ,NOR_INTEL_28F128L18_B
    ,NOR_INTEL_28F256L18_B
    ,NOR_INTEL_28F640L30_T
    ,NOR_INTEL_28F128L30_T
    ,NOR_INTEL_28F256L30_T
    ,NOR_INTEL_28F640L30_B
    ,NOR_INTEL_28F256L30_B
    ,NOR_INTEL_28F320W30_T
    ,NOR_INTEL_28F320W30_B
    ,NOR_INTEL_28F640W30_T
    ,NOR_INTEL_28F128W30_T
    ,NOR_INTEL_28F128W30_B
    ,NOR_INTEL_28F320W18_T
    ,NOR_INTEL_28F320W18_B
    ,NOR_INTEL_28F640W18_T
    ,NOR_INTEL_28F640W18_B
    ,NOR_INTEL_28F128W18_T
    ,NOR_INTEL_28F128W18_B
    ,NOR_M6MGD15BM34CDG
    ,NOR_S71PL254J
    ,NOR_TV0057A002AABD
    ,NOR_TV0057A003AABD
    ,NOR_W19B320ATB
    ,NOR_W19B320ATT
    ,NOR_S7_SV7E160XT
    ,NOR_S7_SV7E160XB
    ,NOR_S7_SV7E320XT
    ,NOR_S7_SV7E320XB
    ,NOR_S71GL032R3_T
    ,NOR_S71GL032R4_B
    ,NOR_S71GL032R1R2
    ,NOR_FM91L03208UA
    ,NOR_TV00569002BABD
    ,NOR_TV00569003BABD
    ,NOR_TV00569002AABD
    ,NOR_TV00569003AABD
    ,NOR_INTEL_38F1010C0ZBL0
    ,NOR_INTEL_28F1602C3BD70
    ,NOR_TY0068B012APGG
    ,NOR_TY0068B013APGG
    ,NOR_S71PL127N
    ,NOR_S7_SV6D2832UTA
    ,NOR_S7_SV6D2832UBA
    ,NOR_S7_SV6C2832UTA
    ,NOR_S7_SV6C2832UBA
    ,NOR_SHARP_LH28F16
    ,NOR_TV00578002DABD
    ,NOR_TV00578003DABD
    ,NOR_S71PL256N
    ,NOR_S71PL129N
    ,NOR_INTEL_PF48F50xxM0x0xx
    ,NOR_INTEL_PF48F50xxM0x1xx
    ,NOR_INTEL_PF48F40xxM0x0xx
    ,NOR_INTEL_PF48F40xxM0x1xx
    ,NOR_SST34HF16x1    // 1=Bottom
    ,NOR_SST34HF32x4    // 4=Top
    ,NOR_SHARP_LRS18BK
    ,NOR_INTEL_28F3204C3TD70
    ,NOR_S29WS128N
    ,NOR_ST_M58PPR256J
    ,NOR_ST_M58PPR512J
    ,NOR_SST32HF3241C
    ,NOR_S29WS512P
    ,NOR_S29NS064N
    ,NOR_S29NS128N
    ,NOR_S29NS256N
    ,NOR_S29WS256N
    ,NOR_S29NS128P
    ,NOR_S29NS256P
    ,NOR_S29NS512P
    ,NOR_S29GL128N_T
    ,NOR_S29GL128N_B
    ,NOR_S29GL256N_T
    ,NOR_S29GL256N_B
    ,NOR_S29GL512N_T
    ,NOR_S29GL512N_B
    ,NOR_K5L2731CAM
    ,NOR_K5L2931CAM
    ,NOR_S71PL127J
    ,NOR_TY00689002APGN
    ,NOR_S29GL064A_T
    ,NOR_S29GL064A_B
    ,NOR_S29WS128P
    ,NOR_S29WS256P
    ,NOR_TV00560002DDGB
    ,NOR_ST_M58WR016QT
    ,NOR_ST_M58WR016QB
    ,NOR_ST_M58WR032QT
    ,NOR_ST_M58WR032QB
    ,NOR_INTEL_PF38F60xxM0x0xx
    ,NOR_INTEL_PF38F60xxM0x1xx
    ,NOR_S29NS016J
    ,NOR_S29NS032J
    ,NOR_S29NS064J
    ,NOR_S29NS128J
    ,NOR_ST_M58WR064HU
    ,NOR_ST_M58WR064HL
    ,NOR_TV00560002EDGB
    ,NOR_TV00560003EDGB
    ,NOR_INTEL_PF38F30xxM0x0xx
    ,NOR_INTEL_PF38F30xxM0x1xx
    ,NOR_SHARP_LRS18CK
    ,NOR_TY00670002APGN
    ,NOR_A82DL3228T
    ,NOR_A82DL3228U
    ,NOR_A82DL3238T
    ,NOR_A82DL3238U
    ,NOR_A82DL3248T
    ,NOR_A82DL3248U
    ,NOR_K5L6331CAA
    ,NOR_KAL5563CAM
    ,NOR_EN29PL032
    ,NOR_EN29PL064
    ,NOR_SHARP_LRS18D5_D7
    ,NOR_SHARP_LRS18A7A
    ,NOR_K5L3316CAM
    ,NOR_SHARP_LRS18R
    ,NOR_TC58FVM7TDD
    ,NOR_TC58FVM7BDD
    ,NOR_TC58FYM8T7D
    ,NOR_TC58FYM8B7D
    ,NOR_K5L2833ATA
    ,NOR_ST_M58WR064KT
    ,NOR_M36C0W6050T0
    ,NOR_M36C0W5030T0
    ,NOR_M36L0T8060T3ZAQ
    ,NOR_TC58FYM8T8D
    ,NOR_TC58FYM8B8D
    ,NOR_S29WS128R_T
    ,NOR_S29WS128R_B
    ,NOR_S29WS128R
    ,NOR_S29WS256R_T
    ,NOR_S29WS256R_B
    ,NOR_S29WS256R
    ,NOR_S29WS512R_T
    ,NOR_S29WS512R_B
    ,NOR_S29WS512R
    ,NOR_S29WS01GR_T
    ,NOR_S29WS01GR_B
    ,NOR_S29WS01GR
    ,NOR_S29VS128R_T
    ,NOR_S29VS128R_B
    ,NOR_S29VS256R_T
    ,NOR_S29VS256R_B
    ,NOR_EN29GL128
    ,NOR_TC58FYM7T8C
    ,NOR_K5N2833ATB
    ,NOR_K5N5666ATB
    ,NOR_MX29GL128E
    ,NOR_K5N5629ABA
    ,NOR_K5L6433ATM
    ,NOR_K5L6433ABM
    ,NOR_EN29NS128
    ,NOR_K5N6433ATM
    ,NOR_EN71GL064B0
    ,NOR_K5N6433ABM
    ,NOR_M58LR128KC
    ,NOR_K5N6433ATB
    ,NOR_MX29GL640E_H_L
    ,NOR_MX29NS640E
    ,NOR_S29VS064R_T
    ,NOR_M58WR032KU
    ,NOR_W71VS128CDC1
        //Serial Flash - SST family
    ,SF_SST26VF016
    ,SF_SST26VF032
    ,SF_SST26VF064
    ,SF_SST26WF080
    ,SF_SST26WF016
    ,SF_SST26WF032
    ,SF_SST26WF064
    ,SF_MX25U8035E
    ,SF_MX25U1635E
    ,SF_MX25U3235E
    ,SF_MX25U6435E
    ,SF_EN25S32
    ,SF_W25Q32DW
    ,SF_W25Q64DW
    ,SF_FM25Q32
    ,SF_W25Q64CV
    ,SF_W25Q32BV
    ,SF_EN25S64
    ,SF_W25Q16DW
    //testing
    ,SF_GD25Q16
    ,SF_GD25Q32
    ,SF_GD25Q64
    ,SF_W25Q16CV
    ,SF_N25Q64
    ,SF_W25Q128CV
    ,SF_GD25LQ16
    ,SF_GD25LQ32
    ,SF_GD25LQ64
    ,SF_N25W128
    ,SF_MX25U12835FZNI_10G
    ,SF_MX25L12835EMI_10G
    ,SF_N25W32
    ,SF_N25W64
    ,SF_N25Q256
    ,SF_GD25LQ128
    ,SF_N25W256
    ,SF_W25Q256FV
    ,SF_MX25L25639FZ4W_08G
    ,SF_W25Q128FW
    ,SF_F25D64QA
    ,SF_FM25M64A
    ,SF_S99FL128S0
    ,SF_GD25LQ256
    ,SF_MX25U25639FZ4I_10G

    // MT6290 DA support only
	,SF_MXIC_MX25L8006E
	,SF_MXIC_MX25L1605D
	,SF_MXIC_MX25L3205D
	,SF_MXIC_MX25L6405D
	,SF_MXIC_MX25L12805DAND45E
	,SF_MXIC_MX25L25735E
	,SF_WNBD_W25X16
	,SF_WNBD_W25X32
	,SF_WNBD_W25Q128BV
	,SF_Spansion_S25FL032P
	,SF_Spansion_S25FL064A
	,SF_Spansion_S25FL128PAND129P
	,SF_ST_M25P32
	,SF_ST_M25P64
	,SF_ST_M25P128
	,SF_EON_EN25Q128
	,SF_SST_SST25VF080B
    ,SF_MXIC_MX25U12835F
    // end of MT6290 DA support only
    ,SF_MX25L8006E
    ,SF_MD25D80
    ,SF_MX25L3291FWJI_09
    ,SF_W25Q32JV
    ,SF_FM25M4AA
    ,SF_MX25R8035F
    ,SF_W25Q80EW
    ,SF_MX25R3235F
    ,SF_FM25M64C
    ,SF_MX25R1635F

    ,NOR_LAST
    ,NOR_DISPLAY_STRING_SV5X   = 0xFFFE
    ,NOR_UNKNOWN = 0xFFFF       // Unknown Device
} NOR_DeviceID_E;

// NAND flash device id
typedef enum {
     NAND_K9F5608Q0C = 0
    ,NAND_K9F5608X0C
    ,NAND_K9F5616Q0C
    ,NAND_K9F5616X0C
    ,NAND_K9K1208X0C
    ,NAND_K9K1G08X0A
    ,NAND_K9F1G08Q0M
    ,NAND_K9F1G08X0M
    ,NAND_K9F1G16Q0M
    ,NAND_K9F1G16X0M
    ,NAND_K9F2G08X0M
    ,NAND_K9F2G16X0M
    ,NAND_K9F2G08R0A
    ,NAND_K9K4G08Q0M
    ,NAND_K9K4G08U0M
    ,NAND_K9K4G16Q0M
    ,NAND_K9K4G16U0M
    ,NAND_TC58DVM82A1FT
    ,NAND_TC58DVM92A1FT
    ,NAND_TC58DVG02A1FT
    ,NAND_TC58NVG0S3AFT
    ,NAND_TH58NVG1S3AFT
    ,NAND_HY27XS08561M
    ,NAND_HY27XS08121M
    ,NAND_HY27XA081G1M
    ,NAND_HY27XA161G1M
    ,NAND_H9LA25G25HAMBR
    ,NAND_ST128W3A
    ,NAND_ST128W4A
    ,NAND_ST256W3A
    ,NAND_ST256W4A
    ,NAND_ST512W3A
    ,NAND_ST512W4A
    ,NAND_ST01GW3A
    ,NAND_ST01GW4A
    ,NAND_ST512R3B
    ,NAND_ST512W3B
    ,NAND_ST512R4B
    ,NAND_ST512W4B
    ,NAND_ST01GR3B
    ,NAND_ST01GW3B
    ,NAND_ST01GR4B
    ,NAND_ST01GW4B
    ,NAND_ST02GR3B
    ,NAND_ST02GW3B
    ,NAND_ST02GR4B
    ,NAND_ST02GW4B
    ,NAND_ST04GR3B
    ,NAND_ST04GW3B
    ,NAND_ST04GR4B
    ,NAND_ST04GW4B
    ,NAND_ST08GR3B
    ,NAND_ST08GW3B
    ,NAND_ST08GR4B
    ,NAND_ST08GW4B
    ,NAND_K9F2808U0C
    ,NAND_K9K8G08U0M
    ,NAND_TH58NVG2D4BFT
    ,NAND_HY27UF081G2M
    ,NAND_TH58NVG1S8BFT
    ,NAND_MT29F2G08AAC
    ,NAND_MT29F2G08ABD
    ,NAND_MT29F4G08BAB
    ,NAND_SUPERAND51208
    ,NAND_SUPERAND51216
    ,NAND_HYF33DS51280
    ,NAND_HYF33DS1G80
    ,NAND_HYF33DS1G16
    ,NAND_KF94G16Q4X
    ,NAND_TC58NWM9S3B
    ,NAND_HY27UF082G2M
    ,NAND_M6MGA157F2LCWG08
    ,NAND_M6MGA157F2LCWG16
    ,NAND_TC58NWM9S8C
    ,NAND_TC58NWG0S8C
    ,NAND_HY27US16561M
    ,NAND_HY27UF162G5A
    ,NAND_HY27UT088G2A
    ,NAND_TC58NVG4D1DTG
    ,NAND_K5E1257ACM
    ,NAND_K5E1G12ACF
    ,NAND_MT29F2G16ABD
    ,NAND_HYD0SFG0MF1P
    ,NAND_HYD0SQG0MF1P
    ,NAND_H8BCS0CG0MBR
    ,NAND_H8BCS0PG0MBP
    ,NAND_TC58NYG0S8C
    ,NAND_TC58NYG1S8C
    ,NAND_TC58NYG1S3E
    ,NAND_TC58NYM9S8E
    ,NAND_TC58NYG0S3E
    ,NAND_ST512R3A
    ,NAND_ST512R4A
    ,NAND_MT29F1G08ABB
    ,NAND_MT29F1G16ABB
    ,NAND_ASU1GA30GT_G30CA
    ,NAND_K522H1GACE
    ,NAND_H8BCS0SI0MBR
    ,NAND_MT29F4G08ABC
    ,NAND_MT29C4G48MAZAPAKD
    ,NAND_H8BCS0UN0MCR
    ,NAND_TYBC0A111236KC10
    ,NAND_HY27UF084G2B
    ,NAND_FM61D1G12A_5BGE
    ,NAND_FM64D1G12A_5BAGE
    ,NAND_KF92G08Q2V
    ,NAND_TC58NYG2S0FTA00
    ,NAND_TC58NVG4D1DTG00
    ,NAND_MT29F2G08ABBFA
    ,NAND_MT29F4G16ABBEA
    ,NAND_MT29F4G08ABBEA
    ,NAND_MT26F2G16ABBEA
    ,NAND_FMN1SD5SBS_50IA
    ,NAND_MD5N01G51MSD1B
    ,NAND_JSFBAD2N72ABC_450
    ,NAND_FM6AD2G1GA_5BAGE
    ,NAND_SMMS1G161216AAEA
    ,NAND_W71NW11GC1DW
    ,NAND_MX63U1GD12EAXNI00
    ,NAND_FM64D1G12MA
    ,NAND_FMN1SD5SBB

    // MT6290 DA support only
	,NAND_MT29F8G16ABACA
	,NAND_MT29F8G08ABACA
    ,NAND_MT29F2G08ABBEA
	,NAND_S34ML02G1
    // end of MT6290 DA support only

    ,NAND_LAST                  = 0x4000
    /* SPI-NAND */
    ,SNAND_GD5F1GQ4UAYIG        = 0x4001
    ,SNAND_ANS51204A3ZE
    ,SNAND_ATO25D1GA

    ,NAND_DISPLAY_STRING_SV5X   = 0xFFFE
    ,NAND_UNKNOWN               = 0xFFFF      // Unknown Device
} NAND_DeviceID_E;

#endif
