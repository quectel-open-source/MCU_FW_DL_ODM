#pragma once
#include "Gen_Header.h"
#include "tlhelp32.h"
#include "atlbase.h"
#include <winioctl.h>//HD



//RF上、下行标识
#define RF_TX_UL_INPUT				0
#define RF_RX_DL_OUTPUT				1


//Division Dual 双工
#define DD_NDF						0
#define DD_FDD						1
#define DD_TDD						2


//NB-IoT 的 Subcarrier Spacing
#define NB_SCSP_S3K75				0//3.75 kHz
#define NB_SCSP_S15K				1//15 kHz


//NB-IoT 的 Modulation Scheme Used By The Measured Signal（与高通代码中的定义相同）
#define NB_MSCH_BPSK				0//0 = BPSK
#define NB_MSCH_QPSK				1//1 = QPSK


//NB-IoT 的 Number Of SubCarrier
#define NB_SINGLE_TONE				1
#define NB_MULTI_TONE_3				3
#define NB_MULTI_TONE_6				6
#define NB_MULTI_TONE_12			12



//方案平台
typedef enum tagEU_PF_IDX
{
	//
	PF_NDF = 0,		//未定义
	//
	PF_MTK,			//MTK
	PF_QUALCOMM,	//高通
	PF_RDA,			//RDA
	PF_GCT,			//GCT
	PF_INTEL,		//Intel
	PF_HUAWEI,
	//PF_MTK_NB,
	PF_ASR,			//ASR
	PF_REALTEK,		//Realtek
	PF_SPREADTRUM,	//展锐 Spreadtrum
	PF_EIGENCOMM,	//移芯 Eigencomm
	PF_TI,			//德州仪器 Texas Instruments
	PF_XINYI,		//芯翼信息科技(上海)有限公司
	PF_ALTAIR,		//Sony Altair
	PF_BEKEN,		//博通集成电路（上海）股份有限公司
	PF_CYPRESS,		//赛普拉斯 Cypress 半导体公司
	PF_SILICON_LABS,	//芯科科技(Silicon Labs)
	PF_AMLOGIC,			//Amlogic 晶晨半导体(上海)股份有限公司
	PF_MCU,			//只烧录MCU Jlink
	PF_ESP32,			//ESp32烧录
	//
	PF_COUNT
}EU_PF_IDX;



//BB芯片
typedef enum tagEU_BB_CHIP_IDX
{
	//
	BB_NDF = 0,		//未定义
	//
	BB_MDM6200,		//高通
	BB_QSC6270,		//高通
	BB_XMM6255,		//Intel
	BB_XMM6250,		//Intel
	BB_MT6261,		//MTK
	BB_MDM9x15,		//高通4G芯片
	BB_MSM8909,		//高通4G芯片
	BB_MDM9x07,		//高通4G芯片
	BB_RDA8851,		//RDA
	BB_MSM8937,		//高通4G芯片
	BB_MSM8953,		//高通4G芯片
	BB_AR6003G,		//高通WiFi芯片
	BB_QCA1023,		//高通WiFi芯片
	BB_MDM9206,		//高通4G芯片
	BB_MDM9628,		//高通4G车载芯片
	BB_HI2110,		//海思Hi2110 NB-IoT芯片
	BB_MSM8917,		//高通4G芯片
	BB_MDM9x40,		//高通4G芯片 CAT-6
	BB_SDM450,		//高通8953芯片的主频降低版
	BB_WCN3610,		//高通WiFi芯片
	BB_WCN3660,		//高通WiFi芯片
	BB_WCN3680,		//高通WiFi芯片
	BB_MSM8905,		//高通4G芯片
	BB_MT2625,		//MTK NB-IoT芯片
	BB_MT2503,		//MTK 2G+GPS芯片
	BB_SDX20,		//高通4G芯片
	BB_RDA8955L,	//RDA的2G芯片
	BB_ASR1802S,	//ASR的4G芯片
	BB_MDM9150,		//高通芯片 只有C-V2X
	BB_QCA6564,		//高通WiFi芯片(车规平台)
	BB_SDM660,		//高通4G芯片
	BB_RDA8908A,	//RDA的NB-IoT芯片
	BB_RDA8909B,	//RDA的2G+NB-IoT芯片
	BB_MT2621,		//MTK的2G+NB-IoT芯片
	BB_SA415M,		//高通车规4G+V2X基线
	BB_SDX24,		//高通4G芯片, A7 at 1.5GHz, 256KB L2, 应用于笔电, LTE-A/车载
	BB_MDM9205,		//高通4G芯片, LTE Cat-M / Cat-NB
	BB_SDX55,		//高通 5G 芯片, 工规
	BB_RTL8189,		//Realtek平台WiFi芯片
	BB_QM215,		//高通4G智能模块(替代MSM8909)
	BB_QCM2150,		//高通4G智能模块(替代MSM8909)
	BB_SA515M,		//高通 5G NR 芯片, SA515M是车规平台, 对应的工规SDX55平台
	BB_WCN3990,		//高通WiFi芯片
	BB_ASR3601,		//ASR的ASR1802S低成本4G芯片
	BB_SL8541E,		//展锐(Spreadtrum)4G智能模块芯片
	BB_HI2152,		//海思HI2152 4G非智能模块芯片
	BB_QCA6391,		//高通 WiFi 蓝牙 芯片
	BB_EC616,		//上海移芯 超低功耗NB-IoT单模芯片
	BB_ASR1601,		//ASR的4G芯片 CAT-1
	BB_CC1200,		//TI 低功耗高性能射频收发器
	BB_HI9500,		//海思 5G 芯片
	BB_ASR1802SL,	//ASR的4G芯片的Linux版本
	BB_ASR1826,		//ASR的4G芯片, LTE CAT-6
	BB_HI2115,		//海思Hi2115 NB-IoT芯片
	BB_QCA6696,		//高通车规级WiFi/BT芯片（支持802.11 ax WiFi, 支持蓝牙 BT 5.1）
	BB_SDA845,		//高通系统级芯片(SoC: System-on-a-Chip), SDA不带modem, 即没有蜂窝移动网络
	BB_UDX710,		//展锐 Spreadtrum 5G平台 工规
	BB_QCX212,		//高通 NB-IoT SoC chipset, Category NB2, 2-HARQ
	BB_SM6115,		//高通 4G 芯片, Snapdragon 662
	BB_QTM527,		//高通 5G mmWave antenna module
	BB_EC617,		//上海移芯 超低成本、超低功耗和超高集成度NB-IoT SoC 芯片，完全支持3GPP Rel14 NB-IoT 标准
	BB_WCN3980,		//高通WiFi芯片
	BB_ESP8285,		//乐鑫WiFi芯片
	BB_UIS8910DM,	//展锐 advanced system-on-chip solution integrated CAT1bis/GSM/GPRS modems
	BB_UIS8581,		//展锐 智能模块芯片 with embedded TD-LTE, LTE FDD, TDSCDMA/HSPA(+), WCDMA/HSPA(+) and GSM/GPRS/EDGE to supportDual SIM Dual Standbyup to LTE+LTE
	BB_XY1100,		//芯翼 适用于低速 IoT 场景的高性能，高集成度的 SOC 芯片。该芯片完全兼容 3GPP TS 36.211 协议
	BB_HI2120,		//海思Hi2120 NB2 NB-IoT芯片
	BB_MT2731,		//MTK 2G/3G/4G 车规平台芯片
	BB_WCN3988,		//高通WiFi芯片
	BB_ALT1250,		//Sony Altair eMTC (CAT M), NB IoT (CAT NB1 / NB2)
	BB_QCA6595,		//高通 MU-MIMO WiFi/BT 5.1 Automotive Dual-MAC Combo Chip for Automotive Infotainment and Telematics applications.
	BB_ASR1603,		//ASR的4G芯片 CAT-1, 1601 + Flash 封装
	BB_QCA1064,		//高通 WiFi 蓝牙 芯片
	BB_MT6880,		//MTK 5G 芯片, T700 套片
	BB_MT6890,		//MTK 5G 芯片, T750 套片
	BB_WCN3615,		//高通WiFi芯片
	BB_EC616S,		//上海移芯 超低成本、超低功耗和超高集成度 NB-IoT SoC 芯片, 完全支持3GPP Rel14 NB-IoT 标准
	BB_QCM6125,		//高通 智能模块 Qualcomm Snapdragon 600 series processor and LTE modem. LTE Cat12/Cat13 (FDD and TDD).
	BB_UIS8811,		//展锐 advanced system-on-chip solution integrated NB-IoT modems. The UIS8811 integrates RF transceiver for low cost.
	BB_SDX12,		//高通 4G 芯片, 高通MDM9x50 cost down 平台, Sixthgeneration LTE multimode modem, 14nm system on chip (SoC).
	BB_QCM2290,		//高通 智能模块 4G芯片
	BB_BK7231,		//集成蓝牙双模5.1和WiFi802.11n的 WiFi SOC 芯片
	BB_SDX62,		//高通 5G 芯片, 工规, SDX55的下一代, Provides Higher Performance mmWave and Features for Global Deployment.
	BB_QCA2064,		//高通 highly integrated System-On-Chip (SoC) supporting 802.11ax Wi-Fi and Bluetooth (BT) Milan. It is part of the QCA206x family of SoC’s supporting operation on 2.4 GHz and 5 GHz.
	BB_SM4350,		//高通 5G 智能芯片, the new generation Qualcomm Snapdragon 400 series processor with integrated 5G modem.
	BB_ASR1803S,	//ASR的4G芯片 CAT-4, ASR1802S Costdown 芯片, RTOS版本
	BB_MT2735,		//MTK 5G 芯片, supporting NR Sub6, LTE Cat19 4x4 MIMO
	BB_WCN3991,		//高通WiFi芯片
	BB_SL8521E,		//展锐(Spreadtrum)4G智能模块芯片, 智能模块系统是Linux, Android4.4/Linux
	BB_QCM6490,		//高通 5G Smart 车载智能模块, 包含5G版本, 用于车机和Tbox整合的一体机. including global multi-gigabit 5G mmW/Sub-6 GHz and WiFi 6E
	BB_QCA9377,		//高通 WLAN/Bluetooth 芯片, a single-die wireless local area network (WLAN) and Bluetooth combination solution to support 1 × 1 802.11a/b/g/n/ac WLAN standards and Bluetooth 5.0 + HS, designed to deliver superior integration of WLAN/Bluetooth and low-energy technology.
	BB_CYW54591,	//The Cypress CYW54591 is a complete dual-band (2.4 GHz and 5 GHz) 5G Wi-Fi 2×2 MIMO MAC/PHY/Radio system-on-Chip.
	BB_ASR1803E,	//ASR的4G芯片 CAT-4, ASR1802S Costdown 芯片, Open Linux版本
	BB_ASR1803L,	//ASR的4G芯片 CAT-4, ASR1802S Costdown 芯片, 支持"Open Linux版本" or "RTOS版本"
	BB_WCN3998,		//高通WiFi芯片
	BB_QCM4290,		//高通4G平台, The QCM4290 includes the next generation of the Qualcomm Snapdragon 600 series processor and LTE modem.
	BB_QCX315,		//高通5G平台, The QCX315 device is a 5G modem from Qualcomm Technologies, Inc. (QTI). It implements the 5G NR standard for sub-6 GHz bands.
	BB_QCS8250,		//高通 QCS8250 is the new generation Qualcomm? Snapdragon? premium-tier processor with robotic application.
	BB_CYW43455,	//The Cypress CYW43455 single-chip device provides the highest level of integration for Internet of Things applications and handheld wireless system with integrated single-stream IEEE 802.11ac MAC/baseband/radio and, Bluetooth 5.0.
	BB_UIS8310AM,	//展锐 UIS8310AM is a highly integrated application processor with embedded TDD-LTE, FDD-LTE, WCDMA and GSM/GPRS/EDGE modem and LPDDR2. The specially optimized architecture of UIS8310 can achieve high performance and low power for a lot of applications.
	BB_SDX65,		//高通 5G 芯片, 工规, The SDX65/SDX62 device is a multimode modem chip, supporting 2G/3G/4G/5G technologies. 4nm premium-tier 5G modem, 3G/4G/5G modem –mmWave (mmW) and sub-6 GHz bands (3GPP Release 16 integrated modem).
	BB_UWE5623DU,	//展锐 WiFi/蓝牙 芯片, UWE5623DU is a high intergrated 2-in-1 connectivity single chip, this chip includes 2.4GHz and 5GHz WLAN IEEE 802.11 a/b/g/n/ac 2x2 MU-MIMO 20/40/80 MHz VHT R2 MAC/PHY/Radio, Bluetooth 5.1.
	BB_EC618,		//上海移芯 Eigencomm, Cat1芯片. EC618 is a Cat.1bis SoC chipset with ultra-low cost, ultra-low power and ultra-high integration, it is fully compliant to 3GPP Rel14 Cat.1bis standard，and compatible with 4G commercial networks.
	BB_EFR32BG21,	//芯科科技(Silicon Labs), 蓝牙+LoRa的芯片, EFR32BG21 Blue Gecko Wireless SoC Family.
	BB_CYW43438,	//The Cypress CYW43438L is a highly integrated single-chip solution and offers the lowest RBOM in the industry for smartphones, tablets, and a wide range of other portable devices.
	BB_UIS8850DG,	//展锐 UIS8850DG is a highly integrated system on chip solution with embedded LTE Cat1bis modems, GNSS modem, RF transceiver, power management unit(PMU), Trust zone and Multi-Function Crypto Engine(CE).
	BB_CYW8X072,	//The Cypress CYW8X072 is a monolithic, single-chip, Bluetooth 4.2 compliant, stand-alone baseband processor with an integrated 2.4 GHz transceiver.
	BB_WCN3950,		//高通WiFi芯片
	BB_W155S1,		//Amlogic W155S1 is an integrated Wi-Fi and Bluetooth combo chip. WiFi&BT芯片, 支持802.11_a/b/g/n/ac@WLAN2.4G&5G, BT5.0（BLE、BDR/EDR）.
	BB_CYW43439,	//The Cypress CYW43439 is a highly integrated single-chip solution and offers the lowest RBOM in the industry for smartphones, tablets, and a wide range of other portable devices.
	BB_CB0201,		//海思 CB0201 是一款海思平台低功耗的NB-IOT芯片，支持3GPP R13/R14/R15 NB-IoT（支持Rel-16升级），同时支持BLE/GNSS。
	BB_ASR1606C,	//ASR的LTE CAT1/M芯片, ASR1606 is a highly cost-efficient System on Chip (SoC) device.（4M RAM+ 4M ROM）
	BB_ASR1606S,	//ASR的LTE CAT1/M芯片, ASR1606 is a highly cost-efficient System on Chip (SoC) device.（8M RAM+ 8M ROM）
	BB_WCN6856,		//高通WiFi芯片
	BB_QCX216,		//高通 compact, low power, low-cost LTE Cat.1bis chip, WiFi2.4GHz Rx.
	BB_QCA206x,		//高通 The Qualcomm QCA206x family consist of System-On-Chips (SoC), Bluetooth Milan and Wi-Fi supported, supporting simultaneous operation on 2.4 GHz, 5 GHz, and 6GHz*, also known as Dual Band Simultaneous (DBS). Part Number(s): QCA2065, QCA2066, QCA2062, QCA2064.
	BB_ASR1828,		//ASR的4G芯片, LTE CAT-6, 1826的迭代产品.
	BB_MCU,		//MCU只烧录MCU Jlink
	BB_ESP32,		//ESP32烧录MCU
	//
	BB_CHIP_COUNT
}EU_BB_CHIP_IDX;



//GSM频段序列号
typedef enum tagEU_GSM_BAND_IDX
{
	G_BAND_GSM900 = 0,
	G_BAND_DCS1800,
	G_BAND_PCS1900,
	G_BAND_GSM850,
	G_BAND_COUNT,		//GSM频段数量
	G_BAND_SIZE = 6		//GSM频段相关的存储BUFF尺寸（“尺寸”必须比“数量”至少大1, 用于存放结束符）
}EU_GSM_BAND_IDX;

//WCDMA频段序列号
typedef enum tagEU_WCDMA_BAND_IDX
{
	W_BAND_1 = 0,
	W_BAND_2,
	W_BAND_3,
	W_BAND_4,
	W_BAND_5,
	W_BAND_6,
	W_BAND_7,
	W_BAND_8,
	W_BAND_9,
	W_BAND_10,
	W_BAND_19,
	W_BAND_COUNT,		//WCDMA频段数量
	W_BAND_SIZE = 16	//WCDMA频段相关的存储BUFF尺寸（“尺寸”必须比“数量”至少大1, 用于存放结束符）
}EU_WCDMA_BAND_IDX;

//cdma2000频段序列号（老）
typedef enum tagEU_CDMA2K_BAND_IDX
{
	C2K_BAND_800 = 0,
	C2K_BAND_1900,
	C2K_BAND_COUNT,		//cdma2000频段数量
	C2K_BAND_SIZE = 3	//cdma2000频段相关的存储BUFF尺寸（“尺寸”必须比“数量”至少大1, 用于存放结束符）
}EU_CDMA2K_BAND_IDX;

//cdma2000频段序列号（新）
typedef enum tagEU_C2K_BAND_IDX
{
	C2K_BC_0 = 0,
	C2K_BC_COUNT,		//cdma2000频段数量
	C2K_BC_SIZE = 3		//cdma2000频段相关的存储BUFF尺寸（“尺寸”必须比“数量”至少大1, 用于存放结束符）
}EU_C2K_BAND_IDX;

//LTE频段序列号
typedef enum tagEU_LTE_BAND_IDX
{
	L_BAND_1 = 0,
	L_BAND_2,
	L_BAND_3,
	L_BAND_4,
	L_BAND_5,
	L_BAND_7,
	L_BAND_8,
	L_BAND_12,
	L_BAND_17,
	L_BAND_20,
	L_BAND_38,
	L_BAND_39,
	L_BAND_40,
	L_BAND_41,
	L_BAND_13,
	L_BAND_18,
	L_BAND_19,
	L_BAND_25,
	L_BAND_26,
	L_BAND_28,
	L_BAND_34,
	L_BAND_14,
	L_BAND_29,			//LTE的B29和B32，只有DL，是FDD，只有做CA才有用
	L_BAND_30,
	L_BAND_66,
	L_BAND_71,
	L_BAND_42,
	L_BAND_43,
	L_BAND_48,
	L_BAND_9,
	L_BAND_11,
	L_BAND_21,
	L_BAND_47,
	L_BAND_46,
	L_BAND_27,
	L_BAND_85,
	L_BAND_31,
	L_BAND_72,
	L_BAND_73,
	L_BAND_32,			//LTE的B29和B32，只有DL，是FDD，只有做CA才有用
	L_BAND_87,
	L_BAND_88,
	L_BAND_COUNT,		//LTE频段数量
	L_BAND_SIZE = 46	//LTE频段相关的存储BUFF尺寸（“尺寸”必须比“数量”至少大1, 用于存放结束符）
}EU_LTE_BAND_IDX;

//TD频段序列号
typedef enum tagEU_TD_BAND_IDX
{
	TD_BAND_34 = 0,
	TD_BAND_39,
	TD_BAND_COUNT,		//TD频段数量
	TD_BAND_SIZE = 4	//TD频段相关的存储BUFF尺寸（“尺寸”必须比“数量”至少大1, 用于存放结束符）
}EU_TD_BAND_IDX;

//NB-IoT频段名
typedef enum tagEU_NB_BAND_IDX
{
	NB_BAND_1 = 1,
	NB_BAND_2,
	NB_BAND_3,
	NB_BAND_4,
	NB_BAND_5,
	NB_BAND_7 = 7,
	NB_BAND_8,
	NB_BAND_9,
	NB_BAND_10,
	NB_BAND_11,
	NB_BAND_12,
	NB_BAND_13,
	NB_BAND_14,
	NB_BAND_17 = 17,
	NB_BAND_18,
	NB_BAND_19,
	NB_BAND_20,
	NB_BAND_21,
	NB_BAND_22,
	NB_BAND_24 = 24,
	NB_BAND_25,
	NB_BAND_26,
	NB_BAND_27,
	NB_BAND_28,
	NB_BAND_30 = 30,
	NB_BAND_31,
	NB_BAND_66 = 66,
	NB_BAND_68 = 68,
	NB_BAND_70 = 70,
	NB_BAND_71,
	NB_BAND_72,
	NB_BAND_73,
	NB_BAND_74,
	NB_BAND_85 = 85,
	NB_BAND_86,
	NB_BAND_87,
	NB_BAND_88
}EU_NB_BAND_IDX;

//NB-IoT频段序列号
typedef enum tagEU_NB_BAND_INDEX
{
	NB_IDX_B_1 = 0,
	NB_IDX_B_2,
	NB_IDX_B_3,
	NB_IDX_B_4,
	NB_IDX_B_5,
	NB_IDX_B_7,
	NB_IDX_B_8,
	NB_IDX_B_9,
	NB_IDX_B_10,
	NB_IDX_B_11,
	NB_IDX_B_12,
	NB_IDX_B_13,
	NB_IDX_B_14,
	NB_IDX_B_17,
	NB_IDX_B_18,
	NB_IDX_B_19,
	NB_IDX_B_20,
	NB_IDX_B_21,
	NB_IDX_B_22,
	NB_IDX_B_24,
	NB_IDX_B_25,
	NB_IDX_B_26,
	NB_IDX_B_27,
	NB_IDX_B_28,
	NB_IDX_B_30,
	NB_IDX_B_31,
	NB_IDX_B_66,
	NB_IDX_B_68,
	NB_IDX_B_70,
	NB_IDX_B_71,
	NB_IDX_B_72,
	NB_IDX_B_73,
	NB_IDX_B_74,
	NB_IDX_B_85,
	NB_IDX_B_86,
	NB_IDX_B_87,
	NB_IDX_B_88,
	NB_IDX_B_COUNT,		//NB-IoT频段数量
	NB_IDX_B_SIZE = 40	//NB-IoT频段相关的存储BUFF尺寸（“尺寸”必须比“数量”至少大1, 用于存放结束符）
}EU_NB_BAND_INDEX;

//C-V2X频段序列号
typedef enum tagEU_CV2X_BAND_IDX
{
	CV2X_BAND_46 = 0,
	CV2X_BAND_47,
	CV2X_BAND_COUNT,	//C-V2X频段数量
	CV2X_BAND_SIZE = 6	//C-V2X频段相关的存储BUFF尺寸（“尺寸”必须比“数量”至少大1, 用于存放结束符）
}EU_CV2X_BAND_IDX;


//WiFi频段(FreqBand)序列号
typedef enum tagEU_WIFI_FB_IDX
{
	WIFI_FB_B24G = 0,	//2.4G
	WIFI_FB_B5G,		//5G
	WIFI_FB_COUNT
}EU_WIFI_FB_IDX;


//WiFi标准(WiFi Standard)序列号
typedef enum tagEU_WIFI_STAN_IDX
{
	WIFI_STAN_80211A = 0,
	WIFI_STAN_80211B,
	WIFI_STAN_80211G,
	WIFI_STAN_80211N_20M,
	WIFI_STAN_80211N_40M,
	WIFI_STAN_80211N_5G_20M,
	WIFI_STAN_80211AC_20M,
	WIFI_STAN_80211AC_40M,
	WIFI_STAN_80211AC_80M,
	WIFI_STAN_80211AX_20M,
	WIFI_STAN_80211AX_40M,
	WIFI_STAN_80211AX_5G_20M,
	WIFI_STAN_80211AX_5G_40M,
	WIFI_STAN_80211AX_5G_80M,
	WIFI_STAN_80211N_5G_40M,
	WIFI_STAN_COUNT
}EU_WIFI_STAN_IDX;


//蓝牙版本序列号
typedef enum tagEU_BLU_VER_IDX
{
	BLU_VER_10_BR = 0,
	BLU_VER_20_EDR,
	BLU_VER_40_BLE,		//Bluetooth Low Energy
	BLU_VER_COUNT
}EU_BLU_VER_IDX;


//NR 5G SUB6 频段序列号
typedef enum tagEU_NR5GSUB6_BAND_INDEX
{
	NR5GSUB6_N_1 = 0,
	NR5GSUB6_N_2,
	NR5GSUB6_N_3,
	NR5GSUB6_N_5,
	NR5GSUB6_N_7,
	NR5GSUB6_N_8,
	NR5GSUB6_N_20,
	NR5GSUB6_N_28,
	NR5GSUB6_N_41,
	NR5GSUB6_N_66,
	NR5GSUB6_N_71,
	NR5GSUB6_N_77,
	NR5GSUB6_N_78,
	NR5GSUB6_N_79,
	NR5GSUB6_N_12,
	NR5GSUB6_N_25,
	NR5GSUB6_N_48,
	NR5GSUB6_N_38,
	NR5GSUB6_N_40,
	NR5GSUB6_N_14,
	NR5GSUB6_N_13,
	NR5GSUB6_N_18,
	NR5GSUB6_N_26,
	NR5GSUB6_N_29,
	NR5GSUB6_N_30,
	NR5GSUB6_N_75,
	NR5GSUB6_N_76,
	NR5GSUB6_N_91,
	NR5GSUB6_N_92,
	NR5GSUB6_N_93,
	NR5GSUB6_N_94,
	NR5GSUB6_N_70,
	NR5GSUB6_N_257,
	NR5GSUB6_N_258,
	NR5GSUB6_N_260,
	NR5GSUB6_N_261,
	NR5GSUB6_N_COUNT,		//NR 5G SUB6 频段数量
	NR5GSUB6_N_SIZE = 49	//NR 5G SUB6 频段相关的存储BUFF尺寸（“尺寸”必须比“数量”至少大1, 用于存放结束符）
}EU_NR5GSUB6_BAND_INDEX;


//NR 5G MMW 毫米波频段序列号
typedef enum tagEU_NR5GMMW_BAND_INDEX
{
	NR5GMMW_N_257 = 0,
	NR5GMMW_N_258,
	NR5GMMW_N_260,
	NR5GMMW_N_261,
	NR5GMMW_N_COUNT,		//NR 5G MMW 频段数量
	NR5GMMW_N_SIZE = 6		//NR 5G MMW 频段相关的存储BUFF尺寸（“尺寸”必须比“数量”至少大1, 用于存放结束符）
}EU_NR5GMMW_BAND_INDEX;


//天线类型序列号
typedef enum tagEU_ANT_TYPE_INDEX
{
	ANT_TYPE_IDX_NDF = -1,
	ANT_TYPE_IDX_MAIN_1 = 0,
	ANT_TYPE_IDX_DIV_1,
	ANT_TYPE_IDX_GPS,
	ANT_TYPE_IDX_WIFI_BLU,
	ANT_TYPE_IDX_DIV_GPS,
	ANT_TYPE_IDX_MULTI_DG,
	ANT_TYPE_IDX_MULTI_M,
	ANT_TYPE_IDX_MULTI_M1,
	ANT_TYPE_IDX_MULTI_M2,
	ANT_TYPE_IDX_NR5GSUB6_1,
	ANT_TYPE_IDX_NR5GSUB6_2,
	ANT_TYPE_IDX_NR5GSUB6_3,
	ANT_TYPE_IDX_NR5GSUB6_4,
	ANT_TYPE_IDX_NR5GSUB6_5,
	ANT_TYPE_IDX_NR5GSUB6_6,
	ANT_TYPE_IDX_NR5GSUB6_7,
	ANT_TYPE_IDX_NR5GSUB6_8,
	ANT_TYPE_IDX_MAIN_2,
	ANT_TYPE_IDX_DIV_2,
	ANT_TYPE_IDX_MULTI_M3,
	ANT_TYPE_IDX_MULTI_M4,
	ANT_TYPE_IDX_WIFI_MIMO,
	ANT_TYPE_IDX_V2X_MAIN,
	ANT_TYPE_IDX_V2X_DIV,
	ANT_TYPE_IDX_DSDA_MAIN,
	ANT_TYPE_IDX_DSDA_DIV,
	ANT_TYPE_IDX_V2X_TRX0,
	ANT_TYPE_IDX_V2X_TRX1,
	ANT_TYPE_IDX_NR5GSUB6_9,
	ANT_TYPE_IDX_NR5GSUB6_10,
	ANT_TYPE_IDX_NR5GSUB6_11,
	ANT_TYPE_IDX_NR5GSUB6_12,
	ANT_TYPE_IDX_WIFI_BLU_GPS,
	ANT_TYPE_IDX_COUNT,			//天线类型的总数量
	ANT_TYPE_IDX_SIZE = 39,		//“每个天线（所有天线）都有自己的参数存储BUFF”的尺寸（“尺寸”必须比“天线类型的总数量”至少大1, 用于存放结束符）
	ANT_TYPE_BUFF_SIZE = 16		//“每个机型拥有的天线（有限天线）的参数存储BUFF”天线的存储BUFF尺寸（“尺寸”必须比“机型拥有的天线数量”至少大1, 用于存放结束符）
	//天线的存储BUFF尺寸占用内存空间资源太多，存储天线数据的相关结构体未来最好还是要调整为“有限天线”的排序式存储。
}EU_ANT_TYPE_INDEX;


//ASR 平台 RF Key Type
typedef enum tagEU_RF_KEY_TYPE_INDEX
{
	RF_KEY_TYPE_IDX_NDF = -1,
	RF_KEY_TYPE_IDX_PXA1802S_RF858 = 0,		//ASR平台RF Type:"1802S+RF858"
	RF_KEY_TYPE_IDX_PAX1908_RF858,			//ASR平台RF Type:"1802+RF858"
	RF_KEY_TYPE_IDX_RF866,					//ASR平台RF Type:"1826+RF866"
	RF_KEY_TYPE_IDX_RF859C,					//ASR平台RF Type:"1802S+RF859C"
	RF_KEY_TYPE_IDX_1601_3601,				//ASR平台RF Type:"3601_1601"
	RF_KEY_TYPE_IDX_1603,					//ASR平台RF Type:"1603"
	RF_KEY_TYPE_IDX_3603,					//ASR平台RF Type:"3603"
	RF_KEY_TYPE_IDX_1803_FAICON,			//ASR平台RF Type:"1803"
	RF_KEY_TYPE_IDX_1606,					//ASR平台RF Type:"1606"
	RF_KEY_TYPE_IDX_1828_RF866,				//ASR平台RF Type:"1828+RF866"
	RF_KEY_TYPE_IDX_COUNT
}EU_RF_KEY_TYPE_INDEX;



//“网络制式”规范
#define FORMAT_GSM_S			0
#define FORMAT_WCDMA_S			1
#define FORMAT_C2K_S			2
#define FORMAT_GSM_NS			3
#define FORMAT_WCDMA_NS			4
#define FORMAT_TD_NS			5
#define FORMAT_LTE_NS			6
#define FORMAT_C2K_NS			7
#define FORMAT_NBIOT_S			8
#define FORMAT_NBIOT_NS			9
#define FORMAT_NR5GSUB6_NS		10
#define FORMAT_GPS_NS			11
#define FORMAT_WIFI_NS			12
#define FORMAT_BLUETOOTH_NS		13
#define FORMAT_CV2X_NS			14
#define FORMAT_NR5GMMW_NS		15



//高、中、低信道标识
#define CH_ALL					-1
#define CH_LOW					0
#define CH_MID					1
#define CH_HIGH					2
#define CH_LOW_EX				3
#define CH_LMH_COUNT			4



//仪器型号
#define CMU200					0
#define AG_E1960				1
#define AG_E1968				2
#define AG6601A					3
#define MT8820					4
#define CMD55					5
#define AG8922					6
#define CMW500					7
#define AG_E1987A				8
#define AG_E1962B				9
#define LP_IQXS					10
#define CMW100					11
#define LP_IQXM					12
#define LP_5G_IQXS				13
#define CMP180					14



//仪器未测试到结果的标识
#define ERROR_RTN_VALUE			9.91E9	//9.91E37



//DUT端ANT天线组合与仪器端RF口接线方案
#define PLAN_ANT4_MAIN1_DIV1_GPS1_WIFI1			1//4根ANT天线, “1主集 + 1分集 + 1GPS + 1WiFi蓝牙”
#define PLAN_ANT4_MAIN2_DIV2					2//4根ANT天线, “2根MAIN主天线 + 2根DIV分集天线”



//当前测试的RF通路（RF链路）
#define RX_CHAIN_MAIN			1
#define RX_CHAIN_DIV			2
#define RX_CHAIN_GPS			3
#define RX_CHAIN_MAIN_DIV		4



//通用的“unsigned char”BUFF
typedef struct tagPKT_BUFF_S
{
	unsigned char * ucp_Buff;
	unsigned long ul_Total_Size;
	unsigned long ul_Data_Length;
	//结构体/构造函数初始化
	tagPKT_BUFF_S() : ucp_Buff( NULL ), ul_Total_Size( 0 ), ul_Data_Length( 0 )
	{
		//定义变量时, 如果没有给出变量的值, 就都赋值成0, 否则按照实际的值来赋值。
	}
}PKT_BUFF_S;



//测试流程属性
#define PROCESS_ATT_ERROR			-1//无法识别的测试流程属性
#define PROCESS_ATT_ALL_TEST		0//全检流程
#define PROCESS_ATT_ONLY_LABEL		1//只打标签
#define PROCESS_ATT_TO_DL			2//只做到DL
#define PROCESS_ATT_TO_BT			3//只做到BT
#define PROCESS_ATT_TO_FT			4//只做到FT
#define PROCESS_ATT_TO_FCT			5//只做到FCT



//仪器线损结构体 V1版
typedef struct tagTESTER_RF_OFFSET_S
{
	double dr_GSM_RF_Loss[G_BAND_SIZE * 6];		//依据每个GSM频段的低、中、高信道存放损耗值
	double dr_W_RF_Loss[W_BAND_SIZE * 2];		//依据每个W频段的index号存放损耗值
	double dr_TD_RF_Loss[TD_BAND_SIZE * 2];		//依据每个TD频段的index号存放损耗值
	double dr_C2K_RF_Loss[C2K_BC_SIZE * 2];		//依据每个cdma2000频段的index号存放损耗值
	double dr_LTE_RF_Loss[L_BAND_SIZE * 2];		//依据每个LTE频段的index号存放损耗值
	double dr_NBIoT_RF_Loss[NB_IDX_B_SIZE * 2];	//依据每个NB-IoT频段的index号存放损耗值
}TESTER_RF_OFFSET_S;



//仪器线损结构体 V2版
typedef struct tagTESTER_RF_OFFSET_V2_S
{
	double dr_GSM_RF_Loss[G_BAND_SIZE * 6];				//依据每个GSM频段index号的低、中、高信道存放线损值
	double dr_W_RF_Loss[W_BAND_SIZE * 6];				//依据每个W频段的index号的低、中、高信道存放线损值
	double dr_TD_RF_Loss[TD_BAND_SIZE * 6];				//依据每个TD频段的index号的低、中、高信道存放线损值
	double dr_C2K_RF_Loss[C2K_BC_SIZE * 6];				//依据每个cdma2000频段的index号的低、中、高信道存放线损值
	double dr_LTE_RF_Loss[L_BAND_SIZE * 6];				//依据每个LTE频段的index号的低、中、高信道存放线损值
	double dr_NBIoT_RF_Loss[NB_IDX_B_SIZE * 6];			//依据每个NB-IoT频段的index号的低、中、高信道存放线损值
	double dr_NR_5G_SUB6_RF_Loss[NR5GSUB6_N_SIZE * 6];	//5G NR SUB6 频段的index号的低、中、高信道存放线损值
	double dr_GPS_RF_Loss[2];							//GPS的 1.57G 一个频点的存放线损值
	double dr_WiFi_RF_Loss[WIFI_FB_COUNT * 6];			//WiFi的2.4G和5G频点的低、中、高信道存放线损值
	double dr_BLU_RF_Loss[2];							//蓝牙的 2.4G 一个频点的存放线损值
	double dr_CV2X_RF_Loss[CV2X_BAND_SIZE * 6];			//依据每个CV2X频段的index号的低、中、高信道存放线损值
}TESTER_RF_OFFSET_V2_S;



//仪器配置参数结构体
typedef struct tagTESTER_CONFIG_S
{
	int i_GPIB_Card_Numb;		//卡号
	int i_GPIB_Card_Type;		//卡类型
	int i_GPIB_Addr_Tester;		//仪器地址
	int i_GPIB_Addr_PS;			//电源地址
	bool b_Ctrl_PS;				//是否控电源
	int i_Instru_Option;		//具体仪器型号 或 可自动识别仪器
	CString cs_IP_SN;			//用IP或SN访问仪器
	int i_RF_Port_Numb;			//仪器的指定RF端口, 工具界面选择输入RF端口号(一般为多RF端口仪器的设置)
	TESTER_RF_OFFSET_S str_Instru_RF_Loss;	//仪器各频段线损
}TESTER_CONFIG_S;



//测试台配置参数
typedef struct tagTS_CONFIG_S
{
	int i_Com_Port;				//DM 通信口（通用口）
	int i_Com_AT_Port;			//AT 通信口
	int i_BR;					//波特率
	int i_TS_Curr_Type_Option;	//当前运行(选择)的“测试工位种类、制式(例如:GSM、WCDMA、2G+3G、2G+3G+4G等选择)”
	bool b_Debug_Log_File;		//保存 debug log 文件
}TS_CONFIG_S;



//MOB产品定制测试台配置参数
typedef struct tagTS_CONFIG_MOB_S
{
	int i_Com_Port;				//DM 通信口（通用口）
	int i_Com_AT_Port;			//AT 通信口
	int i_Com_MCU_Port;			//MCU 通信口
	int i_Com_DBG_Port;			//DBG 通信口
	int i_BR;					//波特率
	int i_TS_Curr_Type_Option;	//当前运行(选择)的“测试工位种类、制式(例如:GSM、WCDMA、2G+3G、2G+3G+4G等选择)”
	bool b_Debug_Log_File;		//保存 debug log 文件
	CString cs_BT_MAC;			//广播的蓝牙地址，从界面输入
	double d_GPS_Loss;          //GPS LOSS
}TS_CONFIG_MOB_S;



//频段配置参数
typedef struct tagBAND_CONFIG_S
{
	int i_Band_Name;			//频段名（2G使用index号）,“-1”代表不启用
	int i_Tx_Quan;				//频段的TX数量
	int ir_Rx_For_Tx_Quan[8];	//频段每个TX对应的RX数量
}BAND_CONFIG_S;



//频段结构体 V1版
typedef struct tagBAND_LIST_S
{
	int ir_GSM_Band_List[G_BAND_SIZE];		//GSM“频段index值”列表，“-1”代表结束
	int ir_WCDMA_Band_List[W_BAND_SIZE];	//WCDMA“频段值”列表，“-1”代表结束
	int ir_C2K_Band_List[C2K_BC_SIZE];		//cdma2000“频段值”列表，“-1”代表结束
	int ir_TD_Band_List[TD_BAND_SIZE];		//TD“频段值”列表，“-1”代表结束
	int ir_LTE_Band_List[L_BAND_SIZE];		//LTE“频段值”列表，“-1”代表结束
	int ir_NBIoT_Band_List[NB_IDX_B_SIZE];	//NB-IoT“频段值”列表，“-1”代表结束
	int i_GSM_Band_Quan;	//GSM“频段”数量
	int i_WCDMA_Band_Quan;	//WCDMA“频段”数量
	int i_C2K_Band_Quan;	//cdma2000“频段”数量
	int i_TD_Band_Quan;		//TD“频段”数量
	int i_LTE_Band_Quan;	//LTE“频段”数量
	int i_NBIoT_Band_Quan;	//NB-IoT“频段”数量
}BAND_LIST_S;



//单个天线对应的频段结构体 V2版
typedef struct tagBAND_LIST_V2_S
{
	int ir_GSM_Band_List[G_BAND_SIZE];				//GSM“频段index值”列表，“-1”代表结束
	int ir_WCDMA_Band_List[W_BAND_SIZE];			//WCDMA“频段值”列表，“-1”代表结束
	int ir_C2K_Band_List[C2K_BC_SIZE];				//cdma2000“频段值”列表，“-1”代表结束
	int ir_TD_Band_List[TD_BAND_SIZE];				//TD“频段值”列表，“-1”代表结束
	int ir_LTE_Band_List[L_BAND_SIZE];				//LTE“频段值”列表，“-1”代表结束
	int ir_NBIoT_Band_List[NB_IDX_B_SIZE];			//NB-IoT“频段值”列表，“-1”代表结束
	int ir_NR_5G_SUB6_Band_List[NR5GSUB6_N_SIZE];	//NR 5G SUB6 “频段值”列表，“-1”代表结束
	int ir_CV2X_Band_List[CV2X_BAND_SIZE];			//C-V2X“频段值”列表，“-1”代表结束
	int i_GSM_Band_Quan;		//GSM“频段”数量
	int i_WCDMA_Band_Quan;		//WCDMA“频段”数量
	int i_C2K_Band_Quan;		//cdma2000“频段”数量
	int i_TD_Band_Quan;			//TD“频段”数量
	int i_LTE_Band_Quan;		//LTE“频段”数量
	int i_NBIoT_Band_Quan;		//NB-IoT“频段”数量
	int i_NR_5G_SUB6_Band_Quan;	//NR 5G SUB6 “频段”数量
	int i_CV2X_Band_Quan;		//C-V2X“频段”数量
	BAND_CONFIG_S strr_GSM_Band_CFG[G_BAND_SIZE];				//2G “频段配置参数”
	BAND_CONFIG_S strr_WCDMA_Band_CFG[W_BAND_SIZE];				//WCDMA “频段配置参数”
	BAND_CONFIG_S strr_LTE_Band_CFG[L_BAND_SIZE];				//LTE “频段配置参数”
	BAND_CONFIG_S strr_NR_5G_SUB6_Band_CFG[NR5GSUB6_N_SIZE];	//NR 5G SUB6 “频段配置参数”
}BAND_LIST_V2_S;



//DUT单个天线名和属性结构体
typedef struct tagANT_PROP_S
{
	EU_ANT_TYPE_INDEX eu_ANT_HW_Type_Name;	//硬件实体天线（类型）名,“-1”代表无天线
	int i_ANT_SW_Numb;						//高通定义的软件天线号（0~7）, 无此号码则设定为“-1”
}ANT_PROP_S;


//天线列表结构体
typedef struct tagANT_LIST_S
{
	ANT_PROP_S strr_ANT_Prop_List[ANT_TYPE_BUFF_SIZE];			//DUT的天线（属性、Index号）列表（以天线Index号列表排列，以“-1”结尾）
	int i_ANT_Quan;												//DUT的天线数量
}ANT_LIST_S;


//DUT所有天线完整结构体 V1 版
typedef struct tagANT_CFG_ALL_S
{
	ANT_LIST_S str_ANT_List;									//DUT的所有天线（属性、Index号）列表
	TESTER_RF_OFFSET_V2_S strr_ANT_RF_Loss[ANT_TYPE_IDX_SIZE];	//天线的所有频段“线损”（以“天线Index号”作为结构体数组Buff的存储、访问index号）
}ANT_CFG_ALL_S;


//DUT所有天线完整结构体 V2 版
typedef struct tagANT_CFG_ALL_V2_S
{
	ANT_LIST_S str_ANT_List;											//DUT的所有天线（属性、Index号）列表
	TESTER_RF_OFFSET_V2_S strr_ANT_RF_Loss_List[ANT_TYPE_BUFF_SIZE];	//DUT的天线的所有频段“线损”（以“str_ANT_List”中的“天线Index号的排列顺序”作为结构体数组Buff的“存储顺序”，不再存储用不到的天线）
}ANT_CFG_ALL_V2_S;


//单个天线、单个频段的金板低中高信道数据
typedef struct tagREF_DATA_BASIC_S
{
	int i_Band_Name_And_Enable;										//Band名 和 Band使能标识
	double dr_Tx_Power[CH_LMH_COUNT];								//TX金板功率数据
	double dr_Rx_Level[CH_LMH_COUNT];								//RX金板接收电平数据
}REF_DATA_BASIC_S;


//单个天线、所有频段的金板数据
typedef struct tagREF_DATA_BAND_S
{
	REF_DATA_BASIC_S strr_GSM_REF_Data[G_BAND_SIZE];				//2G 所有频段的金板数据（用频段index号存储，像线损那样存储）
	REF_DATA_BASIC_S strr_WCDMA_REF_Data[W_BAND_SIZE];				//WCDMA 所有频段的金板数据（用频段index号存储，像线损那样存储）
	REF_DATA_BASIC_S strr_C2K_REF_Data[C2K_BC_SIZE];				//cdma2000 所有频段的金板数据（用频段index号存储，像线损那样存储）
	REF_DATA_BASIC_S strr_TD_REF_Data[TD_BAND_SIZE];				//TD 所有频段的金板数据（用频段index号存储，像线损那样存储）
	REF_DATA_BASIC_S strr_LTE_REF_Data[L_BAND_SIZE];				//LTE 所有频段的金板数据（用频段index号存储，像线损那样存储）
	REF_DATA_BASIC_S strr_NBIoT_REF_Data[NB_IDX_B_SIZE];			//NB-IoT 所有频段的金板数据（用频段index号存储，像线损那样存储）
	REF_DATA_BASIC_S strr_NR_5G_SUB6_REF_Data[NR5GSUB6_N_SIZE];		//NR 5G SUB6 所有频段的金板数据（用频段index号存储，像线损那样存储）
	REF_DATA_BASIC_S strr_CV2X_REF_Data[CV2X_BAND_SIZE];			//C-V2X 所有频段的金板数据（用频段index号存储，像线损那样存储）
	REF_DATA_BASIC_S str_GPS_REF_Data;								//GPS的金板数据（用频段index号存储，像线损那样存储）
	REF_DATA_BASIC_S strr_WiFi_REF_Data[WIFI_FB_COUNT];				//WiFi的2.4G和5G的金板数据（用频段index号存储，像线损那样存储）
	REF_DATA_BASIC_S str_Bluetooth_REF_Data;						//蓝牙的金板数据（用频段index号存储，像线损那样存储）
}REF_DATA_BAND_S;


//所有天线、所有频段的金板数据 V1 版
typedef struct tagREF_DATA_ALL_S
{
	ANT_LIST_S str_Ref_ANT_List;									//金板制作和校准涉及的天线（属性、Index号）列表
	REF_DATA_BAND_S strr_Ref_Band_Data[ANT_TYPE_IDX_SIZE];			//天线的所有频段“金板数据”（以“天线Index号”作为结构体数组Buff的存储、访问index号）
}REF_DATA_ALL_S;


//所有天线、所有频段的金板数据 V2 版
typedef struct tagREF_DATA_ALL_V2_S
{
	ANT_LIST_S str_Ref_Ant_List;									//金板DUT所操作到的天线（属性、Index号）列表
	REF_DATA_BAND_S strr_Ref_Data_List[ANT_TYPE_BUFF_SIZE];			//金板DUT所操作到的天线的所有频段“金板数据”（以“str_Ref_Ant_List”中的“天线Index号的排列顺序”作为结构体数组Buff的“存储顺序”，不再存储用不到的天线）
}REF_DATA_ALL_V2_S;


//频段使能控制
typedef struct tagBAND_ENABLE_S
{
	int ir_GSM_Band_Enable[G_BAND_SIZE];				//GSM 使能列表，“0以下”代表不测试
	int ir_WCDMA_Band_Enable[W_BAND_SIZE];				//WCDMA 使能列表，“0以下”代表不测试
	int ir_C2K_Band_Enable[C2K_BC_SIZE];				//cdma2000 使能列表，“0以下”代表不测试
	int ir_TD_Band_Enable[TD_BAND_SIZE];				//TD 使能列表，“0以下”代表不测试
	int ir_LTE_Band_Enable[L_BAND_SIZE];				//LTE 使能列表，“0以下”代表不测试
	int ir_NBIoT_Band_Enable[NB_IDX_B_SIZE];			//NB-IoT 使能列表，“0以下”代表不测试
	int ir_NR_5G_SUB6_Band_Enable[NR5GSUB6_N_SIZE];		//NR 5G SUB6 使能列表，“0以下”代表不测试
	int ir_CV2X_Band_Enable[CV2X_BAND_SIZE];			//C-V2X 使能列表，“0以下”代表不测试
}BAND_ENABLE_S;



//机型“通用参数”（与exe界面设置匹配）
typedef struct tagPRODU_PARAMS_GEN_S
{
	CString cs_Produ_Name;			//机型名
	int i_Platform_Solution;		//平台方案
	int i_BB_Chip;					//BB芯片型号
	int ir_TS_Type_Seq[10];			//测试工位(Flag相关)列表, 测试属性列表
	BAND_LIST_V2_S str_Band_List;	//机型的完整频段数据
	CString cs_Produ_Params_File;	//参数文件路径
}PRODU_PARAMS_GEN_S;



//Max number of drives assuming primary/secondary, master/slave topology
#define MAX_IDE_DRIVES			16



//
typedef struct _IDENTIFY_DATA
{
	USHORT GeneralConfiguration;            // 00 00
	USHORT NumberOfCylinders;               // 02  1
	USHORT Reserved1;                       // 04  2
	USHORT NumberOfHeads;                   // 06  3
	USHORT UnformattedBytesPerTrack;        // 08  4
	USHORT UnformattedBytesPerSector;       // 0A  5
	USHORT SectorsPerTrack;                 // 0C  6
	USHORT VendorUnique1[3];                // 0E  7-9
	USHORT SerialNumber[10];                // 14  10-19
	USHORT BufferType;                      // 28  20
	USHORT BufferSectorSize;                // 2A  21
	USHORT NumberOfEccBytes;                // 2C  22
	USHORT FirmwareRevision[4];             // 2E  23-26
	USHORT ModelNumber[20];                 // 36  27-46
	UCHAR  MaximumBlockTransfer;            // 5E  47
	UCHAR  VendorUnique2;                   // 5F
	USHORT DoubleWordIo;                    // 60  48
	USHORT Capabilities;                    // 62  49
	USHORT Reserved2;                       // 64  50
	UCHAR  VendorUnique3;                   // 66  51
	UCHAR  PioCycleTimingMode;              // 67
	UCHAR  VendorUnique4;                   // 68  52
	UCHAR  DmaCycleTimingMode;              // 69
	USHORT TranslationFieldsValid : 1;      // 6A  53
	USHORT Reserved3 : 15;
	USHORT NumberOfCurrentCylinders;        // 6C  54
	USHORT NumberOfCurrentHeads;            // 6E  55
	USHORT CurrentSectorsPerTrack;          // 70  56
	ULONG  CurrentSectorCapacity;           // 72  57-58
	USHORT CurrentMultiSectorSetting;       //     59
	ULONG  UserAddressableSectors;          //     60-61
	USHORT SingleWordDMASupport : 8;        //     62
	USHORT SingleWordDMAActive : 8;
	USHORT MultiWordDMASupport : 8;         //     63
	USHORT MultiWordDMAActive : 8;
	USHORT AdvancedPIOModes : 8;            //     64
	USHORT Reserved4 : 8;
	USHORT MinimumMWXferCycleTime;          //     65
	USHORT RecommendedMWXferCycleTime;      //     66
	USHORT MinimumPIOCycleTime;             //     67
	USHORT MinimumPIOCycleTimeIORDY;        //     68
	USHORT Reserved5[2];                    //     69-70
	USHORT ReleaseTimeOverlapped;           //     71
	USHORT ReleaseTimeServiceCommand;       //     72
	USHORT MajorRevision;                   //     73
	USHORT MinorRevision;                   //     74
	USHORT Reserved6[50];                   //     75-126
	USHORT SpecialFunctionsEnabled;         //     127
	USHORT Reserved7[128];                  //     128-255
} IDENTIFY_DATA, *PIDENTIFY_DATA;



//产品类型
#define PRODU_TYPE_NDF				0
#define PRODU_TYPE_PCBA				1
#define PRODU_TYPE_MINIPCIE			2
#define PRODU_TYPE_TE_A				3
#define PRODU_TYPE_TE_C				4
#define PRODU_TYPE_TE_B				5
#define PRODU_TYPE_KIT				6
#define PRODU_TYPE_MOB				7//Module On Board
#define PRODU_TYPE_COB				8//Chip On Board
#define PRODU_TYPE_ASM				9//Assembly 整机（使用自定义流程）



//产品版本
#define PRODU_VER_NDF				0
#define PRODU_VER_STD				1
#define PRODU_VER_GW				2//国网
#define PRODU_VER_NW				3//南网
#define PRODU_VER_ING				4//ingenico
#define PRODU_VER_ZJXD				5//中交信达
#define PRODU_VER_ZJ01				6//中交信达
#define PRODU_VER_HIK				7
#define PRODU_VER_KT				8
#define PRODU_VER_SKT				9
#define PRODU_VER_CM				10



//CAT ( Category 值, 用户设备能够支持的4GLTE网络传输速率的等级, 也可以说成是4G网络速度的一个技术标准 )
typedef enum tagEU_LTE_CAT_IDX
{
	LTE_CAT_NDF = -1,	//未定义
	LTE_CAT_0 = 0,
	LTE_CAT_1,
	LTE_CAT_2,
	LTE_CAT_3,
	LTE_CAT_4,
	LTE_CAT_5,
	LTE_CAT_6,
	LTE_CAT_7,
	LTE_CAT_8,
	LTE_CAT_9,
	LTE_CAT_10,
	LTE_CAT_M,
	LTE_CAT_NB,
	LTE_CAT_11,
	LTE_CAT_12,
	LTE_CAT_18,
	LTE_CAT_16,
	LTE_CAT_20,
	LTE_CAT_19,
	LTE_CAT_15,
	LTE_CAT_13,
	//
	LTE_CAT_COUNT
}EU_LTE_CAT_IDX;




//prototype of callback function
typedef void( __stdcall * CALLBACK_SHOW_REDIT_INFO )(const wchar_t * wcp_Msg, void * p_usr_Arg);
typedef void( __stdcall * CALLBACK_SHOW_STATE )(const wchar_t * wcp_State, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_SHOW_TEST_RESULT )(const wchar_t * wcp_Name, double d_Low, double d_High, double d_Data, void * p_usr_Arg);
typedef void( __stdcall * CALLBACK_SAVE_TRACE_LOG )(const wchar_t * wcp_Trace_Log, void * p_usr_Arg);
typedef void( __stdcall * CALLBACK_SHOW_TITLE )(const wchar_t * wcp_Title, void * p_usr_Arg);
//参数读取接口
typedef bool( __stdcall * CALLBACK_LOAD_PRODU_PARAMS_GEN )(const wchar_t * wcp_Produ_Name, int i_Produ_Type, int i_Produ_Ver, void * p_usr_Arg);
typedef void( __stdcall * CALLBACK_OCP_GET_ERROR_INFO )(CString * csp_Error_Info, void * p_usr_Arg);
//
typedef bool( __stdcall * CALLBACK_READ_PRODU_GSM_BAND_LIST )(int * ip_GSM_Band_List, int * ip_GSM_Band_Quan, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_WCDMA_BAND_LIST )(int * ip_WCDMA_Band_List, int * ip_WCDMA_Band_Quan, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_C2K_BAND_LIST )(int * ip_C2K_Band_List, int * ip_C2K_Band_Quan, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_TD_BAND_LIST )(int * ip_TD_Band_List, int * ip_TD_Band_Quan, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_LTE_BAND_LIST )(int * ip_LTE_Band_List, int * ip_LTE_Band_Quan, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_NBIOT_BAND_LIST )(int * ip_NBIoT_Band_List, int * ip_NBIoT_Band_Quan, void * p_usr_Arg);
//
typedef bool( __stdcall * CALLBACK_READ_PRODU_PF_AND_BB_CHIP )(int * ip_Platform_Solution, int * ip_BB_Chip, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_TS_TYPE_SEQ )(int * ip_TS_Type_Seq, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_INIT_TIME )(int * ip_Init_Time, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_CAL_TREE_NAME )(CString * csp_Cal_Tree_Name, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_VOLT_AND_CURR )(double * dp_Voltage, double * dp_Current, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_LTE_CAT )(int * ip_LTE_CAT, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_LANDI_ENABLE )(bool * bp_Landi_Enable, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_WIFI_ENABLE )(bool * bp_WiFi_Enable, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_NW_SCAN_MODE )(int * ip_Nw_Scan_Mode, void * p_usr_Arg);
typedef void( __stdcall * CALLBACK_READ_PRODU_HW_SIM_ENABLE )(const wchar_t * wcp_OC, bool * bp_HW_SIM_Enable, void * p_usr_Arg);
//
typedef void( __stdcall * CALLBACK_SHOW_PLC_REG_DATA )(int i_Addr, unsigned short us_Data, void * p_usr_Arg);
typedef void( __stdcall * CALLBACK_SEND_PLC_TRAY_DATA )(unsigned short us_Tray_Data, unsigned short us_TS_Data, const wchar_t * wcp_Barcode, void * p_usr_Arg);
typedef void( __stdcall * CALLBACK_UPDATE_TS_STATUS )(int i_TS_Channel, int i_TS_Status, void * p_usr_Arg);
typedef void( __stdcall * CALLBACK_UPDATE_TS_PFQUAN )(int i_TS_Channel, int i_Quan_Pass, int i_Quan_Fail, void * p_usr_Arg);
typedef void( __stdcall * CALLBACK_UPDATE_TS_TRD )(int i_TS_Channel, const wchar_t * wcp_TRD, void * p_usr_Arg);
//外部供电控制
typedef void( __stdcall * CALLBACK_EXT_PS_POWER_ON )(void * p_usr_Arg);
typedef void( __stdcall * CALLBACK_EXT_PS_POWER_OFF )(void * p_usr_Arg);
//callback 结构体 V2 中新增、追加的ocp函数接口
typedef bool( __stdcall * CALLBACK_READ_PRODU_PS_CH2_VOLT_AND_CURR )(double * dp_Voltage_CH2, double * dp_Current_CH2, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_NETWORK_FORMAT_BAND_LIST )(int i_Network_Format, int * ip_Band_List, int * ip_Band_Quan, void * p_usr_Arg);
//callback 结构体 V3 中新增、追加的ocp函数接口
typedef bool( __stdcall * CALLBACK_READ_PRODU_NETWORK_FORMAT_BAND_ANT_LIST )(int i_Network_Format, int i_Rx_Chain_Type, int * ip_Band_ANT_List, int * ip_Band_ANT_Quan, void * p_usr_Arg);
typedef bool( __stdcall * CALLBACK_READ_PRODU_ANT_LIST )(ANT_PROP_S * strp_ANT_Prop_List, int * ip_ANT_Quan, void * p_usr_Arg);
//callback 结构体 sql_db 函数接口
typedef bool( __stdcall * CALLBACK_FIND_READ_REF_STD_2017_CONN_V1 )(const wchar_t * wcp_Key_IMEI, CString & oj_cs_Ref_Produ_Name, CString & oj_cs_Ref_Data_Str, CString & oj_cs_Err_Info, void * p_usr_Arg);




//主界面显示 callback 结构体
typedef struct tagQTI_PROG_ARG
{
	//qti progress callback
	void *							p_usr_Arguments;
	CALLBACK_SHOW_REDIT_INFO		cb_Show_REdit_Info;
	CALLBACK_SHOW_STATE				cb_Show_State;
	CALLBACK_SHOW_TEST_RESULT		cb_Show_Test_Result;
	CALLBACK_SAVE_TRACE_LOG			cb_Save_Trace_Log;
	CALLBACK_SHOW_TITLE				cb_Show_Title;
}QTI_PROG_ARG;



//读取、输入OCP参数的 callback 结构体 V1
typedef struct tagREAD_OCP_PARAMS_ARG
{
	//read ocp params callback
	void *									p_usr_Arguments;
	//
	CALLBACK_LOAD_PRODU_PARAMS_GEN			cb_Load_Produ_Params_Gen;
	CALLBACK_OCP_GET_ERROR_INFO				cb_OCP_Get_Error_Info;
	//
	CALLBACK_READ_PRODU_GSM_BAND_LIST		cb_Read_Produ_GSM_Band_List;
	CALLBACK_READ_PRODU_WCDMA_BAND_LIST		cb_Read_Produ_WCDMA_Band_List;
	CALLBACK_READ_PRODU_C2K_BAND_LIST		cb_Read_Produ_C2K_Band_List;
	CALLBACK_READ_PRODU_TD_BAND_LIST		cb_Read_Produ_TD_Band_List;
	CALLBACK_READ_PRODU_LTE_BAND_LIST		cb_Read_Produ_LTE_Band_List;
	CALLBACK_READ_PRODU_NBIOT_BAND_LIST		cb_Read_Produ_NBIoT_Band_List;
	//
	CALLBACK_READ_PRODU_PF_AND_BB_CHIP		cb_Read_Produ_PF_And_BB_Chip;
	CALLBACK_READ_PRODU_TS_TYPE_SEQ			cb_Read_Produ_TS_Type_Seq;
	CALLBACK_READ_PRODU_INIT_TIME			cb_Read_Produ_Init_Time;
	CALLBACK_READ_PRODU_CAL_TREE_NAME		cb_Read_Produ_Cal_Tree_Name;
	CALLBACK_READ_PRODU_VOLT_AND_CURR		cb_Read_Produ_Volt_And_Curr;
	CALLBACK_READ_PRODU_LTE_CAT				cb_Read_Produ_LTE_CAT;
	CALLBACK_READ_PRODU_LANDI_ENABLE		cb_Read_Produ_Landi_Enable;
	CALLBACK_READ_PRODU_WIFI_ENABLE			cb_Read_Produ_WiFi_Enable;
	CALLBACK_READ_PRODU_NW_SCAN_MODE		cb_Read_Produ_Nw_Scan_Mode;
	CALLBACK_READ_PRODU_HW_SIM_ENABLE		cb_Read_Produ_HW_SIM_Enable;
}READ_OCP_PARAMS_ARG;



//读取、输入OCP参数的 callback 结构体 V2 （新增、追加的ocp函数接口）
typedef struct tagREAD_OCP_PARAMS_V2_ARG
{
	//read ocp params callback
	void *												p_usr_Arguments;
	//
	CALLBACK_READ_PRODU_PS_CH2_VOLT_AND_CURR			cb_Read_Produ_PS_CH2_Volt_And_Curr;
	CALLBACK_READ_PRODU_NETWORK_FORMAT_BAND_LIST		cb_Read_Produ_Network_Format_Band_List;
}READ_OCP_PARAMS_V2_ARG;



//读取、输入OCP参数的 callback 结构体 V3 （新增、追加的ocp函数接口）
typedef struct tagREAD_OCP_PARAMS_V3_ARG
{
	//read ocp params callback
	void *												p_usr_Arguments;
	//
	CALLBACK_READ_PRODU_NETWORK_FORMAT_BAND_ANT_LIST	cb_Read_Produ_Network_Format_Band_ANT_List;
	CALLBACK_READ_PRODU_ANT_LIST						cb_Read_Produ_ANT_List;
}READ_OCP_PARAMS_V3_ARG;



//控制中心数据传输 callback 结构体
typedef struct tagCTU_PROG_ARG
{
	//ctu(控制中心) progress callback
	void *							p_usr_Arguments;
	CALLBACK_SHOW_REDIT_INFO		cb_Show_REdit_Info;
	CALLBACK_SHOW_PLC_REG_DATA		cb_Show_PLC_Reg_Data;
	CALLBACK_SEND_PLC_TRAY_DATA		cb_Send_PLC_Tray_Data;
	CALLBACK_UPDATE_TS_STATUS		cb_Update_TS_Status;
	CALLBACK_UPDATE_TS_PFQUAN		cb_Update_TS_PFQuan;
	CALLBACK_UPDATE_TS_TRD			cb_Update_TS_TRD;
}CTU_PROG_ARG;



//外部供电的控制 callback 结构体
typedef struct tagEXT_PS_ARG
{
	//外部供电的控制回调接口
	void *							p_usr_Arguments;
	CALLBACK_EXT_PS_POWER_ON		cb_Ext_PS_Power_On;
	CALLBACK_EXT_PS_POWER_OFF		cb_Ext_PS_Power_Off;
}EXT_PS_ARG;



//用于 sql_db 操作的 callback 结构体 V1
typedef struct tagOP_SQL_DB_V1_ARG
{
	//sql_db 操作的 callback
	void *										p_usr_Arguments;
	//
	CALLBACK_FIND_READ_REF_STD_2017_CONN_V1		cb_Find_Read_REF_STD_2017_CONN_V1;
}OP_SQL_DB_V1_ARG;




//
class CGFC
{
public:
	CGFC( void );
	~CGFC( void );
	void WCSTOK_INT( const wchar_t * wcp_StrToken, int * ip_List, int * ip_Quan );
	void WCSTOK_DOUBLE( const wchar_t * wcp_StrToken, double * dp_List, int * ip_Quan );
	bool WCSTOK_STRING_C( const wchar_t * wcp_StrToken, const wchar_t * wcp_StrDelimit, int i_Get_Index, CString * csp_Get_CString );
	bool WCSTOK_STRING_S( const wchar_t * wcp_StrToken, int i_Get_Index, CString * csp_Get_CString );
	void BUFF_BYTE_CALLOC( unsigned char ** ucp2_Buff, unsigned long ul_Buff_Size );
	void BUFF_BYTE_FREE( unsigned char ** ucp2_Buff );
	void BUFF_CHAR_CALLOC( char ** cp2_Buff, unsigned long ul_Buff_Size );
	void BUFF_CHAR_FREE( char ** cp2_Buff );
	void BUFF_DOUBLE_CALLOC( double ** dp2_Buff, unsigned long ul_Buff_Size );
	void BUFF_DOUBLE_FREE( double ** dp2_Buff );
	void BUFF_DWORD_CALLOC( DWORD ** dwp2_Buff, unsigned long ul_Buff_Size );
	void BUFF_DWORD_FREE( DWORD ** dwp2_Buff );
	void BUFF_WCHAR_CALLOC( wchar_t ** wcp2_Buff, unsigned long ul_Buff_Size );
	void BUFF_WCHAR_FREE( wchar_t ** wcp2_Buff );
	bool Confirm_Reg_Com_Port( const int i_Tgt_Com_Port );
	bool Confirm_Reg_Dynamic_Com_Port( int i_Tgt_Com_Port, int i_TimeOut );
	bool Get_Reg_Com_Port_List( int * ip_COM_List, int * ip_COM_Quan );
	bool PKT_BUFF_ADD_DATA( PKT_BUFF_S * strp_Pkt_Buff, const unsigned char * ucp_Input_Data, const unsigned long ul_Add_Quan );
	void PKT_BUFF_CALLOC( PKT_BUFF_S * strp_Pkt_Buff, const unsigned long ul_Buff_Size );
	bool PKT_BUFF_DEL_ALL( PKT_BUFF_S * strp_Pkt_Buff );
	void PKT_BUFF_DEL_DATA( PKT_BUFF_S * strp_Pkt_Buff, const unsigned long ul_Del_Quan );
	unsigned long PKT_BUFF_EXTRACT_DATA( PKT_BUFF_S * strp_Pkt_Buff, unsigned char * ucp_Get_Data, const unsigned long ul_Get_Quan );
	void PKT_BUFF_FREE( PKT_BUFF_S * strp_Pkt_Buff );
	void PKT_BUFF_INIT( PKT_BUFF_S * strp_Pkt_Buff );
	DWORD HEX_BYTE_TO_DWORD( unsigned char * ucp_Hex_Char, int i_Hex_Char_Quan );
	void MultiByte_To_WideChar( const char * cp_Char_8, wchar_t * wcp_WCHAR_16, int * ip_Len_16 );
	bool WideChar_To_MultiByte( const wchar_t * wcp_WCHAR_16, char * cp_Char_8, int * ip_Len_8 );
	bool mem_uc_str( unsigned char * ucp_Buff_Src, unsigned long ul_Quan_Src, unsigned char * ucp_Buff_Tgt, unsigned long ul_Quan_Tgt, unsigned long * ulp_Tgt_Index );
	bool Get_Local_PC_Name( CString * csp_PC_Name );
	bool GET_AT_RSP_DIGIT_DATA( const wchar_t * wcp_Src_Data, const wchar_t * wcp_Key_Word, int i_Line_Offset, double * dp_Digit_Data );
	bool GET_AT_RSP_STRING( const wchar_t * wcp_Src_Data, const wchar_t * wcp_Key_Word, int i_Line_Offset, CString * csp_String );
	bool Get_Reg_Usb_Dev_List( const wchar_t * wcp_USB_PID_VID, CString * csp_USB_Dev_List, int * ip_USB_Quan );
	bool Get_File_Directory( const wchar_t * wcp_File_Path, CString * csp_File_Directory );
	bool Kill_Process( const wchar_t * wcp_Process_Name );
	bool Calcu_ARFCN_GSM( int i_G_Band_Index, double d_Center_Freq, int i_Up_Down_Link, int * ip_ARFCN );
	bool Calcu_ARFCN_WCDMA( int i_W_Band_Index, double d_Center_Freq, int i_Up_Down_Link, int * ip_ARFCN );
	bool Calcu_Center_Freq_GSM( int i_G_Band_Index, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq );
	bool Calcu_Center_Freq_WCDMA( int i_W_Band_Index, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq );
	CString m_csError_Info;
	bool GET_AT_RSP_MID_STRING( const wchar_t * wcp_Src_Data, const wchar_t * wcp_Start_Key, const wchar_t * wcp_End_Key, CString * csp_Useful_String );
	void Empty_Directory( const wchar_t * wcp_Dir_Path );
	bool Get_Reg_Usb_Dev_Friendly_Name( const wchar_t * wcp_USB_PID_VID, const wchar_t * wcp_USB_Dev_Name, CString * csp_Friendly_Name );
	void WCSTOK_STRING_T( const wchar_t * wcp_StrToken, CString * csp_Get_CString_Array, int * ip_Get_Quan );
	bool Calcu_Center_Freq_LTE( int i_L_Band_Name, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq_MHz );
	bool Calcu_ARFCN_LTE( int i_L_Band_Name, double d_Center_Freq_MHz, int i_Up_Down_Link, int * ip_ARFCN );
	bool Get_LTE_Band_Name( int i_LTE_Band_Index, int * ip_L_Band_Name );
	bool Get_LTE_Band_Index( int i_LTE_Band_Name, int * ip_L_Band_Index );
	int Trans_UL_ARFCN_LTE( int i_L_Band_Name, int i_Arfcn_DL );
	int Trans_UL_ARFCN_WCDMA( int i_W_Band_Index, int i_Arfcn_DL );
	bool Calcu_Center_Freq_TD( int i_TD_Band_Name, int i_ARFCN, double * dp_Center_Freq_MHz );
	bool Calcu_ARFCN_TD( int i_TD_Band, double d_Center_Freq_MHz, int * ip_ARFCN );
	bool Get_TD_Band_Name( int i_TD_Band_Index, int * ip_TD_Band_Name );
	bool Get_TD_Band_Index( int i_TD_Band_Name, int * ip_TD_Band_Index );
	bool Encode_RF_Ref_Tx_Data_To_Str_V1( double d_Code_Key, int i_Band_And_Tx_Data_Quan, int * ip_Band_List, double * dp_RF_Tx_Data, CString * csp_Str_Data );
	bool Decode_RF_Ref_Tx_Data_From_Str_V1( const wchar_t * wcp_Ref_Str_Data, double d_Code_Key, int * ip_Band_And_Tx_Data_Quan, int * ip_Band_List, double * dp_RF_Tx_Data );
	void WCSTOK_STRING_T_C( const wchar_t * wcp_StrToken, const wchar_t * wcp_StrDelimit, CString * csp_Get_CString_Array, int * ip_Get_Quan );
	int Calcu_IMEI_Checksum( const wchar_t * wcp_IMEI_14_Bits );
	bool Get_CDMA2000_Band_Name( int i_C2K_Band_Index, int * ip_C2K_Band_Name );
	bool Get_CDMA2000_Band_Index( int i_C2K_Band_Name, int * ip_C2K_Band_Index );
	bool Calcu_Center_Freq_CDMA2000( int i_C2K_Band_Name, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq_MHz );
	bool Get_WCDMA_Band_Name( int i_W_Band_Index, int * ip_W_Band_Name );
	bool Get_WCDMA_Band_Index( int i_W_Band_Name, int * ip_W_Band_Index );
	void Get_RF_Offset_Freq_Band_Seg( int i_Format, int i_Band_Index, double * dp_Freq_Tx_S, double * dp_Freq_Tx_E, double * dp_Freq_Rx_S, double * dp_Freq_Rx_E );
	bool GET_ARFCN_L_M_H_TYPE( int i_Band, int i_Format_Network, int i_ARFCN, int * ip_LMH_TYPE );
	void Adjust_From_Small_To_Large_Array_Int( int * ip_Data_List, int i_Quan );
	int Rand_One_Number( int i_Range );
	bool Calcu_MEID_Check_Digit( const wchar_t * wcp_MEID_Hex, wchar_t * wcp_Checksum );
	void WCSTOK_STRING_T_Q( const wchar_t * wcp_StrToken, int i_Take_Quan, CString * csp_Take_CString_Array );
	void Adjust_From_Small_To_Large_Array_Double( double * dp_Data_List, int i_Quan );
	bool ReadPhysicalDriveInNTUsingSmart( CString * csp_HDD_SN );
	void PrintIdeInfo( int drive, DWORD * diskdata, CString * csp_HDD_SN );
	void ConvertToString( DWORD * diskdata, int firstIndex, int lastIndex, char * buf );
	bool Get_Local_PC_IP_Name(CString * csp_PC_IP);
	int HEX_WCHAR_TO_SIGNED_INT32( const wchar_t * wcp_HEX_WCHAR );
	unsigned long HEX_WCHAR_TO_UNSIGNED_INT32( const wchar_t * wcp_HEX_WCHAR );
	bool Encode_RF_Ref_Tx_Data_To_Str_V2( double d_Code_Key, double * dp_RF_Tx_Data, int i_RF_Tx_Quan, int * ip_Band_List, int i_Band_Quan, CString * csp_Str_Data );
	bool Decode_RF_Ref_Tx_Data_From_Str_V2( const wchar_t * wcp_Ref_Str_Data, double d_Code_Key, int * ip_Band_List, int * ip_Band_Quan, int * ip_RF_Tx_Data_Quan, double * dp_RF_Tx_Data );
	bool Get_Data_In_Quotation_Marks( const wchar_t * wcp_Src_Data, CString * csp_Get_Data );
	void Get_PF_Name( int i_PF_Index, CString * csp_PF_Name );
	void Get_BB_Name( int i_BB_Index, CString * csp_BB_Name );
	void Get_Produ_Type_Name( int i_Produ_Type_Index, CString * csp_Produ_Type_Name );
	void Get_Produ_Ver_Name( int i_Produ_Ver_Index, CString * csp_Produ_Ver_Name );
	long HEX_WCHAR_TO_LONG_INT( const wchar_t * wcp_HEX_WCHAR );
	bool Calcu_Center_Freq_WiFi( int i_WiFi_CH, double * dp_Center_Freq_MHz );
	bool Get_WiFi_FreqBand_Name( int i_WiFi_FB_Index, CString * csp_WiFi_FB_Name );
	bool Get_WiFi_Standard_Name( int i_WiFi_STAN_Index, CString * csp_WiFi_STAN_Name );
	bool Calcu_Center_Freq_Bluetooth( int i_BLU_VER_Index, int i_BLU_CH, double * dp_Center_Freq_MHz );
	bool Get_Bluetooth_Version_Name( int i_BLU_VER_Index, CString * csp_BLU_VER_Name );
	bool Get_FileName_From_FilePath( const wchar_t * wcp_File_Path, bool b_Need_Extend_Name, CString * csp_File_Name );
	bool Del_Reg_Com_Port_List(void);
	void WCSTOK_STRING_S_C_Ex( const wchar_t * wcp_StrToken, wchar_t wc_StrDelimit, CString * csp_Get_CString_Array, int * ip_Get_Quan );
	bool Get_WiFi_Standard_FreqBand( int i_WiFi_STAN_Index, int * ip_WiFi_FB_Index );
	bool Get_LTE_Category_Name( int i_LTE_CAT_Index, CString * csp_LTE_CAT_Name );
	bool Get_GSM_Band_Name( int i_G_Band_Index, CString * csp_G_Band_Name );

	int  TimeCmp(CString strTimeSrc);
	bool Get_Locoal_IP_Mac(CString  cs_PC_IP, CString & cs_PC_Mac);
	int Get_Process_Attributes_Index( const CString cs_Process_Attributes_Name );
	bool Get_Process_Attributes_Name( const int i_Process_Attributes_Index, CString & csref_Process_Attributes );
	bool Get_NBIoT_Band_Index( int i_NBIoT_Band_Name, int * ip_NBIoT_Band_Index );
	bool Get_NBIoT_Band_Name( int i_NBIoT_Band_Index, int * ip_NBIoT_Band_Name );
	bool Get_LTE_Band_DD_FDD_TDD( int i_LTE_Band_Name, int & oj_i_LTE_DD );
	bool Get_DD_FDD_TDD_Name( int i_FDD_TDD, CString & oj_cs_DD_Name );
	bool Get_NR_5G_SUB6_Band_Index( int i_Band_Name, int & oj_i_Band_Index );
	bool Get_NR_5G_SUB6_Band_Name( int i_Band_Index, int & oj_i_Band_Name );
	bool Get_ANT_Type_Name( EU_ANT_TYPE_INDEX eu_Ant_Type_Index, CString & oj_cs_Ant_Type_Name );
	bool IS_SMART_MODULE( int i_BB_Chip );

	bool Find_Process( const wchar_t * wcp_Process_Name );//查找进程是否存在，若存在返回TRUE，不存在返回FALSE
	void Empty_Struct_TESTER_RF_OFFSET_S( TESTER_RF_OFFSET_S & oj_str_RF_Loss_V1 );
	void Empty_Struct_TESTER_RF_OFFSET_V2_S( TESTER_RF_OFFSET_V2_S & oj_str_RF_Loss_V2 );
	void Empty_Struct_ANT_CFG_ALL_S( ANT_CFG_ALL_S & oj_str_ANT_Config_All );
	void Empty_Struct_ANT_LIST_S( ANT_LIST_S & oj_str_ANT_List );
	void Empty_Struct_REF_DATA_ALL_S( REF_DATA_ALL_S & oj_str_Ref_Data_All );
	void Empty_Struct_REF_DATA_BAND_S( REF_DATA_BAND_S & oj_str_Ref_Data_Band );
	void Copy_Struct_ANT_CFG_ALL_S( ANT_CFG_ALL_S & oj_str_ANT_CFG_Dest, ANT_CFG_ALL_S str_ANT_CFG_Source );
	void Copy_Struct_ANT_LIST_S( ANT_LIST_S & oj_str_ANT_List_Dest, ANT_LIST_S str_ANT_List_Source );
	void Copy_Struct_TESTER_RF_OFFSET_V2_S( TESTER_RF_OFFSET_V2_S & oj_str_RF_Loss_Dest, TESTER_RF_OFFSET_V2_S str_RF_Loss_Source );
	void Copy_Struct_TESTER_RF_OFFSET_V2_S_To_TESTER_RF_OFFSET_S( TESTER_RF_OFFSET_S & oj_str_RF_Loss_V1_Dest, TESTER_RF_OFFSET_V2_S str_RF_Loss_V2_Source );
	void Copy_Struct_TESTER_RF_OFFSET_S_To_TESTER_RF_OFFSET_V2_S( TESTER_RF_OFFSET_V2_S & oj_str_RF_Loss_V2_Dest, TESTER_RF_OFFSET_S str_RF_Loss_V1_Source );
	bool Get_RF_Key_Type_Name( const EU_RF_KEY_TYPE_INDEX eu_RF_Key_Type_Index, CString & oj_cs_RF_Key_Type_Name );
	void Unicode_To_UTF8( const wchar_t * wcp_Unicode_Str, CStringA & oj_csa_UTF_8_Str );
	bool Get_CV2X_Band_Index( int i_CV2X_Band_Name, int * ip_CV2X_Band_Index );
	bool Get_CV2X_Band_Name( int i_CV2X_Band_Index, int * ip_CV2X_Band_Name );
	void Copy_Struct_BAND_ENABLE_S( BAND_ENABLE_S & oj_str_Band_Enable_Dest, BAND_ENABLE_S str_Band_Enable_Source );
	void Assign_Struct_BAND_ENABLE_S( BAND_ENABLE_S & oj_str_Band_Enable, int i_Value_Data );
	bool Get_NB_SCSP_Name( int i_S3K75_S15K, CString & oj_cs_NB_SCSP_Name );
	bool Get_NB_Mod_Scheme_Name( int i_BPSK_QPSK, CString & oj_cs_NB_Mod_Scheme_Name );
	bool Get_NR_5G_MMW_Band_Index( int i_Band_Name, int & oj_i_Band_Index );
	bool Get_NR_5G_MMW_Band_Name( int i_Band_Index, int & oj_i_Band_Name );
	bool Get_ANT_Type_Index( const wchar_t * wcp_Ant_Type_Name, EU_ANT_TYPE_INDEX & oj_eu_Ant_Type_Idx );
	void Get_RF_Instru_Name( int i_Instru_Index, CString & oj_cs_Instru_Name );
	void Empty_Struct_REF_DATA_ALL_V2_S( REF_DATA_ALL_V2_S & oj_str_Ref_Data_All_V2 );
	bool Get_ANT_Type_List_Buff_Index( ANT_LIST_S str_Ant_List, EU_ANT_TYPE_INDEX eu_Ant_Type_Idx, int & oj_i_Ant_Buff_Index );
	void Empty_Struct_ANT_CFG_ALL_V2_S( ANT_CFG_ALL_V2_S & oj_str_ANT_Config_All );
	void Copy_Struct_ANT_CFG_ALL_V2_S( ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_Dest, ANT_CFG_ALL_V2_S str_ANT_CFG_Source );
	void Copy_Struct_ANT_CFG_ALL_V2_S_To_ANT_CFG_ALL_S( ANT_CFG_ALL_S & oj_str_ANT_CFG_V1_Dest, const ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_V2_Source );
	void Copy_Struct_ANT_CFG_ALL_S_To_ANT_CFG_ALL_V2_S( ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_V2_Dest, const ANT_CFG_ALL_S & oj_str_ANT_CFG_V1_Source );
	bool Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S( const ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_V2, EU_ANT_TYPE_INDEX eu_Ant_Type_Idx, int i_Format_Network, int i_Band_Name, int i_L_M_H, double & oj_d_RF_Loss_Tx_Input, double & oj_d_RF_Loss_Rx_Output );
	bool Get_Rx_Chain_Type_Name( int i_Rx_Chain_Type, CString & oj_cs_Rx_Chain_Type_Name );
	bool Determine_Type_Of_Plan_Connect_ANT_And_RF_Port( const ANT_CFG_ALL_V2_S str_ANT_CFG_ALL_V2, int & oj_i_Plan_Connect_ANT_And_RF_Port );
};

