#include "stdafx.h"
#include "GFC.h"

#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库


CCriticalSection g_del_exe_Ctrl;

/*
Ver2.0.67 (2022-4-24)

2014-2-26，xjli
1.添加读PC名的函数“Get_Local_PC_Name”。

2014-3-5，xjli
1.修正了“WCSTOK_STRING_S”、“WCSTOK_DOUBLE”、“WCSTOK_INT”函数运行时会破坏“源数据”影响反复提取数据的问题。

2014-3-9，xjli
1.修正了“WCSTOK_STRING_S”、“WCSTOK_DOUBLE”、“WCSTOK_INT”函数的间隔符为“ ,;\r\n”。

2014-3-18，xjli
1.添加了“WCSTOK_STRING_C”函数，允许自定义“间隔符”。

2014-3-28，xjli
1.添加了“GET_AT_RSP_STRING”和“GET_AT_RSP_DIGIT_DATA”函数。

2014-5-27，xjli
1.添加了“Get_Reg_Usb_Dev_List”、“Get_File_Directory”、“Kill_Process”函数。

2014-6-19，xjli
1.添加了“Calcu_ARFCN_GSM”、“Calcu_Center_Freq_GSM”、“Calcu_Center_Freq_WCDMA”函数。

2014-6-23，xjli
1.添加了“GET_AT_RSP_MID_STRING”函数。

2014-6-24，xjli
1.修正了“WCSTOK_STRING_C”和“WCSTOK_STRING_S”函数的内存没有释放的问题。

2014-8-8，xjli
1.添加了“Empty_Directory”函数。

2014-12-4，xjli
1.添加了“Get_Reg_Usb_Dev_Friendly_Name”函数。

2015-2-9，xjli
1.添加了“HEX_WCHAR_TO_LONG_INT”函数。

2015-4-29，xjli
1.添加了“WCSTOK_STRING_T”函数。

2015-7-11，xjli
1.添加了“Calcu_Center_Freq_LTE”、“Calcu_ARFCN_LTE”、“Get_LTE_Band_Index”函数。

2015-7-15，xjli
1.添加了“Get_LTE_Band_Name”、“Trans_UL_ARFCN_LTE”、“Trans_UL_ARFCN_WCDMA”、“Calcu_Center_Freq_TD”、“Calcu_ARFCN_TD”、“Get_TD_Band_Name”、“Get_TD_Band_Index”、“Encode_RF_Ref_Tx_Data_To_Str”、“Decode_RF_Ref_Tx_Data_From_Str”函数。

2015-9-14，xjli
1.把“RF_TX_UL_INPUT、RF_RX_DL_OUTPUT定义”和“各频段的index定义”改放到“GFC.h”文件中。

2015-9-18，xjli
1.添加了“WCSTOK_STRING_T_C()”函数。

2015-10-9，xjli
1.添加了“Calcu_IMEI_Checksum()”函数。

2015-11-8，xjli
1.更新了LTE频段号、频率、频段index号的计算函数。
2.增加了新的cdma2000的枚举“EU_C2K_BAND_IDX”。

2015-12-8，xjli
1.增加了“TD_BAND_39”定义。
2.添加了“Get_WCDMA_Band_Name()”和“Get_WCDMA_Band_Index()”函数。

2015-12-30，xjli
1.添加了“Get_RF_Offset_Freq_Band_Seg()”函数。

2016-1-6，xjli
1.添加了“GET_ARFCN_L_M_H_TYPE()”函数。

2016-1-12，xjli
1.添加了“Adjust_From_Small_To_Large_Array_Int()”函数。

2016-2-2，xjli
1.编辑了“Calcu_Center_Freq_TD()”函数，增加了39频段的支持。

2016-2-22，xjli
1.添加了“Rand_One_Number()”函数。

2016-3-11，xjli
1.变更了“BAND_LIST_S”结构体的定义，增加了“频段数量”参数。

2016-4-6，xjli
1.变更了“QTI_PROG_ARG”结构体的定义，去掉了“debug log”参数。

2016-4-6，xjli
1.添加了“Calcu_MEID_Check_Digit()”函数。

2016-4-6，xjli
1.变更了“TS_CONFIG_S”结构体的定义，增加了“debug log”参数。

2016-4-12，xjli
1.变更了“TS_CONFIG_S”结构体的定义，增加了“i_Com_AT_Port”参数。

2016-4-15，xjli
1.添加了“WCSTOK_STRING_T_Q()”函数。

2016-5-19，xjli
1.修改了“Trans_UL_ARFCN_WCDMA()函数”和“Calcu_Center_Freq_WCDMA()函数”的参数名，明确了参数是index还是name。

2016-6-7，xjli
1.添加了“Adjust_From_Small_To_Large_Array_Double()”函数。

2016-7-28，xjli
1.添加了读取PC硬盘号的“ReadPhysicalDriveInNTUsingSmart()”函数。

2016-8-5，xjli
1.添加了“HEX_WCHAR_TO_SIGNED_INT32()”和“HEX_WCHAR_TO_UNSIGNED_INT32()”函数。

2016-10-18，xjli
1.添加了“Encode_RF_Ref_Tx_Data_To_Str_V2()”和修改了函数名“Encode_RF_Ref_Tx_Data_To_Str_V1()”。
2.添加了“Decode_RF_Ref_Tx_Data_From_Str_V2()”和修改了函数名“Decode_RF_Ref_Tx_Data_From_Str_V1()”。

2016-10-31，xjli
1.添加了“LTE B13、B18、B19、B25、B26、B28 频段”的定义，以及相关信道、频点计算的支持。

2017-2-17，xjli
1.把callback回调函数的定义转移到GFC.h文件中。
2.添加了“CTU_PROG_ARG”callback回调函数结构体定义。

2017-3-1，xjli
1.添加了“Get_Data_In_Quotation_Marks()”函数，用于从两个“引号”间提取数据。
2.增加了“WCDMA的 B6 和 B19 频段”的定义和相关数据、算法的支持。
3.修改了“Get_WCDMA_Band_Name()”函数和“Get_WCDMA_Band_Index()”函数的转换代码。

2017-4-5，xjli
1.添加了“外部供电的控制 callback 结构体”的定义。

2017-7-3，xjli
1.添加了“LTE B34 频段”的定义，以及相关信道、频点计算的支持。

2017-7-7，xjli
1.添加了“Get_BB_Name()”、“Get_PF_Name()”、“Get_Produ_Type_Name()”、“Get_Produ_Ver_Name()”函数。

2017-7-27，xjli
1.更新了“CTU_PROG_ARG”的结构体定义，添加了“CALLBACK_UPDATE_TS_PFQUAN()”函数。

2017-8-6，xjli
1.更新了“CTU_PROG_ARG”的结构体定义，添加了“CALLBACK_UPDATE_TS_TRD()”函数。

2017-8-10，xjli
1.添加了“HEX_WCHAR_TO_LONG_INT()”函数。

2017-10-11，xjli
1.变更了“HEX_WCHAR_TO_SIGNED_INT32()”函数的代码（算法），解决了个别HEX字串转换“有符号int”出错的问题。

2017-11-3，xjli
1.添加了“高通 SDM450”芯片的定义。

2017-11-22，xjli
1.添加了“高通 BB_WCN3610、BB_WCN3660、BB_WCN3680”芯片的定义。
2.添加了WiFi的“WIFI_FREQ_B24G、WIFI_FREQ_B5G”频段的定义。
3.添加了计算WiFi中心频点的函数“Calcu_Center_Freq_WiFi()”。

2017-11-30，xjli
1.添加了“Get_WiFi_FreqBand_Name()”和“Get_WiFi_Standard_Name()”函数。

2017-12-1，xjli
1.添加了Bluetooth的“蓝牙版本序列号”的EU_BLU_VER_IDX定义。
2.添加了“Calcu_Center_Freq_Bluetooth()”函数。

2017-12-8，xjli
1.添加了“Get_Bluetooth_Version_Name()”函数。
2.添加了“高通 MSM8905”芯片的定义。

2018-1-5，xjli
1.添加了“MTK MT2625”芯片的定义。

2018-1-23，xjli
1.添加了“Get_FileName_From_FilePath()”函数。

2018-2-7，xjli
1.完成了向VS2013系统的转化，变更了“swprintf”、“wcstok_s”、“_ui64tow_s”函数相关代码，消除了“C4996错误”。

2018-4-2，xjli
1.添加了“MTK MT2503”芯片的定义。

2018-5-9，xjli
1.添加了“WCSTOK_STRING_S_C_Ex()”函数。

2018-6-6，xjli
1.变更了“Trans_UL_ARFCN_LTE()”函数的判定结构。
2.添加了“WCDMA B3”和“LTE B14/B29/B30/B66/B71”频段的定义。

2018-6-26，xjli
1.添加了“PRODU_TYPE_TE_B”产品类型的定义。
2.删除了显示相关的信息中的中文。保留了金板制作时的中文提示。

2018-7-2，xjli
1.添加了“WIFI_STAN_80211N_5G_20M”WiFi标准的定义和相关处理。
2.添加了“Get_WiFi_Standard_FreqBand()”函数。

2018-7-16，xjli
1.添加了“高通 SDX20”芯片的定义。

2018-7-29，xjli
1.把“tagEU_LTE_CAT_IDX”值的定义转移到了“GFC.h”文件中定义。
2.添加了“Get_LTE_Category_Name()”函数。

2018-9-3，xjli
1.添加了“LTE B42/B43/B48”频段的定义和频段相关数据的处理。
2.金板SN数据的编码、解码函数中的“频段字母编码”从“a~z”扩展到“A~Z”，以应对频段数量超过了26个的情况。
3.添加了“Get_GSM_Band_Name()”函数。

2018-9-21，xjli
1.添加了“RDA的2G芯片 RDA8955L”和“ASR的4G芯片 ASR1802S”芯片的定义和相关数据转换。
2.添加了“ASR”方案平台的定义和相关数据转换。
3.添加了“CAT-18”的Category值的定义和相关数据转换。

2018-9-26，xjli
1.添加了“PRODU_TYPE_KIT”产品类型的定义和处理。

2018-10-28，xjli
1.添加了“BB_MDM9150”BB芯片定义和处理。
2.添加了“LTE_CAT_NDF”CAT值定义和处理。

2018-11-20，xjli
1.添加了“BB_QCA6564”BB芯片定义和处理。
2.添加了“WCDMA B9”和“LTE B9/B11/B21”频段的定义。

2018-11-22，xjli
1.添加了“BB_SDM660”BB芯片定义和处理。

2018-11-28，xjli
1.添加了“Get_Process_Attributes_Index()”和“Get_Process_Attributes_Name()”。
2.添加了“测试流程属性”的宏定义。

2018-12-4，xjli
1.添加了“LTE B47”频段的定义。

2018-12-18，xjli
1.添加了“Get_NBIoT_Band_Index()”和“Get_NBIoT_Band_Name()”函数。
2.添加了“NB-IoT频段名”和“NB-IoT频段序列号”的宏定义。

2018-12-20，xjli
1.各频段的枚举定义增加了“SIZE”变量，用于控制频段相关的存储BUFF尺寸。
2.结构体“tagTESTER_RF_OFFSET_S”、“tagBAND_LIST_S”、“tagBAND_ENABLE_S”变更使用“SIZE”变量定义BUFF尺寸。
3.结构体“tagTESTER_RF_OFFSET_S”、“tagBAND_LIST_S”、“tagBAND_ENABLE_S”中添加了NB-IoT相关部分。

2018-12-21，xjli
1.添加了“BB_RDA8908A”和“BB_RDA8909B”BB芯片定义和处理。
2.更新了“tagREAD_OCP_PARAMS_ARG”ocp callback 结构体，添加了“CALLBACK_READ_PRODU_NBIOT_BAND_LIST”函数。

2018-12-25，xjli
1.添加了“BB_MT2621”BB芯片定义和处理。

2019-4-22，xjli
1.添加了“BB_SA415M”和“BB_SDX24”BB芯片定义和处理。
2.添加了“LTE_CAT_16”和“LTE_CAT_20”CAT值定义和处理。

2019-5-10，xjli
1.添加了“BB_MDM9205”、“BB_SDX55”BB芯片定义和处理。
2.添加了“LTE B46”、“LTE B27”和“LTE B85”频段的定义。

2019-7-3，xjli
1.添加了“FORMAT_NBIOT_S”、“FORMAT_NBIOT_NS”定义。
2.更新了“Get_RF_Offset_Freq_Band_Seg()”函数，添加了NB-IoT部分的频段、频点获取。
3.更新了“Get_RF_Offset_Freq_Band_Seg()”函数，修正了“LTE B4”的频段、频点数据。

2019-7-19，xjli
1.添加了“PF_REALTEK”平台、“BB_RTL8189”BB芯片的定义和处理。
2.添加了“Get_LTE_Band_DD_FDD_TDD()”函数，统一计算LTE频段的FDD与TDD类型。
3.添加了“Get_DD_FDD_TDD_Name()”函数，提取FDD与TDD类型的字符名称。
4.把FDD和TDD的定义从Global_Header.h文件转移到GFC.h文件中。

2019-8-24，xjli
1.添加了“BB_QM215”和“BB_QCM2150”BB芯片的定义和处理。
2.添加了“EU_NR5GSUB6_BAND_INDEX”、“EU_ANT_TYPE_INDEX”、“TESTER_RF_OFFSET_V2_S”、“BAND_CONFIG_S”、“BAND_LIST_V2_S”、“ANT_PROP_S”、“ANT_LIST_S”、“ANT_CFG_ALL_S”struct和enum的定义和处理。
3.添加了“REF_DATA_BASIC_S”、“REF_DATA_BAND_S”、“REF_DATA_ALL_S”struct和enum的定义和处理。
4.添加了“Get_NR_5G_SUB6_Band_Index()”、“Get_NR_5G_SUB6_Band_Name()”函数。
5.添加了“Get_ANT_Type_Name()”函数。
6.添加了“FORMAT_NR5GSUB6_NS”、“FORMAT_GPS_NS”、“FORMAT_WIFI_NS”、“FORMAT_BLUETOOTH_NS”的定义。

2019-8-30，xjli
1.添加了“LTE_CAT_19”CAT值定义和处理。
2.添加了“BB_SA515M”BB芯片定义和处理。
3.添加了“IS_SMART_MODULE( int i_BB_Chip )”函数。

2019-9-3，xjli
1.添加了“Empty_Struct_TESTER_RF_OFFSET_S()”、“Empty_Struct_TESTER_RF_OFFSET_V2_S()”、“Empty_Struct_ANT_CFG_ALL_S()”、“Empty_Struct_ANT_LIST_S()”、“Empty_Struct_REF_DATA_ALL_S()”、“Empty_Struct_REF_DATA_BAND_S()”函数。
2.添加了“Copy_Struct_ANT_CFG_ALL_S()”、“Copy_Struct_ANT_LIST_S()”、“Copy_Struct_TESTER_RF_OFFSET_V2_S()”、“Copy_Struct_TESTER_RF_OFFSET_V2_S_To_TESTER_RF_OFFSET_S()”、“Copy_Struct_TESTER_RF_OFFSET_S_To_TESTER_RF_OFFSET_V2_S()”函数。

2019-9-11，xjli
1.添加了“EU_RF_KEY_TYPE_INDEX”ASR平台 RF Type 定义和处理。
2.添加了“Get_RF_Key_Type_Name()”函数。

2019-9-16，xjli
1.添加了“BB_WCN3990”BB芯片定义和处理。

2019-10-8，xjli
1.添加了“Unicode_To_UTF8()”函数。

2019-10-11，xjli
1.用“结构体/构造函数初始化”的方式对结构体“PKT_BUFF_S”做初始化赋值。

2019-10-18，xjli
1.添加了“BB_ASR3601”BB芯片定义和处理。

2019-11-6，xjli
1.添加了“PF_SPREADTRUM”平台、“BB_SL8541E”BB芯片的定义和处理。

2019-11-18，xjli
1.添加了“PRODU_TYPE_WM_A”产品类型的定义和处理。

2019-11-20，xjli
1.更新了“Get_RF_Offset_Freq_Band_Seg()”函数，添加了 NR 5G SUB6 部分的频段、频点获取。

2019-11-25，xjli
1.添加了“BB_HI2152”和“BB_QCA6391”BB芯片的定义和处理。

2019-12-2，xjli
1.添加了“FORMAT_CV2X_NS”的定义。
2.结构体“TESTER_RF_OFFSET_V2_S”中的“dr_WiFi_RF_Loss”扩展为“低、中、高信道线损值”。

2019-12-20，xjli
1.添加了“PF_EIGENCOMM”平台、“BB_EC616”BB芯片的定义和处理。

2019-12-25，xjli
1.取消了“PRODU_TYPE_WM_A”产品类型的定义和处理。
2.添加了“PRODU_TYPE_MOB”和“PRODU_TYPE_COB”产品类型的定义和处理。
3.添加了存储“有限天线数量“的“ANT_TYPE_BUFF_SIZE”定义。（未来其功能和使用方式还要再优化）
4.修改“ANT_TYPE_IDX_SIZE”值从26提高到28。
5.添加了“ANT_TYPE_IDX_MAIN_2”、“ANT_TYPE_IDX_DIV_2”、“ANT_TYPE_IDX_MULTI_M3”、“ANT_TYPE_IDX_MULTI_M4”、“ANT_TYPE_IDX_WIFI_MIMO”、“ANT_TYPE_IDX_V2X_MAIN”、“ANT_TYPE_IDX_V2X_DIV”的天线类型序列号的定义和处理。
6.添加了新的ocp回调API函数定义“CALLBACK_READ_PRODU_PS_CH2_VOLT_AND_CURR”和“CALLBACK_READ_PRODU_NETWORK_FORMAT_BAND_LIST”。
7.添加了新的V2版ocp回调结构体定义“READ_OCP_PARAMS_V2_ARG”。

2020-1-8，xjli
1.添加了“EU_CV2X_BAND_IDX”C-V2X频段序列号的定义和处理。
2.添加了“Get_CV2X_Band_Index()”和“Get_CV2X_Band_Name()”函数，处理了C-V2X频段名和序列号的转换。
3.更新了“TESTER_RF_OFFSET_V2_S”定义，添加了“double dr_CV2X_RF_Loss[CV2X_BAND_SIZE * 6];”变量。
4.更新了“Empty_Struct_TESTER_RF_OFFSET_V2_S()”和“Copy_Struct_TESTER_RF_OFFSET_V2_S()”函数，添加了“dr_CV2X_RF_Loss”的处理。
5.更新了“Get_RF_Offset_Freq_Band_Seg()”函数，为“FORMAT_CV2X_NS”制式单独处理频段提取。

2020-1-10，xjli
1.更新了“BAND_LIST_V2_S”、“REF_DATA_BAND_S”、“BAND_ENABLE_S”结构体定义，添加了“C-V2X”的相关变量。

2020-1-12，xjli
1.添加了“NB_SCSP_S3K75”、“NB_SCSP_S15K”、“NB_MSCH_BPSK”、“NB_MSCH_QPSK”、“NB_SINGLE_TONE”、“NB_MULTI_TONE_3”、“NB_MULTI_TONE_6”、“NB_MULTI_TONE_12”的宏定义和处理。
2.添加了“Copy_Struct_BAND_ENABLE_S()”、“Assign_Struct_BAND_ENABLE_S()”、“Get_NB_SCSP_Name()”、“Get_NB_Mod_Scheme_Name()”函数。
3.更新了“Empty_Struct_REF_DATA_BAND_S()”函数，添加了清空“strr_CV2X_REF_Data”的操作。

2020-1-14，xjli
1.更新了“PRODU_PARAMS_GEN_S”结构体定义，把“str_Band_List”的类型定义从“BAND_LIST_S”变更为“BAND_LIST_V2_S”。

2020-2-12，xjli
1.添加了“NR5GSUB6_N_12”、“NR5GSUB6_N_25”、“NR5GSUB6_N_48”的定义和处理。

2020-2-17，xjli
1.添加了“BB_ASR1601”BB芯片定义和处理。

2020-2-20，xjli
1.添加了“NR5GMMW_N_257”、“NR5GMMW_N_258”、“NR5GMMW_N_260”、“NR5GMMW_N_261”的定义和处理，以及频段区间。

2020-3-3，xjli
1.添加了“NR5GSUB6_N_38”、“NR5GSUB6_N_40”的定义和处理。

2020-3-11，xjli
1.添加了“PF_TI”平台、“BB_CC1200”、“BB_HI9500”BB芯片的定义和处理。

2020-3-16，xjli
1.添加了“BB_ASR1802SL”、“BB_ASR1826”BB芯片的定义和处理。

2020-3-27，xjli
1.添加了“Get_ANT_Type_Index()”函数。

2020-4-8，xjli
1.添加了“L_BAND_31”、“L_BAND_72”、“L_BAND_73”的定义和处理，以及频段区间。
2.LTE频段相关的存储BUFF尺寸“L_BAND_SIZE”从40扩大到46。

2020-4-17，xjli
1.添加了“BB_HI2115”BB芯片的定义和处理。
2.添加了“WIFI_STAN_80211AX_20M”、“WIFI_STAN_80211AX_40M”、“WIFI_STAN_80211AX_5G_20M”、“WIFI_STAN_80211AX_5G_40M”、“WIFI_STAN_80211AX_5G_80M”WiFi标准(WiFi Standard)序列号的定义和处理。

2020-4-30，xjli
1.添加了“BB_QCA6696”BB芯片定义和处理。
2.把“仪器型号”的宏定义从“Global_Header.h”中转移到“GFC.h”中。
3.添加了“LP_5G_IQXS”仪器的宏定义和处理。
4.添加了“Get_RF_Instru_Name()”函数。

2020-5-23，xjli
1.更新“Calcu_Center_Freq_LTE()”函数，添加了频段“NB_BAND_10、NB_BAND_22、NB_BAND_24、NB_BAND_68、NB_BAND_70、NB_BAND_74”的频点计算。

2020-6-10，xjli
1.添加了“ANT_CFG_ALL_V2_S”、“REF_DATA_ALL_V2_S”结构体定义。
2.更新了“ANT_LIST_S”结构体定义，把“strr_ANT_Prop_List”的尺寸从“ANT_TYPE_IDX_SIZE”变更为“ANT_TYPE_BUFF_SIZE”。
3.添加了“L_BAND_32”的定义和处理，以及频段区间。
4.添加了“BB_SDA845”、“BB_UDX710”BB芯片定义和处理。
5.添加了“Empty_Struct_REF_DATA_ALL_V2_S()”函数。
6.添加了“Get_ANT_Type_List_Buff_Index()”函数。
7.添加了“ANT_TYPE_IDX_DSDA_MAIN”、“ANT_TYPE_IDX_DSDA_DIV”的天线类型序列号的定义和处理。
8.添加了“Empty_Struct_ANT_CFG_ALL_V2_S()”函数。

2020-6-30，xjli
1.添加了“Copy_Struct_ANT_CFG_ALL_V2_S_To_ANT_CFG_ALL_S()”和“Copy_Struct_ANT_CFG_ALL_S_To_ANT_CFG_ALL_V2_S()”函数。
2.添加了“Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S()”函数。
3.添加了“BB_QCX212”BB芯片定义和处理。

2020-7-30，xjli
1.添加了“BB_SM6115”、“BB_QTM527”BB芯片定义和处理。

2020-8-3，xjli
1.添加了“BB_EC617”BB芯片定义和处理。

2020-8-6，xjli
1.更新“IS_SMART_MODULE()”函数，配置“BB_SDA845、BB_SM6115”为智能模块。

2020-8-26，xjli
1.添加了“BB_WCN3980”BB芯片定义和处理。

2020-9-2，xjli
1.添加了“PRODU_TYPE_ASM”产品类型的定义和处理。

2020-9-11，xjli
1.添加了“NR5GSUB6_N_14”的定义和处理。

2020-9-22，xjli
1.添加了“BB_ESP8285”、“BB_UIS8910DM”、“BB_UIS8581”BB芯片定义和处理。
2.更新“IS_SMART_MODULE()”函数，添加“BB_UIS8581”BB芯片。

2020-10-16，xjli
1.添加了“PF_XINYI”平台、“BB_XY1100”、“BB_HI2120”、“BB_MT2731”BB芯片的定义和处理。

2020-10-17，xjli
1.添加了“BB_WCN3988”BB芯片的定义和处理。

2020-11-6，xjli
1.添加了“PF_ALTAIR”平台、“BB_ALT1250”、“BB_QCA6595”、“BB_ASR1603”BB芯片的定义和处理。

2020-11-9，xjli
1.添加了“BB_QCA1064”、“BB_MT6880”BB芯片的定义和处理。
2.添加了“RF_KEY_TYPE_IDX_CRANE_M”、“RF_KEY_TYPE_IDX_CRANE_G”ASR 平台 RF Key Type 的定义和处理。

2020-11-12，xjli
1.添加了“BB_MT6890”、“BB_WCN3615”BB芯片的定义和处理。

2020-11-30，xjli
1.添加了“BB_EC616S”BB芯片的定义和处理。

2020-12-10，xjli
1.添加了“BB_QCM6125”BB芯片的定义和处理。

2021-2-10，xjli
1.添加了“BB_UIS8811”BB芯片定义和处理。

2021-3-9，xjli
1.添加了“NB_BAND_86”、“NB_IDX_B_86”的定义和处理。
2.添加了“BB_SDX12”、“BB_QCM2290”、“BB_BK7231”BB芯片定义和处理。
3.添加了“PF_BEKEN”平台的定义和处理。

2021-3-11，xjli
1.添加了“BB_SDX62”BB芯片定义和处理。

2021-3-17，xjli
1.添加了“BB_QCA2064”BB芯片定义和处理。

2021-3-26，xjli
1.添加了“BB_SM4350”BB芯片定义和处理。

2021-3-30，xjli
1.添加了“LTE_CAT_15”CAT值定义和处理。

2021-4-7，xjli
1.添加了“BB_ASR1803”BB芯片的定义和处理。
2.修改枚举“RF_KEY_TYPE_IDX_PXA1802_RF858”，变更为“RF_KEY_TYPE_IDX_PXA1802S_RF858”。
3.添加枚举“RF_KEY_TYPE_IDX_1803_FAICON”。

2021-4-9，xjli
1.添加了“BB_MT2735”BB芯片的定义和处理。

2021-4-16，xjli
1.添加了“CALLBACK_FIND_READ_REF_STD_2017_CONN_V1”、“OP_SQL_DB_V1_ARG”回调函数类型定义和结构体的定义和处理。

2021-5-19，xjli
1.添加了“PLAN_ANT4_MAIN1_DIV1_GPS1_WIFI1”和“PLAN_ANT4_MAIN2_DIV2”宏定义。

2021-5-20，xjli
1.添加了“Get_Rx_Chain_Type_Name()”函数。

2021-5-26，xjli
1.添加了“NR5GSUB6_N_13”、“NR5GSUB6_N_18”、“NR5GSUB6_N_26”、“NR5GSUB6_N_29”、“NR5GSUB6_N_30”、“NR5GSUB6_N_75”、“NR5GSUB6_N_76”、“NR5GSUB6_N_91”、“NR5GSUB6_N_92”、“NR5GSUB6_N_93”、“NR5GSUB6_N_94”的定义和处理。
2.枚举“NR5GSUB6_N_SIZE”的数值变更为“49”。

2021-6-1，xjli
1.添加了“ANT_TYPE_IDX_V2X_TRX0”、“ANT_TYPE_IDX_V2X_TRX1”的天线类型序列号的定义和处理。
2.枚举“ANT_TYPE_IDX_SIZE”的数值变更为“39”。

2021-6-3，xjli
1.添加了“LTE_CAT_13”CAT值定义和处理。

2021-6-21，xjli
1.添加了“BB_WCN3991”BB芯片定义和处理。
2.添加了“WIFI_STAN_80211N_5G_40M”WiFi标准的定义和相关处理。
3.添加了新函数“Determine_Type_Of_Plan_Connect_ANT_And_RF_Port()”。
4.添加了新的ocp回调API函数定义“CALLBACK_READ_PRODU_NETWORK_FORMAT_BAND_ANT_LIST”。
5.添加了新结构体定义“READ_OCP_PARAMS_V3_ARG”。

2021-6-29，xjli
1.添加了“BB_SL8521E”BB芯片的定义和处理。

2021-7-9，xjli
1.添加了“BB_QCM6490”BB芯片定义和处理。

2021-7-14，xjli
1.添加了“BB_QCA9377”BB芯片定义和处理。

2021-7-21，xjli
1.更新了“Get_RF_Key_Type_Name()”函数，RF_KEY_TYPE_IDX_RF866 的字串变更为“1826+RF866”。

2021-8-6，xjli
1.添加了“PF_CYPRESS”平台的定义和处理。
2.添加了“BB_CYW54591”BB芯片定义和处理。
3.添加了“NR5GSUB6_N_70”的定义和处理。

2021-8-11，xjli
1.BB芯片“BB_ASR1803”的定义和处理统一变更为“BB_ASR1803S”。
2.添加了“BB_ASR1803E”和“BB_ASR1803L”BB芯片的定义和处理。

2021-8-12，xjli
1.添加了“BB_WCN3998”BB芯片的定义和处理。

2021-8-24，xjli
1.添加了“BB_QCM4290”BB芯片的定义和处理。

2021-9-1，xjli
1.添加了新的ocp回调API函数定义“CALLBACK_READ_PRODU_ANT_LIST”。
2.添加了“BB_QCX315”BB芯片的定义和处理。

2021-9-16，xjli
1.添加了“BB_QCS8250”和“BB_CYW43455”BB芯片的定义和处理。

2021-9-23，xjli
1.添加了“BB_UIS8310AM”BB芯片的定义和处理。

2021-9-27，xjli
1.添加了“BB_SDX65”BB芯片定义和处理。
2.更新了函数“Calcu_ARFCN_WCDMA()”和“Calcu_Center_Freq_WCDMA()”中WCDMA B4的开始和结束信道修改为“1312~1513”和“1537~1738”。

2021-9-29，xjli
1.添加了“BB_UWE5623DU”BB芯片的定义和处理。

2021-10-24，xjli
1.添加了“BB_EC618”BB芯片的定义和处理。

2021-10-25，xjli
1.添加了“TS_CONFIG_MOB_S”结构体的定义。

2021-11-5，xjli
1.在函数“IS_SMART_MODULE()”中对BB芯片“BB_QCM4290”和“BB_QCS8250”设定为智能SMART产品芯片。

2021-11-10，xjli
1.添加了“PF_SILICON_LABS”平台的定义和处理。
2.添加了“BB_EFR32BG21”BB芯片定义和处理。

2021-11-26，xjli
1.添加了“BB_CYW43438”BB芯片的定义和处理。

2021-12-1，xjli
1.添加了“ANT_TYPE_IDX_NR5GSUB6_9”、“ANT_TYPE_IDX_NR5GSUB6_10”、“ANT_TYPE_IDX_NR5GSUB6_11”、“ANT_TYPE_IDX_NR5GSUB6_12”ANT天线的定义和处理。
2.添加了“NR5GSUB6_N_257”、“NR5GSUB6_N_258”、“NR5GSUB6_N_260”、“NR5GSUB6_N_261”的定义和处理。

2021-12-3，xjli
1.添加了“ANT_TYPE_IDX_WIFI_GPS”ANT天线的定义和处理。

2021-12-4，xjli
1.ANT天线“ANT_TYPE_IDX_WIFI_GPS”定义修改为“ANT_TYPE_IDX_WIFI_BLU_GPS”。

2021-12-6，xjli
1.添加了“BB_UIS8850DG”BB芯片的定义和处理。

2021-12-16，xjli
1.添加了“BB_CYW8X072”BB芯片的定义和处理。

2021-12-22，summer
1.更新ASR RF Type定义，结构体“EU_RF_KEY_TYPE_INDEX”，函数“Get_RF_Key_Type_Name”。

2022-1-6，xjli
1.添加了“BB_WCN3950”、“BB_W155S1”BB芯片的定义和处理。
2.添加了“PF_AMLOGIC”平台的定义和处理。

2022-1-24，xjli
1.添加了“BB_CYW43439”BB芯片的定义和处理。

2022-1-28，xjli
1.添加了“BB_CB0201”BB芯片的定义和处理。
2.添加了“NB_BAND_87”、“NB_BAND_88”、“NB_IDX_B_87”、“NB_IDX_B_88”、“L_BAND_87”、“L_BAND_88”的定义和处理。

2022-2-7，xjli
1.更新了函数“Get_LTE_Band_DD_FDD_TDD()”，对新LTE频段B87和B88识别为FDD频段。

2022-2-16，xjli
1.添加了“BB_ASR1606”BB芯片的定义和处理。

2022-2-18，xjli
1.把“BB_ASR1606”BB芯片修改为“BB_ASR1606C”和“BB_ASR1606S”两款芯片，并重新定义和处理。
2.添加枚举“RF_KEY_TYPE_IDX_1606”。
3.添加了“BB_WCN6856”BB芯片的定义和处理。

2022-2-23，xjli
1.添加了“BB_QCX216”BB芯片的定义和处理。

2022-3-30，xjli
1.添加了“CMP180”仪器的宏定义和处理。
2.添加了“BB_QCA206x”BB芯片的定义和处理。

2022-4-24，xjli
1.添加了“BB_ASR1828”BB芯片的定义和处理。
2.添加枚举“RF_KEY_TYPE_IDX_1828_RF866”RF-Type的定义和处理。
*/




//
CGFC::CGFC( void )
	: m_csError_Info( _T( "" ) )
{
}

CGFC::~CGFC( void )
{
}

void CGFC::WCSTOK_INT( const wchar_t * wcp_StrToken, int * ip_List, int * ip_Quan )
{
	//从双字符串中提取“全部”int数据
	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//“源数据”的buff，wcstok在提取数据时会改变源数据内容，所以先把源数据放入buff，解析buff数据，不伤害参数源。
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//用于存放“源数据”的buff长度
	WCHAR * wcpResult = NULL;

	//分配内存
	this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

	//载入“源数据”
	swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

	//提取第一个值(0)
	wcpEach = wcstok_s( wcpSrc, _T( " ,;\r\n" ), &wcpResult );

	//循环提取
	while( wcpEach != NULL )
	{
		//赋值
		*(ip_List + i) = _wtoi( wcpEach );

		//计数
		i++;

		//提取下一个值
		wcpEach = wcstok_s( NULL, _T( " ,;\r\n" ), &wcpResult );
	}

	//提取的元素数量统计
	*ip_Quan = i;

	//释放内存
	this->BUFF_WCHAR_FREE( &wcpSrc );
}

void CGFC::WCSTOK_DOUBLE( const wchar_t * wcp_StrToken, double * dp_List, int * ip_Quan )
{
	//从双字符串中提取“全部”double数据
	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//“源数据”的buff，wcstok在提取数据时会改变源数据内容，所以先把源数据放入buff，解析buff数据，不伤害参数源。
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//用于存放“源数据”的buff长度
	WCHAR * wcpResult = NULL;

	//分配内存
	this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

	//载入“源数据”
	swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

	//提取第一个值(0)
	wcpEach = wcstok_s( wcpSrc, _T( " ,;\r\n" ), &wcpResult );

	//循环提取
	while( wcpEach != NULL )
	{
		//赋值
		*(dp_List + i) = _wtof( wcpEach );

		//计数
		i++;

		//提取下一个值
		wcpEach = wcstok_s( NULL, _T( " ,;\r\n" ), &wcpResult );
	}

	//提取的元素数量统计
	*ip_Quan = i;

	//释放内存
	this->BUFF_WCHAR_FREE( &wcpSrc );
}

bool CGFC::WCSTOK_STRING_C( const wchar_t * wcp_StrToken, const wchar_t * wcp_StrDelimit, int i_Get_Index, CString * csp_Get_CString )
{
	//从双字符串中提取“指定位置”的字符串数据，位置起始号为“0”。
	//间隔符号为“参数自定义”

	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//“源数据”的buff，wcstok在提取数据时会改变源数据内容，所以先把源数据放入buff，解析buff数据，不伤害参数源。
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//用于存放“源数据”的buff长度
	bool bFlag = false;
	WCHAR * wcpResult = NULL;

	//
	try
	{
		//分配内存
		this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

		//载入“源数据”
		swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

		//提取第一个值(0)
		wcpEach = wcstok_s( wcpSrc, wcp_StrDelimit, &wcpResult );

		//循环提取
		while( wcpEach != NULL )
		{
			if( i == i_Get_Index )
			{
				//赋值
				csp_Get_CString->Format( _T( "%s" ), wcpEach );

				bFlag = true;
				break;//退出while
			}

			//提取下一个值
			wcpEach = wcstok_s( NULL, wcp_StrDelimit, &wcpResult );

			//计数
			i++;
		}//while(wcpEach != NULL
	}
	catch( ... )
	{
		//释放内存
		this->BUFF_WCHAR_FREE( &wcpSrc );
		//fail
		return false;
	}

	//释放内存
	this->BUFF_WCHAR_FREE( &wcpSrc );

	//返回
	return bFlag;
}

bool CGFC::WCSTOK_STRING_S( const wchar_t * wcp_StrToken, int i_Get_Index, CString * csp_Get_CString )
{
	//从双字符串中提取“指定位置”的字符串数据，位置起始号为“0”。
	//间隔符号为“空格，逗号，分号，\r，\n”
	//注意：如果两个“分隔符之间无数据（两个分隔符相连）”则不“提取”；所以必须是“两个分隔符之间有数据”

	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//“源数据”的buff，wcstok在提取数据时会改变源数据内容，所以先把源数据放入buff，解析buff数据，不伤害参数源。
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//用于存放“源数据”的buff长度
	bool bFlag = false;
	WCHAR * wcpResult = NULL;

	//
	try
	{
		//分配内存
		this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

		//载入“源数据”
		swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

		//提取第一个值(0)
		wcpEach = wcstok_s( wcpSrc, _T( " ,;\r\n" ), &wcpResult );

		while( wcpEach != NULL )
		{
			if( i == i_Get_Index )
			{
				//赋值
				csp_Get_CString->Format( _T( "%s" ), wcpEach );

				bFlag = true;
				break;//退出while
			}

			//提取下一个值
			wcpEach = wcstok_s( NULL, _T( " ,;\r\n" ), &wcpResult );

			//计数
			i++;
		}//while(wcpEach != NULL
	}
	catch( ... )
	{
		//释放内存
		this->BUFF_WCHAR_FREE( &wcpSrc );
		//fail
		return false;
	}

	//释放内存
	this->BUFF_WCHAR_FREE( &wcpSrc );

	//返回结果
	return bFlag;
}

void CGFC::BUFF_BYTE_CALLOC( unsigned char ** ucp2_Buff, unsigned long ul_Buff_Size )
{
	//分配unsigned char（BYTE）字符串内存

	if( *ucp2_Buff != NULL )
	{
		return;
	}

	//分配内存
	*ucp2_Buff = (unsigned char *)calloc( ul_Buff_Size, sizeof( unsigned char ) );
}

void CGFC::BUFF_BYTE_FREE( unsigned char ** ucp2_Buff )
{
	//释放unsigned char（BYTE）字符串内存

	if( *ucp2_Buff != NULL )
	{
		free( *ucp2_Buff );
		*ucp2_Buff = NULL;
	}
}

void CGFC::BUFF_CHAR_CALLOC( char ** cp2_Buff, unsigned long ul_Buff_Size )
{
	//分配char字符串内存
	if( *cp2_Buff != NULL )
	{
		return;
	}

	//分配内存
	*cp2_Buff = (char *)calloc( ul_Buff_Size, sizeof( char ) );
}

void CGFC::BUFF_CHAR_FREE( char ** cp2_Buff )
{
	//释放char字符串内存
	if( *cp2_Buff != NULL )
	{
		free( *cp2_Buff );
		*cp2_Buff = NULL;
	}
}

void CGFC::BUFF_DOUBLE_CALLOC( double ** dp2_Buff, unsigned long ul_Buff_Size )
{
	//分配double字符串内存
	if( *dp2_Buff != NULL )
	{
		return;
	}

	//分配内存
	*dp2_Buff = (double *)calloc( ul_Buff_Size, sizeof( double ) );
}

void CGFC::BUFF_DOUBLE_FREE( double ** dp2_Buff )
{
	//释放double字符串内存
	if( *dp2_Buff != NULL )
	{
		free( *dp2_Buff );
		*dp2_Buff = NULL;
	}
}

void CGFC::BUFF_DWORD_CALLOC( DWORD ** dwp2_Buff, unsigned long ul_Buff_Size )
{
	//分配unsigned long（DWORD）字符串内存

	if( *dwp2_Buff != NULL )
	{
		return;
	}

	//分配内存
	*dwp2_Buff = (unsigned long *)calloc( ul_Buff_Size, sizeof( unsigned long ) );
}

void CGFC::BUFF_DWORD_FREE( DWORD ** dwp2_Buff )
{
	//释放unsigned long（DWORD）字符串内存

	if( *dwp2_Buff != NULL )
	{
		free( *dwp2_Buff );
		*dwp2_Buff = NULL;
	}
}

void CGFC::BUFF_WCHAR_CALLOC( wchar_t ** wcp2_Buff, unsigned long ul_Buff_Size )
{
	//分配wchar_t字符串内存
	//参数为地址数据——存放指针的地址（二级指针）

	if( *wcp2_Buff != NULL )//根据变量地址信息找到其含有的指针数据
	{
		return;
	}

	//分配内存（修改指针数据）
	*wcp2_Buff = (wchar_t *)calloc( ul_Buff_Size, sizeof( wchar_t ) );//返回的是“分配内存的指针（地址）”
}

void CGFC::BUFF_WCHAR_FREE( wchar_t ** wcp2_Buff )
{
	//释放wchar_t字符串内存
	if( *wcp2_Buff != NULL )
	{
		free( *wcp2_Buff );
		*wcp2_Buff = NULL;
	}
}

bool CGFC::Confirm_Reg_Com_Port( const int i_Tgt_Com_Port )
{
	//从注册表中读出串口序列
	HKEY hKey;						// handle to key to query
	DWORD dwIndex;					// index of value to query
	wchar_t * wcpValueName = NULL;	// address of buffer for value string
	DWORD dwValueSize;				// address for size of value buffer
	DWORD dwTypeCode;				// address of buffer for type code
	wchar_t * wcpValueData = NULL;	// address of buffer for value data
	DWORD dwDataSize;				// address for size of data buffer
	CRegKey theRegKey;
	wchar_t * wcpPoint;
	int iTemp;
	bool bRtn = false;

	//
	try
	{
		//分配内存
		wcpValueName = (wchar_t *)calloc( 128, sizeof( wchar_t ) );
		wcpValueData = (wchar_t *)calloc( 128, sizeof( wchar_t ) );

		//
		if( ERROR_SUCCESS == ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T( "HARDWARE\\DEVICEMAP\\SERIALCOMM" ), 0, KEY_READ, &hKey ) )
		{
			for( dwIndex = 0; dwIndex < 256; dwIndex++ )
			{
				dwValueSize = 254;
				dwDataSize = 254;
				wmemset( wcpValueName, 0, 128 );
				wmemset( wcpValueData, 0, 128 );

				//If the function succeeds, the return value is ERROR_SUCCESS
				if( ::RegEnumValue( hKey, dwIndex, wcpValueName, &dwValueSize, 0, &dwTypeCode, (BYTE *)wcpValueData, &dwDataSize ) != ERROR_SUCCESS )
				{
					break;//读出错，退出循环
				}

				//
				wcpPoint = wcsstr( wcpValueData, _T( "COM" ) );

				//
				if( wcpPoint )
				{
					wcpPoint += 3;
					iTemp = _wtoi( wcpPoint );
					//
					if( iTemp == i_Tgt_Com_Port )
					{
						bRtn = true;//找到目标串口，退出循环
						break;
					}
				}
			}//for(dwIndex=0; dwIndex<256;

			//
			::RegCloseKey( hKey );
		}
		else
		{
			throw(0);
		}
	}
	catch( ... )
	{
		//释放内存
		if( wcpValueName != NULL )
		{
			free( wcpValueName );
		}
		//
		if( wcpValueData != NULL )
		{
			free( wcpValueData );
		}

		//fail
		return false;
	}

	//释放内存
	if( wcpValueName != NULL )
	{
		free( wcpValueName );
	}
	//
	if( wcpValueData != NULL )
	{
		free( wcpValueData );
	}

	//返回结果
	return bRtn;
}

bool CGFC::Get_Reg_Com_Port_List( int * ip_COM_List, int * ip_COM_Quan )
{
	//从注册表中读出串口序列
	HKEY hKey;						// handle to key to query
	DWORD dwIndex;					// index of value to query
	wchar_t * wcpValueName = NULL;	// address of buffer for value string
	DWORD dwValueSize;				// address for size of value buffer
	DWORD dwTypeCode;				// address of buffer for type code
	wchar_t * wcpValueData = NULL;	// address of buffer for value data
	DWORD dwDataSize;				// address for size of data buffer
	CRegKey theRegKey;
	wchar_t * wcpPoint;

	//
	try
	{
		//分配内存
		this->BUFF_WCHAR_CALLOC( &wcpValueName, 128 );
		this->BUFF_WCHAR_CALLOC( &wcpValueData, 128 );

		if( ERROR_SUCCESS == ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T( "HARDWARE\\DEVICEMAP\\SERIALCOMM" ), 0, KEY_READ, &hKey ) )
		{
			for( dwIndex = 0; dwIndex < 256; dwIndex++ )
			{
				dwValueSize = 254;
				dwDataSize = 254;
				wmemset( wcpValueName, 0, 128 );
				wmemset( wcpValueData, 0, 128 );

				if( ::RegEnumValue( hKey, dwIndex, wcpValueName, &dwValueSize, 0, &dwTypeCode, (BYTE *)wcpValueData, &dwDataSize ) != ERROR_SUCCESS )
				{
					break;
				}

				//
				wcpPoint = wcsstr( wcpValueData, _T( "COM" ) );

				//
				if( wcpPoint )
				{
					wcpPoint += 3;
					*(ip_COM_List + dwIndex) = _wtoi( wcpPoint );
				}
			}

			//
			::RegCloseKey( hKey );

			//
			*ip_COM_Quan = dwIndex;//找到的串口的数目

			//从小到大排序
			this->Adjust_From_Small_To_Large_Array_Int( ip_COM_List, *ip_COM_Quan );
		}
		else
		{
			throw(0);
		}
	}
	catch( ... )
	{
		//释放内存
		this->BUFF_WCHAR_FREE( &wcpValueName );
		this->BUFF_WCHAR_FREE( &wcpValueData );
		//fail
		return false;
	}

	//释放内存
	this->BUFF_WCHAR_FREE( &wcpValueName );
	this->BUFF_WCHAR_FREE( &wcpValueData );
	//pass
	return true;
}

bool CGFC::PKT_BUFF_ADD_DATA( PKT_BUFF_S * strp_Pkt_Buff, const unsigned char * ucp_Input_Data, const unsigned long ul_Add_Quan )
{
	CString csData;

	//
	if( strp_Pkt_Buff->ucp_Buff == NULL )
	{
		::AfxMessageBox( _T( "ERROR, PKT_BUFF_ADD_DATA, ucp_Buff = NULL" ) );
		//fail
		return false;
	}

	//
	if( strp_Pkt_Buff->ul_Total_Size < strp_Pkt_Buff->ul_Data_Length )
	{
		csData.Format( _T( "ERROR, PKT_BUFF_ADD_DATA, Total_Size = %d, Data_Length = %d, Add_Quan = %d;" ),
			strp_Pkt_Buff->ul_Total_Size, strp_Pkt_Buff->ul_Data_Length, ul_Add_Quan );
		::AfxMessageBox( csData );

		//fail
		return false;
	}
	else
	{
		unsigned long ul_SpaceFree = strp_Pkt_Buff->ul_Total_Size - strp_Pkt_Buff->ul_Data_Length;//总空间-已用空间=可用空间

		//
		if( ul_SpaceFree > ul_Add_Quan )//可用空间足够加入数据
		{
			memcpy( (strp_Pkt_Buff->ucp_Buff + strp_Pkt_Buff->ul_Data_Length), ucp_Input_Data, ul_Add_Quan );

			strp_Pkt_Buff->ul_Data_Length += ul_Add_Quan;

			//pass
			return true;
		}
		else
		{
			csData.Format( _T( "ERROR, PKT_BUFF_ADD_DATA, Total_Size = %d, Data_Length = %d, Add_Quan = %d;" ),
				strp_Pkt_Buff->ul_Total_Size, strp_Pkt_Buff->ul_Data_Length, ul_Add_Quan );
			::AfxMessageBox( csData );

			//fail
			return false;
		}
	}

	//fail
	return false;
}

void CGFC::PKT_BUFF_CALLOC( PKT_BUFF_S * strp_Pkt_Buff, const unsigned long ul_Buff_Size )
{
	if( strp_Pkt_Buff->ucp_Buff != NULL )
	{
		::AfxMessageBox( _T( "ERROR, PKT_BUFF_CALLOC, ucp_Buff != NULL" ) );
		//
		return;
	}

	//分配内存
	strp_Pkt_Buff->ucp_Buff = (unsigned char *)calloc( ul_Buff_Size, sizeof( unsigned char ) );

	//值初始化
	memset( strp_Pkt_Buff->ucp_Buff, 0, ul_Buff_Size );
	strp_Pkt_Buff->ul_Total_Size = ul_Buff_Size;
	strp_Pkt_Buff->ul_Data_Length = 0;
}

bool CGFC::PKT_BUFF_DEL_ALL( PKT_BUFF_S * strp_Pkt_Buff )
{
	if( strp_Pkt_Buff->ucp_Buff == NULL )
	{
		//::AfxMessageBox( _T( "ERROR, PKT_BUFF_DEL_ALL, ucp_Buff = NULL" ) );
		return false;
	}

	strp_Pkt_Buff->ul_Data_Length = 0;
	memset( strp_Pkt_Buff->ucp_Buff, 0, strp_Pkt_Buff->ul_Total_Size );
	return true;
}

void CGFC::PKT_BUFF_DEL_DATA( PKT_BUFF_S * strp_Pkt_Buff, const unsigned long ul_Del_Quan )
{
	if( NULL == strp_Pkt_Buff->ucp_Buff )
	{
		::AfxMessageBox( _T( "ERROR, PKT_BUFF_DEL_DATA, ucp_Buff = NULL" ) );
		//fail
		return;
	}

	if( strp_Pkt_Buff->ul_Data_Length < ul_Del_Quan )
	{
		CString csData;
		//
		csData.Format( _T( "ERROR, PKT_BUFF_DEL_DATA, Data_Length = %d, Del_Quan = %d;" ), strp_Pkt_Buff->ul_Data_Length, ul_Del_Quan );
		::AfxMessageBox( csData );
		//fail
		return;
	}
	else if( strp_Pkt_Buff->ul_Data_Length == ul_Del_Quan )
	{
		strp_Pkt_Buff->ul_Data_Length = 0;
		memset( strp_Pkt_Buff->ucp_Buff, 0, strp_Pkt_Buff->ul_Total_Size );
	}
	else
	{
		strp_Pkt_Buff->ul_Data_Length -= ul_Del_Quan;
		memcpy( strp_Pkt_Buff->ucp_Buff, (strp_Pkt_Buff->ucp_Buff + ul_Del_Quan), strp_Pkt_Buff->ul_Data_Length );
	}
}

unsigned long CGFC::PKT_BUFF_EXTRACT_DATA( PKT_BUFF_S * strp_Pkt_Buff, unsigned char * ucp_Get_Data, const unsigned long ul_Get_Quan )
{
	if( NULL == strp_Pkt_Buff->ucp_Buff )
	{
		::AfxMessageBox( _T( "ERROR, PKT_BUFF_EXTRACT_DATA, ucp_Buff = NULL" ) );
		//fail
		return 0;
	}

	//
	unsigned long ul_Extract_Quan = ul_Get_Quan;

	//
	if( ul_Extract_Quan > strp_Pkt_Buff->ul_Data_Length )
	{
		ul_Extract_Quan = strp_Pkt_Buff->ul_Data_Length;
	}

	//提取要提取的数量
	memcpy( ucp_Get_Data, strp_Pkt_Buff->ucp_Buff, ul_Extract_Quan );
	//计算剩余有用数据
	strp_Pkt_Buff->ul_Data_Length -= ul_Extract_Quan;

	if( strp_Pkt_Buff->ul_Data_Length > 0 )
	{
		//剩余数据“前移”
		memcpy( strp_Pkt_Buff->ucp_Buff, (strp_Pkt_Buff->ucp_Buff + ul_Extract_Quan), strp_Pkt_Buff->ul_Data_Length );
	}
	else if( strp_Pkt_Buff->ul_Data_Length == 0 )
	{
		//无剩余数据了
		memset( strp_Pkt_Buff->ucp_Buff, 0, strp_Pkt_Buff->ul_Total_Size );
	}
	else
	{
		CString csData;
		//
		csData.Format( _T( "ERROR, PKT_BUFF_EXTRACT_DATA, Total_Size = %d, Data_Length = %d;" ),
			strp_Pkt_Buff->ul_Total_Size, strp_Pkt_Buff->ul_Data_Length );
		::AfxMessageBox( csData );
		//fail
		return 0;
	}

	//返回
	return ul_Extract_Quan;
}

void CGFC::PKT_BUFF_FREE( PKT_BUFF_S * strp_Pkt_Buff )
{
	if( strp_Pkt_Buff->ucp_Buff != NULL )
	{
		free( strp_Pkt_Buff->ucp_Buff );
	}

	strp_Pkt_Buff->ucp_Buff = NULL;
	strp_Pkt_Buff->ul_Total_Size = 0;
	strp_Pkt_Buff->ul_Data_Length = 0;
}

void CGFC::PKT_BUFF_INIT( PKT_BUFF_S * strp_Pkt_Buff )
{
	strp_Pkt_Buff->ucp_Buff = NULL;
	strp_Pkt_Buff->ul_Total_Size = 0;
	strp_Pkt_Buff->ul_Data_Length = 0;
}

DWORD CGFC::HEX_BYTE_TO_DWORD( unsigned char * ucp_Hex_Char, int i_Hex_Char_Quan )
{
	char * cpData = NULL;
	char * d;
	int i, j;
	DWORD dwRtn;

	//分配内存
	j = i_Hex_Char_Quan + 3;
	cpData = (char *)calloc( j, sizeof( char ) );

	*(cpData + 0) = '0';
	*(cpData + 1) = 'X';

	for( i = 0; i < i_Hex_Char_Quan; i++ )
	{
		*(cpData + 2 + i) = (char)*(ucp_Hex_Char + i);
	}

	*(cpData + 2 + i) = '\0';

	dwRtn = strtoul( cpData, &d, 16 );

	//释放内存
	free( cpData );

	//返回
	return dwRtn;
}

void CGFC::MultiByte_To_WideChar( const char * cp_Char_8, wchar_t * wcp_WCHAR_16, int * ip_Len_16 )
{
	//utf8编码，char*  ---> Unicode，wchar_t*

	int nLen = strlen( cp_Char_8 ) + 1;
	int nwLen = MultiByteToWideChar( CP_ACP, 0, cp_Char_8, nLen, NULL, 0 );

	//
	MultiByteToWideChar( CP_ACP, 0, cp_Char_8, nLen, wcp_WCHAR_16, nwLen );//CP_UTF8(中文字无法显示)

	//
	*ip_Len_16 = nwLen - 1;//注意：nwLen这个长度（字符数）是包含了终止符（\0）的

	//
	*(wcp_WCHAR_16 + nwLen - 1) = '\0';
}

bool CGFC::WideChar_To_MultiByte( const wchar_t * wcp_WCHAR_16, char * cp_Char_8, int * ip_Len_8 )
{
	//Unicode，wchar_t* ---> utf8编码，char*

	int iSize = WideCharToMultiByte( CP_ACP, 0, wcp_WCHAR_16, -1, NULL, 0, NULL, NULL );

	//
	if( !WideCharToMultiByte( CP_ACP, 0, wcp_WCHAR_16, -1, cp_Char_8, iSize, NULL, NULL ) )
	{
		//fail
		return false;
	}

	//
	*ip_Len_8 = iSize;

	//pass
	return true;
}

bool CGFC::mem_uc_str( unsigned char * ucp_Buff_Src, unsigned long ul_Quan_Src, unsigned char * ucp_Buff_Tgt, unsigned long ul_Quan_Tgt, unsigned long * ulp_Tgt_Index )
{
	unsigned long i, j, ulRtn;

	//
	for( i = 0; i < ul_Quan_Src; i++ )
	{
		//发现BUFF_1有个字符与BUFF_2的首字符一致
		if( *(ucp_Buff_Src + i) == *(ucp_Buff_Tgt + 0) )
		{
			ulRtn = i;//记录这个可能的其实Index值

			//开始按BUFF_2的数据进行全面核对
			for( j = 0; j < ul_Quan_Tgt; j++ )
			{
				//检查串口BUFF的Index号是否超越了串口BUFF中的数据
				if( (i + j) < ul_Quan_Src )
				{
					if( *(ucp_Buff_Src + i + j) != *(ucp_Buff_Tgt + j) )
					{
						break;//发现任意数据不同，则退出，没找到期望数据
					}
				}
				else
				{
					break;//有用数据结束了，也退出循环
				}
			}//for(j=0; j<ul_Quan_Tgt;

			//若BUFF_2核对到了最后一位，便是在BUFF_1中找到一个BUFF_2的匹配
			if( j == ul_Quan_Tgt )
			{
				//找到了BUFF_TGT匹配
				*ulp_Tgt_Index = ulRtn;
				//有匹配
				return true;
			}
		}//if(*(ucp_Buff_Src + i) == *(ucp_Buff_Tgt + 0), 发现BUFF_SRC有个字符与BUFF_TGT的首字符一致
	}//for(i=0; i<ul_Quan_Src;


	//无匹配
	return false;
}

bool CGFC::Confirm_Reg_Dynamic_Com_Port( int i_Tgt_Com_Port, int i_TimeOut )
{
	time_t t_Start, t_End;

	//
	try
	{
		time( &t_Start );

		//
		do
		{
			//读出regedit中的COM列表
			if( this->Confirm_Reg_Com_Port( i_Tgt_Com_Port ) == true )
			{
				break;
			}

			//检查是否超时
			time( &t_End );

			if( (int)difftime( t_End, t_Start ) >= i_TimeOut )
			{
				throw(0);
			}

			Sleep( 500 );
		}
		while( true );
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_Local_PC_Name( CString * csp_PC_Name )
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char crData[64];
	wchar_t wcrTemp[64];
	int i;

	memset( crData, 0, sizeof( crData ) );
	memset( wcrTemp, 0, sizeof( wcrTemp ) );

	//Get Computer Name
	wVersionRequested = MAKEWORD( 2, 0 );
	WSAStartup( wVersionRequested, &wsaData );

	//
	if( ::gethostname( crData, 60 ) != 0 )
	{
		memset( crData, 0, sizeof( crData ) );
	}

	//
	WSACleanup();

	//
	if( strlen( crData ) > 0 )
	{
		this->MultiByte_To_WideChar( crData, wcrTemp, &i );
		csp_PC_Name->Format( _T( "%s" ), wcrTemp );
		return true;
	}
	else
	{
		csp_PC_Name->Format( _T( "No Name" ) );
		return false;
	}
}

bool CGFC::GET_AT_RSP_DIGIT_DATA( const wchar_t * wcp_Src_Data, const wchar_t * wcp_Key_Word, int i_Line_Offset, double * dp_Digit_Data )
{
	//提取“类似AT指令返回格式的应答”中与“关键词”有联系的特定“行”数据，以\r\n换行符为间隔符
	//然后去掉关键词，提取一个“数值”作为返回值
	//与“数字”相关的“关键字符”必须是连续的

	CString csUseful, csData;
	int iLen, i;
	wchar_t wcByte;
	bool bGet_Numb_Byte = false;//找到第一个与“数字”相关的“关键字符”

	try
	{
		//循环搜索，找关键词数据段
		if( this->GET_AT_RSP_STRING( wcp_Src_Data, wcp_Key_Word, i_Line_Offset, &csUseful ) == false )
		{
			throw(0);
		}

		//有“key word”则要去掉关键词以及之前的数据，再提取数值
		iLen = csUseful.Find( wcp_Key_Word );
		if( iLen >= 0 )
		{
			csUseful.Delete( 0, (iLen + wcslen( wcp_Key_Word )) );
		}

		//把字符串转化为数字
		csUseful.MakeUpper();
		iLen = csUseful.GetLength();
		csData.Format( _T( "" ) );

		for( i = 0; i < iLen; i++ )
		{
			wcByte = csUseful.GetAt( i );

			if( bGet_Numb_Byte == false )//开始找第一个与“数字”相关的“关键字符”
			{
				//没有开始载入“数字”数据

				if( ((wcByte >= '0') && (wcByte <= '9')) ||
					(wcByte == '+') || (wcByte == '-') || (wcByte == '.') || (wcByte == 'E') )
				{
					//有效“数字”数据，载入buff
					csData += wcByte;
					bGet_Numb_Byte = true;
				}
			}
			else
			{
				//已经开始载入“数字”数据

				if( ((wcByte >= '0') && (wcByte <= '9')) ||
					(wcByte == '+') || (wcByte == '-') || (wcByte == '.') || (wcByte == 'E') )
				{
					//有效“数字”数据，载入buff
					csData += wcByte;
				}
				else
				{
					//无效“数字”数据，结束载入buff
					break;//退出for
				}
			}
		}//for(i=0; i<iTemp; i++

		//转换成double值，返回
		if( csData.GetLength() > 0 )
		{
			*dp_Digit_Data = _wtof( csData );
		}
		else
		{
			throw(0);
		}
	}
	catch( ... )
	{
		return false;
	}

	return true;
}

bool CGFC::GET_AT_RSP_STRING( const wchar_t * wcp_Src_Data, const wchar_t * wcp_Key_Word, int i_Line_Offset, CString * csp_String )
{
	//提取“类似AT指令返回格式的应答”中与“关键词”有联系的特定“行”数据，以\r\n换行符为间隔符

	int iIndex = 0;//搜索的起始index，从第一个开始找。
	bool bGet_Key = false;
	CString csUseful;

	//
	try
	{
		//循环搜索，找关键词数据段
		while( this->WCSTOK_STRING_C( wcp_Src_Data, _T( "\r\n" ), iIndex, &csUseful ) == true )
		{
			if( wcsstr( csUseful, wcp_Key_Word ) != NULL )//找到OK了
			{
				bGet_Key = true;
				break;//找到数据字段了，退出while
			}
			else
			{
				iIndex++;//找下一个
			}
		}

		if( bGet_Key == false )//没有找到关键词
		{
			throw(0);
		}

		if( i_Line_Offset != 0 )//不是从“关键词”行提取数据，而是需要调整“行数”的
		{
			iIndex += i_Line_Offset;

			if( iIndex < 0 )
			{
				throw(0);
			}

			//调整行数，再次提取行数
			if( this->WCSTOK_STRING_C( wcp_Src_Data, _T( "\r\n" ), iIndex, &csUseful ) == false )
			{
				throw(0);
			}
		}

		//返回值
		csp_String->Format( _T( "%s" ), csUseful );
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_Reg_Usb_Dev_List( const wchar_t * wcp_USB_PID_VID, CString * csp_USB_Dev_List, int * ip_USB_Quan )
{
	/*
	打开注册表，读出对应的“USB_PID_VID”项中的所有USB设备（列表）
	scan register, and list all USB Port installed with USB Flash Utility Driver
	根据USB规范的规定，所有的USB设备都有供应商ID（VID）和产品识别码（PID），主机通过不同的VID和PID来区别不同的设备。
	VID和PID都是两个字节长，其中，供应商ID（VID）由供应商向USB执行论坛申请，每个供应商的VID是唯一的，PID由供应商自行决定。
	理论上来说，不同的产品、相同产品的不同型号、相同型号的不同设计的产品最好采用不同的PID，以便区别相同厂家的不同设备。
	*/

	HKEY hKey_Root = HKEY_LOCAL_MACHINE;
	CString csSub_Key;
	HKEY hKey_Handle;
	DWORD dwIndex = 0;
	wchar_t * wcpValueName = NULL;
	DWORD dwValueSize = 128;
	DWORD dwDataSize;

	//
	try
	{
		//sub key
		csSub_Key.Format( _T( "SYSTEM\\CurrentControlSet\\Enum\\USB\\%s" ), wcp_USB_PID_VID );

		//open reg
		if( ::RegOpenKeyEx( hKey_Root, csSub_Key, 0, KEY_READ, &hKey_Handle ) != ERROR_SUCCESS )
		{
			throw(1);
		}

		//分配内存
		this->BUFF_WCHAR_CALLOC( &wcpValueName, dwValueSize );

		//
		do
		{
			//
			dwDataSize = dwValueSize;
			memset( wcpValueName, 0, dwValueSize );

			//读reg
			if( ::RegEnumKeyEx( hKey_Handle, dwIndex, wcpValueName, &dwDataSize, (LPDWORD)NULL, (LPTSTR)NULL, (LPDWORD)NULL, (PFILETIME)NULL ) != ERROR_SUCCESS )
			{
				break;//退出while, ERROR_NO_MORE_ITEMS
			}

			//载入数据
			(csp_USB_Dev_List + dwIndex)->Format( _T( "%s" ), wcpValueName );

			//走张
			dwIndex++;

			if( dwIndex >= 64 )
			{
				::AfxMessageBox( _T( "ERROR, Get_Reg_Usb_Dev_List, Too many usb device!" ) );
				throw(2);
			}
		}
		while( true );

		//close reg
		::RegCloseKey( hKey_Handle );

		//
		*ip_USB_Quan = dwIndex;//找到的USB的数目
	}
	catch( ... )
	{
		//释放内存
		this->BUFF_WCHAR_FREE( &wcpValueName );
		//fail
		return false;
	}

	//释放内存
	this->BUFF_WCHAR_FREE( &wcpValueName );
	//pass
	return true;
}

bool CGFC::Get_File_Directory( const wchar_t * wcp_File_Path, CString * csp_File_Directory )
{
	//提取文件路径（所在文件夹）

	int i, iIdx;
	CString csPath;

	csPath.Format( _T( "%s" ), wcp_File_Path );

	iIdx = csPath.GetLength() - 1;

	for( i = iIdx; i >= 0; i-- )
	{
		if( csPath.GetAt( i ) == '\\' )
		{
			csPath.SetAt( i, '\0' );
			break;
		}
	}

	if( i < 0 )
	{
		return false;
	}
	else
	{
		csp_File_Directory->Format( _T( "%s" ), csPath );
	}

	return true;
}

bool CGFC::Kill_Process( const wchar_t * wcp_Process_Name )
{
	//终止某进程（process）

	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
	HANDLE hTargetProcess = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 strProcess_Info;
	CString csCheck, csTgt_Name;
	BOOL bStart;

	g_del_exe_Ctrl.Lock();//只允许一次一个线程进入删除

	try
	{
		//给系统内所有的进程拍个快照
		hProcessSnap = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

		if( hProcessSnap == INVALID_HANDLE_VALUE )
		{
			throw(0);
		}


		//载入目标名
		csTgt_Name.Format( _T( "%s" ), wcp_Process_Name );
		csTgt_Name.MakeLower();


		//在使用这个结构前，先设置它的大小
		strProcess_Info.dwSize = sizeof( PROCESSENTRY32 );

		//遍历当前的进程列表，寻找是否有用户输入的进程
		//遍历进程快照，轮流显示每个进程的信息
		bStart = Process32First( hProcessSnap, &strProcess_Info );

		while( bStart )
		{
			//获得进程列表中的进程名
			csCheck.Format( _T( "%s" ), strProcess_Info.szExeFile );
			csCheck.MakeLower();

			if( csCheck.Compare( csTgt_Name ) == 0 )//Zero if the strings are identical
			{
				//从快照进程中获取该进程的PID(即任务管理器中的PID)
				hTargetProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, strProcess_Info.th32ProcessID );

				if( hTargetProcess == NULL )
				{
					//					::AfxMessageBox(_T("TargetProcess = NULL"));

					throw(0);
				}

				//终止进程
				TerminateProcess( hTargetProcess, 0 );
				CloseHandle( hTargetProcess );
			}

			//找下一个
			bStart = Process32Next( hProcessSnap, &strProcess_Info );
		}//while(bStart

		//注意：关闭这个句柄的方式还有争议
		CloseHandle( hProcessSnap );
	}
	catch( ... )
	{
		g_del_exe_Ctrl.Unlock();

		return false;
	}

	g_del_exe_Ctrl.Unlock();
	return true;
}

bool CGFC::Calcu_ARFCN_GSM( int i_G_Band_Index, double d_Center_Freq, int i_Up_Down_Link, int * ip_ARFCN )
{
	//由GSM频点（MHz）计算获取信道号
	//注意：浮点在做完减法，再做除法可能会因为小数点后的数据而不准，所以计算信道时，把MHz转换为KHz能保证正确
	//注意：“浮点”转换“整型”极可能丢失小数点前的一个数据，例如：1.0 = 0.999999999 = 0

	try
	{
		switch( i_G_Band_Index )
		{
		case G_BAND_GSM900:
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//Rx下行
				{
					if( (d_Center_Freq >= 925.2) && (d_Center_Freq <= 934.8) )//G900: 975 ~ 1023
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 925200) / 200 + 975);
					}
					else if( (d_Center_Freq >= 935.0) && (d_Center_Freq <= 959.8) )//G900: 0 ~ 124
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 935000) / 200 + 0);
					}
					else
					{
						throw(0);
					}
				}
				else//Tx上行
				{
					if( (d_Center_Freq >= 880.2) && (d_Center_Freq <= 889.8) )//G900: 975 ~ 1023
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 880200) / 200 + 975);
					}
					else if( (d_Center_Freq >= 890.0) && (d_Center_Freq <= 914.8) )//G900: 0 ~ 124
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 890000) / 200 + 0);
					}
					else
					{
						throw(0);
					}
				}
			}
			break;
		case G_BAND_DCS1800:
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//Rx下行
				{
					if( (d_Center_Freq >= 1805.2) && (d_Center_Freq <= 1879.8) )//DCS: 512 ~ 885
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1805200) / 200 + 512);
					}
					else
					{
						throw(0);
					}
				}
				else//Tx上行
				{
					if( (d_Center_Freq >= 1710.2) && (d_Center_Freq <= 1784.8) )//DCS: 512 ~ 885
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1710200) / 200 + 512);
					}
					else
					{
						throw(0);
					}
				}
			}
			break;
		case G_BAND_PCS1900:
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//Rx下行
				{
					if( (d_Center_Freq >= 1930.2) && (d_Center_Freq <= 1989.8) )//PCS: 512 ~ 810
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1930200) / 200 + 512);
					}
					else
					{
						throw(0);
					}
				}
				else//Tx上行
				{
					if( (d_Center_Freq >= 1850.2) && (d_Center_Freq <= 1909.8) )//PCS: 512 ~ 810
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1850200) / 200 + 512);
					}
					else
					{
						throw(0);
					}
				}
			}
			break;
		case G_BAND_GSM850:
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//Rx下行
				{
					if( (d_Center_Freq >= 869.2) && (d_Center_Freq <= 893.8) )//G850: 128 ~ 251
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 869200) / 200 + 128);
					}
					else
					{
						throw(0);
					}
				}
				else//Tx上行
				{
					if( (d_Center_Freq >= 824.2) && (d_Center_Freq <= 848.8) )//G850: 128 ~ 251
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 824200) / 200 + 128);
					}
					else
					{
						throw(0);
					}
				}
			}
			break;
		default:
			throw(0);
		}//switch( i_G_Band_Index
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "ERROR, Calcu_ARFCN_GSM(%d, %.2f, %d), Data is wrong!" ), i_G_Band_Index, d_Center_Freq, i_Up_Down_Link );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Calcu_Center_Freq_GSM( int i_G_Band_Index, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq )
{
	//由GSM信道号计算获取频点（MHz）

	switch( i_G_Band_Index )
	{
	case G_BAND_GSM900:
		{
			if( (0 <= i_ARFCN) && (i_ARFCN <= 124) ) // PGSM, EGSM
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					*dp_Center_Freq = 890.0 + 0.2 * i_ARFCN + 45.0;
				}
				else
				{
					*dp_Center_Freq = 890.0 + 0.2 * i_ARFCN;
				}
			}
			else if( (975 <= i_ARFCN) && (i_ARFCN <= 1023) ) // EGSM, PGSM
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					*dp_Center_Freq = 890.0 + 0.2 * (i_ARFCN - 1024) + 45.0;
				}
				else
				{
					*dp_Center_Freq = 890.0 + 0.2 * (i_ARFCN - 1024);
				}
			}
			else
			{
				return false;
			}
		}
		break;

	case G_BAND_DCS1800:
		{
			if( (512 <= i_ARFCN) && (i_ARFCN <= 885) )
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					*dp_Center_Freq = 1710.2 + 0.2 * (i_ARFCN - 512) + 95.0;
				}
				else
				{
					*dp_Center_Freq = 1710.2 + 0.2 * (i_ARFCN - 512);
				}
			}
			else
			{
				return false;
			}
		}
		break;

	case G_BAND_PCS1900:
		{
			if( (512 <= i_ARFCN) && (i_ARFCN <= 810) )
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					*dp_Center_Freq = 1850.2 + 0.2 * (i_ARFCN - 512) + 80.0;
				}
				else
				{
					*dp_Center_Freq = 1850.2 + 0.2 * (i_ARFCN - 512);
				}
			}
			else
			{
				return false;
			}
		}
		break;

	case G_BAND_GSM850:
		{
			if( (128 <= i_ARFCN) && (i_ARFCN <= 251) )
			{
				if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					*dp_Center_Freq = 824.2 + 0.2 * (i_ARFCN - 128) + 45.0;
				}
				else
				{
					*dp_Center_Freq = 824.2 + 0.2 * (i_ARFCN - 128);
				}
			}
			else
			{
				return false;
			}
		}
		break;

	default:
		return false;
	}//switch( i_G_Band_Index

	//pass
	return true;
}

bool CGFC::Calcu_Center_Freq_WCDMA( int i_W_Band_Index, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq )
{
	//由WCDMA信道号计算获取频点（MHz）
	//WCDMA新加频段代码位置

	//
	try
	{
		switch( i_W_Band_Index )
		{
		case W_BAND_1:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (9575 <= i_ARFCN) && (i_ARFCN <= 9925) ) //W_BAND_1 上行
					{
						*dp_Center_Freq = 1922.6 + 0.2 * (i_ARFCN - 9613);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (10563 <= i_ARFCN) && (i_ARFCN <= 10838) ) //W_BAND_1 下行
					{
						*dp_Center_Freq = 2112.6 + 0.2 * (i_ARFCN - 10563);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_2:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (9225 <= i_ARFCN) && (i_ARFCN <= 9575) ) //W_BAND_2 上行
					{
						*dp_Center_Freq = 1852.6 + 0.2 * (i_ARFCN - 9263);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (9662 <= i_ARFCN) && (i_ARFCN <= 9938) ) //W_BAND_2 下行
					{
						*dp_Center_Freq = 1932.6 + 0.2 * (i_ARFCN - 9663);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_3:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (937 <= i_ARFCN) && (i_ARFCN <= 1288) ) //W_BAND_3 上行
					{
						*dp_Center_Freq = 1712.4 + 0.2 * (i_ARFCN - 937);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (1162 <= i_ARFCN) && (i_ARFCN <= 1513) ) //W_BAND_3 下行
					{
						*dp_Center_Freq = 1807.4 + 0.2 * (i_ARFCN - 1162);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_4:
			{
				if( RF_TX_UL_INPUT == i_Up_Down_Link )
				{
					if( (1312 <= i_ARFCN) && (i_ARFCN <= 1513) ) //W_BAND_4 上行
					{
						*dp_Center_Freq = 1712.4 + 0.2 * (i_ARFCN - 1312);
					}
					else
					{
						throw(0);
					}
				}
				else if( RF_RX_DL_OUTPUT == i_Up_Down_Link )
				{
					if( (1537 <= i_ARFCN) && (i_ARFCN <= 1738) ) //W_BAND_4 下行
					{
						*dp_Center_Freq = 2112.4 + 0.2 * (i_ARFCN - 1537);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case W_BAND_5:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (4107 <= i_ARFCN) && (i_ARFCN <= 4257) ) //W_BAND_5 上行
					{
						*dp_Center_Freq = 826.6 + 0.2 * (i_ARFCN - 4133);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (4357 <= i_ARFCN) && (i_ARFCN <= 4458) ) //W_BAND_5 下行
					{
						*dp_Center_Freq = 871.6 + 0.2 * (i_ARFCN - 4358);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_6:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//W_BAND_6 上行
				{
					if( i_ARFCN == 812 )
					{
						*dp_Center_Freq = 832.5;
					}
					else if( i_ARFCN == 837 )
					{
						*dp_Center_Freq = 837.5;
					}
					else if( (4162 <= i_ARFCN) && (i_ARFCN <= 4188) )
					{
						*dp_Center_Freq = 832.4 + 0.2 * (i_ARFCN - 4162);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//W_BAND_6 下行
				{
					if( i_ARFCN == 1037 )
					{
						*dp_Center_Freq = 877.5;
					}
					else if( i_ARFCN == 1062 )
					{
						*dp_Center_Freq = 882.5;
					}
					else if( (4387 <= i_ARFCN) && (i_ARFCN <= 4413) )
					{
						*dp_Center_Freq = 877.4 + 0.2 * (i_ARFCN - 4387);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_8:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (2687 <= i_ARFCN) && (i_ARFCN <= 2887) ) //W_BAND_8 上行
					{
						*dp_Center_Freq = 882.4 + 0.2 * (i_ARFCN - 2712);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (2937 <= i_ARFCN) && (i_ARFCN <= 3088) ) //W_BAND_8 下行
					{
						*dp_Center_Freq = 927.4 + 0.2 * (i_ARFCN - 2937);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_9:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (8762 <= i_ARFCN) && (i_ARFCN <= 8912) ) //W_BAND_9 上行
					{
						*dp_Center_Freq = 1752.4 + 0.2 * (i_ARFCN - 8762);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (9237 <= i_ARFCN) && (i_ARFCN <= 9387) ) //W_BAND_9 下行
					{
						*dp_Center_Freq = 1847.4 + 0.2 * (i_ARFCN - 9237);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_19:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//W_BAND_19 上行
				{
					if( i_ARFCN == 387 )
					{
						*dp_Center_Freq = 832.5;
					}
					else if( i_ARFCN == 412 )
					{
						*dp_Center_Freq = 837.5;
					}
					else if( i_ARFCN == 437 )
					{
						*dp_Center_Freq = 842.5;
					}
					else if( (312 <= i_ARFCN) && (i_ARFCN <= 363) )
					{
						*dp_Center_Freq = 832.4 + 0.2 * (i_ARFCN - 312);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//W_BAND_19 下行
				{
					if( i_ARFCN == 787 )
					{
						*dp_Center_Freq = 877.5;
					}
					else if( i_ARFCN == 812 )
					{
						*dp_Center_Freq = 882.5;
					}
					else if( i_ARFCN == 837 )
					{
						*dp_Center_Freq = 887.5;
					}
					else if( (712 <= i_ARFCN) && (i_ARFCN <= 763) )
					{
						*dp_Center_Freq = 877.4 + 0.2 * (i_ARFCN - 712);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		default:
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "Calcu_Center_Freq_WCDMA( W_Band_Index = %d, ARFCN = %d, Up_Down_Link = %d ), Fail !" ), i_W_Band_Index, i_ARFCN, i_Up_Down_Link );
		//
		return false;
	}

	//
	return true;
}

bool CGFC::GET_AT_RSP_MID_STRING( const wchar_t * wcp_Src_Data, const wchar_t * wcp_Start_Key, const wchar_t * wcp_End_Key, CString * csp_Useful_String )
{
	//提取AT指令相应中，“开始”关键词和“结束”关键词之间的字符数据

	wchar_t * wcpSrc = NULL;//“源数据”的buff，wcstok在提取数据时会改变源数据内容，所以先把源数据放入buff，解析buff数据，不伤害参数源。
	unsigned long ulBuff_Len = wcslen( wcp_Src_Data ) + 8;//用于存放“源数据”的buff长度
	wchar_t * wcpHead = NULL;
	wchar_t * wcpEnd = NULL;
	int iLen, i;
	CString csData;

	try
	{

		//分配内存
		this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

		//载入“源数据”
		swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_Src_Data );

		//检查是否有“开始”关键词
		wcpHead = wcsstr( wcpSrc, wcp_Start_Key );
		if( wcpHead == NULL )
		{
			this->m_csError_Info.Format( _T( "ERROR, GET_AT_RSP_MID_STRING, No start key!" ) );
			throw(0);
		}

		//去掉“开始”关键词
		wcpHead += wcslen( wcp_Start_Key );


		//检查是否有“结束”关键词
		wcpEnd = wcsstr( wcpHead, wcp_End_Key );
		if( wcpEnd == NULL )
		{
			this->m_csError_Info.Format( _T( "ERROR, GET_AT_RSP_MID_STRING, No end key!" ) );
			throw(0);
		}

		//计算“有用数据”数量
		iLen = wcpEnd - wcpHead;

		//提取有用数据
		csData.Format( _T( "" ) );
		for( i = 0; i < iLen; i++ )
		{
			csData += *(wcpHead + i);
		}


		//返回值
		csp_Useful_String->Format( _T( "%s" ), csData );
	}
	catch( ... )
	{
		//释放内存
		this->BUFF_WCHAR_FREE( &wcpSrc );
		return false;
	}

	//释放内存
	this->BUFF_WCHAR_FREE( &wcpSrc );
	return true;
}

bool CGFC::Calcu_ARFCN_WCDMA( int i_W_Band_Index, double d_Center_Freq, int i_Up_Down_Link, int * ip_ARFCN )
{
	//由WCDMA频点（MHz）计算信道号
	//WCDMA新加频段代码位置

	//
	try
	{
		switch( i_W_Band_Index )
		{
		case W_BAND_1:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 1922.6) && (d_Center_Freq <= 1977.4) )//BAND_1 上行
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1922600) / 200 + 9613);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 2112.6) && (d_Center_Freq <= 2167.4) )//BAND_1 下行
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 2112600) / 200 + 10563);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_2:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 1852.4) && (d_Center_Freq <= 1907.6) )//BAND_2 上行
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1852600) / 200 + 9263);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 1932.4) && (d_Center_Freq <= 1987.6) )//BAND_2 下行
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1932600) / 200 + 9663);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_3:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 1712.4) && (d_Center_Freq <= 1782.6) )//BAND_3 上行
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1712400) / 200 + 937);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 1807.4) && (d_Center_Freq <= 1877.6) )//BAND_3 下行
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 1807400) / 200 + 1162);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_4:
			{
				if( RF_TX_UL_INPUT == i_Up_Down_Link )
				{
					if( (d_Center_Freq >= 1712.4) && (d_Center_Freq <= 1752.6) )//W_BAND_4 上行
					{
						*ip_ARFCN = (d_Center_Freq * 1000 - 1712400) / 200 + 1312;
					}
					else
					{
						throw(0);
					}
				}
				else if( RF_RX_DL_OUTPUT == i_Up_Down_Link )
				{
					if( (d_Center_Freq >= 2112.4) && (d_Center_Freq <= 2152.6) )//W_BAND_4 下行
					{
						*ip_ARFCN = (d_Center_Freq * 1000 - 2112400) / 200 + 1537;
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_5:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 826.4) && (d_Center_Freq <= 846.6) )//BAND_5 上行
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 826600) / 200 + 4133);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 871.4) && (d_Center_Freq <= 891.6) )//BAND_5 下行
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 871600) / 200 + 4358);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_6:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//W_BAND_6 上行
				{
					if( d_Center_Freq == 832.5 )
					{
						*ip_ARFCN = 812;
					}
					else if( d_Center_Freq == 837.5 )
					{
						*ip_ARFCN = 837;
					}
					else if( (d_Center_Freq >= 832.4) && (d_Center_Freq <= 837.6) )
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 832400) / 200 + 4162);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//W_BAND_6 下行
				{
					if( d_Center_Freq == 877.5 )
					{
						*ip_ARFCN = 1037;
					}
					else if( d_Center_Freq == 882.5 )
					{
						*ip_ARFCN = 1062;
					}
					else if( (d_Center_Freq >= 877.4) && (d_Center_Freq <= 882.6) )
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 877400) / 200 + 4387);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_8:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 882.4) && (d_Center_Freq <= 912.6) )//BAND_8 上行
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 882400) / 200 + 2712);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 927.4) && (d_Center_Freq <= 957.6) )//BAND_8 下行
					{
						*ip_ARFCN = (d_Center_Freq * 1000 - 927400) / 200 + 2937;
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_9:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )
				{
					if( (d_Center_Freq >= 1752.4) && (d_Center_Freq <= 1782.4) )//W_BAND_9 上行
					{
						*ip_ARFCN = (d_Center_Freq * 1000.0 - 1752400) / 200 + 8762;
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )
				{
					if( (d_Center_Freq >= 1847.4) && (d_Center_Freq <= 1877.4) )//W_BAND_9 下行
					{
						*ip_ARFCN = (d_Center_Freq * 1000.0 - 1847400) / 200 + 9237;
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		case W_BAND_19:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//W_BAND_19 上行
				{
					if( d_Center_Freq == 832.5 )
					{
						*ip_ARFCN = 387;
					}
					else if( d_Center_Freq == 837.5 )
					{
						*ip_ARFCN = 412;
					}
					else if( d_Center_Freq == 842.5 )
					{
						*ip_ARFCN = 437;
					}
					else if( (d_Center_Freq >= 832.4) && (d_Center_Freq <= 842.6) )
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 832400) / 200 + 312);
					}
					else
					{
						throw(0);
					}
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//W_BAND_19 下行
				{
					if( d_Center_Freq == 877.5 )
					{
						*ip_ARFCN = 787;
					}
					else if( d_Center_Freq == 882.5 )
					{
						*ip_ARFCN = 812;
					}
					else if( d_Center_Freq == 887.5 )
					{
						*ip_ARFCN = 837;
					}
					else if( (d_Center_Freq >= 877.4) && (d_Center_Freq <= 887.6) )
					{
						*ip_ARFCN = (int)((d_Center_Freq * 1000 - 877400) / 200 + 712);
					}
					else
					{
						throw(0);
					}
				}
				else
				{
					throw(0);
				}
			}
			break;

		default:
			throw(0);
		}//switch( i_W_Band_Index
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "Calcu_ARFCN_WCDMA( W_Band_Index = %d, Center_Freq = %.4f, Up_Down_Link = %d ), Fail !" ), i_W_Band_Index, d_Center_Freq, i_Up_Down_Link );
		//fail
		return false;
	}

	//pass
	return true;
}

void CGFC::Empty_Directory( const wchar_t * wcp_Dir_Path )
{
	//清空文件夹

	CString csDir;

	//
	csDir.Format( _T( "%s" ), wcp_Dir_Path );

	//
	if( csDir.IsEmpty() )//true便是“空”字串
	{
		//是“空”字串
		//不做处理
		return;
	}

	// 首先删除文件及子文件夹
	CFileFind cla_Finder;
	BOOL bFound = cla_Finder.FindFile( csDir + _T( "\\*" ), 0 );

	while( bFound )
	{
		bFound = cla_Finder.FindNextFile();
		if( (cla_Finder.GetFileName() == _T( "." )) || (cla_Finder.GetFileName() == _T( ".." )) )
		{
			continue;
		}

		// 去掉文件(夹)只读等属性
		SetFileAttributes( cla_Finder.GetFilePath(), FILE_ATTRIBUTE_NORMAL );

		if( cla_Finder.IsDirectory() )
		{
			// 递归删除子文件夹
			Empty_Directory( cla_Finder.GetFilePath() );
			RemoveDirectory( cla_Finder.GetFilePath() );
		}
		else
		{
			// 删除文件
			DeleteFile( cla_Finder.GetFilePath() );
		}
	}

	//
	cla_Finder.Close();

	//可以删除当前文件夹
//	RemoveDirectory(csDir);
}

bool CGFC::Get_Reg_Usb_Dev_Friendly_Name( const wchar_t * wcp_USB_PID_VID, const wchar_t * wcp_USB_Dev_Name, CString * csp_Friendly_Name )
{
	//读USB设备的“FriendlyName”

	HKEY hKey_Root = HKEY_LOCAL_MACHINE;
	CString csSub_Key;
	HKEY hKey_Handle;
	wchar_t wcrValue_Data[128];
	DWORD dwData_Size;
	DWORD dwData_Type = REG_SZ;

	//
	try
	{
		//sub key
		csSub_Key.Format( _T( "SYSTEM\\CurrentControlSet\\Enum\\USB\\%s\\%s" ), wcp_USB_PID_VID, wcp_USB_Dev_Name );

		//open reg
		if( ::RegOpenKeyEx( hKey_Root, csSub_Key, 0, KEY_READ, &hKey_Handle ) != ERROR_SUCCESS )
		{
			throw(1);
		}

		//
		dwData_Size = 128;
		memset( wcrValue_Data, 0, sizeof( wcrValue_Data ) );

		//读指定项目的数据
		if( ::RegQueryValueEx( hKey_Handle, _T( "FriendlyName" ), (LPDWORD)NULL, &dwData_Type, (BYTE *)wcrValue_Data, &dwData_Size ) != ERROR_SUCCESS )
		{
			throw(2);
		}

		//close reg
		::RegCloseKey( hKey_Handle );

		//
		csp_Friendly_Name->Format( _T( "%s" ), wcrValue_Data );
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

void CGFC::WCSTOK_STRING_T( const wchar_t * wcp_StrToken, CString * csp_Get_CString_Array, int * ip_Get_Quan )
{
	//传统的提取“子串”

	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//“源数据”的buff，wcstok在提取数据时会改变源数据内容，所以先把源数据放入buff，解析buff数据，不伤害参数源。
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//用于存放“源数据”的buff长度
	WCHAR * wcpResult = NULL;


	//分配内存
	this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

	//载入“源数据”
	swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

	//提取第一个值(0)
	wcpEach = wcstok_s( wcpSrc, _T( " ,;\r\n" ), &wcpResult );

	while( wcpEach != NULL )
	{
		//赋值
		(csp_Get_CString_Array + i)->Format( _T( "%s" ), wcpEach );

		//提取下一个值
		wcpEach = wcstok_s( NULL, _T( " ,;\r\n" ), &wcpResult );

		//计数
		i++;
	}//while(wcpEach != NULL

	//返回数量
	*ip_Get_Quan = i;


	//释放内存
	this->BUFF_WCHAR_FREE( &wcpSrc );
}

bool CGFC::Calcu_Center_Freq_LTE( int i_L_Band_Name, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq_MHz )
{
	//由LTE信道号计算获取频点（MHz）
	//LTE新加频段代码位置

	//
	try
	{
		switch( i_L_Band_Name )
		{
		case 1://L_BAND_1
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1920.0 + 0.1 * (i_ARFCN - 18000);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 2110.0 + 0.1 * (i_ARFCN - 0);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 2://L_BAND_2
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1850.0 + 0.1 * (i_ARFCN - 18600);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 1930.0 + 0.1 * (i_ARFCN - 600);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 3://L_BAND_3
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1710.0 + 0.1 * (i_ARFCN - 19200);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 1805.0 + 0.1 * (i_ARFCN - 1200);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 4://L_BAND_4
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1710.0 + 0.1 * (i_ARFCN - 19950);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 2110.0 + 0.1 * (i_ARFCN - 1950);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 5://L_BAND_5
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 824.0 + 0.1 * (i_ARFCN - 20400);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 869.0 + 0.1 * (i_ARFCN - 2400);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 7://L_BAND_7
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 2500.0 + 0.1 * (i_ARFCN - 20750);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 2620.0 + 0.1 * (i_ARFCN - 2750);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 8://L_BAND_8
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 880.0 + 0.1 * (i_ARFCN - 21450);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 925.0 + 0.1 * (i_ARFCN - 3450);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 9://L_BAND_9
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1749.9 + 0.1 * (i_ARFCN - 21800);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 1844.9 + 0.1 * (i_ARFCN - 3800);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 10:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1710.0 + 0.1 * (i_ARFCN - 22150);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 2110.0 + 0.1 * (i_ARFCN - 4150);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 11://L_BAND_11
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1427.9 + 0.1 * (i_ARFCN - 22750);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 1475.9 + 0.1 * (i_ARFCN - 4750);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 12://L_BAND_12
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 699.0 + 0.1 * (i_ARFCN - 23010);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 729.0 + 0.1 * (i_ARFCN - 5010);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 13://L_BAND_13
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 777.0 + 0.1 * (i_ARFCN - 23180);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 746.0 + 0.1 * (i_ARFCN - 5180);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 14://L_BAND_14
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 788.0 + 0.1 * (i_ARFCN - 23280);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 758.0 + 0.1 * (i_ARFCN - 5280);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 17://L_BAND_17
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 704.0 + 0.1 * (i_ARFCN - 23730);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 734.0 + 0.1 * (i_ARFCN - 5730);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 18://L_BAND_18
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 815.0 + 0.1 * (i_ARFCN - 23850);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 860.0 + 0.1 * (i_ARFCN - 5850);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 19://L_BAND_19
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 830.0 + 0.1 * (i_ARFCN - 24000);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 875.0 + 0.1 * (i_ARFCN - 6000);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 20://L_BAND_20
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 832.0 + 0.1 * (i_ARFCN - 24150);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 791.0 + 0.1 * (i_ARFCN - 6150);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 21://L_BAND_21
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1447.9 + 0.1 * (i_ARFCN - 24450);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 1495.9 + 0.1 * (i_ARFCN - 6450);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 22:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 3410.0 + 0.1 * (i_ARFCN - 24600);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 3510.0 + 0.1 * (i_ARFCN - 6600);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 24:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1626.5 + 0.1 * (i_ARFCN - 25700);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 1525.0 + 0.1 * (i_ARFCN - 7700);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 25://L_BAND_25
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1850.0 + 0.1 * (i_ARFCN - 26040);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 1930.0 + 0.1 * (i_ARFCN - 8040);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 26://L_BAND_26
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 814.0 + 0.1 * (i_ARFCN - 26690);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 859.0 + 0.1 * (i_ARFCN - 8690);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 27://L_BAND_27
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 807.0 + 0.1 * (i_ARFCN - 27040);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 852.0 + 0.1 * (i_ARFCN - 9040);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 28://L_BAND_28
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 703.0 + 0.1 * (i_ARFCN - 27210);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 758.0 + 0.1 * (i_ARFCN - 9210);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 29://L_BAND_29
			{
				*dp_Center_Freq_MHz = 717.0 + 0.1 * (i_ARFCN - 9660);
			}
			//
			break;

		case 30://L_BAND_30
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 2305.0 + 0.1 * (i_ARFCN - 27660);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 2350.0 + 0.1 * (i_ARFCN - 9770);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 31://L_BAND_31 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 452.5 + 0.1 * (i_ARFCN - 27760);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 462.5 + 0.1 * (i_ARFCN - 9870);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 32://L_BAND_32
			{
				*dp_Center_Freq_MHz = 1452.0 + 0.1 * (i_ARFCN - 9920);
			}
			//
			break;

		case 34://L_BAND_34
			{
				*dp_Center_Freq_MHz = 2010.0 + 0.1 * (i_ARFCN - 36200);
			}
			//
			break;

		case 38://L_BAND_38
			{
				*dp_Center_Freq_MHz = 2570.0 + 0.1 * (i_ARFCN - 37750);
			}
			//
			break;

		case 39://L_BAND_39
			{
				*dp_Center_Freq_MHz = 1880.0 + 0.1 * (i_ARFCN - 38250);
			}
			//
			break;

		case 40://L_BAND_40
			{
				*dp_Center_Freq_MHz = 2300.0 + 0.1 * (i_ARFCN - 38650);
			}
			//
			break;

		case 41://L_BAND_41
			{
				*dp_Center_Freq_MHz = 2496.0 + 0.1 * (i_ARFCN - 39650);
			}
			//
			break;

		case 42://L_BAND_42
			{
				*dp_Center_Freq_MHz = 3400.0 + 0.1 * (i_ARFCN - 41590);
			}
			//
			break;

		case 43://L_BAND_43
			{
				*dp_Center_Freq_MHz = 3600.0 + 0.1 * (i_ARFCN - 43590);
			}
			//
			break;

		case 46://L_BAND_46
			{
				*dp_Center_Freq_MHz = 5150.0 + 0.1 * (i_ARFCN - 46790);
			}
			//
			break;

		case 47://L_BAND_47
			{
				*dp_Center_Freq_MHz = 5855.0 + 0.1 * (i_ARFCN - 54540);
			}
			//
			break;

		case 48://L_BAND_48
			{
				*dp_Center_Freq_MHz = 3550.0 + 0.1 * (i_ARFCN - 55240);
			}
			//
			break;

		case 66://L_BAND_66 FDD
			{
				//注意：Band66的DL的2180-2200 MHz是配置CA时候用的
				//The range 2180-2200 MHz of the DL operating band is restricted to E-UTRA operation when carrier aggregation is configured.
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1710.0 + 0.1 * (i_ARFCN - 131972);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 2110.0 + 0.1 * (i_ARFCN - 66436);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 68:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 698.0 + 0.1 * (i_ARFCN - 132672);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 753.0 + 0.1 * (i_ARFCN - 67536);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 70:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1695.0 + 0.1 * (i_ARFCN - 132972);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 1995.0 + 0.1 * (i_ARFCN - 68336);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 71://L_BAND_71 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 663.0 + 0.1 * (i_ARFCN - 133122);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 617.0 + 0.1 * (i_ARFCN - 68586);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 72://L_BAND_72 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 451.0 + 0.1 * (i_ARFCN - 133472);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 461.0 + 0.1 * (i_ARFCN - 68936);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 73://L_BAND_73 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 450.0 + 0.1 * (i_ARFCN - 133522);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 460.0 + 0.1 * (i_ARFCN - 68986);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 74:
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 1427.0 + 0.1 * (i_ARFCN - 133572);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 1475.0 + 0.1 * (i_ARFCN - 69036);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 85://L_BAND_85 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 698.0 + 0.1 * (i_ARFCN - 134002);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 728.0 + 0.1 * (i_ARFCN - 70366);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 86://L_BAND_86 NB-B86 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*dp_Center_Freq_MHz = 787.0 + 0.1 * (i_ARFCN - 134182);//Uplink Channel:134182-134191, FREQ:787.0-787.9MHz
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*dp_Center_Freq_MHz = 757.0 + 0.1 * (i_ARFCN - 70546);//Downlink Channel:70546-70555, FREQ:757.0-757.9MHz
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 87://L_BAND_87 FDD
			{
				if( RF_TX_UL_INPUT == i_Up_Down_Link )//上行
				{
					*dp_Center_Freq_MHz = 410.0 + 0.1 * (i_ARFCN - 134182);
				}
				else if( RF_RX_DL_OUTPUT == i_Up_Down_Link )//下行
				{
					*dp_Center_Freq_MHz = 420.0 + 0.1 * (i_ARFCN - 70546);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 88://L_BAND_88 FDD
			{
				if( RF_TX_UL_INPUT == i_Up_Down_Link )//上行
				{
					*dp_Center_Freq_MHz = 412.0 + 0.1 * (i_ARFCN - 134231);
				}
				else if( RF_RX_DL_OUTPUT == i_Up_Down_Link )//下行
				{
					*dp_Center_Freq_MHz = 422.0 + 0.1 * (i_ARFCN - 70596);
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		default:
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "ERROR, Calcu_Center_Freq_LTE( L_Band_Name = %d, ARFCN = %d, Up_Down_Link = %d )" ), i_L_Band_Name, i_ARFCN, i_Up_Down_Link );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Calcu_ARFCN_LTE( int i_L_Band_Name, double d_Center_Freq_MHz, int i_Up_Down_Link, int * ip_ARFCN )
{
	//由LTE频点（MHz）计算获取信道号
	//LTE新加频段代码位置

	try
	{
		switch( i_L_Band_Name )
		{
		case 1://L_BAND_1
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1920.0) + 18000);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 2110.0) + 0);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 2://L_BAND_2
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1850.0) + 18600);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1930.0) + 600);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 3://L_BAND_3
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1710.0) + 19200);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1805.0) + 1200);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 4://L_BAND_4
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 1710.0) + 19950);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 2110.0) + 1950);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 5://L_BAND_5
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 824.0) + 20400);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 869.0) + 2400);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 7://L_BAND_7
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 2500.0) + 20750);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 2620.0) + 2750);
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 8://L_BAND_8
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = (int)(10 * (d_Center_Freq_MHz - 880.0) + 21450);
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 925.0) + 3450;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;
			
		case 9://L_BAND_9
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1749.9) + 21800;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1844.9) + 3800;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 11://L_BAND_11
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1427.9) + 22750;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1475.9) + 4750;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 12://L_BAND_12
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 699.0) + 23010;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 729.0) + 5010;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 13://L_BAND_13
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 777.0) + 23180;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 746.0) + 5180;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 14://L_BAND_14
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 788.0) + 23280;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 758.0) + 5280;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 17://L_BAND_17
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 704.0) + 23730;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 734.0) + 5730;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 18://L_BAND_18
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 815.0) + 23850;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 860.0) + 5850;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 19://L_BAND_19
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 830.0) + 24000;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 875.0) + 6000;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 20://L_BAND_20
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 832.0) + 24150;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 791.0) + 6150;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 21://L_BAND_21
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1447.9) + 24450;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1495.9) + 6450;
				}
				else
				{
					throw(0);
				}
			}
			break;

		case 25://L_BAND_25
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1850.0) + 26040;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1930.0) + 8040;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 26://L_BAND_26
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 814.0) + 26690;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 859.0) + 8690;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 27://L_BAND_27
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 807.0) + 27040;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 852.0) + 9040;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 28://L_BAND_28
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 703.0) + 27210;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 758.0) + 9210;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 29://L_BAND_29
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 717.0) + 9660;
			}
			//
			break;

		case 30://L_BAND_30
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2305.0) + 27660;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2350.0) + 9770;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 31://L_BAND_31 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 452.5) + 27760;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 462.5) + 9870;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 32://L_BAND_32
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1452.0) + 9920;
			}
			//
			break;

		case 34://L_BAND_34
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2010.0) + 36200;
			}
			//
			break;

		case 38://L_BAND_38
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2570.0) + 37750;
			}
			//
			break;

		case 39://L_BAND_39
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1880.0) + 38250;
			}
			//
			break;

		case 40://L_BAND_40
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2300.0) + 38650;
			}
			//
			break;

		case 41://L_BAND_41
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2496.0) + 39650;
			}
			//
			break;

		case 42://L_BAND_42
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 3400.0) + 41590;
			}
			//
			break;

		case 43://L_BAND_43
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 3600.0) + 43590;
			}
			//
			break;

		case 46://L_BAND_46
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 5150.0) + 46790;
			}
			//
			break;

		case 47://L_BAND_47
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 5855.0) + 54540;
			}
			//
			break;

		case 48://L_BAND_48
			{
				*ip_ARFCN = 10 * (d_Center_Freq_MHz - 3550.0) + 55240;
			}
			//
			break;

		case 66://L_BAND_66 FDD
			{
				//注意：Band66的DL的2180-2200 MHz是配置CA时候用的
				//The range 2180-2200 MHz of the DL operating band is restricted to E-UTRA operation when carrier aggregation is configured.
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 1710.0) + 131972;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 2110.0) + 66436;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 71://L_BAND_71 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 663.0) + 133122;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 617.0) + 68586;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 72://L_BAND_72 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 451.0) + 133472;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 461.0) + 68936;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 73://L_BAND_73 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 450.0) + 133522;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 460.0) + 68986;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 85://L_BAND_85 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 698.0) + 134002;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 728.0) + 70366;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 86://L_BAND_86 NB-B86 FDD
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 787.0) + 134182;//Uplink Channel:134182-134191, FREQ:787.0-787.9MHz
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 757.0) + 70546;//Downlink Channel:70546-70555, FREQ:757.0-757.9MHz
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 87://L_BAND_87, FDD, NB_BAND_87
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 410.0) + 134182;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 420.0) + 70546;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		case 88://L_BAND_88, FDD, NB_BAND_88
			{
				if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 412.0) + 134231;
				}
				else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
				{
					*ip_ARFCN = 10 * (d_Center_Freq_MHz - 422.0) + 70596;
				}
				else
				{
					throw(0);
				}
			}
			//
			break;

		default:
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "Calcu_ARFCN_LTE( L_Band = %d, Center_Freq = %.4f, Up_Down_Link = %d ), Fail !" ), i_L_Band_Name, d_Center_Freq_MHz, i_Up_Down_Link );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_LTE_Band_Name( int i_LTE_Band_Index, int * ip_L_Band_Name )
{
	//LTE频段index号 转化 LTE频段名
	//LTE新加频段代码位置

	//
	switch( i_LTE_Band_Index )
	{
	case L_BAND_1:
		*ip_L_Band_Name = 1;
		break;
	case L_BAND_2:
		*ip_L_Band_Name = 2;
		break;
	case L_BAND_3:
		*ip_L_Band_Name = 3;
		break;
	case L_BAND_4:
		*ip_L_Band_Name = 4;
		break;
	case L_BAND_5:
		*ip_L_Band_Name = 5;
		break;
	case L_BAND_7:
		*ip_L_Band_Name = 7;
		break;
	case L_BAND_8:
		*ip_L_Band_Name = 8;
		break;
	case L_BAND_9:
		*ip_L_Band_Name = 9;
		break;
	case L_BAND_11:
		*ip_L_Band_Name = 11;
		break;
	case L_BAND_12:
		*ip_L_Band_Name = 12;
		break;
	case L_BAND_13:
		*ip_L_Band_Name = 13;
		break;
	case L_BAND_14:
		*ip_L_Band_Name = 14;
		break;
	case L_BAND_17:
		*ip_L_Band_Name = 17;
		break;
	case L_BAND_18:
		*ip_L_Band_Name = 18;
		break;
	case L_BAND_19:
		*ip_L_Band_Name = 19;
		break;
	case L_BAND_20:
		*ip_L_Band_Name = 20;
		break;
	case L_BAND_21:
		*ip_L_Band_Name = 21;
		break;
	case L_BAND_25:
		*ip_L_Band_Name = 25;
		break;
	case L_BAND_26:
		*ip_L_Band_Name = 26;
		break;
	case L_BAND_27:
		*ip_L_Band_Name = 27;
		break;
	case L_BAND_28:
		*ip_L_Band_Name = 28;
		break;
	case L_BAND_29:
		*ip_L_Band_Name = 29;
		break;
	case L_BAND_30:
		*ip_L_Band_Name = 30;
		break;
	case L_BAND_31:
		*ip_L_Band_Name = 31;
		break;
	case L_BAND_32:
		*ip_L_Band_Name = 32;
		break;
	case L_BAND_34:
		*ip_L_Band_Name = 34;
		break;
	case L_BAND_38:
		*ip_L_Band_Name = 38;
		break;
	case L_BAND_39:
		*ip_L_Band_Name = 39;
		break;
	case L_BAND_40:
		*ip_L_Band_Name = 40;
		break;
	case L_BAND_41:
		*ip_L_Band_Name = 41;
		break;
	case L_BAND_42:
		*ip_L_Band_Name = 42;
		break;
	case L_BAND_43:
		*ip_L_Band_Name = 43;
		break;
	case L_BAND_46:
		*ip_L_Band_Name = 46;
		break;
	case L_BAND_47:
		*ip_L_Band_Name = 47;
		break;
	case L_BAND_48:
		*ip_L_Band_Name = 48;
		break;
	case L_BAND_66:
		*ip_L_Band_Name = 66;
		break;
	case L_BAND_71:
		*ip_L_Band_Name = 71;
		break;
	case L_BAND_72:
		*ip_L_Band_Name = 72;
		break;
	case L_BAND_73:
		*ip_L_Band_Name = 73;
		break;
	case L_BAND_85:
		*ip_L_Band_Name = 85;
		break;
	case L_BAND_87:
		*ip_L_Band_Name = 87;
		break;
	case L_BAND_88:
		*ip_L_Band_Name = 88;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_LTE_Band_Name, LTE_Band_Index(%d) is wrong!" ), i_LTE_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_LTE_Band_Index

	//pass
	return true;
}

bool CGFC::Get_LTE_Band_Index( int i_LTE_Band_Name, int * ip_L_Band_Index )
{
	//LTE频段名 转化 LTE频段index号
	//LTE新加频段代码位置

	//
	switch( i_LTE_Band_Name )
	{
	case 1://L_BAND_1
		*ip_L_Band_Index = L_BAND_1;
		break;
	case 2://L_BAND_2
		*ip_L_Band_Index = L_BAND_2;
		break;
	case 3://L_BAND_3
		*ip_L_Band_Index = L_BAND_3;
		break;
	case 4://L_BAND_4
		*ip_L_Band_Index = L_BAND_4;
		break;
	case 5://L_BAND_5
		*ip_L_Band_Index = L_BAND_5;
		break;
	case 7://L_BAND_7
		*ip_L_Band_Index = L_BAND_7;
		break;
	case 8://L_BAND_8
		*ip_L_Band_Index = L_BAND_8;
		break;
	case 9://L_BAND_9
		*ip_L_Band_Index = L_BAND_9;
		break;
	case 11://L_BAND_11
		*ip_L_Band_Index = L_BAND_11;
		break;
	case 12://L_BAND_12
		*ip_L_Band_Index = L_BAND_12;
		break;
	case 13://L_BAND_13
		*ip_L_Band_Index = L_BAND_13;
		break;
	case 14://L_BAND_14
		*ip_L_Band_Index = L_BAND_14;
		break;
	case 17://L_BAND_17
		*ip_L_Band_Index = L_BAND_17;
		break;
	case 18://L_BAND_18
		*ip_L_Band_Index = L_BAND_18;
		break;
	case 19://L_BAND_19
		*ip_L_Band_Index = L_BAND_19;
		break;
	case 20://L_BAND_20
		*ip_L_Band_Index = L_BAND_20;
		break;
	case 21://L_BAND_21
		*ip_L_Band_Index = L_BAND_21;
		break;
	case 25://L_BAND_25
		*ip_L_Band_Index = L_BAND_25;
		break;
	case 26://L_BAND_26
		*ip_L_Band_Index = L_BAND_26;
		break;
	case 27://L_BAND_27
		*ip_L_Band_Index = L_BAND_27;
		break;
	case 28://L_BAND_28
		*ip_L_Band_Index = L_BAND_28;
		break;
	case 29://L_BAND_29
		*ip_L_Band_Index = L_BAND_29;
		break;
	case 30://L_BAND_30
		*ip_L_Band_Index = L_BAND_30;
		break;
	case 31://L_BAND_31
		*ip_L_Band_Index = L_BAND_31;
		break;
	case 32://L_BAND_32
		*ip_L_Band_Index = L_BAND_32;
		break;
	case 34://L_BAND_34
		*ip_L_Band_Index = L_BAND_34;
		break;
	case 38://L_BAND_38
		*ip_L_Band_Index = L_BAND_38;
		break;
	case 39://L_BAND_39
		*ip_L_Band_Index = L_BAND_39;
		break;
	case 40://L_BAND_40
		*ip_L_Band_Index = L_BAND_40;
		break;
	case 41://L_BAND_41
		*ip_L_Band_Index = L_BAND_41;
		break;
	case 42://L_BAND_42
		*ip_L_Band_Index = L_BAND_42;
		break;
	case 43://L_BAND_43
		*ip_L_Band_Index = L_BAND_43;
		break;
	case 46://L_BAND_46
		*ip_L_Band_Index = L_BAND_46;
		break;
	case 47://L_BAND_47
		*ip_L_Band_Index = L_BAND_47;
		break;
	case 48://L_BAND_48
		*ip_L_Band_Index = L_BAND_48;
		break;
	case 66://L_BAND_66
		*ip_L_Band_Index = L_BAND_66;
		break;
	case 71://L_BAND_71
		*ip_L_Band_Index = L_BAND_71;
		break;
	case 72://L_BAND_72
		*ip_L_Band_Index = L_BAND_72;
		break;
	case 73://L_BAND_73
		*ip_L_Band_Index = L_BAND_73;
		break;
	case 85://L_BAND_85
		*ip_L_Band_Index = L_BAND_85;
		break;
	case 87://L_BAND_87
		*ip_L_Band_Index = L_BAND_87;
		break;
	case 88://L_BAND_88
		*ip_L_Band_Index = L_BAND_88;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_LTE_Band_Index, LTE_Band_Name(%d) is wrong!" ), i_LTE_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_LTE_Band_Name

	//pass
	return true;
}

int CGFC::Trans_UL_ARFCN_LTE( int i_L_Band_Name, int i_Arfcn_DL )
{
	//LTE的下行信道号转上行信道号
	//LTE新加频段代码位置

	int iTx_Arfcn;

	//
	switch( i_L_Band_Name )
	{
		//FDD 1~28
	case 1://L_BAND_1
	case 2://L_BAND_2
	case 3://L_BAND_3
	case 4://L_BAND_4
	case 5://L_BAND_5
	case 7://L_BAND_7
	case 8://L_BAND_8
	case 9://L_BAND_9
	case 11://L_BAND_11
	case 12://L_BAND_12
	case 13://L_BAND_13
	case 14://L_BAND_14
	case 17://L_BAND_17
	case 18://L_BAND_18
	case 19://L_BAND_19
	case 20://L_BAND_20
	case 21://L_BAND_21
	case 25://L_BAND_25
	case 26://L_BAND_26
	case 28://L_BAND_28
	case 27://L_BAND_27
		iTx_Arfcn = i_Arfcn_DL + 18000;
		break;

		//TDD 33~51
	case 34://L_BAND_34
	case 38://L_BAND_38		
	case 39://L_BAND_39
	case 40://L_BAND_40
	case 41://L_BAND_41
	case 42://L_BAND_42
	case 43://L_BAND_43
	case 48://L_BAND_48
	case 47://L_BAND_47
	case 46://L_BAND_46
		iTx_Arfcn = i_Arfcn_DL;
		break;

		//LTE的B29和B32，只有DL，是FDD，只有做CA才有用
	case 29://L_BAND_29
	case 32://L_BAND_32
		iTx_Arfcn = i_Arfcn_DL;
		break;

	case 31://L_BAND_31
		iTx_Arfcn = i_Arfcn_DL + 17890;
		break;

		//FDD 30~31
	case 30://L_BAND_30
		iTx_Arfcn = i_Arfcn_DL + 17890;
		break;

	case 66://L_BAND_66
		//注意：Band66的DL的2180-2200 MHz是配置CA时候用的
		//The range 2180-2200 MHz of the DL operating band is restricted to E-UTRA operation when carrier aggregation is configured.
		iTx_Arfcn = i_Arfcn_DL + 65536;//66频段信道以上行开始信道的差值计算(131972-66436)
		break;

	case 71://L_BAND_71
		iTx_Arfcn = i_Arfcn_DL + 64536;
		break;

	case 72://L_BAND_72
		iTx_Arfcn = i_Arfcn_DL + 64536;
		break;

	case 73://L_BAND_73
		iTx_Arfcn = i_Arfcn_DL + 64536;
		break;

	case 85://L_BAND_85
		iTx_Arfcn = i_Arfcn_DL + 63636;
		break;

	case 86://L_BAND_86, NB_BAND_86
		iTx_Arfcn = i_Arfcn_DL + 63636;
		break;

	case 87://L_BAND_87, NB_BAND_87
		iTx_Arfcn = i_Arfcn_DL + 63636;
		break;

	case 88://L_BAND_88, NB_BAND_88
		iTx_Arfcn = i_Arfcn_DL + 63635;
		break;

	default:
		this->m_csError_Info.Format( _T( "ERROR, Trans_UL_ARFCN_LTE, LTE_Band_Name(%d) is wrong!" ), i_L_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		iTx_Arfcn = 0;
		//
		break;
	}//switch( i_L_Band_Name


	//返回
	return iTx_Arfcn;
}

int CGFC::Trans_UL_ARFCN_WCDMA( int i_W_Band_Index, int i_Arfcn_DL )
{
	//把WCDMA的下行信道转上行信道
	//WCDMA新加频段代码位置

	int iTx_Arfcn;

	//WCDMA频段的index
	switch( i_W_Band_Index )
	{
	case W_BAND_1:
		iTx_Arfcn = i_Arfcn_DL - 950;
		break;
	case W_BAND_2:
		iTx_Arfcn = i_Arfcn_DL - 400;
		break;
	case W_BAND_3:
		iTx_Arfcn = i_Arfcn_DL - 225;
		break;
	case W_BAND_4:
		iTx_Arfcn = i_Arfcn_DL - 225;
		break;
	case W_BAND_5:
		iTx_Arfcn = i_Arfcn_DL - 225;
		break;
	case W_BAND_6:
		iTx_Arfcn = i_Arfcn_DL - 225;
		break;
	case W_BAND_8:
		iTx_Arfcn = i_Arfcn_DL - 225;
		break;
	case W_BAND_9:
		iTx_Arfcn = i_Arfcn_DL - 475;
		break;
	case W_BAND_19:
		iTx_Arfcn = i_Arfcn_DL - 400;
		break;
	default:
		iTx_Arfcn = 0;
		break;
	}

	//返回
	return iTx_Arfcn;
}

bool CGFC::Calcu_Center_Freq_TD( int i_TD_Band_Name, int i_ARFCN, double * dp_Center_Freq_MHz )
{
	//由 TD 信道号计算获取频点（MHz）

	try
	{
		switch( i_TD_Band_Name )
		{
		case 34://A频段（2010～2025 MHz，原B频段）：共计15MHz
			{
				*dp_Center_Freq_MHz = i_ARFCN / 5.0;
			}
			break;
		case 39:
			{
				*dp_Center_Freq_MHz = i_ARFCN / 5.0;
			}
			break;
		default:
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "Calcu_Center_Freq_TD( TD_Band = %d, ARFCN = %d ), Fail !" ), i_TD_Band_Name, i_ARFCN );
		return false;
	}

	return true;
}

bool CGFC::Calcu_ARFCN_TD( int i_TD_Band, double d_Center_Freq_MHz, int * ip_ARFCN )
{
	//由 TD 频点（MHz）计算获取信道号

	try
	{
		switch( i_TD_Band )
		{
		case 34://A频段（2010～2025 MHz，原B频段）：共计15MHz
			{
				*ip_ARFCN = d_Center_Freq_MHz * 5;
			}
			break;
		default:
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "Calcu_ARFCN_TD( TD_Band = %d, Center_Freq = %.4f ), Fail !" ), i_TD_Band, d_Center_Freq_MHz );
		return false;
	}

	return true;
}

bool CGFC::Get_TD_Band_Name( int i_TD_Band_Index, int * ip_TD_Band_Name )
{
	switch( i_TD_Band_Index )
	{
	case TD_BAND_34:
		*ip_TD_Band_Name = 34;
		break;
	case TD_BAND_39:
		*ip_TD_Band_Name = 39;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_TD_Band_Name, TD_Band_Index(%d) is wrong!" ), i_TD_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_TD_Band_Index( int i_TD_Band_Name, int * ip_TD_Band_Index )
{
	switch( i_TD_Band_Name )
	{
	case 34:
		*ip_TD_Band_Index = TD_BAND_34;
		break;
	case 39:
		*ip_TD_Band_Index = TD_BAND_39;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_TD_Band_Index, TD_Band_Name(%d) is wrong!" ), i_TD_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		return false;
	}

	return true;
}

bool CGFC::Encode_RF_Ref_Tx_Data_To_Str_V1( double d_Code_Key, int i_Band_And_Tx_Data_Quan, int * ip_Band_List, double * dp_RF_Tx_Data, CString * csp_Str_Data )
{
	//所有频段和Tx功率数据转成“字符串”，用于金板数据的写入。
	//每个频段只有一个功率

	CString csAll, csBand, csCh;
	int i, iCode;

	//清空
	csAll.Format( _T( "" ) );

	//频段循环
	for( i = 0; i < i_Band_And_Tx_Data_Quan; i++ )
	{
		//频段信息“编码”
		if( (*(ip_Band_List + i) >= 0) && (*(ip_Band_List + i) <= 25) )
		{
			//a~z
			csBand.Format( _T( "%c" ), (*(ip_Band_List + i) + 'a') );
		}
		else if( (*(ip_Band_List + i) >= 26) && (*(ip_Band_List + i) <= 51) )
		{
			//A~Z
			csBand.Format( _T( "%c" ), (*(ip_Band_List + i) - 26 + 'A') );
		}
		else
		{
			this->m_csError_Info.Format( _T( "ERROR, 频段index数据异常，终止做金板!" ) );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			return false;
		}


		//小数点后第一位左移一位；小数点后第二位四舍五入；
		iCode =(int) ((*(dp_RF_Tx_Data + i) + 0.05 - d_Code_Key) * 10.0);

		//
		if( iCode < 0 )
		{
			this->m_csError_Info.Format( _T( "ERROR, Ref数据偏小，不适合做金板!" ) );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			return false;
		}

		//
		if( iCode > 99 )
		{
			this->m_csError_Info.Format( _T( "ERROR, Ref数据偏大，不适合做金板!" ) );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			return false;
		}


		csCh.Format( _T( "%02d" ), iCode );

		csBand += csCh;

		csAll += csBand;
	}//for(i=0; i<i_Data_And_Band_Quan; i++


	//返回的字串
	csp_Str_Data->Format( _T( "%s" ), csAll );


	//pass
	return true;
}

bool CGFC::Decode_RF_Ref_Tx_Data_From_Str_V1( const wchar_t * wcp_Ref_Str_Data, double d_Code_Key, int * ip_Band_And_Tx_Data_Quan, int * ip_Band_List, double * dp_RF_Tx_Data )
{
	//从“金板字符串”中提取所有频段对应的功率数据。
	//每个频段只有一个功率

	int iLen, iBand_Quan, i;
	CString csData;

	//
	try
	{
		iLen = wcslen( wcp_Ref_Str_Data );

		//检查是否能被3整除，每个频段完整数据是3个（1+2）
		if( (iLen % 3) != 0 )
		{
			::AfxMessageBox( _T( "ERROR, 金板数据长度错误!" ) );
			throw(0);
		}

		//频段数
		iBand_Quan = iLen / 3;

		//
		for( i = 0; i < iBand_Quan; i++ )
		{
			//检查频段标识符（分隔符）
			if( (*(wcp_Ref_Str_Data + i * 3) >= 'a') && (*(wcp_Ref_Str_Data + i * 3) <= 'z') )
			{
				//a~z
				//频段号的index
				*(ip_Band_List + i) = *(wcp_Ref_Str_Data + i * 3) - 'a';
			}
			else if( (*(wcp_Ref_Str_Data + i * 3) >= 'A') && (*(wcp_Ref_Str_Data + i * 3) <= 'Z') )
			{
				//A~Z
				//频段号的index
				*(ip_Band_List + i) = *(wcp_Ref_Str_Data + i * 3) - 'A' + 26;
			}
			else
			{
				::AfxMessageBox( _T( "ERROR, 金板频段标识符数据错误!" ) );
				throw(0);//频段标识符错误
			}


			//功率信息
			csData.Format( _T( "%c%c" ), *(wcp_Ref_Str_Data + i * 3 + 1), *(wcp_Ref_Str_Data + i * 3 + 2) );
			*(dp_RF_Tx_Data + i) = _wtof( csData ) / 10.0 + d_Code_Key;
		}//for( i = 0; i < iBand_Quan; i++


		//返回数量
		*ip_Band_And_Tx_Data_Quan = iBand_Quan;
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

void CGFC::WCSTOK_STRING_T_C( const wchar_t * wcp_StrToken, const wchar_t * wcp_StrDelimit, CString * csp_Get_CString_Array, int * ip_Get_Quan )
{
	//以“特别指定的分隔符”一次性提取“全部子串”

	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//“源数据”的buff，wcstok在提取数据时会改变源数据内容，所以先把源数据放入buff，解析buff数据，不伤害参数源。
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//用于存放“源数据”的buff长度
	WCHAR * wcpResult = NULL;


	//分配内存
	this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );

	//载入“源数据”
	swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

	//提取第一个值(0)
	wcpEach = wcstok_s( wcpSrc, wcp_StrDelimit, &wcpResult );

	while( wcpEach != NULL )
	{
		//赋值
		(csp_Get_CString_Array + i)->Format( _T( "%s" ), wcpEach );

		//提取下一个值
		wcpEach = wcstok_s( NULL, wcp_StrDelimit, &wcpResult );

		//计数
		i++;
	}//while(wcpEach != NULL

	//返回数量
	*ip_Get_Quan = i;


	//释放内存
	this->BUFF_WCHAR_FREE( &wcpSrc );
}

int CGFC::Calcu_IMEI_Checksum( const wchar_t * wcp_IMEI_14_Bits )
{
	int iSum = 0;
	int irInt_Buff[16], i;
	wchar_t wcrTemp[2];

	memset( wcrTemp, 0, sizeof( wcrTemp ) );
	memset( irInt_Buff, 0, sizeof( irInt_Buff ) );

	for( i = 0; i < 14; i++ )
	{
		wcrTemp[0] = *(wcp_IMEI_14_Bits + i);
		irInt_Buff[i] = _wtoi( wcrTemp );
	}

	for( i = 0; i < 7; i++ )
	{
		iSum += irInt_Buff[2 * i + 1] * 2 / 10;
		iSum += irInt_Buff[2 * i + 1] * 2 % 10;
		iSum += irInt_Buff[2 * i];
	}

	iSum = iSum % 10;

	if( iSum != 0 )
	{
		iSum = 10 - iSum;
	}

	//返回值
	return iSum;
}

bool CGFC::Get_CDMA2000_Band_Name( int i_C2K_Band_Index, int * ip_C2K_Band_Name )
{
	switch( i_C2K_Band_Index )
	{
	case C2K_BC_0://band class 0
		*ip_C2K_Band_Name = 0;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_CDMA2000_Band_Name, cdma2000_Band_Index(%d) is wrong!" ), i_C2K_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_CDMA2000_Band_Index( int i_C2K_Band_Name, int * ip_C2K_Band_Index )
{
	switch( i_C2K_Band_Name )
	{
	case 0://band class 0
		*ip_C2K_Band_Index = C2K_BC_0;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_CDMA2000_Band_Index, cdma2000_Band_Name(%d) is wrong!" ), i_C2K_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		return false;
	}

	//pass
	return true;
}

bool CGFC::Calcu_Center_Freq_CDMA2000( int i_C2K_Band_Name, int i_ARFCN, int i_Up_Down_Link, double * dp_Center_Freq_MHz )
{
	try
	{
		switch( i_C2K_Band_Name )
		{
		case 0://band class 0
			{
				switch( i_ARFCN )
				{
				case 1013:
					{
						if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
						{
							*dp_Center_Freq_MHz = 824.7;
						}
						else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
						{
							*dp_Center_Freq_MHz = 869.7;
						}
						else
						{
							throw(0);
						}
					}
					break;
				case 384:
					{
						if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
						{
							*dp_Center_Freq_MHz = 836.52;
						}
						else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
						{
							*dp_Center_Freq_MHz = 881.52;
						}
						else
						{
							throw(0);
						}
					}
					break;
				case 777:
					{
						if( i_Up_Down_Link == RF_TX_UL_INPUT )//上行
						{
							*dp_Center_Freq_MHz = 848.31;
						}
						else if( i_Up_Down_Link == RF_RX_DL_OUTPUT )//下行
						{
							*dp_Center_Freq_MHz = 893.31;
						}
						else
						{
							throw(0);
						}
					}
					break;
				default:
					throw(0);
				}
			}
			break;
		default:
			throw(0);
		}
	}
	catch( ... )
	{
		this->m_csError_Info.Format( _T( "ERROR, Calcu_Center_Freq_CDMA2000( C2K_Band_Name = %d, ARFCN = %d, Up_Down_Link = %d )" ), i_C2K_Band_Name, i_ARFCN, i_Up_Down_Link );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_WCDMA_Band_Name( int i_W_Band_Index, int * ip_W_Band_Name )
{
	//WCDMA频段index号 转化 WCDMA频段名
	//WCDMA新加频段代码位置

	switch( i_W_Band_Index )//WCDMA频段index号
	{
	case W_BAND_1:
		*ip_W_Band_Name = 1;
		break;
	case W_BAND_2:
		*ip_W_Band_Name = 2;
		break;
	case W_BAND_3:
		*ip_W_Band_Name = 3;
		break;
	case W_BAND_4:
		*ip_W_Band_Name = 4;
		break;
	case W_BAND_5:
		*ip_W_Band_Name = 5;
		break;
	case W_BAND_6:
		*ip_W_Band_Name = 6;
		break;
	case W_BAND_7:
		*ip_W_Band_Name = 7;
		break;
	case W_BAND_8:
		*ip_W_Band_Name = 8;
		break;
	case W_BAND_9:
		*ip_W_Band_Name = 9;
		break;
	case W_BAND_10:
		*ip_W_Band_Name = 10;
		break;
	case W_BAND_19:
		*ip_W_Band_Name = 19;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_WCDMA_Band_Name, WCDMA_Band_Index(%d) is wrong!" ), i_W_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_WCDMA_Band_Index( int i_W_Band_Name, int * ip_W_Band_Index )
{
	//WCDMA频段名 转化 WCDMA频段index号
	//WCDMA新加频段代码位置

	switch( i_W_Band_Name )
	{
	case 1://W_BAND_1
		*ip_W_Band_Index = W_BAND_1;
		break;
	case 2://W_BAND_2
		*ip_W_Band_Index = W_BAND_2;
		break;
	case 3://W_BAND_3
		*ip_W_Band_Index = W_BAND_3;
		break;
	case 4://W_BAND_4
		*ip_W_Band_Index = W_BAND_4;
		break;
	case 5://W_BAND_5
		*ip_W_Band_Index = W_BAND_5;
		break;
	case 6://W_BAND_6
		*ip_W_Band_Index = W_BAND_6;
		break;
	case 7://W_BAND_7
		*ip_W_Band_Index = W_BAND_7;
		break;
	case 8://W_BAND_8
		*ip_W_Band_Index = W_BAND_8;
		break;
	case 9://W_BAND_9
		*ip_W_Band_Index = W_BAND_9;
		break;
	case 10://W_BAND_10
		*ip_W_Band_Index = W_BAND_10;
		break;
	case 19://W_BAND_19
		*ip_W_Band_Index = W_BAND_19;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_WCDMA_Band_Index, WCDMA_Band_Name(%d) is wrong!" ), i_W_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

void CGFC::Get_RF_Offset_Freq_Band_Seg( int i_Format, int i_Band_Index, double * dp_Freq_Tx_S, double * dp_Freq_Tx_E, double * dp_Freq_Rx_S, double * dp_Freq_Rx_E )
{
	//线损补偿表所用的起始频点
	//WCDMA新加频段代码位置
	//LTE新加频段代码位置

	//
	try
	{
		switch( i_Format )
		{
		case FORMAT_GSM_S:
		case FORMAT_GSM_NS:
			{
				switch( i_Band_Index )//GSM Band
				{
				case G_BAND_GSM900:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 925.0;
					*dp_Freq_Rx_E = 960.0;
					break;
				case G_BAND_DCS1800:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1785.0;
					*dp_Freq_Rx_S = 1805.0;
					*dp_Freq_Rx_E = 1880.0;
					break;
				case G_BAND_PCS1900:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1910.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1990.0;
					break;
				case G_BAND_GSM850:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_WCDMA_S:
		case FORMAT_WCDMA_NS:
			{
				switch( i_Band_Index )//WCDMA Band
				{
				case W_BAND_1:
					*dp_Freq_Tx_S = 1920.0;
					*dp_Freq_Tx_E = 1980.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2170.0;
					break;
				case W_BAND_2:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1910.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1990.0;
					break;
				case W_BAND_3:
					*dp_Freq_Tx_S = 1712.4;
					*dp_Freq_Tx_E = 1782.6;
					*dp_Freq_Rx_S = 1807.4;
					*dp_Freq_Rx_E = 1877.6;
					break;
				case W_BAND_4:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1755.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2155.0;
					break;
				case W_BAND_5:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case W_BAND_6:
					*dp_Freq_Tx_S = 832.4;
					*dp_Freq_Tx_E = 837.6;
					*dp_Freq_Rx_S = 877.4;
					*dp_Freq_Rx_E = 882.6;
					break;
				case W_BAND_8:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 925.0;
					*dp_Freq_Rx_E = 960.0;
					break;
				case W_BAND_9:
					*dp_Freq_Tx_S = 1752.4;
					*dp_Freq_Tx_E = 1782.4;
					*dp_Freq_Rx_S = 1847.4;
					*dp_Freq_Rx_E = 1877.4;
					break;
				case W_BAND_19:
					*dp_Freq_Tx_S = 832.4;
					*dp_Freq_Tx_E = 842.6;
					*dp_Freq_Rx_S = 877.4;
					*dp_Freq_Rx_E = 887.6;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_TD_NS:
			{
				switch( i_Band_Index )//TD Band
				{
				case TD_BAND_34:
					*dp_Freq_Tx_S = 2000.0;
					*dp_Freq_Tx_E = 2030.0;
					*dp_Freq_Rx_S = 2000.0;
					*dp_Freq_Rx_E = 2030.0;
					break;
				case TD_BAND_39:
					*dp_Freq_Tx_S = 1875.0;
					*dp_Freq_Tx_E = 1925.0;
					*dp_Freq_Rx_S = 1875.0;
					*dp_Freq_Rx_E = 1925.0;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_C2K_S:
		case FORMAT_C2K_NS:
			{
				switch( i_Band_Index )//cdma2000 Band
				{
				case C2K_BC_0:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				default:
					//fail
					throw(0);
				}
			}
			//
			break;

		case FORMAT_LTE_NS:
			{
				switch( i_Band_Index )//LTE Band
				{
				case L_BAND_1:
					*dp_Freq_Tx_S = 1920.0;
					*dp_Freq_Tx_E = 1980.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2170.0;
					break;
				case L_BAND_2:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1910.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1990.0;
					break;
				case L_BAND_3:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1785.0;
					*dp_Freq_Rx_S = 1805.0;
					*dp_Freq_Rx_E = 1880.0;
					break;
				case L_BAND_4:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1755.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2155.0;
					break;
				case L_BAND_5:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case L_BAND_7:
					*dp_Freq_Tx_S = 2500.0;
					*dp_Freq_Tx_E = 2571.0;
					*dp_Freq_Rx_S = 2620.0;
					*dp_Freq_Rx_E = 2691.0;
					break;
				case L_BAND_8:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 925.0;
					*dp_Freq_Rx_E = 960.0;
					break;
				case L_BAND_12:
					*dp_Freq_Tx_S = 698.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 728.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case L_BAND_17:
					*dp_Freq_Tx_S = 704.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 734.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case L_BAND_20:
					*dp_Freq_Tx_S = 832.0;
					*dp_Freq_Tx_E = 862.0;
					*dp_Freq_Rx_S = 791.0;
					*dp_Freq_Rx_E = 821.0;
					break;
				case L_BAND_38:
					*dp_Freq_Tx_S = 2570.0;
					*dp_Freq_Tx_E = 2620.0;
					*dp_Freq_Rx_S = 2570.0;
					*dp_Freq_Rx_E = 2620.0;
					break;
				case L_BAND_39:
					*dp_Freq_Tx_S = 1880.0;
					*dp_Freq_Tx_E = 1920.0;
					*dp_Freq_Rx_S = 1880.0;
					*dp_Freq_Rx_E = 1920.0;
					break;
				case L_BAND_40:
					*dp_Freq_Tx_S = 2300.0;
					*dp_Freq_Tx_E = 2400.0;
					*dp_Freq_Rx_S = 2300.0;
					*dp_Freq_Rx_E = 2400.0;
					break;
				case L_BAND_41:
					*dp_Freq_Tx_S = 2496.0;
					*dp_Freq_Tx_E = 2690.0;
					*dp_Freq_Rx_S = 2496.0;
					*dp_Freq_Rx_E = 2690.0;
					break;
				case L_BAND_13:
					*dp_Freq_Tx_S = 777.0;
					*dp_Freq_Tx_E = 787.0;
					*dp_Freq_Rx_S = 746.0;
					*dp_Freq_Rx_E = 756.0;
					break;
				case L_BAND_18:
					*dp_Freq_Tx_S = 815.0;
					*dp_Freq_Tx_E = 830.0;
					*dp_Freq_Rx_S = 860.0;
					*dp_Freq_Rx_E = 875.0;
					break;
				case L_BAND_19:
					*dp_Freq_Tx_S = 830.0;
					*dp_Freq_Tx_E = 845.0;
					*dp_Freq_Rx_S = 875.0;
					*dp_Freq_Rx_E = 890.0;
					break;
				case L_BAND_25:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1915.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1995.0;
					break;
				case L_BAND_26:
					*dp_Freq_Tx_S = 814.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 859.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case L_BAND_28:
					*dp_Freq_Tx_S = 703.0;
					*dp_Freq_Tx_E = 748.0;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 803.0;
					break;
				case L_BAND_34:
					*dp_Freq_Tx_S = 2010.0;
					*dp_Freq_Tx_E = 2025.0;
					*dp_Freq_Rx_S = 2010.0;
					*dp_Freq_Rx_E = 2025.0;
					break;
				case L_BAND_14:
					*dp_Freq_Tx_S = 788.0;
					*dp_Freq_Tx_E = 797.9;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 767.9;
					break;
				case L_BAND_29:
					*dp_Freq_Tx_S = 717.0;
					*dp_Freq_Tx_E = 727.9;
					*dp_Freq_Rx_S = 717.0;
					*dp_Freq_Rx_E = 727.9;
					break;
				case L_BAND_30:
					*dp_Freq_Tx_S = 2305.0;
					*dp_Freq_Tx_E = 2314.9;
					*dp_Freq_Rx_S = 2350.0;
					*dp_Freq_Rx_E = 2359.9;
					break;
				case L_BAND_32:
					*dp_Freq_Tx_S = 1452.0;
					*dp_Freq_Tx_E = 1495.9;
					*dp_Freq_Rx_S = 1452.0;
					*dp_Freq_Rx_E = 1495.9;
					break;
				case L_BAND_66:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1779.9;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2199.9;
					break;
				case L_BAND_71:
					*dp_Freq_Tx_S = 663.0;
					*dp_Freq_Tx_E = 697.9;
					*dp_Freq_Rx_S = 617.0;
					*dp_Freq_Rx_E = 651.9;
					break;
				case L_BAND_42:
					*dp_Freq_Tx_S = 3400.0;
					*dp_Freq_Tx_E = 3599.9;
					*dp_Freq_Rx_S = 3400.0;
					*dp_Freq_Rx_E = 3599.9;
					break;
				case L_BAND_43:
					*dp_Freq_Tx_S = 3600.0;
					*dp_Freq_Tx_E = 3799.9;
					*dp_Freq_Rx_S = 3600.0;
					*dp_Freq_Rx_E = 3799.9;
					break;
				case L_BAND_48:
					*dp_Freq_Tx_S = 3550.0;
					*dp_Freq_Tx_E = 3699.9;
					*dp_Freq_Rx_S = 3550.0;
					*dp_Freq_Rx_E = 3699.9;
					break;
				case L_BAND_9:
					*dp_Freq_Tx_S = 1749.9;
					*dp_Freq_Tx_E = 1784.8;
					*dp_Freq_Rx_S = 1844.9;
					*dp_Freq_Rx_E = 1879.8;
					break;
				case L_BAND_11:
					*dp_Freq_Tx_S = 1427.9;
					*dp_Freq_Tx_E = 1447.8;
					*dp_Freq_Rx_S = 1475.9;
					*dp_Freq_Rx_E = 1495.8;
					break;
				case L_BAND_21:
					*dp_Freq_Tx_S = 1447.9;
					*dp_Freq_Tx_E = 1462.8;
					*dp_Freq_Rx_S = 1495.9;
					*dp_Freq_Rx_E = 1510.8;
					break;
				case L_BAND_47:
					*dp_Freq_Tx_S = 5855.0;
					*dp_Freq_Tx_E = 5924.9;
					*dp_Freq_Rx_S = 5855.0;
					*dp_Freq_Rx_E = 5924.9;
					break;
				case L_BAND_46:
					*dp_Freq_Tx_S = 5150.0;
					*dp_Freq_Tx_E = 5925.0;
					*dp_Freq_Rx_S = 5150.0;
					*dp_Freq_Rx_E = 5925.0;
					break;
				case L_BAND_27:
					*dp_Freq_Tx_S = 807.0;
					*dp_Freq_Tx_E = 824.0;
					*dp_Freq_Rx_S = 852.0;
					*dp_Freq_Rx_E = 869.0;
					break;
				case L_BAND_31:
					*dp_Freq_Tx_S = 452.5;
					*dp_Freq_Tx_E = 457.4;
					*dp_Freq_Rx_S = 462.5;
					*dp_Freq_Rx_E = 467.4;
					break;
				case L_BAND_72:
					*dp_Freq_Tx_S = 451.0;
					*dp_Freq_Tx_E = 456.0;
					*dp_Freq_Rx_S = 461.0;
					*dp_Freq_Rx_E = 466.0;
					break;
				case L_BAND_73:
					*dp_Freq_Tx_S = 450.0;
					*dp_Freq_Tx_E = 455.0;
					*dp_Freq_Rx_S = 460.0;
					*dp_Freq_Rx_E = 465.0;
					break;
				case L_BAND_85:
					*dp_Freq_Tx_S = 698.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 728.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case L_BAND_87:
					*dp_Freq_Tx_S = 410.0;
					*dp_Freq_Tx_E = 414.9;
					*dp_Freq_Rx_S = 420.0;
					*dp_Freq_Rx_E = 424.9;
					break;
				case L_BAND_88:
					*dp_Freq_Tx_S = 412.0;
					*dp_Freq_Tx_E = 416.9;
					*dp_Freq_Rx_S = 422.0;
					*dp_Freq_Rx_E = 426.9;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_CV2X_NS://C-V2X
			{
				switch( i_Band_Index )//C-V2X Band
				{
				case CV2X_BAND_46:
					*dp_Freq_Tx_S = 5150.0;
					*dp_Freq_Tx_E = 5925.0;
					*dp_Freq_Rx_S = 5150.0;
					*dp_Freq_Rx_E = 5925.0;
					break;
				case CV2X_BAND_47:
					*dp_Freq_Tx_S = 5855.0;
					*dp_Freq_Tx_E = 5924.9;
					*dp_Freq_Rx_S = 5855.0;
					*dp_Freq_Rx_E = 5924.9;
					break;
				default:
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_NBIOT_S:
		case FORMAT_NBIOT_NS:
			{
				switch( i_Band_Index )//NB-IoT Band
				{
				case NB_IDX_B_1:
					*dp_Freq_Tx_S = 1920.0;
					*dp_Freq_Tx_E = 1980.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2170.0;
					break;
				case NB_IDX_B_2:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1910.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1990.0;
					break;
				case NB_IDX_B_3:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1785.0;
					*dp_Freq_Rx_S = 1805.0;
					*dp_Freq_Rx_E = 1880.0;
					break;
				case NB_IDX_B_4:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1755.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2155.0;
					break;
				case NB_IDX_B_5:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case NB_IDX_B_7:
					*dp_Freq_Tx_S = 2500.0;
					*dp_Freq_Tx_E = 2571.0;
					*dp_Freq_Rx_S = 2620.0;
					*dp_Freq_Rx_E = 2691.0;
					break;
				case NB_IDX_B_8:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 925.0;
					*dp_Freq_Rx_E = 960.0;
					break;
				case NB_IDX_B_9:
					*dp_Freq_Tx_S = 1749.9;
					*dp_Freq_Tx_E = 1784.8;
					*dp_Freq_Rx_S = 1844.9;
					*dp_Freq_Rx_E = 1879.8;
					break;
				case NB_IDX_B_10:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1770.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2170.0;
					break;
				case NB_IDX_B_11:
					*dp_Freq_Tx_S = 1427.9;
					*dp_Freq_Tx_E = 1447.8;
					*dp_Freq_Rx_S = 1475.9;
					*dp_Freq_Rx_E = 1495.8;
					break;
				case NB_IDX_B_12:
					*dp_Freq_Tx_S = 698.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 728.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case NB_IDX_B_13:
					*dp_Freq_Tx_S = 777.0;
					*dp_Freq_Tx_E = 787.0;
					*dp_Freq_Rx_S = 746.0;
					*dp_Freq_Rx_E = 756.0;
					break;
				case NB_IDX_B_14:
					*dp_Freq_Tx_S = 788.0;
					*dp_Freq_Tx_E = 797.9;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 767.9;
					break;
				case NB_IDX_B_17:
					*dp_Freq_Tx_S = 704.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 734.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case NB_IDX_B_18:
					*dp_Freq_Tx_S = 815.0;
					*dp_Freq_Tx_E = 830.0;
					*dp_Freq_Rx_S = 860.0;
					*dp_Freq_Rx_E = 875.0;
					break;
				case NB_IDX_B_19:
					*dp_Freq_Tx_S = 830.0;
					*dp_Freq_Tx_E = 845.0;
					*dp_Freq_Rx_S = 875.0;
					*dp_Freq_Rx_E = 890.0;
					break;
				case NB_IDX_B_20:
					*dp_Freq_Tx_S = 832.0;
					*dp_Freq_Tx_E = 862.0;
					*dp_Freq_Rx_S = 791.0;
					*dp_Freq_Rx_E = 821.0;
					break;
				case NB_IDX_B_21:
					*dp_Freq_Tx_S = 1447.9;
					*dp_Freq_Tx_E = 1462.8;
					*dp_Freq_Rx_S = 1495.9;
					*dp_Freq_Rx_E = 1510.8;
					break;
				case NB_IDX_B_22:
					*dp_Freq_Tx_S = 3410.0;
					*dp_Freq_Tx_E = 3490.0;
					*dp_Freq_Rx_S = 3510.0;
					*dp_Freq_Rx_E = 3590.0;
					break;
				case NB_IDX_B_24:
					*dp_Freq_Tx_S = 1626.5;
					*dp_Freq_Tx_E = 1660.4;
					*dp_Freq_Rx_S = 1525.0;
					*dp_Freq_Rx_E = 1558.9;
					break;
				case NB_IDX_B_25:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1915.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1995.0;
					break;
				case NB_IDX_B_26:
					*dp_Freq_Tx_S = 814.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 859.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case NB_IDX_B_27:
					*dp_Freq_Tx_S = 807.0;
					*dp_Freq_Tx_E = 824.0;
					*dp_Freq_Rx_S = 852.0;
					*dp_Freq_Rx_E = 869.0;
					break;
				case NB_IDX_B_28:
					*dp_Freq_Tx_S = 703.0;
					*dp_Freq_Tx_E = 748.0;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 803.0;
					break;
				case NB_IDX_B_30:
					*dp_Freq_Tx_S = 2305.0;
					*dp_Freq_Tx_E = 2314.9;
					*dp_Freq_Rx_S = 2350.0;
					*dp_Freq_Rx_E = 2359.9;
					break;
				case NB_IDX_B_31:
					*dp_Freq_Tx_S = 452.5;
					*dp_Freq_Tx_E = 457.4;
					*dp_Freq_Rx_S = 462.5;
					*dp_Freq_Rx_E = 467.4;
					break;
				case NB_IDX_B_66:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1779.9;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2199.9;
					break;
				case NB_IDX_B_68:
					*dp_Freq_Tx_S = 698.0;
					*dp_Freq_Tx_E = 728.0;
					*dp_Freq_Rx_S = 753.0;
					*dp_Freq_Rx_E = 783.0;
					break;
				case NB_IDX_B_70:
					*dp_Freq_Tx_S = 1695.0;
					*dp_Freq_Tx_E = 1710.0;
					*dp_Freq_Rx_S = 1995.0;
					*dp_Freq_Rx_E = 2020.0;
					break;
				case NB_IDX_B_71:
					*dp_Freq_Tx_S = 663.0;
					*dp_Freq_Tx_E = 697.9;
					*dp_Freq_Rx_S = 617.0;
					*dp_Freq_Rx_E = 651.9;
					break;
				case NB_IDX_B_72:
					*dp_Freq_Tx_S = 451.0;
					*dp_Freq_Tx_E = 456.0;
					*dp_Freq_Rx_S = 461.0;
					*dp_Freq_Rx_E = 466.0;
					break;
				case NB_IDX_B_73:
					*dp_Freq_Tx_S = 450.0;
					*dp_Freq_Tx_E = 455.0;
					*dp_Freq_Rx_S = 460.0;
					*dp_Freq_Rx_E = 465.0;
					break;
				case NB_IDX_B_74:
					*dp_Freq_Tx_S = 1427.0;
					*dp_Freq_Tx_E = 1470.0;
					*dp_Freq_Rx_S = 1475.0;
					*dp_Freq_Rx_E = 1518.0;
					break;
				case NB_IDX_B_85:
					*dp_Freq_Tx_S = 698.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 728.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case NB_IDX_B_86:
					*dp_Freq_Tx_S = 787.0;
					*dp_Freq_Tx_E = 788.0;//Uplink Channel:134182-134191, FREQ:787.0-787.9MHz
					*dp_Freq_Rx_S = 757.0;
					*dp_Freq_Rx_E = 758.0;//Downlink Channel:70546-70555, FREQ:757.0-757.9MHz
					break;
				case NB_IDX_B_87:
					*dp_Freq_Tx_S = 410.0;
					*dp_Freq_Tx_E = 414.9;
					*dp_Freq_Rx_S = 420.0;
					*dp_Freq_Rx_E = 424.9;
					break;
				case NB_IDX_B_88:
					*dp_Freq_Tx_S = 412.0;
					*dp_Freq_Tx_E = 416.9;
					*dp_Freq_Rx_S = 422.0;
					*dp_Freq_Rx_E = 426.9;
					break;
				default:
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_NR5GSUB6_NS:
			{
				switch( i_Band_Index )
				{
				case NR5GSUB6_N_1:
					*dp_Freq_Tx_S = 1920.0;
					*dp_Freq_Tx_E = 1980.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2170.0;
					break;
				case NR5GSUB6_N_2:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1910.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1990.0;
					break;
				case NR5GSUB6_N_3:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1785.0;
					*dp_Freq_Rx_S = 1805.0;
					*dp_Freq_Rx_E = 1880.0;
					break;
				case NR5GSUB6_N_5:
					*dp_Freq_Tx_S = 824.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 869.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case NR5GSUB6_N_7:
					*dp_Freq_Tx_S = 2500.0;
					*dp_Freq_Tx_E = 2570.0;
					*dp_Freq_Rx_S = 2620.0;
					*dp_Freq_Rx_E = 2690.0;
					break;
				case NR5GSUB6_N_8:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 925.0;
					*dp_Freq_Rx_E = 960.0;
					break;
				case NR5GSUB6_N_20:
					*dp_Freq_Tx_S = 832.0;
					*dp_Freq_Tx_E = 862.0;
					*dp_Freq_Rx_S = 791.0;
					*dp_Freq_Rx_E = 821.0;
					break;
				case NR5GSUB6_N_28:
					*dp_Freq_Tx_S = 703.0;
					*dp_Freq_Tx_E = 748.0;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 803.0;
					break;
				case NR5GSUB6_N_41:
					*dp_Freq_Tx_S = 2496.0;
					*dp_Freq_Tx_E = 2690.0;
					*dp_Freq_Rx_S = 2496.0;
					*dp_Freq_Rx_E = 2690.0;
					break;
				case NR5GSUB6_N_66:
					*dp_Freq_Tx_S = 1710.0;
					*dp_Freq_Tx_E = 1780.0;
					*dp_Freq_Rx_S = 2110.0;
					*dp_Freq_Rx_E = 2200.0;
					break;
				case NR5GSUB6_N_71:
					*dp_Freq_Tx_S = 663.0;
					*dp_Freq_Tx_E = 698.0;
					*dp_Freq_Rx_S = 617.0;
					*dp_Freq_Rx_E = 652.0;
					break;
				case NR5GSUB6_N_77:
					*dp_Freq_Tx_S = 3300.0;
					*dp_Freq_Tx_E = 4200.0;
					*dp_Freq_Rx_S = 3300.0;
					*dp_Freq_Rx_E = 4200.0;
					break;
				case NR5GSUB6_N_78:
					*dp_Freq_Tx_S = 3300.0;
					*dp_Freq_Tx_E = 3800.0;
					*dp_Freq_Rx_S = 3300.0;
					*dp_Freq_Rx_E = 3800.0;
					break;
				case NR5GSUB6_N_79:
					*dp_Freq_Tx_S = 4400.0;
					*dp_Freq_Tx_E = 5000.0;
					*dp_Freq_Rx_S = 4400.0;
					*dp_Freq_Rx_E = 5000.0;
					break;
				case NR5GSUB6_N_12:
					*dp_Freq_Tx_S = 699.0;
					*dp_Freq_Tx_E = 716.0;
					*dp_Freq_Rx_S = 729.0;
					*dp_Freq_Rx_E = 746.0;
					break;
				case NR5GSUB6_N_25:
					*dp_Freq_Tx_S = 1850.0;
					*dp_Freq_Tx_E = 1915.0;
					*dp_Freq_Rx_S = 1930.0;
					*dp_Freq_Rx_E = 1995.0;
					break;
				case NR5GSUB6_N_48:
					*dp_Freq_Tx_S = 3550.0;
					*dp_Freq_Tx_E = 3700.0;
					*dp_Freq_Rx_S = 3550.0;
					*dp_Freq_Rx_E = 3700.0;
					break;
				case NR5GSUB6_N_38:
					*dp_Freq_Tx_S = 2570.0;
					*dp_Freq_Tx_E = 2620.0;
					*dp_Freq_Rx_S = 2570.0;
					*dp_Freq_Rx_E = 2620.0;
					break;
				case NR5GSUB6_N_40:
					*dp_Freq_Tx_S = 2300.0;
					*dp_Freq_Tx_E = 2400.0;
					*dp_Freq_Rx_S = 2300.0;
					*dp_Freq_Rx_E = 2400.0;
					break;
				case NR5GSUB6_N_14:
					*dp_Freq_Tx_S = 788.0;
					*dp_Freq_Tx_E = 798.0;
					*dp_Freq_Rx_S = 758.0;
					*dp_Freq_Rx_E = 768.0;
					break;
				case NR5GSUB6_N_13:
					*dp_Freq_Tx_S = 777.0;
					*dp_Freq_Tx_E = 787.0;
					*dp_Freq_Rx_S = 746.0;
					*dp_Freq_Rx_E = 756.0;
					break;
				case NR5GSUB6_N_18:
					*dp_Freq_Tx_S = 815.0;
					*dp_Freq_Tx_E = 830.0;
					*dp_Freq_Rx_S = 860.0;
					*dp_Freq_Rx_E = 875.0;
					break;
				case NR5GSUB6_N_26:
					*dp_Freq_Tx_S = 814.0;
					*dp_Freq_Tx_E = 849.0;
					*dp_Freq_Rx_S = 859.0;
					*dp_Freq_Rx_E = 894.0;
					break;
				case NR5GSUB6_N_29:
					*dp_Freq_Tx_S = 717.0;
					*dp_Freq_Tx_E = 728.0;
					*dp_Freq_Rx_S = 717.0;
					*dp_Freq_Rx_E = 728.0;
					break;
				case NR5GSUB6_N_30:
					*dp_Freq_Tx_S = 2305.0;
					*dp_Freq_Tx_E = 2315.0;
					*dp_Freq_Rx_S = 2350.0;
					*dp_Freq_Rx_E = 2360.0;
					break;
				case NR5GSUB6_N_75:
					*dp_Freq_Tx_S = 1432.0;
					*dp_Freq_Tx_E = 1517.0;
					*dp_Freq_Rx_S = 1432.0;
					*dp_Freq_Rx_E = 1517.0;
					break;
				case NR5GSUB6_N_76:
					*dp_Freq_Tx_S = 1427.0;
					*dp_Freq_Tx_E = 1432.0;
					*dp_Freq_Rx_S = 1427.0;
					*dp_Freq_Rx_E = 1432.0;
					break;
				case NR5GSUB6_N_91:
					*dp_Freq_Tx_S = 832.0;
					*dp_Freq_Tx_E = 862.0;
					*dp_Freq_Rx_S = 1427.0;
					*dp_Freq_Rx_E = 1432.0;
					break;
				case NR5GSUB6_N_92:
					*dp_Freq_Tx_S = 832.0;
					*dp_Freq_Tx_E = 862.0;
					*dp_Freq_Rx_S = 1432.0;
					*dp_Freq_Rx_E = 1517.0;
					break;
				case NR5GSUB6_N_93:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 1427.0;
					*dp_Freq_Rx_E = 1432.0;
					break;
				case NR5GSUB6_N_94:
					*dp_Freq_Tx_S = 880.0;
					*dp_Freq_Tx_E = 915.0;
					*dp_Freq_Rx_S = 1432.0;
					*dp_Freq_Rx_E = 1517.0;
					break;
				case NR5GSUB6_N_70:
					*dp_Freq_Tx_S = 1695.0;
					*dp_Freq_Tx_E = 1710.0;
					*dp_Freq_Rx_S = 1995.0;
					*dp_Freq_Rx_E = 2020.0;
					break;
				case NR5GSUB6_N_257:
					*dp_Freq_Tx_S = 26500.0;
					*dp_Freq_Tx_E = 29499.94;
					*dp_Freq_Rx_S = 26500.0;
					*dp_Freq_Rx_E = 29499.94;
					break;
				case NR5GSUB6_N_258:
					*dp_Freq_Tx_S = 24250.0;
					*dp_Freq_Tx_E = 27499.90;
					*dp_Freq_Rx_S = 24250.0;
					*dp_Freq_Rx_E = 27499.90;
					break;
				case NR5GSUB6_N_260:
					*dp_Freq_Tx_S = 37000.0;
					*dp_Freq_Tx_E = 39999.94;
					*dp_Freq_Rx_S = 37000.0;
					*dp_Freq_Rx_E = 39999.94;
					break;
				case NR5GSUB6_N_261:
					*dp_Freq_Tx_S = 27500.0;
					*dp_Freq_Tx_E = 28349.96;
					*dp_Freq_Rx_S = 27500.0;
					*dp_Freq_Rx_E = 28349.96;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		case FORMAT_NR5GMMW_NS:
			{
				switch( i_Band_Index )
				{
				case NR5GMMW_N_257:
					*dp_Freq_Tx_S = 26500.0;
					*dp_Freq_Tx_E = 29500.0;
					*dp_Freq_Rx_S = 26500.0;
					*dp_Freq_Rx_E = 29500.0;
					break;
				case NR5GMMW_N_258:
					*dp_Freq_Tx_S = 24250.0;
					*dp_Freq_Tx_E = 27500.0;
					*dp_Freq_Rx_S = 24250.0;
					*dp_Freq_Rx_E = 27500.0;
					break;
				case NR5GMMW_N_260:
					*dp_Freq_Tx_S = 37000.0;
					*dp_Freq_Tx_E = 40000.0;
					*dp_Freq_Rx_S = 37000.0;
					*dp_Freq_Rx_E = 40000.0;
					break;
				case NR5GMMW_N_261:
					*dp_Freq_Tx_S = 27500.0;
					*dp_Freq_Tx_E = 28350.0;
					*dp_Freq_Rx_S = 27500.0;
					*dp_Freq_Rx_E = 28350.0;
					break;
				default:
					//fail
					throw(0);
				}//switch( i_Band_Index
			}
			//
			break;

		default:
			//fail
			throw(0);
		}//switch( i_Format
	}
	catch( ... )
	{
		//无数据
		::AfxMessageBox( _T( "ERROR, Get_RF_Offset_Freq_Band_Seg!" ) );
		//fail
		return;
	}

	//pass
	return;
}

bool CGFC::GET_ARFCN_L_M_H_TYPE( int i_Band, int i_Format_Network, int i_ARFCN, int * ip_LMH_TYPE )
{
	try
	{
		if( (i_Format_Network == FORMAT_GSM_S) || (i_Format_Network == FORMAT_GSM_NS) )
		{
			switch( i_Band )
			{
			case G_BAND_GSM900:
				{
					if( (975 <= i_ARFCN) && (i_ARFCN <= 1023) ) // EGSM
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (0 <= i_ARFCN) && (i_ARFCN <= 31) ) // PGSM
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (32 <= i_ARFCN) && (i_ARFCN <= 71) ) // PGSM
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (72 <= i_ARFCN) && (i_ARFCN <= 124) ) // PGSM
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;

			case G_BAND_DCS1800:
				{
					if( (512 <= i_ARFCN) && (i_ARFCN <= 636) )
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (637 <= i_ARFCN) && (i_ARFCN <= 760) )
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (761 <= i_ARFCN) && (i_ARFCN <= 885) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;

			case G_BAND_PCS1900:
				{
					if( (512 <= i_ARFCN) && (i_ARFCN <= 611) )
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (612 <= i_ARFCN) && (i_ARFCN <= 711) )
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (712 <= i_ARFCN) && (i_ARFCN <= 810) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;

			case G_BAND_GSM850:
				{
					if( (128 <= i_ARFCN) && (i_ARFCN <= 169) )
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (170 <= i_ARFCN) && (i_ARFCN <= 210) )
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (211 <= i_ARFCN) && (i_ARFCN <= 251) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;

			default:
				throw(0);
			}
		}
		else if( i_Format_Network == FORMAT_WCDMA_S )
		{
			throw(0);
		}
		else if( i_Format_Network == FORMAT_C2K_S )
		{
			switch( i_Band )
			{
			case C2K_BAND_800:
				{
					if( (1 <= i_ARFCN) && (i_ARFCN <= 341) )
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (342 <= i_ARFCN) && (i_ARFCN <= 682) )
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (683 <= i_ARFCN) && (i_ARFCN <= 1023) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else if( (1024 <= i_ARFCN) && (i_ARFCN <= 1323) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;
			case C2K_BAND_1900:
				{
					if( (0 <= i_ARFCN) && (i_ARFCN <= 400) )
					{
						*ip_LMH_TYPE = CH_LOW;
					}
					else if( (401 <= i_ARFCN) && (i_ARFCN <= 801) )
					{
						*ip_LMH_TYPE = CH_MID;
					}
					else if( (802 <= i_ARFCN) && (i_ARFCN <= 1199) )
					{
						*ip_LMH_TYPE = CH_HIGH;
					}
					else
					{
						throw(0);
					}
				}
				break;
			default:
				throw(0);
			}
		}
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

void CGFC::Adjust_From_Small_To_Large_Array_Int( int * ip_Data_List, int i_Quan )
{
	if( i_Quan <= 1 )
	{
		return;
	}


	//从小到大排序
	int a, b, c;

	for( a = 0; a < i_Quan; a++ )
	{
		for( b = a + 1; b < i_Quan; b++ )
		{
			if( *(ip_Data_List + a) > *(ip_Data_List + b) )
			{
				c = *(ip_Data_List + a);
				*(ip_Data_List + a) = *(ip_Data_List + b);
				*(ip_Data_List + b) = c;
			}
		}
	}
}

int CGFC::Rand_One_Number( int i_Range )
{
	//按“i_Range”范围产生一个随机数
	int i;

	//
	srand( (unsigned)time( NULL ) );

	//
	i = rand();
	i %= i_Range;

	//返回
	return i;
}

bool CGFC::Calcu_MEID_Check_Digit( const wchar_t * wcp_MEID_Hex, wchar_t * wcp_Checksum )
{
	//计算MEID的校验位

	if( wcslen( wcp_MEID_Hex ) != 14 )
	{
		return false;
	}

	int i, iTemp;
	int iTotal = 0;
	CString csTemp;
	wchar_t wrBits[2];


	//
	wrBits[1] = '\0';


	//将奇数位数字相加，再加上上一步算得的值
	for( i = 0; i < 14; i += 2 )
	{
		wrBits[0] = *(wcp_MEID_Hex + i);
		iTotal += wcstoul( wrBits, NULL, 16 );
	}


	//将偶数位数字分别乘以2，分别计算“个位数”和“十位数”之和。（注意是16进制数）
	for( i = 1; i < 14; i += 2 )
	{
		//偶数位数字分别乘以2
		wrBits[0] = *(wcp_MEID_Hex + i);
		iTemp = 2 * wcstoul( wrBits, NULL, 16 );

		csTemp.Format( _T( "%02X" ), iTemp );

		//分别计算“个位数”和“十位数”之和
		iTotal += wcstoul( csTemp.Mid( 0, 1 ), NULL, 16 );
		iTotal += wcstoul( csTemp.Mid( 1, 1 ), NULL, 16 );
	}


	//如果得出的数个位是0则校验位为0，否则为10(这里的10是16进制)减去个位数
	iTotal %= 16;//获取十六进制的末位数
	iTotal = (iTotal == 0) ? 0 : (16 - iTotal);


	//数字转为16进制
	*wcp_Checksum = (iTotal >= 0 && iTotal <= 9) ? (iTotal + '0') : (iTotal + 'A' - 10);

	//
	return true;
}

void CGFC::WCSTOK_STRING_T_Q( const wchar_t * wcp_StrToken, int i_Take_Quan, CString * csp_Take_CString_Array )
{
	//从“数据源”提取“特定数量”的“子串”
	//以“默认分隔符”提取

	int i = 0;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//“源数据”的buff，wcstok在提取数据时会改变源数据内容，所以先把源数据放入buff，解析buff数据，不伤害参数源。
	unsigned long ulBuff_Len = wcslen( wcp_StrToken ) + 8;//用于存放“源数据”的buff长度
	WCHAR * wcpResult = NULL;


	//清空buff
	for( i = 0; i < i_Take_Quan; i++ )
	{
		(csp_Take_CString_Array + i)->Format( _T( "" ) );
	}


	//分配内存
	this->BUFF_WCHAR_CALLOC( &wcpSrc, ulBuff_Len );


	//载入“源数据”
	swprintf( wcpSrc, ulBuff_Len, _T( "%s" ), wcp_StrToken );

	//提取第一个值(0)
	wcpEach = wcstok_s( wcpSrc, _T( " ,;\r\n" ), &wcpResult );

	//计数清零
	i = 0;

	//循环提取
	while( wcpEach != NULL )
	{
		//赋值
		(csp_Take_CString_Array + i)->Format( _T( "%s" ), wcpEach );

		//提取下一个值
		wcpEach = wcstok_s( NULL, _T( " ,;\r\n" ), &wcpResult );

		//计数
		i++;

		//
		if( i == i_Take_Quan )
		{
			break;
		}
	}//while(wcpEach != NULL


	//释放内存
	this->BUFF_WCHAR_FREE( &wcpSrc );
}

void CGFC::Adjust_From_Small_To_Large_Array_Double( double * dp_Data_List, int i_Quan )
{
	if( i_Quan <= 1 )
	{
		return;
	}


	//从小到大排序
	int a, b;
	double v;

	for( a = 0; a < i_Quan; a++ )
	{
		for( b = a + 1; b < i_Quan; b++ )
		{
			if( *(dp_Data_List + a) > *(dp_Data_List + b) )
			{
				v = *(dp_Data_List + a);
				*(dp_Data_List + a) = *(dp_Data_List + b);
				*(dp_Data_List + b) = v;
			}
		}
	}
}

bool CGFC::ReadPhysicalDriveInNTUsingSmart( CString * csp_HDD_SN )
{
	bool done = false;
	int drive = 0;
	HANDLE hPhysicalDriveIOCTL;
	wchar_t driveName[256];


	//
	try
	{
		for( drive = 0; drive < MAX_IDE_DRIVES; drive++ )
		{
			hPhysicalDriveIOCTL = INVALID_HANDLE_VALUE;

			//  Try to get a handle to PhysicalDrive IOCTL, report failure
			//  and exit if can't.
			swprintf_s( driveName, _T( "\\\\.\\PhysicalDrive%d" ), drive );


			//  Windows NT, Windows 2000, Windows Server 2003, Vista
			hPhysicalDriveIOCTL = CreateFile( driveName,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL, OPEN_EXISTING, 0, NULL );
			// if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
			//    printf ("Unable to open physical drive %d, error code: 0x%lX\n",
			//            drive, GetLastError ());

			//判定
			if( hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE )
			{
				/*
				#ifdef PRINTING_TO_CONSOLE_ALLOWED
				if (PRINT_DEBUG)
				printf ("\n%d ReadPhysicalDriveInNTUsingSmart ERROR"
				"\nCreateFile(%s) returned INVALID_HANDLE_VALUE\n"
				"Error Code %d\n",
				__LINE__, driveName, GetLastError ());
				#endif
				*/
			}
			else
			{
				GETVERSIONINPARAMS GetVersionParams;
				DWORD cbBytesReturned = 0;

				// Get the version, etc of PhysicalDrive IOCTL
				memset( (void*)& GetVersionParams, 0, sizeof( GetVersionParams ) );

				if( !DeviceIoControl( hPhysicalDriveIOCTL, SMART_GET_VERSION,
					NULL,
					0,
					&GetVersionParams,
					sizeof( GETVERSIONINPARAMS ),
					&cbBytesReturned,
					NULL ) )
				{
					/*
					#ifdef PRINTING_TO_CONSOLE_ALLOWED
					if (PRINT_DEBUG)
					{
					DWORD err = GetLastError ();
					printf ("\n%d ReadPhysicalDriveInNTUsingSmart ERROR"
					"\nDeviceIoControl(%d, SMART_GET_VERSION) returned 0, error is %d\n",
					__LINE__, (int) hPhysicalDriveIOCTL, (int) err);
					}
					#endif
					*/
				}
				else
				{
					// Print the SMART version
					// PrintVersion (& GetVersionParams);
					// Allocate the command buffer
					ULONG CommandSize = sizeof( SENDCMDINPARAMS ) + IDENTIFY_BUFFER_SIZE;
					PSENDCMDINPARAMS Command = (PSENDCMDINPARAMS)malloc( CommandSize );
					// Retrieve the IDENTIFY data
					// Prepare the command

					//
					Command->irDriveRegs.bCommandReg = ID_CMD;//#define ID_CMD 0xEC// Returns ID sector for ATA
					DWORD BytesReturned = 0;

					//
					if( !DeviceIoControl( hPhysicalDriveIOCTL,
						SMART_RCV_DRIVE_DATA, Command, sizeof( SENDCMDINPARAMS ),
						Command, CommandSize,
						&BytesReturned, NULL ) )
					{
						// Print the error
						//PrintError ("SMART_RCV_DRIVE_DATA IOCTL", GetLastError());
					}
					else
					{
						// Print the IDENTIFY data
						DWORD diskdata[256];

						USHORT *pIdSector = (USHORT *)
							(PIDENTIFY_DATA)((PSENDCMDOUTPARAMS)Command)->bBuffer;

						for( int ijk = 0; ijk < 256; ijk++ )
						{
							diskdata[ijk] = pIdSector[ijk];
						}

						//提取各数据
						this->PrintIdeInfo( drive, diskdata, csp_HDD_SN );

						//统一大写
						csp_HDD_SN->MakeUpper();

						//pass
						done = true;
					}


					//free
					CloseHandle( hPhysicalDriveIOCTL );
					free( Command );


					//
					if( done == true )
					{
						//pass
						break;
					}
				}
			}
		}//for(drive = 0; drive < MAX_IDE_DRIVES; drive++

	}
	catch( ... )
	{
		//fail
		return false;
	}

	//返回
	return done;
}

void CGFC::PrintIdeInfo( int drive, DWORD * diskdata, CString * csp_HDD_SN )
{
	char serialNumber[1024];
	char modelNumber[1024];
	char revisionNumber[1024];
	char bufferSize[32];

	__int64 sectors = 0;
	__int64 bytes = 0;

	wchar_t wcrData[512];
	int iTemp;


	//  copy the hard drive serial number to the buffer
	this->ConvertToString( diskdata, 10, 19, serialNumber );
	this->ConvertToString( diskdata, 27, 46, modelNumber );
	this->ConvertToString( diskdata, 23, 26, revisionNumber );
	sprintf_s( bufferSize, "%u", diskdata[21] * 512 );


	//
	this->MultiByte_To_WideChar( serialNumber, wcrData, &iTemp );
	csp_HDD_SN->Format( _T( "%s" ), wcrData );


	//去掉“空格”
	csp_HDD_SN->Remove( _T( ' ' ) );
}

void CGFC::ConvertToString( DWORD * diskdata, int firstIndex, int lastIndex, char * buf )
{
	int index = 0;
	int position = 0;

	//  each integer has two characters stored in it backwards
	for( index = firstIndex; index <= lastIndex; index++ )
	{
		//  get high byte for 1st character
		buf[position++] = (char)(diskdata[index] / 256);

		//  get low byte for 2nd character
		buf[position++] = (char)(diskdata[index] % 256);
	}

	//  end the string 
	buf[position] = '\0';

	//  cut off the trailing blanks
	for( index = position - 1; index > 0 && isspace( buf[index] ); index-- )
		buf[index] = '\0';
}

int CGFC::HEX_WCHAR_TO_SIGNED_INT32( const wchar_t * wcp_HEX_WCHAR )
{
	//把8位16进制（32位二进制，4个字节）转换为“有符号int”。
	//输入的HEX字串无需“0x”开头。
	//同一个“二进制 原码”可以解读为“有符号”，也可解读为“无符号”。
	//实际就是把数据最高位字节（4位）的左边第一位的“1”解读为“正负号标识”还是解读为“数值1”的过程。

	//先检查HEX字串
	if( wcslen( wcp_HEX_WCHAR ) <= 0 )
	{
		return 0;
	}


	//
	unsigned long i32;
	wchar_t wcrHex_Data[64];	//放“重新组建的”16进制数据
	wchar_t wcrBinary[128];		//放二进制字符串
	int iRtn_Signed_Int = 0;
	int iRatio = 1;
	int iLen, i;



	//16进制值转换“无符号长整型”
	//注意：为了防止数据丢失（防止丢失第一位标志位数据“1”），必须转为“无符号长整型”，否者只能转换4位16进制
	i32 = wcstoul( wcp_HEX_WCHAR, NULL, 16 );


	//重新组建16进制字符串，可以自动去掉“16进制字符串前的0”
	swprintf_s( wcrHex_Data, _T( "%X" ), i32 );


	//判断二进制首位是否是“1”，1则为负数
	if( wcrHex_Data[0] > '8' )//8 = 1000
	{
		//负数		

		//
		memset( wcrBinary, 0, sizeof( wcrBinary ) );

		//输出为二进制字符串
		_ui64tow_s( i32, wcrBinary, 128, 2 );

		//二进制位数
		iLen = wcslen( wcrBinary );


		//除第一位外“取反”
		for( i = 1; i < iLen; i++ )
		{
			wcrBinary[i] = '0' + !(wcrBinary[i] - '0');
		}


		//二进制转十进制，然后加一（i=0是标志位，不列入计算）
		for( i = (iLen - 1); i > 0; i-- )
		{
			iRtn_Signed_Int = iRtn_Signed_Int + (wcrBinary[i] - '0') * iRatio;
			iRatio = iRatio * 2;
		}
		//
		iRtn_Signed_Int++;//加一

		//负数
		iRtn_Signed_Int = iRtn_Signed_Int * (-1);
	}
	else
	{
		//正数
		iRtn_Signed_Int = i32;
	}


	//返回结果
	return iRtn_Signed_Int;



	/*另一种实现的代码，但有问题，有个负数无法转换
	CString csData;
	int iSigned_Int32_Value;


	//防止丢失第一位标志位数据“1”，所以要先转化为“无符号长整型”
	unsigned long ul32bits = wcstoul(wcp_HEX_WCHAR, NULL, 16);

	//转为“带正负符号的十进制字符串”
	csData.Format(_T("%d"), (signed long)ul32bits);

	//字符串转为“有符号整型数”
	iSigned_Int32_Value = _wtoi(csData);

	//返回
	return iSigned_Int32_Value;
	*/

}

unsigned long CGFC::HEX_WCHAR_TO_UNSIGNED_INT32( const wchar_t * wcp_HEX_WCHAR )
{
	//把8位16进制（32位二进制，4个字节）转换为“无符号int”
	//默认hex数据全是正数，无符号

	unsigned long ul32bits = wcstoul( wcp_HEX_WCHAR, NULL, 16 );

	//返回
	return ul32bits;
}

bool CGFC::Encode_RF_Ref_Tx_Data_To_Str_V2( double d_Code_Key, double * dp_RF_Tx_Data, int i_RF_Tx_Quan, int * ip_Band_List, int i_Band_Quan, CString * csp_Str_Data )
{
	//所有频段和Tx功率数据转成“字符串”，用于金板数据的写入。
	//每个频段有“低中高3个功率”

	CString csAll, csBand, csCh;
	int i, j, iCode;


	//
	try
	{

		//清空完整频段信息
		csAll.Format( _T( "" ) );


		//频段循环
		for( i = 0; i < i_Band_Quan; i++ )
		{
			//频段信息“编码”
			if( (*(ip_Band_List + i) >= 0) && (*(ip_Band_List + i) <= 25) )
			{
				//a~z
				csBand.Format( _T( "%c" ), (*(ip_Band_List + i) + 'a') );
			}
			else if( (*(ip_Band_List + i) >= 26) && (*(ip_Band_List + i) <= 51) )
			{
				//A~Z
				csBand.Format( _T( "%c" ), (*(ip_Band_List + i) - 26 + 'A') );
			}
			else
			{
				this->m_csError_Info.Format( _T( "ERROR, 频段index数据异常，终止做金板!" ) );
				::AfxMessageBox( this->m_csError_Info );
				//fail
				throw(0);
			}


			//低中高3个功率循环
			for( j = 0; j < 3; j++ )
			{
				//小数点后第一位左移一位；小数点后第二位四舍五入；
				iCode = (int)((*(dp_RF_Tx_Data + i * 3 + j) + 0.05 - d_Code_Key) * 10.0);

				//
				if( iCode < 0 )
				{
					this->m_csError_Info.Format( _T( "ERROR, Ref数据偏小，不适合做金板!" ) );
					::AfxMessageBox( this->m_csError_Info );
					throw(0);
				}

				if( iCode > 99 )
				{
					this->m_csError_Info.Format( _T( "ERROR, Ref数据偏大，不适合做金板!" ) );
					::AfxMessageBox( this->m_csError_Info );
					throw(0);
				}


				//单个功率信息“编码”
				csCh.Format( _T( "%02d" ), iCode );

				//添加单个功率信息“编码”
				csBand += csCh;
			}//for(j=0; j<3; j++


			//添加单个完整频段信息“编码”
			csAll += csBand;
		}//for(i=0; i<i_Band_Quan; i++


		//返回的字串
		csp_Str_Data->Format( _T( "%s" ), csAll );
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Decode_RF_Ref_Tx_Data_From_Str_V2( const wchar_t * wcp_Ref_Str_Data, double d_Code_Key, int * ip_Band_List, int * ip_Band_Quan, int * ip_RF_Tx_Data_Quan, double * dp_RF_Tx_Data )
{
	//从“金板字符串”中提取所有频段对应的功率数据。
	//每个频段有“低中高3个功率”

	int iLen, iBand_Quan, i, iPwr_Quan, j;
	CString csData;

	//
	try
	{
		iLen = wcslen( wcp_Ref_Str_Data );

		//检查长度（频段号 + 2位低信道 + 2位中信道 + 2位高信道）
		if( (iLen % 7) != 0 )
		{
			::AfxMessageBox( _T( "ERROR, 金板数据长度错误!" ) );
			throw(0);
		}

		//频段数量
		iBand_Quan = iLen / 7;

		//功率数量
		iPwr_Quan = 0;


		//
		for( i = 0; i < iBand_Quan; i++ )//频段循环
		{
			//检查频段标识符（分隔符）
			if( (*(wcp_Ref_Str_Data + i * 7) >= 'a') && (*(wcp_Ref_Str_Data + i * 7) <= 'z') )
			{
				//a~z
				//频段号的index
				*(ip_Band_List + i) = *(wcp_Ref_Str_Data + i * 7) - 'a';
			}
			else if( (*(wcp_Ref_Str_Data + i * 7) >= 'A') && (*(wcp_Ref_Str_Data + i * 7) <= 'Z') )
			{
				//A~Z
				//频段号的index
				*(ip_Band_List + i) = *(wcp_Ref_Str_Data + i * 7) - 'A' + 26;
			}
			else
			{
				::AfxMessageBox( _T( "ERROR, 金板频段标识符数据错误!" ) );
				throw(0);//频段标识符错误
			}


			//功率信息
			for( j = 0; j < 3; j++ )//低中高3个功率循环
			{
				csData.Format( _T( "%c%c" ), *(wcp_Ref_Str_Data + i * 7 + j * 2 + 1), *(wcp_Ref_Str_Data + i * 7 + j * 2 + 2) );
				*(dp_RF_Tx_Data + iPwr_Quan) = _wtof( csData ) / 10.0 + d_Code_Key;
				//
				iPwr_Quan++;
			}
		}//for( i = 0; i < iBand_Quan; i++


		//返回数量
		*ip_Band_Quan = iBand_Quan;
		*ip_RF_Tx_Data_Quan = iPwr_Quan;
	}
	catch( ... )
	{
		return false;
	}

	//
	return true;
}

bool CGFC::Get_Data_In_Quotation_Marks( const wchar_t * wcp_Src_Data, CString * csp_Get_Data )
{
	//在“数据源”中提取“两个引号之间的字符数据”

	int iLen = wcslen( wcp_Src_Data );//源数据长度
	wchar_t * wcpSrc_Buff = NULL;
	CString csGet_Buff;
	int i;
	bool bGet_First = false;
	bool bGet_Second = false;
	unsigned long ulBuff_Len = wcslen( wcp_Src_Data ) + 8;//用于存放“源数据”的buff长度


	//
	try
	{
		//清空提取BUFF
		csGet_Buff.Format( _T( "" ) );


		//分配内存
		this->BUFF_WCHAR_CALLOC( &wcpSrc_Buff, ulBuff_Len );

		//加载源数据
		swprintf( wcpSrc_Buff, ulBuff_Len, _T( "%s" ), wcp_Src_Data );


		//循环搜索“引号”
		for( i = 0; i < iLen; i++ )
		{
			if( bGet_First == false )
			{
				//找第一个引号
				if( *(wcpSrc_Buff + i) == '"' )
				{
					bGet_First = true;
				}
			}
			else
			{
				//找第二个引号
				if( *(wcpSrc_Buff + i) == '"' )
				{
					//成功找到了第二个引号，可以结尾了
					bGet_Second = true;
					//pass
					break;//退出for
				}
				else
				{
					//两个引号间的数据
					csGet_Buff += *(wcpSrc_Buff + i);
				}
			}
		}//for(i=0; i<iLen; i++


		//判断“最后结果”
		if( (bGet_First == true) && (bGet_Second == true) )
		{
			csp_Get_Data->Format( _T( "%s" ), csGet_Buff );
		}
		else
		{
			throw(0);
		}


		//释放内存
		this->BUFF_WCHAR_FREE( &wcpSrc_Buff );
	}
	catch( ... )
	{
		//释放内存
		this->BUFF_WCHAR_FREE( &wcpSrc_Buff );
		//
		return false;
	}

	//
	return true;
}

void CGFC::Get_PF_Name( int i_PF_Index, CString * csp_PF_Name )
{
	//芯片平台的index号 转化为芯片平台名

	switch( i_PF_Index )
	{
	case PF_MTK:		//MTK
		csp_PF_Name->Format( _T( "MTK" ) );
		break;
	case PF_QUALCOMM:	//高通
		csp_PF_Name->Format( _T( "QUALCOMM" ) );
		break;
	case PF_RDA:		//RDA
		csp_PF_Name->Format( _T( "RDA" ) );
		break;
	case PF_GCT:		//GCT
		csp_PF_Name->Format( _T( "GCT" ) );
		break;
	case PF_INTEL:		//Intel
		csp_PF_Name->Format( _T( "INTEL" ) );
		break;
	case PF_HUAWEI://海思
		csp_PF_Name->Format( _T( "HISILICON" ) );
		break;
	case PF_ASR:		//ASR
		csp_PF_Name->Format( _T( "ASR" ) );
		break;
	case PF_REALTEK:		//Realtek
		csp_PF_Name->Format( _T( "Realtek" ) );
		break;
	case PF_SPREADTRUM:	//展锐 Spreadtrum
		csp_PF_Name->Format( _T( "Spreadtrum" ) );
		break;
	case PF_EIGENCOMM:	//移芯 Eigencomm
		csp_PF_Name->Format( _T( "Eigencomm" ) );
		break;
	case PF_TI:	//德州仪器 Texas Instruments
		csp_PF_Name->Format( _T( "TI" ) );
		break;
	case PF_XINYI:		//芯翼信息科技(上海)有限公司
		csp_PF_Name->Format( _T( "XINYI" ) );
		break;
	case PF_ALTAIR:		//Sony Altair
		csp_PF_Name->Format( _T( "Altair" ) );
		break;
	case PF_BEKEN:		//博通集成电路（上海）股份有限公司
		csp_PF_Name->Format( _T( "BEKEN" ) );
		break;
	case PF_CYPRESS:		//赛普拉斯 Cypress 半导体公司
		csp_PF_Name->Format( _T( "Cypress" ) );
		break;
	case PF_SILICON_LABS:	//芯科科技(Silicon Labs)
		csp_PF_Name->Format( _T( "Silicon_Labs" ) );
		break;
	case PF_AMLOGIC:		//Amlogic 晶晨半导体(上海)股份有限公司
		csp_PF_Name->Format( _T( "Amlogic" ) );
		break;
	default:
		csp_PF_Name->Format( _T( "NDF" ) );
		break;
	}//switch( i_PF_Index
}

void CGFC::Get_BB_Name( int i_BB_Index, CString * csp_BB_Name )
{
	//BB芯片的index号 转化为BB芯片名

	switch( i_BB_Index )
	{
	case BB_MDM6200:	//高通 UC20 系列
		csp_BB_Name->Format( _T( "MDM6200" ) );
		break;
	case BB_QSC6270:	//高通 UC15 系列
		csp_BB_Name->Format( _T( "QSC6270" ) );
		break;
	case BB_XMM6255:	//Intel UG95 系列
		csp_BB_Name->Format( _T( "XMM6255" ) );
		break;
	case BB_XMM6250:	//Intel UG96 系列
		csp_BB_Name->Format( _T( "XMM6250" ) );
		break;
	case BB_MT6261:		//MTK
		csp_BB_Name->Format( _T( "MT6261" ) );
		break;
	case BB_MDM9x15:	//高通
		csp_BB_Name->Format( _T( "MDM9x15" ) );
		break;
	case BB_MSM8909:	//高通4G芯片
		csp_BB_Name->Format( _T( "MSM8909" ) );
		break;
	case BB_MDM9x07:	//高通
		csp_BB_Name->Format( _T( "MDM9x07" ) );
		break;
	case BB_RDA8851:	//RDA
		csp_BB_Name->Format( _T( "RDA8851" ) );
		break;
	case BB_MSM8937:	//高通4G芯片
		csp_BB_Name->Format( _T( "MSM8937" ) );
		break;
	case BB_MSM8953:	//高通4G芯片
		csp_BB_Name->Format( _T( "MSM8953" ) );
		break;
	case BB_AR6003G:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "AR6003G" ) );
		break;
	case BB_QCA1023:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "QCA1023" ) );
		break;
	case BB_MDM9206:	//高通4G芯片
		csp_BB_Name->Format( _T( "MDM9206" ) );
		break;
	case BB_MDM9628:	//高通4G车载芯片
		csp_BB_Name->Format( _T( "MDM9628" ) );
		break;
	case BB_HI2110:		//海思Hi2110 NB-IoT芯片
		csp_BB_Name->Format( _T( "HI2110" ) );
		break;
	case BB_MSM8917:	//高通4G芯片
		csp_BB_Name->Format( _T( "MSM8917" ) );
		break;
	case BB_MDM9x40:	//高通4G芯片 CAT-6
		csp_BB_Name->Format( _T( "MDM9x40" ) );
		break;
	case BB_SDM450:		//高通SDM450
		csp_BB_Name->Format( _T( "SDM450" ) );
		break;
	case BB_WCN3610:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "WCN3610" ) );
		break;
	case BB_WCN3660:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "WCN3660" ) );
		break;
	case BB_WCN3680:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "WCN3680" ) );
		break;
	case BB_MSM8905:	//高通4G芯片
		csp_BB_Name->Format( _T( "MSM8905" ) );
		break;
	case BB_MT2625:		//MTK NB-IoT芯片
		csp_BB_Name->Format( _T( "MT2625" ) );
		break;
	case BB_MT2503:		//MTK 2G+GPS芯片
		csp_BB_Name->Format( _T( "MT2503" ) );
		break;
	case BB_SDX20:		//高通4G芯片
		csp_BB_Name->Format( _T( "SDX20" ) );
		break;
	case BB_RDA8955L:	//RDA的2G芯片
		csp_BB_Name->Format( _T( "RDA8955L" ) );
		break;
	case BB_ASR1802S:	//ASR的4G芯片
		csp_BB_Name->Format( _T( "ASR1802S" ) );
		break;
	case BB_MDM9150:	//高通芯片 只有C-V2X
		csp_BB_Name->Format( _T( "MDM9150" ) );
		break;
	case BB_QCA6564:	//高通WiFi芯片(车规平台)
		csp_BB_Name->Format( _T( "QCA6564" ) );
		break;
	case BB_SDM660:		//高通4G芯片
		csp_BB_Name->Format( _T( "SDM660" ) );
		break;
	case BB_RDA8908A:	//RDA的NB-IoT芯片
		csp_BB_Name->Format( _T( "RDA8908A" ) );
		break;
	case BB_RDA8909B:	//RDA的2G+NB-IoT芯片
		csp_BB_Name->Format( _T( "RDA8909B" ) );
		break;
	case BB_MT2621:		//MTK的2G+NB-IoT芯片
		csp_BB_Name->Format( _T( "MT2621" ) );
		break;
	case BB_SA415M:		//高通车规4G+V2X基线
		csp_BB_Name->Format( _T( "SA415M" ) );
		break;
	case BB_SDX24:		//高通4G芯片, A7 at 1.5GHz, 256KB L2, 应用于笔电, LTE-A/车载
		csp_BB_Name->Format( _T( "SDX24" ) );
		break;
	case BB_MDM9205:	//高通4G芯片, LTE Cat-M / Cat-NB
		csp_BB_Name->Format( _T( "MDM9205" ) );
		break;
	case BB_SDX55:		//高通 5G 芯片, 工规
		csp_BB_Name->Format( _T( "SDX55" ) );
		break;
	case BB_RTL8189:	//Realtek平台WiFi芯片
		csp_BB_Name->Format( _T( "RTL8189" ) );
		break;
	case BB_QM215:		//高通4G智能模块(替代MSM8909)
		csp_BB_Name->Format( _T( "QM215" ) );
		break;
	case BB_QCM2150:	//高通4G智能模块(替代MSM8909)
		csp_BB_Name->Format( _T( "QCM2150" ) );
		break;
	case BB_SA515M:		//高通 5G NR 芯片, SA515M是车规平台, 对应的工规SDX55平台
		csp_BB_Name->Format( _T( "SA515M" ) );
		break;
	case BB_WCN3990:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "WCN3990" ) );
		break;
	case BB_ASR3601:	//ASR的ASR1802S低成本4G芯片
		csp_BB_Name->Format( _T( "ASR3601" ) );
		break;
	case BB_SL8541E:	//展锐(Spreadtrum)4G智能模块芯片
		csp_BB_Name->Format( _T( "SL8541E" ) );
		break;
	case BB_HI2152:		//海思HI2152 4G非智能模块芯片
		csp_BB_Name->Format( _T( "HI2152" ) );
		break;
	case BB_QCA6391:	//高通 WiFi 蓝牙 芯片
		csp_BB_Name->Format( _T( "QCA6391" ) );
		break;
	case BB_EC616:		//移芯 超低功耗NB-IoT单模芯片
		csp_BB_Name->Format( _T( "EC616" ) );
		break;
	case BB_ASR1601:	//ASR的4G芯片 CAT1
		csp_BB_Name->Format( _T( "ASR1601" ) );
		break;
	case BB_CC1200:		//TI 低功耗高性能射频收发器
		csp_BB_Name->Format( _T( "CC1200" ) );
		break;
	case BB_HI9500:		//海思 5G 芯片
		csp_BB_Name->Format( _T( "HI9500" ) );
		break;
	case BB_ASR1802SL:	//ASR的4G芯片的Linux版本
		csp_BB_Name->Format( _T( "ASR1802SL" ) );
		break;
	case BB_ASR1826:	//ASR的4G芯片, LTE CAT-6
		csp_BB_Name->Format( _T( "ASR1826" ) );
		break;
	case BB_HI2115:		//海思Hi2115 NB-IoT芯片
		csp_BB_Name->Format( _T( "HI2115" ) );
		break;
	case BB_QCA6696:	//高通车规级WiFi/BT芯片（支持802.11 ax WiFi, 支持蓝牙 BT 5.1）
		csp_BB_Name->Format( _T( "QCA6696" ) );
		break;
	case BB_SDA845:		//高通系统级芯片(SoC: System-on-a-Chip), SDA不带modem, 即没有蜂窝移动网络
		csp_BB_Name->Format( _T( "SDA845" ) );
		break;
	case BB_UDX710:		//展锐 Spreadtrum 5G平台 工规
		csp_BB_Name->Format( _T( "UDX710" ) );
		break;
	case BB_QCX212:		//高通 NB-IoT SoC chipset, Category NB2, 2-HARQ
		csp_BB_Name->Format( _T( "QCX212" ) );
		break;
	case BB_SM6115:		//高通 4G 芯片, Snapdragon 662
		csp_BB_Name->Format( _T( "SM6115" ) );
		break;
	case BB_QTM527:		//高通 5G mmWave antenna module
		csp_BB_Name->Format( _T( "QTM527" ) );
		break;
	case BB_EC617:		//上海移芯的超低成本、超低功耗和超高集成度NB-IoT SoC 芯片，完全支持3GPP Rel14 NB-IoT 标准
		csp_BB_Name->Format( _T( "EC617" ) );
		break;
	case BB_WCN3980:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "WCN3980" ) );
		break;
	case BB_ESP8285:	//乐鑫WiFi芯片
		csp_BB_Name->Format( _T( "ESP8285" ) );
		break;
	case BB_UIS8910DM:	//展锐 advanced system-on-chip solution integrated CAT1bis/GSM/GPRS modems
		csp_BB_Name->Format( _T( "UIS8910DM" ) );
		break;
	case BB_UIS8581:	//展锐 智能模块芯片 with embedded TD-LTE, LTE FDD, TDSCDMA/HSPA(+), WCDMA/HSPA(+) and GSM/GPRS/EDGE to supportDual SIM Dual Standbyup to LTE+LTE
		csp_BB_Name->Format( _T( "UIS8581" ) );
		break;
	case BB_XY1100:		//芯翼 适用于低速 IoT 场景的高性能，高集成度的 SOC 芯片。该芯片完全兼容 3GPP TS 36.211 协议
		csp_BB_Name->Format( _T( "XY1100" ) );
		break;
	case BB_HI2120:		//海思Hi2120 NB2 NB-IoT芯片
		csp_BB_Name->Format( _T( "HI2120" ) );
		break;
	case BB_MT2731:		//MTK 2G/3G/4G 车规平台芯片
		csp_BB_Name->Format( _T( "MT2731" ) );
		break;
	case BB_WCN3988:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "WCN3988" ) );
		break;
	case BB_ALT1250:	//Sony Altair eMTC (CAT M), NB IoT (CAT NB1 / NB2)
		csp_BB_Name->Format( _T( "ALT1250" ) );
		break;
	case BB_QCA6595:	//高通 MU-MIMO WiFi/BT 5.1 Automotive Dual-MAC Combo Chip for Automotive Infotainment and Telematics applications
		csp_BB_Name->Format( _T( "QCA6595" ) );
		break;
	case BB_ASR1603:	//ASR的4G芯片 CAT-1, 1601 + Flash 封装
		csp_BB_Name->Format( _T( "ASR1603" ) );
		break;
	case BB_QCA1064:	//高通 WiFi 蓝牙 芯片
		csp_BB_Name->Format( _T( "QCA1064" ) );
		break;
	case BB_MT6880:		//MTK 5G 芯片, T700 套片
		csp_BB_Name->Format( _T( "MT6880" ) );
		break;
	case BB_MT6890:		//MTK 5G 芯片, T750 套片
		csp_BB_Name->Format( _T( "MT6890" ) );
		break;
	case BB_WCN3615:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "WCN3615" ) );
		break;
	case BB_EC616S:		//上海移芯 超低成本、超低功耗和超高集成度 NB-IoT SoC 芯片, 完全支持3GPP Rel14 NB-IoT 标准
		csp_BB_Name->Format( _T( "EC616S" ) );
		break;
	case BB_QCM6125:	//高通 智能模块 Qualcomm Snapdragon 600 series processor and LTE modem. LTE Cat12/Cat13 (FDD and TDD)
		csp_BB_Name->Format( _T( "QCM6125" ) );
		break;
	case BB_UIS8811:	//展锐 advanced system-on-chip solution integrated NB-IoT modems. The UIS8811 integrates RF transceiver for low cost.
		csp_BB_Name->Format( _T( "UIS8811" ) );
		break;
	case BB_SDX12:		//高通4G芯片, 高通MDM9x50 cost down 平台, Sixthgeneration LTE multimode modem, 14nm system on chip (SoC)
		csp_BB_Name->Format( _T( "SDX12" ) );
		break;
	case BB_QCM2290:	//高通 智能模块 4G芯片
		csp_BB_Name->Format( _T( "QCM2290" ) );
		break;
	case BB_BK7231:		//集成蓝牙双模5.1和WiFi802.11n的 WiFi SOC 芯片
		csp_BB_Name->Format( _T( "BK7231" ) );
		break;
	case BB_SDX62:		//高通 5G 芯片, 工规, SDX55的下一代, Provides Higher Performance mmWave and Features for Global Deployment
		csp_BB_Name->Format( _T( "SDX62" ) );
		break;
	case BB_QCA2064:	//高通 highly integrated System-On-Chip (SoC) supporting 802.11ax Wi-Fi and Bluetooth (BT) Milan. It is part of the QCA206x family of SoC’s supporting operation on 2.4 GHz and 5 GHz.
		csp_BB_Name->Format( _T( "QCA2064" ) );
		break;
	case BB_SM4350:		//高通 5G 智能芯片, the new generation Qualcomm Snapdragon 400 series processor with integrated 5G modem.
		csp_BB_Name->Format( _T( "SM4350" ) );
		break;
	case BB_ASR1803S:	//ASR的4G芯片 CAT-4, ASR1802S Costdown 芯片, RTOS版本
		csp_BB_Name->Format( _T( "ASR1803S" ) );
		break;
	case BB_MT2735:		//MTK 5G 芯片, supporting NR Sub6, LTE Cat19 4x4 MIMO
		csp_BB_Name->Format( _T( "MT2735" ) );
		break;
	case BB_WCN3991:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "WCN3991" ) );
		break;
	case BB_SL8521E:	//展锐(Spreadtrum)4G智能模块芯片, 智能模块系统是Linux, Android4.4/Linux
		csp_BB_Name->Format( _T( "SL8521E" ) );
		break;
	case BB_QCM6490:	//高通 5G Smart 车载智能模块, 包含5G版本, 用于车机和Tbox整合的一体机. including global multi-gigabit 5G mmW/Sub-6 GHz and WiFi 6E
		csp_BB_Name->Format( _T( "QCM6490" ) );
		break;
	case BB_QCA9377:	//高通 WLAN/Bluetooth 芯片, a single-die wireless local area network (WLAN) and Bluetooth combination solution to support 1 × 1 802.11a/b/g/n/ac WLAN standards and Bluetooth 5.0 + HS, designed to deliver superior integration of WLAN/Bluetooth and low-energy technology.
		csp_BB_Name->Format( _T( "QCA9377" ) );
		break;
	case BB_CYW54591:	//The Cypress CYW54591 is a complete dual-band (2.4 GHz and 5 GHz) 5G Wi-Fi 2×2 MIMO MAC/PHY/Radio system-on-Chip.
		csp_BB_Name->Format( _T( "CYW54591" ) );
		break;
	case BB_ASR1803E:	//ASR的4G芯片 CAT-4, ASR1802S Costdown 芯片, Open Linux版本
		csp_BB_Name->Format( _T( "ASR1803E" ) );
		break;
	case BB_ASR1803L:	//ASR的4G芯片 CAT-4, ASR1802S Costdown 芯片, 支持"Open Linux版本" or "RTOS版本"
		csp_BB_Name->Format( _T( "ASR1803L" ) );
		break;
	case BB_WCN3998:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "WCN3998" ) );
		break;
	case BB_QCM4290:	//高通4G平台, The QCM4290 includes the next generation of the Qualcomm Snapdragon 600 series processor and LTE modem.
		csp_BB_Name->Format( _T( "QCM4290" ) );
		break;
	case BB_QCX315:		//高通5G平台, The QCX315 device is a 5G modem from Qualcomm Technologies, Inc. (QTI). It implements the 5G NR standard for sub-6 GHz bands.
		csp_BB_Name->Format( _T( "QCX315" ) );
		break;
	case BB_QCS8250:	//高通 QCS8250 is the new generation Qualcomm? Snapdragon? premium-tier processor with robotic application.
		csp_BB_Name->Format( _T( "QCS8250" ) );
		break;
	case BB_CYW43455:	//The Cypress CYW43455 single-chip device provides the highest level of integration for Internet of Things applications and handheld wireless system with integrated single-stream IEEE 802.11ac MAC/baseband/radio and, Bluetooth 5.0.
		csp_BB_Name->Format( _T( "CYW43455" ) );
		break;
	case BB_UIS8310AM:	//展锐 UIS8310AM is a highly integrated application processor with embedded TDD-LTE, FDD-LTE, WCDMA and GSM/GPRS/EDGE modem and LPDDR2. The specially optimized architecture of UIS8310 can achieve high performance and low power for a lot of applications.
		csp_BB_Name->Format( _T( "UIS8310AM" ) );
		break;
	case BB_SDX65:		//高通 5G 芯片, 工规, The SDX65/SDX62 device is a multimode modem chip, supporting 2G/3G/4G/5G technologies. 4nm premium-tier 5G modem, 3G/4G/5G modem –mmWave (mmW) and sub-6 GHz bands (3GPP Release 16 integrated modem).
		csp_BB_Name->Format( _T( "SDX65" ) );
		break;
	case BB_UWE5623DU:	//展锐 WiFi/蓝牙 芯片, UWE5623DU is a high intergrated 2-in-1 connectivity single chip, this chip includes 2.4GHz and 5GHz WLAN IEEE 802.11 a/b/g/n/ac 2x2 MU-MIMO 20/40/80 MHz VHT R2 MAC/PHY/Radio, Bluetooth 5.1.
		csp_BB_Name->Format( _T( "UWE5623DU" ) );
		break;
	case BB_EC618:		//上海移芯, Eigencomm, Cat1芯片. EC618 is a Cat.1bis SoC chipset with ultra-low cost, ultra-low power and ultra-high integration, it is fully compliant to 3GPP Rel14 Cat.1bis standard，and compatible with 4G commercial networks.
		csp_BB_Name->Format( _T( "EC618" ) );
		break;
	case BB_EFR32BG21:	//芯科科技(Silicon Labs), 蓝牙+LoRa的芯片, EFR32BG21 Blue Gecko Wireless SoC Family.
		csp_BB_Name->Format( _T( "EFR32BG21" ) );
		break;
	case BB_CYW43438:	//The Cypress CYW43438L is a highly integrated single-chip solution and offers the lowest RBOM in the industry for smartphones, tablets, and a wide range of other portable devices.
		csp_BB_Name->Format( _T( "CYW43438" ) );
		break;
	case BB_UIS8850DG:	//展锐 UIS8850DG is a highly integrated system on chip solution with embedded LTE Cat1bis modems, GNSS modem, RF transceiver, power management unit(PMU), Trust zone and Multi-Function Crypto Engine(CE).
		csp_BB_Name->Format( _T( "UIS8850DG" ) );
		break;
	case BB_CYW8X072:	//The Cypress CYW8X072 is a monolithic, single-chip, Bluetooth 4.2 compliant, stand-alone baseband processor with an integrated 2.4 GHz transceiver.
		csp_BB_Name->Format( _T( "CYW8X072" ) );
		break;
	case BB_WCN3950:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "WCN3950" ) );
		break;
	case BB_W155S1:		//Amlogic W155S1 is an integrated Wi-Fi and Bluetooth combo chip. WiFi&BT芯片, 支持802.11_a/b/g/n/ac@WLAN2.4G&5G, BT5.0（BLE、BDR/EDR）.
		csp_BB_Name->Format( _T( "W155S1" ) );
		break;
	case BB_CYW43439:	//The Cypress CYW43439 is a highly integrated single-chip solution and offers the lowest RBOM in the industry for smartphones,tablets, and a wide range of other portable devices.
		csp_BB_Name->Format( _T( "CYW43439" ) );
		break;
	case BB_CB0201:		//海思 CB0201 是一款海思平台低功耗的NB-IOT芯片，支持3GPP R13/R14/R15 NB-IoT（支持Rel-16升级），同时支持BLE/GNSS。
		csp_BB_Name->Format( _T( "CB0201" ) );
		break;
	case BB_ASR1606C:	//ASR的LTE CAT1/M芯片, ASR1606 is a highly cost-efficient System on Chip (SoC) device.（4M RAM+ 4M ROM）
		csp_BB_Name->Format( _T( "ASR1606C" ) );
		break;
	case BB_ASR1606S:	//ASR的LTE CAT1/M芯片, ASR1606 is a highly cost-efficient System on Chip (SoC) device.（8M RAM+ 8M ROM）
		csp_BB_Name->Format( _T( "ASR1606S" ) );
		break;
	case BB_WCN6856:	//高通WiFi芯片
		csp_BB_Name->Format( _T( "WCN6856" ) );
		break;
	case BB_QCX216:		//高通 compact, low power, low-cost LTE Cat.1bis chip, WiFi2.4GHz Rx.
		csp_BB_Name->Format( _T( "QCX216" ) );
		break;
	case BB_QCA206x:	//高通 The Qualcomm QCA206x family consist of System-On-Chips (SoC), Bluetooth Milan and Wi-Fi supported, supporting simultaneous operation on 2.4 GHz, 5 GHz, and 6GHz*, also known as Dual Band Simultaneous (DBS). Part Number(s): QCA2065, QCA2066, QCA2062, QCA2064.
		csp_BB_Name->Format( _T( "QCA206x" ) );
		break;
	case BB_ASR1828:	//ASR的4G芯片, LTE CAT-6, 1826的迭代产品.
		csp_BB_Name->Format( _T( "ASR1828" ) );
		break;
	default:
		csp_BB_Name->Format( _T( "NDF" ) );
		break;
	}//switch( i_BB_Index
}

void CGFC::Get_Produ_Type_Name( int i_Produ_Type_Index, CString * csp_Produ_Type_Name )
{
	//产品类型的index号 转化为产品类型名

	switch( i_Produ_Type_Index )
	{
	case PRODU_TYPE_PCBA:		//PCBA
		csp_Produ_Type_Name->Format( _T( "PCBA" ) );
		break;
	case PRODU_TYPE_MINIPCIE:	//MINI-PCIE
		csp_Produ_Type_Name->Format( _T( "MINI-PCIE" ) );
		break;
	case PRODU_TYPE_TE_A:		//TE-A
		csp_Produ_Type_Name->Format( _T( "TE-A" ) );
		break;
	case PRODU_TYPE_TE_C:		//TE-C
		csp_Produ_Type_Name->Format( _T( "TE-C" ) );
		break;
	case PRODU_TYPE_TE_B:		//TE-B
		csp_Produ_Type_Name->Format( _T( "TE-B" ) );
		break;
	case PRODU_TYPE_KIT:		//KIT
		csp_Produ_Type_Name->Format( _T( "KIT" ) );
		break;
	case PRODU_TYPE_MOB:		//Module On Board
		csp_Produ_Type_Name->Format( _T( "MOB" ) );
		break;
	case PRODU_TYPE_COB:		//Chip On Board
		csp_Produ_Type_Name->Format( _T( "COB" ) );
		break;
	case PRODU_TYPE_ASM:		//Assembly 整机
		csp_Produ_Type_Name->Format( _T( "ASM" ) );
		break;
	default:
		csp_Produ_Type_Name->Format( _T( "NDF" ) );
		break;
	}//switch( i_Produ_Type_Index
}

void CGFC::Get_Produ_Ver_Name( int i_Produ_Ver_Index, CString * csp_Produ_Ver_Name )
{
	//产品版本的index号 转化为产品版本名

	switch( i_Produ_Ver_Index )
	{
	case PRODU_VER_STD://标准版
		csp_Produ_Ver_Name->Format( _T( "STD" ) );
		break;
	case PRODU_VER_GW://国网版
		csp_Produ_Ver_Name->Format( _T( "GW" ) );
		break;
	case PRODU_VER_NW://
		csp_Produ_Ver_Name->Format( _T( "NW" ) );
		break;
	case PRODU_VER_ING://
		csp_Produ_Ver_Name->Format( _T( "ING" ) );
		break;
	case PRODU_VER_ZJXD://
		csp_Produ_Ver_Name->Format( _T( "ZJXD" ) );
		break;
	case PRODU_VER_ZJ01://
		csp_Produ_Ver_Name->Format( _T( "ZJ01" ) );
		break;
	case PRODU_VER_HIK://
		csp_Produ_Ver_Name->Format( _T( "HIK" ) );
		break;
	case PRODU_VER_KT://
		csp_Produ_Ver_Name->Format( _T( "KT" ) );
		break;
	case PRODU_VER_SKT://
		csp_Produ_Ver_Name->Format( _T( "SKT" ) );
		break;
	case PRODU_VER_CM://
		csp_Produ_Ver_Name->Format( _T( "CM" ) );
		break;
	default:
		csp_Produ_Ver_Name->Format( _T( "NDF" ) );
		break;
	}
}

long CGFC::HEX_WCHAR_TO_LONG_INT( const wchar_t * wcp_HEX_WCHAR )
{
	//16进制字符数据转长整形（long）

	wchar_t wcrData[128];
	wchar_t * d;
	long lInt_Data;

	//
	memset( wcrData, 0, sizeof( wcrData ) );

	//
	swprintf_s( wcrData, _T( "0X%s" ), wcp_HEX_WCHAR );

	//转换值
	lInt_Data = wcstol( wcrData, &d, 16 );

	//返回
	return lInt_Data;
}

bool CGFC::Calcu_Center_Freq_WiFi( int i_WiFi_CH, double * dp_Center_Freq_MHz )
{
	if( i_WiFi_CH > 14 )//5G
	{
		*dp_Center_Freq_MHz = 5 * i_WiFi_CH + 5000.0;
	}
	else if( (i_WiFi_CH >= 1) && (i_WiFi_CH <= 14) )//2.4G
	{
		*dp_Center_Freq_MHz = 5 * (i_WiFi_CH - 1) + 2412.0;
	}
	else
	{
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_WiFi_FreqBand_Name( int i_WiFi_FB_Index, CString * csp_WiFi_FB_Name )
{
	switch( i_WiFi_FB_Index )
	{
	case WIFI_FB_B24G://2.4G
		csp_WiFi_FB_Name->Format( _T( "2.4G" ) );
		break;
	case WIFI_FB_B5G://5G
		csp_WiFi_FB_Name->Format( _T( "5G" ) );
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_WiFi_FreqBand_Name, WiFi_FreqBand_Index(%d) is wrong!" ), i_WiFi_FB_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_WiFi_Standard_Name( int i_WiFi_STAN_Index, CString * csp_WiFi_STAN_Name )
{
	//提取WiFi标准名
	switch( i_WiFi_STAN_Index )
	{
	case WIFI_STAN_80211A:
		csp_WiFi_STAN_Name->Format( _T( "802.11a" ) );
		break;
	case WIFI_STAN_80211B:
		csp_WiFi_STAN_Name->Format( _T( "802.11b" ) );
		break;
	case WIFI_STAN_80211G:
		csp_WiFi_STAN_Name->Format( _T( "802.11g" ) );
		break;
	case WIFI_STAN_80211N_20M:
		csp_WiFi_STAN_Name->Format( _T( "802.11n.20M" ) );
		break;
	case WIFI_STAN_80211N_40M:
		csp_WiFi_STAN_Name->Format( _T( "802.11n.40M" ) );
		break;
	case WIFI_STAN_80211N_5G_20M:
		csp_WiFi_STAN_Name->Format( _T( "802.11n.5G.20M" ) );
		break;
	case WIFI_STAN_80211AC_20M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ac.20M" ) );
		break;
	case WIFI_STAN_80211AC_40M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ac.40M" ) );
		break;
	case WIFI_STAN_80211AC_80M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ac.80M" ) );
		break;
	case WIFI_STAN_80211AX_20M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ax.20M" ) );
		break;
	case WIFI_STAN_80211AX_40M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ax.40M" ) );
		break;
	case WIFI_STAN_80211AX_5G_20M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ax.5G.20M" ) );
		break;
	case WIFI_STAN_80211AX_5G_40M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ax.5G.40M" ) );
		break;
	case WIFI_STAN_80211AX_5G_80M:
		csp_WiFi_STAN_Name->Format( _T( "802.11ax.5G.80M" ) );
		break;
	case WIFI_STAN_80211N_5G_40M:
		csp_WiFi_STAN_Name->Format( _T( "802.11n.5G.40M" ) );
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_WiFi_Standard_Name, WiFi_Standard_Index(%d) is wrong!" ), i_WiFi_STAN_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_WiFi_STAN_Index

	//pass
	return true;
}

bool CGFC::Calcu_Center_Freq_Bluetooth( int i_BLU_VER_Index, int i_BLU_CH, double * dp_Center_Freq_MHz )
{
	switch( i_BLU_VER_Index )
	{
	case BLU_VER_10_BR:
	case BLU_VER_20_EDR://带宽1M
		{
			if( (i_BLU_CH >= 0) && (i_BLU_CH <= 78) )
			{
				*dp_Center_Freq_MHz = 2402.0 + i_BLU_CH * 1.0;
			}
			else
			{
				this->m_csError_Info.Format( _T( "ERROR, Calcu_Center_Freq_Bluetooth, BLU_CH(%d) is wrong!" ), i_BLU_CH );
				::AfxMessageBox( this->m_csError_Info );
				//fail
				return false;
			}
		}
		break;

	case BLU_VER_40_BLE://带宽2M
		{
			if( (i_BLU_CH >= 0) && (i_BLU_CH <= 39) )
			{
				*dp_Center_Freq_MHz = 2402.0 + i_BLU_CH * 2.0;
			}
			else
			{
				this->m_csError_Info.Format( _T( "ERROR, Calcu_Center_Freq_Bluetooth, BLU_CH(%d) is wrong!" ), i_BLU_CH );
				::AfxMessageBox( this->m_csError_Info );
				//fail
				return false;
			}
		}
		break;

	default:
		this->m_csError_Info.Format( _T( "ERROR, Calcu_Center_Freq_Bluetooth, BLU_VER_Index(%d) is wrong!" ), i_BLU_VER_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_Bluetooth_Version_Name( int i_BLU_VER_Index, CString * csp_BLU_VER_Name )
{
	switch( i_BLU_VER_Index )
	{
	case BLU_VER_10_BR:
		csp_BLU_VER_Name->Format( _T( "BR" ) );
		break;
	case BLU_VER_20_EDR:
		csp_BLU_VER_Name->Format( _T( "EDR" ) );
		break;
	case BLU_VER_40_BLE:
		csp_BLU_VER_Name->Format( _T( "BLE" ) );
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_Bluetooth_Version_Name, BLU_VER_Index(%d) is wrong!" ), i_BLU_VER_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}
bool CGFC::Get_Local_PC_IP_Name(CString * csp_PC_IP)
{
	WORD wVersionRequested;
	WSADATA wsaData ;
	char crData[64];
	char *crIpData=NULL;
	wchar_t wcrTemp[64];

	memset(crData, 0, sizeof(crData));
	memset(wcrTemp, 0, sizeof(wcrTemp));

	//Get Computer Name
	wVersionRequested = MAKEWORD(2, 0);
	WSAStartup(wVersionRequested, &wsaData);

	if(::gethostname(crData, 60) != 0)
	{
		memset(crData, 0, sizeof(crData));
	}
	if(strlen(crData) > 0)
	{
		struct hostent *p=::gethostbyname(crData);
		if(p!=NULL)
		{
			struct in_addr in;
			for(int i=0;p->h_addr_list[i]!=0;i++)
			{
				memcpy(&in,p->h_addr_list[i],sizeof(struct in_addr));
				crIpData=inet_ntoa(in);
				this->MultiByte_To_WideChar(crIpData, wcrTemp, &i);
				csp_PC_IP->Format(_T("%s"), wcrTemp);
				WSACleanup();
				return true;
			}
		}
		else
		{
			WSACleanup();
			return false;
		}

	}
	else
	{
		WSACleanup();
		return false;
	}
	return false;
}

bool CGFC::Get_FileName_From_FilePath( const wchar_t * wcp_File_Path, bool b_Need_Extend_Name, CString * csp_File_Name )
{
	int iWhere;
	CString csFile_Path, csFileTitle;


	//
	try
	{
		//加载文件路径
		csFile_Path.Format( _T( "%s" ), wcp_File_Path );

		//
		iWhere = csFile_Path.ReverseFind( '\\' );
		//判断
		if( iWhere == -1 )//–1 if the character is not found
		{
			//fail
			throw(0);
		}


		//提取文件名
		csFileTitle = csFile_Path.Right( csFile_Path.GetLength() - 1 - iWhere );


		//去掉扩展名
		if( b_Need_Extend_Name == false )
		{
			iWhere = csFileTitle.ReverseFind( '.' );
			//
			if( iWhere != -1 )
			{
				//发现有扩展名，去掉扩展名
				csFileTitle = csFileTitle.Left( iWhere );
			}
		}


		//返回数据
		csp_File_Name->Format( _T( "%s" ), csFileTitle );

	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}


void CGFC::WCSTOK_STRING_S_C_Ex( const wchar_t * wcp_StrToken, wchar_t wc_StrDelimit, CString * csp_Get_CString_Array, int * ip_Get_Quan )
{
	//此函数会对“头尾分隔符”和“两个连续分隔符”间的空数据也会计入统计，作为一个“空”字符串返回

	int i_Index;
	CString csStrToken;			//源数据临时存放
	CStringArray csaTempArray;	//目标字符集临时存放
	CString csTemp, csStrDelimit;


	//1、载入“源数据”
	csStrToken.Format( _T( "%s" ), wcp_StrToken );
	csStrDelimit.Format( _T( "%c" ), wc_StrDelimit );

	//2、确定有多少个参数
	*ip_Get_Quan = csStrToken.Replace( csStrDelimit, csStrDelimit ) + (csStrToken.IsEmpty() ? 0 : 1);

	//3、清空临时Array
	csaTempArray.RemoveAll();

	//4、解析数据
	while( TRUE )//若源数据为“空”也可正常退出循环
	{
		i_Index = csStrToken.Find( csStrDelimit );
		//
		if( i_Index == -1 )
		{
			//最后一个数据了
			csaTempArray.Add( csStrToken );
			//退出while
			break;
		}

		csTemp = csStrToken.Left( i_Index );

		csaTempArray.Add( csTemp );

		csStrToken = csStrToken.Right( csStrToken.GetLength() - i_Index - 1 );
	}

	//5、临时赋值到目标
	for( int i = 0; i < *ip_Get_Quan; i++ )
	{
		(csp_Get_CString_Array + i)->Format( _T( "%s" ), csaTempArray.GetAt( i ) );
	}
}


bool CGFC::Get_WiFi_Standard_FreqBand( int i_WiFi_STAN_Index, int * ip_WiFi_FB_Index )
{
	//提取WiFi标准的WiFi频段
	switch( i_WiFi_STAN_Index )
	{
		//2.4G
	case WIFI_STAN_80211B:
	case WIFI_STAN_80211G:
	case WIFI_STAN_80211N_20M:
	case WIFI_STAN_80211N_40M:
	case WIFI_STAN_80211AX_20M:
	case WIFI_STAN_80211AX_40M:
		*ip_WiFi_FB_Index = WIFI_FB_B24G;
		//
		break;

		//5G
	case WIFI_STAN_80211A:
	case WIFI_STAN_80211N_5G_20M:
	case WIFI_STAN_80211N_5G_40M:
	case WIFI_STAN_80211AC_20M:
	case WIFI_STAN_80211AC_40M:
	case WIFI_STAN_80211AC_80M:
	case WIFI_STAN_80211AX_5G_20M:
	case WIFI_STAN_80211AX_5G_40M:
	case WIFI_STAN_80211AX_5G_80M:
		*ip_WiFi_FB_Index = WIFI_FB_B5G;
		//
		break;

	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_WiFi_Standard_FreqBand, WiFi_Standard_Index(%d) is wrong!" ), i_WiFi_STAN_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_LTE_Category_Name( int i_LTE_CAT_Index, CString * csp_LTE_CAT_Name )
{
	//提取 LTE_CAT 字符型名称
	switch( i_LTE_CAT_Index )
	{
	case LTE_CAT_NDF:
		csp_LTE_CAT_Name->Format( _T( "CAT-NDF" ) );
		break;
	case LTE_CAT_0:
		csp_LTE_CAT_Name->Format( _T( "CAT-0" ) );
		break;
	case LTE_CAT_1:
		csp_LTE_CAT_Name->Format( _T( "CAT-1" ) );
		break;
	case LTE_CAT_2:
		csp_LTE_CAT_Name->Format( _T( "CAT-2" ) );
		break;
	case LTE_CAT_3:
		csp_LTE_CAT_Name->Format( _T( "CAT-3" ) );
		break;
	case LTE_CAT_4:
		csp_LTE_CAT_Name->Format( _T( "CAT-4" ) );
		break;
	case LTE_CAT_5:
		csp_LTE_CAT_Name->Format( _T( "CAT-5" ) );
		break;
	case LTE_CAT_6:
		csp_LTE_CAT_Name->Format( _T( "CAT-6" ) );
		break;
	case LTE_CAT_7:
		csp_LTE_CAT_Name->Format( _T( "CAT-7" ) );
		break;
	case LTE_CAT_8:
		csp_LTE_CAT_Name->Format( _T( "CAT-8" ) );
		break;
	case LTE_CAT_9:
		csp_LTE_CAT_Name->Format( _T( "CAT-9" ) );
		break;
	case LTE_CAT_10:
		csp_LTE_CAT_Name->Format( _T( "CAT-10" ) );
		break;
	case LTE_CAT_M:
		csp_LTE_CAT_Name->Format( _T( "CAT-M" ) );
		break;
	case LTE_CAT_NB:
		csp_LTE_CAT_Name->Format( _T( "CAT-NB" ) );
		break;
	case LTE_CAT_11:
		csp_LTE_CAT_Name->Format( _T( "CAT-11" ) );
		break;
	case LTE_CAT_12:
		csp_LTE_CAT_Name->Format( _T( "CAT-12" ) );
		break;
	case LTE_CAT_18:
		csp_LTE_CAT_Name->Format( _T( "CAT-18" ) );
		break;
	case LTE_CAT_16:
		csp_LTE_CAT_Name->Format( _T( "CAT-16" ) );
		break;
	case LTE_CAT_20:
		csp_LTE_CAT_Name->Format( _T( "CAT-20" ) );
		break;
	case LTE_CAT_19:
		csp_LTE_CAT_Name->Format( _T( "CAT-19" ) );
		break;
	case LTE_CAT_15:
		csp_LTE_CAT_Name->Format( _T( "CAT-15" ) );
		break;
	case LTE_CAT_13:
		csp_LTE_CAT_Name->Format( _T( "CAT-13" ) );
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_LTE_Category_Name, LTE_CAT_Index(%d) is wrong!" ), i_LTE_CAT_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_LTE_CAT_Index

	//pass
	return true;
}


bool CGFC::Get_GSM_Band_Name( int i_G_Band_Index, CString * csp_G_Band_Name )
{
	switch( i_G_Band_Index )
	{
	case G_BAND_GSM900:
		csp_G_Band_Name->Format( _T( "GSM900" ) );
		break;
	case G_BAND_DCS1800:
		csp_G_Band_Name->Format( _T( "DCS1800" ) );
		break;
	case G_BAND_PCS1900:
		csp_G_Band_Name->Format( _T( "PCS1900" ) );
		break;
	case G_BAND_GSM850:
		csp_G_Band_Name->Format( _T( "GSM850" ) );
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_GSM_Band_Name, GSM_Band_Index(%d) is wrong!" ), i_G_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}


int CGFC::Get_Process_Attributes_Index( const CString cs_Process_Attributes_Name )
{
	CString csProcess_Attributes_Name;


	//输入流程属性数据统一大写
	csProcess_Attributes_Name.Format( _T( "%s" ), cs_Process_Attributes_Name );
	csProcess_Attributes_Name.MakeUpper();


	//识别流程属性数据
	if( csProcess_Attributes_Name.Find( _T( "ALL_TEST" ) ) != -1 )//全检流程
	{
		return PROCESS_ATT_ALL_TEST;
	}
	else if( csProcess_Attributes_Name.Find( _T( "ONLY_LABEL" ) ) != -1 )//只打标签
	{
		return PROCESS_ATT_ONLY_LABEL;
	}
	else if( csProcess_Attributes_Name.Find( _T( "TO_DL" ) ) != -1 )//只做到DL
	{
		return PROCESS_ATT_TO_DL;
	}
	else if( csProcess_Attributes_Name.Find( _T( "TO_BT" ) ) != -1 )//只做到BT
	{
		return PROCESS_ATT_TO_BT;
	}
	else if( csProcess_Attributes_Name.Find( _T( "TO_FT" ) ) != -1 )//只做到FT
	{
		return PROCESS_ATT_TO_FT;
	}
	else if( csProcess_Attributes_Name.Find( _T( "TO_FCT" ) ) != -1 )//只做到FCT
	{
		return PROCESS_ATT_TO_FCT;
	}
	else
	{
	}

	//fail info
	this->m_csError_Info.Format( _T( "ERROR, Get_Process_Attributes_Index, \"%s\" is wrong!" ), csProcess_Attributes_Name );
	::AfxMessageBox( this->m_csError_Info );
	//fail
	return PROCESS_ATT_ERROR;
}


bool CGFC::Get_Process_Attributes_Name( const int i_Process_Attributes_Index, CString & csref_Process_Attributes )
{
	switch( i_Process_Attributes_Index )
	{
	case PROCESS_ATT_ERROR:
		csref_Process_Attributes.Format( _T( "ERROR" ) );
		break;
	case PROCESS_ATT_ALL_TEST:
		csref_Process_Attributes.Format( _T( "All_Test" ) );
		break;
	case PROCESS_ATT_ONLY_LABEL:
		csref_Process_Attributes.Format( _T( "Only_Label" ) );
		break;
	case PROCESS_ATT_TO_DL:
		csref_Process_Attributes.Format( _T( "To_DL" ) );
		break;
	case PROCESS_ATT_TO_BT:
		csref_Process_Attributes.Format( _T( "To_BT" ) );
		break;
	case PROCESS_ATT_TO_FT:
		csref_Process_Attributes.Format( _T( "To_FT" ) );
		break;
	case PROCESS_ATT_TO_FCT:
		csref_Process_Attributes.Format( _T( "To_FCT" ) );
		break;
	default:
		csref_Process_Attributes.Format( _T( "ERROR" ) );
		//fail info
		this->m_csError_Info.Format( _T( "ERROR, Get_Process_Attributes_Name, Process_Attributes_Index(%d) is wrong!" ), i_Process_Attributes_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NBIoT_Band_Index( int i_NBIoT_Band_Name, int * ip_NBIoT_Band_Index )
{
	//NB-IoT频段名 转化 NB-IoT频段index号
	//NB-IoT新加频段代码位置

	//
	switch( i_NBIoT_Band_Name )
	{
	case NB_BAND_1://NB_IDX_B_1
		*ip_NBIoT_Band_Index = NB_IDX_B_1;
		break;
	case NB_BAND_2://NB_IDX_B_2
		*ip_NBIoT_Band_Index = NB_IDX_B_2;
		break;
	case NB_BAND_3://NB_IDX_B_3
		*ip_NBIoT_Band_Index = NB_IDX_B_3;
		break;
	case NB_BAND_4://NB_IDX_B_4
		*ip_NBIoT_Band_Index = NB_IDX_B_4;
		break;
	case NB_BAND_5://NB_IDX_B_5
		*ip_NBIoT_Band_Index = NB_IDX_B_5;
		break;
	case NB_BAND_7://NB_IDX_B_7
		*ip_NBIoT_Band_Index = NB_IDX_B_7;
		break;
	case NB_BAND_8://NB_IDX_B_8
		*ip_NBIoT_Band_Index = NB_IDX_B_8;
		break;
	case NB_BAND_9://NB_IDX_B_9
		*ip_NBIoT_Band_Index = NB_IDX_B_9;
		break;
	case NB_BAND_10://NB_IDX_B_10
		*ip_NBIoT_Band_Index = NB_IDX_B_10;
		break;
	case NB_BAND_11://NB_IDX_B_11
		*ip_NBIoT_Band_Index = NB_IDX_B_11;
		break;
	case NB_BAND_12://NB_IDX_B_12
		*ip_NBIoT_Band_Index = NB_IDX_B_12;
		break;
	case NB_BAND_13://NB_IDX_B_13
		*ip_NBIoT_Band_Index = NB_IDX_B_13;
		break;
	case NB_BAND_14://NB_IDX_B_14
		*ip_NBIoT_Band_Index = NB_IDX_B_14;
		break;
	case NB_BAND_17://NB_IDX_B_17
		*ip_NBIoT_Band_Index = NB_IDX_B_17;
		break;
	case NB_BAND_18://NB_IDX_B_18
		*ip_NBIoT_Band_Index = NB_IDX_B_18;
		break;
	case NB_BAND_19://NB_IDX_B_19
		*ip_NBIoT_Band_Index = NB_IDX_B_19;
		break;
	case NB_BAND_20://NB_IDX_B_20
		*ip_NBIoT_Band_Index = NB_IDX_B_20;
		break;
	case NB_BAND_21://NB_IDX_B_21
		*ip_NBIoT_Band_Index = NB_IDX_B_21;
		break;
	case NB_BAND_22://NB_IDX_B_22
		*ip_NBIoT_Band_Index = NB_IDX_B_22;
		break;
	case NB_BAND_24://NB_IDX_B_24
		*ip_NBIoT_Band_Index = NB_IDX_B_24;
		break;
	case NB_BAND_25://NB_IDX_B_25
		*ip_NBIoT_Band_Index = NB_IDX_B_25;
		break;
	case NB_BAND_26://NB_IDX_B_26
		*ip_NBIoT_Band_Index = NB_IDX_B_26;
		break;
	case NB_BAND_27://NB_IDX_B_27
		*ip_NBIoT_Band_Index = NB_IDX_B_27;
		break;
	case NB_BAND_28://NB_IDX_B_28
		*ip_NBIoT_Band_Index = NB_IDX_B_28;
		break;
	case NB_BAND_30://NB_IDX_B_30
		*ip_NBIoT_Band_Index = NB_IDX_B_30;
		break;
	case NB_BAND_31://NB_IDX_B_31
		*ip_NBIoT_Band_Index = NB_IDX_B_31;
		break;
	case NB_BAND_66://NB_IDX_B_66
		*ip_NBIoT_Band_Index = NB_IDX_B_66;
		break;
	case NB_BAND_68://NB_IDX_B_68
		*ip_NBIoT_Band_Index = NB_IDX_B_68;
		break;
	case NB_BAND_70://NB_IDX_B_70
		*ip_NBIoT_Band_Index = NB_IDX_B_70;
		break;
	case NB_BAND_71://NB_IDX_B_71
		*ip_NBIoT_Band_Index = NB_IDX_B_71;
		break;
	case NB_BAND_72://NB_IDX_B_72
		*ip_NBIoT_Band_Index = NB_IDX_B_72;
		break;
	case NB_BAND_73://NB_IDX_B_73
		*ip_NBIoT_Band_Index = NB_IDX_B_73;
		break;
	case NB_BAND_74://NB_IDX_B_74
		*ip_NBIoT_Band_Index = NB_IDX_B_74;
		break;
	case NB_BAND_85://NB_IDX_B_85
		*ip_NBIoT_Band_Index = NB_IDX_B_85;
		break;
	case NB_BAND_86://NB_IDX_B_86
		*ip_NBIoT_Band_Index = NB_IDX_B_86;
		break;
	case NB_BAND_87://NB_IDX_B_87
		*ip_NBIoT_Band_Index = NB_IDX_B_87;
		break;
	case NB_BAND_88://NB_IDX_B_88
		*ip_NBIoT_Band_Index = NB_IDX_B_88;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_NBIoT_Band_Index, NB-IoT_Band_Name(%d) is wrong!" ), i_NBIoT_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_NBIoT_Band_Name

	//pass
	return true;
}


bool CGFC::Get_NBIoT_Band_Name( int i_NBIoT_Band_Index, int * ip_NBIoT_Band_Name )
{
	//NB-IoT频段index号 转化 NB-IoT频段名
	//NB-IoT新加频段代码位置

	//
	switch( i_NBIoT_Band_Index )
	{
	case NB_IDX_B_1:
		*ip_NBIoT_Band_Name = NB_BAND_1;
		break;
	case NB_IDX_B_2:
		*ip_NBIoT_Band_Name = NB_BAND_2;
		break;
	case NB_IDX_B_3:
		*ip_NBIoT_Band_Name = NB_BAND_3;
		break;
	case NB_IDX_B_4:
		*ip_NBIoT_Band_Name = NB_BAND_4;
		break;
	case NB_IDX_B_5:
		*ip_NBIoT_Band_Name = NB_BAND_5;
		break;
	case NB_IDX_B_7:
		*ip_NBIoT_Band_Name = NB_BAND_7;
		break;
	case NB_IDX_B_8:
		*ip_NBIoT_Band_Name = NB_BAND_8;
		break;
	case NB_IDX_B_9:
		*ip_NBIoT_Band_Name = NB_BAND_9;
		break;
	case NB_IDX_B_10:
		*ip_NBIoT_Band_Name = NB_BAND_10;
		break;
	case NB_IDX_B_11:
		*ip_NBIoT_Band_Name = NB_BAND_11;
		break;
	case NB_IDX_B_12:
		*ip_NBIoT_Band_Name = NB_BAND_12;
		break;
	case NB_IDX_B_13:
		*ip_NBIoT_Band_Name = NB_BAND_13;
		break;
	case NB_IDX_B_14:
		*ip_NBIoT_Band_Name = NB_BAND_14;
		break;
	case NB_IDX_B_17:
		*ip_NBIoT_Band_Name = NB_BAND_17;
		break;
	case NB_IDX_B_18:
		*ip_NBIoT_Band_Name = NB_BAND_18;
		break;
	case NB_IDX_B_19:
		*ip_NBIoT_Band_Name = NB_BAND_19;
		break;
	case NB_IDX_B_20:
		*ip_NBIoT_Band_Name = NB_BAND_20;
		break;
	case NB_IDX_B_21:
		*ip_NBIoT_Band_Name = NB_BAND_21;
		break;
	case NB_IDX_B_22:
		*ip_NBIoT_Band_Name = NB_BAND_22;
		break;
	case NB_IDX_B_24:
		*ip_NBIoT_Band_Name = NB_BAND_24;
		break;
	case NB_IDX_B_25:
		*ip_NBIoT_Band_Name = NB_BAND_25;
		break;
	case NB_IDX_B_26:
		*ip_NBIoT_Band_Name = NB_BAND_26;
		break;
	case NB_IDX_B_27:
		*ip_NBIoT_Band_Name = NB_BAND_27;
		break;
	case NB_IDX_B_28:
		*ip_NBIoT_Band_Name = NB_BAND_28;
		break;
	case NB_IDX_B_30:
		*ip_NBIoT_Band_Name = NB_BAND_30;
		break;
	case NB_IDX_B_31:
		*ip_NBIoT_Band_Name = NB_BAND_31;
		break;
	case NB_IDX_B_66:
		*ip_NBIoT_Band_Name = NB_BAND_66;
		break;
	case NB_IDX_B_68:
		*ip_NBIoT_Band_Name = NB_BAND_68;
		break;
	case NB_IDX_B_70:
		*ip_NBIoT_Band_Name = NB_BAND_70;
		break;
	case NB_IDX_B_71:
		*ip_NBIoT_Band_Name = NB_BAND_71;
		break;
	case NB_IDX_B_72:
		*ip_NBIoT_Band_Name = NB_BAND_72;
		break;
	case NB_IDX_B_73:
		*ip_NBIoT_Band_Name = NB_BAND_73;
		break;
	case NB_IDX_B_74:
		*ip_NBIoT_Band_Name = NB_BAND_74;
		break;
	case NB_IDX_B_85:
		*ip_NBIoT_Band_Name = NB_BAND_85;
		break;
	case NB_IDX_B_86:
		*ip_NBIoT_Band_Name = NB_BAND_86;
		break;
	case NB_IDX_B_87:
		*ip_NBIoT_Band_Name = NB_BAND_87;
		break;
	case NB_IDX_B_88:
		*ip_NBIoT_Band_Name = NB_BAND_88;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_NBIoT_Band_Name, NB-IoT_Band_Index(%d) is Wrong!" ), i_NBIoT_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}//switch( i_NBIoT_Band_Index

	//pass
	return true;
}


bool CGFC::Get_LTE_Band_DD_FDD_TDD( int i_LTE_Band_Name, int & oj_i_LTE_DD )
{
	//
	try
	{
		//FDD:1~32, 65~88 (B29和B32只有DL，只有CA才有用)，NB-B86是FDD
		if( ((i_LTE_Band_Name >= 1) && (i_LTE_Band_Name <= 32))
			|| ((i_LTE_Band_Name >= 65) && (i_LTE_Band_Name <= 88)) )//FDD:1~32, 65~88 (B29和B32只有DL，只有CA才有用)，NB-B86是FDD
		{
			oj_i_LTE_DD = DD_FDD;
		}
		else if( (i_LTE_Band_Name >= 33) && (i_LTE_Band_Name <= 53) )//TDD:33~53
		{
			oj_i_LTE_DD = DD_TDD;
		}
		else
		{
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		//fail信息
		this->m_csError_Info.Format( _T( "ERROR, Get_LTE_Band_DD_FDD_TDD, LTE_Band_Name(%d) is Wrong!" ), i_LTE_Band_Name );
		::AfxMessageBox( this->m_csError_Info );

		//默认返回值
		oj_i_LTE_DD = DD_NDF;

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_DD_FDD_TDD_Name( int i_FDD_TDD, CString & oj_cs_DD_Name )
{
	try
	{
		switch( i_FDD_TDD )
		{
		case DD_FDD:
			oj_cs_DD_Name.Format( _T( "FDD" ) );
			break;
		case DD_TDD:
			oj_cs_DD_Name.Format( _T( "TDD" ) );
			break;
		default:
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		//fail信息
		this->m_csError_Info.Format( _T( "ERROR, Get_DD_FDD_TDD_Name, FDD_TDD(%d) is Wrong!" ), i_FDD_TDD );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NR_5G_SUB6_Band_Index( int i_Band_Name, int & oj_i_Band_Index )
{
	try
	{
		switch( i_Band_Name )
		{
		case 1://NR5GSUB6_N_1
			oj_i_Band_Index = NR5GSUB6_N_1;
			break;
		case 2://NR5GSUB6_N_2
			oj_i_Band_Index = NR5GSUB6_N_2;
			break;
		case 3://NR5GSUB6_N_3
			oj_i_Band_Index = NR5GSUB6_N_3;
			break;
		case 5://NR5GSUB6_N_5
			oj_i_Band_Index = NR5GSUB6_N_5;
			break;
		case 7://NR5GSUB6_N_7
			oj_i_Band_Index = NR5GSUB6_N_7;
			break;
		case 8://NR5GSUB6_N_8
			oj_i_Band_Index = NR5GSUB6_N_8;
			break;
		case 20://NR5GSUB6_N_20
			oj_i_Band_Index = NR5GSUB6_N_20;
			break;
		case 28://NR5GSUB6_N_28
			oj_i_Band_Index = NR5GSUB6_N_28;
			break;
		case 41://NR5GSUB6_N_41
			oj_i_Band_Index = NR5GSUB6_N_41;
			break;
		case 66://NR5GSUB6_N_66
			oj_i_Band_Index = NR5GSUB6_N_66;
			break;
		case 71://NR5GSUB6_N_71
			oj_i_Band_Index = NR5GSUB6_N_71;
			break;
		case 77://NR5GSUB6_N_77
			oj_i_Band_Index = NR5GSUB6_N_77;
			break;
		case 78://NR5GSUB6_N_78
			oj_i_Band_Index = NR5GSUB6_N_78;
			break;
		case 79://NR5GSUB6_N_79
			oj_i_Band_Index = NR5GSUB6_N_79;
			break;
		case 12://NR5GSUB6_N_12
			oj_i_Band_Index = NR5GSUB6_N_12;
			break;
		case 25://NR5GSUB6_N_25
			oj_i_Band_Index = NR5GSUB6_N_25;
			break;
		case 48://NR5GSUB6_N_48
			oj_i_Band_Index = NR5GSUB6_N_48;
			break;
		case 38://NR5GSUB6_N_38
			oj_i_Band_Index = NR5GSUB6_N_38;
			break;
		case 40://NR5GSUB6_N_40
			oj_i_Band_Index = NR5GSUB6_N_40;
			break;
		case 14://NR5GSUB6_N_14
			oj_i_Band_Index = NR5GSUB6_N_14;
			break;
		case 13://NR5GSUB6_N_13
			oj_i_Band_Index = NR5GSUB6_N_13;
			break;
		case 18://NR5GSUB6_N_18
			oj_i_Band_Index = NR5GSUB6_N_18;
			break;
		case 26://NR5GSUB6_N_26
			oj_i_Band_Index = NR5GSUB6_N_26;
			break;
		case 29://NR5GSUB6_N_29
			oj_i_Band_Index = NR5GSUB6_N_29;
			break;
		case 30://NR5GSUB6_N_30
			oj_i_Band_Index = NR5GSUB6_N_30;
			break;
		case 75://NR5GSUB6_N_75
			oj_i_Band_Index = NR5GSUB6_N_75;
			break;
		case 76://NR5GSUB6_N_76
			oj_i_Band_Index = NR5GSUB6_N_76;
			break;
		case 91://NR5GSUB6_N_91
			oj_i_Band_Index = NR5GSUB6_N_91;
			break;
		case 92://NR5GSUB6_N_92
			oj_i_Band_Index = NR5GSUB6_N_92;
			break;
		case 93://NR5GSUB6_N_93
			oj_i_Band_Index = NR5GSUB6_N_93;
			break;
		case 94://NR5GSUB6_N_94
			oj_i_Band_Index = NR5GSUB6_N_94;
			break;
		case 70://NR5GSUB6_N_70
			oj_i_Band_Index = NR5GSUB6_N_70;
			break;
		case 257://NR5GSUB6_N_257
			oj_i_Band_Index = NR5GSUB6_N_257;
			break;
		case 258://NR5GSUB6_N_258
			oj_i_Band_Index = NR5GSUB6_N_258;
			break;
		case 260://NR5GSUB6_N_260
			oj_i_Band_Index = NR5GSUB6_N_260;
			break;
		case 261://NR5GSUB6_N_261
			oj_i_Band_Index = NR5GSUB6_N_261;
			break;
		default:
			//fail
			throw(0);
		}//switch( i_Band_Name

	}
	catch( ... )
	{
		//fail信息
		this->m_csError_Info.Format( _T( "ERROR, Get_NR_5G_SUB6_Band_Index, Band_Name(%d) is Wrong!" ), i_Band_Name );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NR_5G_SUB6_Band_Name( int i_Band_Index, int & oj_i_Band_Name )
{
	try
	{
		switch( i_Band_Index )
		{
		case NR5GSUB6_N_1:
			oj_i_Band_Name = 1;
			break;
		case NR5GSUB6_N_2:
			oj_i_Band_Name = 2;
			break;
		case NR5GSUB6_N_3:
			oj_i_Band_Name = 3;
			break;
		case NR5GSUB6_N_5:
			oj_i_Band_Name = 5;
			break;
		case NR5GSUB6_N_7:
			oj_i_Band_Name = 7;
			break;
		case NR5GSUB6_N_8:
			oj_i_Band_Name = 8;
			break;
		case NR5GSUB6_N_20:
			oj_i_Band_Name = 20;
			break;
		case NR5GSUB6_N_28:
			oj_i_Band_Name = 28;
			break;
		case NR5GSUB6_N_41:
			oj_i_Band_Name = 41;
			break;
		case NR5GSUB6_N_66:
			oj_i_Band_Name = 66;
			break;
		case NR5GSUB6_N_71:
			oj_i_Band_Name = 71;
			break;
		case NR5GSUB6_N_77:
			oj_i_Band_Name = 77;
			break;
		case NR5GSUB6_N_78:
			oj_i_Band_Name = 78;
			break;
		case NR5GSUB6_N_79:
			oj_i_Band_Name = 79;
			break;
		case NR5GSUB6_N_12:
			oj_i_Band_Name = 12;
			break;
		case NR5GSUB6_N_25:
			oj_i_Band_Name = 25;
			break;
		case NR5GSUB6_N_48:
			oj_i_Band_Name = 48;
			break;
		case NR5GSUB6_N_38:
			oj_i_Band_Name = 38;
			break;
		case NR5GSUB6_N_40:
			oj_i_Band_Name = 40;
			break;
		case NR5GSUB6_N_14:
			oj_i_Band_Name = 14;
			break;
		case NR5GSUB6_N_13:
			oj_i_Band_Name = 13;
			break;
		case NR5GSUB6_N_18:
			oj_i_Band_Name = 18;
			break;
		case NR5GSUB6_N_26:
			oj_i_Band_Name = 26;
			break;
		case NR5GSUB6_N_29:
			oj_i_Band_Name = 29;
			break;
		case NR5GSUB6_N_30:
			oj_i_Band_Name = 30;
			break;
		case NR5GSUB6_N_75:
			oj_i_Band_Name = 75;
			break;
		case NR5GSUB6_N_76:
			oj_i_Band_Name = 76;
			break;
		case NR5GSUB6_N_91:
			oj_i_Band_Name = 91;
			break;
		case NR5GSUB6_N_92:
			oj_i_Band_Name = 92;
			break;
		case NR5GSUB6_N_93:
			oj_i_Band_Name = 93;
			break;
		case NR5GSUB6_N_94:
			oj_i_Band_Name = 94;
			break;
		case NR5GSUB6_N_70:
			oj_i_Band_Name = 70;
			break;
		case NR5GSUB6_N_257:
			oj_i_Band_Name = 257;
			break;
		case NR5GSUB6_N_258:
			oj_i_Band_Name = 258;
			break;
		case NR5GSUB6_N_260:
			oj_i_Band_Name = 260;
			break;
		case NR5GSUB6_N_261:
			oj_i_Band_Name = 261;
			break;
		default:
			//fail
			throw(0);
		}//switch( i_Band_Index

	}
	catch( ... )
	{
		//fail信息
		this->m_csError_Info.Format( _T( "ERROR, Get_NR_5G_SUB6_Band_Name, Band_Index(%d) is Wrong!" ), i_Band_Index );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_ANT_Type_Name( EU_ANT_TYPE_INDEX eu_Ant_Type_Index, CString & oj_cs_Ant_Type_Name )
{
	//天线类型 转 天线名

	//
	try
	{
		switch( eu_Ant_Type_Index )
		{
		case ANT_TYPE_IDX_MAIN_1:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MAIN_1" ) );
			break;
		case ANT_TYPE_IDX_DIV_1:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_DIV_1" ) );
			break;
		case ANT_TYPE_IDX_GPS:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_GPS" ) );
			break;
		case ANT_TYPE_IDX_WIFI_BLU:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_WIFI_BLU" ) );
			break;
		case ANT_TYPE_IDX_DIV_GPS:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_DIV_GPS" ) );
			break;
		case ANT_TYPE_IDX_MULTI_DG:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_DG" ) );
			break;
		case ANT_TYPE_IDX_MULTI_M:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_M" ) );
			break;
		case ANT_TYPE_IDX_MULTI_M1:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_M1" ) );
			break;
		case ANT_TYPE_IDX_MULTI_M2:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_M2" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_1:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_1" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_2:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_2" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_3:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_3" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_4:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_4" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_5:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_5" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_6:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_6" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_7:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_7" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_8:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_8" ) );
			break;
		case ANT_TYPE_IDX_MAIN_2:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MAIN_2" ) );
			break;
		case ANT_TYPE_IDX_DIV_2:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_DIV_2" ) );
			break;
		case ANT_TYPE_IDX_MULTI_M3:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_M3" ) );
			break;
		case ANT_TYPE_IDX_MULTI_M4:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_MULTI_M4" ) );
			break;
		case ANT_TYPE_IDX_WIFI_MIMO:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_WIFI_MIMO" ) );
			break;
		case ANT_TYPE_IDX_V2X_MAIN:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_V2X_MAIN" ) );
			break;
		case ANT_TYPE_IDX_V2X_DIV:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_V2X_DIV" ) );
			break;
		case ANT_TYPE_IDX_DSDA_MAIN:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_DSDA_MAIN" ) );
			break;
		case ANT_TYPE_IDX_DSDA_DIV:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_DSDA_DIV" ) );
			break;
		case ANT_TYPE_IDX_V2X_TRX0:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_V2X_TRX0" ) );
			break;
		case ANT_TYPE_IDX_V2X_TRX1:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_V2X_TRX1" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_9:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_9" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_10:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_10" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_11:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_11" ) );
			break;
		case ANT_TYPE_IDX_NR5GSUB6_12:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_NR_5G_SUB6_12" ) );
			break;
		case ANT_TYPE_IDX_WIFI_BLU_GPS:
			oj_cs_Ant_Type_Name.Format( _T( "ANT_WIFI_BLU_GPS" ) );
			break;
		default:
			//fail
			throw(0);
		}//switch( eu_Ant_Type_Index
	}
	catch( ... )
	{
		//fail信息
		this->m_csError_Info.Format( _T( "ERROR, Get_ANT_Type_Name, Ant_Type_Index(%d) is Wrong!" ), eu_Ant_Type_Index );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::IS_SMART_MODULE( int i_BB_Chip )
{
	//判定是否是高通智能模块平台

	bool bResult = false;

	//
	switch( i_BB_Chip )//高通的MSMxxxx/SDMxxxx都是智能模块平台
	{
	case BB_MSM8909:
	case BB_MSM8937:
	case BB_MSM8953:
	case BB_MSM8917:
	case BB_SDM450:
	case BB_MSM8905:
	case BB_SDM660:
	case BB_QM215:
	case BB_QCM2150:
	case BB_SL8541E:
	case BB_SDA845:
	case BB_SM6115:
	case BB_UIS8581:
	case BB_QCM6125:
	case BB_QCM2290:
	case BB_SM4350:
	case BB_SL8521E:
	case BB_QCM6490:
	case BB_QCM4290:
	case BB_QCS8250:
		//是智能模块平台
		bResult = true;
		//
		break;

	default:
		//不是智能模块平台
		bResult = false;
		//
		break;
	}//switch( i_BB_Chip

	//返回结果
	return bResult;
}


void CGFC::Empty_Struct_TESTER_RF_OFFSET_S( TESTER_RF_OFFSET_S & oj_str_RF_Loss_V1 )
{
	int iBuff_Idx, iQuan;


	//GSM
	iQuan = G_BAND_SIZE * 6;
	//GSM 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_GSM_RF_Loss[iBuff_Idx] = 0.0;
	}

	//WCDMA
	iQuan = W_BAND_SIZE * 2;
	//WCDMA 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_W_RF_Loss[iBuff_Idx] = 0.0;
	}

	//TD-SCDMA
	iQuan = TD_BAND_SIZE * 2;
	//TD-SCDMA 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_TD_RF_Loss[iBuff_Idx] = 0.0;
	}

	//cdma2000
	iQuan = C2K_BC_SIZE * 2;
	//cdma2000 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_C2K_RF_Loss[iBuff_Idx] = 0.0;
	}

	//LTE
	iQuan = L_BAND_SIZE * 2;
	//LTE 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_LTE_RF_Loss[iBuff_Idx] = 0.0;
	}

	//NB-IoT
	iQuan = NB_IDX_B_SIZE * 2;
	//NB-IoT 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V1.dr_NBIoT_RF_Loss[iBuff_Idx] = 0.0;
	}
}


void CGFC::Empty_Struct_TESTER_RF_OFFSET_V2_S( TESTER_RF_OFFSET_V2_S & oj_str_RF_Loss_V2 )
{
	int iBuff_Idx, iQuan;


	//GSM
	iQuan = G_BAND_SIZE * 6;
	//GSM 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_GSM_RF_Loss[iBuff_Idx] = 0.0;
	}

	//WCDMA
	iQuan = W_BAND_SIZE * 6;
	//WCDMA 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_W_RF_Loss[iBuff_Idx] = 0.0;
	}

	//TD-SCDMA
	iQuan = TD_BAND_SIZE * 6;
	//TD-SCDMA 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_TD_RF_Loss[iBuff_Idx] = 0.0;
	}

	//cdma2000
	iQuan = C2K_BC_SIZE * 6;
	//cdma2000 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_C2K_RF_Loss[iBuff_Idx] = 0.0;
	}

	//LTE
	iQuan = L_BAND_SIZE * 6;
	//LTE 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_LTE_RF_Loss[iBuff_Idx] = 0.0;
	}

	//NB-IoT
	iQuan = NB_IDX_B_SIZE * 6;
	//NB-IoT 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_NBIoT_RF_Loss[iBuff_Idx] = 0.0;
	}

	//NR 5G SUB6
	iQuan = NR5GSUB6_N_SIZE * 6;
	//NR 5G SUB6 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_NR_5G_SUB6_RF_Loss[iBuff_Idx] = 0.0;
	}

	//GPS
	iQuan = 2;
	//GPS 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_GPS_RF_Loss[iBuff_Idx] = 0.0;
	}

	//WiFi
	iQuan = WIFI_FB_COUNT * 6;
	//WiFi 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_WiFi_RF_Loss[iBuff_Idx] = 0.0;
	}

	//Bluetooth
	iQuan = 2;
	//Bluetooth 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_BLU_RF_Loss[iBuff_Idx] = 0.0;
	}

	//CV2X
	iQuan = CV2X_BAND_SIZE * 6;
	//CV2X 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_V2.dr_CV2X_RF_Loss[iBuff_Idx] = 0.0;
	}
}


void CGFC::Empty_Struct_ANT_CFG_ALL_S( ANT_CFG_ALL_S & oj_str_ANT_Config_All )
{
	int iAnt_Idx;


	//清空“天线列表”结构体
	this->Empty_Struct_ANT_LIST_S( oj_str_ANT_Config_All.str_ANT_List );


	//清空“线损”数据
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_IDX_SIZE; iAnt_Idx++ )
	{
		this->Empty_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_Config_All.strr_ANT_RF_Loss[iAnt_Idx] );
	}
}


void CGFC::Empty_Struct_ANT_LIST_S( ANT_LIST_S & oj_str_ANT_List )
{
	int iAnt_Idx;

	//清空“天线列表”的数量
	oj_str_ANT_List.i_ANT_Quan = 0;

	//清空“天线列表”的天线数据
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++ )
	{
		oj_str_ANT_List.strr_ANT_Prop_List[iAnt_Idx].eu_ANT_HW_Type_Name = ANT_TYPE_IDX_NDF;
		oj_str_ANT_List.strr_ANT_Prop_List[iAnt_Idx].i_ANT_SW_Numb = -1;
	}
}


void CGFC::Empty_Struct_REF_DATA_ALL_S( REF_DATA_ALL_S & oj_str_Ref_Data_All )
{
	int iAnt_Idx;


	//清空“天线列表”的结构体
	this->Empty_Struct_ANT_LIST_S( oj_str_Ref_Data_All.str_Ref_ANT_List );


	//清空“金板”数据
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_IDX_SIZE; iAnt_Idx++ )
	{
		this->Empty_Struct_REF_DATA_BAND_S( oj_str_Ref_Data_All.strr_Ref_Band_Data[iAnt_Idx] );
	}
}


void CGFC::Empty_Struct_REF_DATA_BAND_S( REF_DATA_BAND_S & oj_str_Ref_Data_Band )
{
	int iBand_Idx, iLMH_Idx;


	//GSM 频段
	for( iBand_Idx = 0; iBand_Idx < G_BAND_SIZE; iBand_Idx++ )
	{
		//清空REF频段名和使能标识
		oj_str_Ref_Data_Band.strr_GSM_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//清空REF值
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_GSM_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_GSM_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//WCDMA 频段
	for( iBand_Idx = 0; iBand_Idx < W_BAND_SIZE; iBand_Idx++ )
	{
		//清空REF频段名和使能标识
		oj_str_Ref_Data_Band.strr_WCDMA_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//清空REF值
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_WCDMA_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_WCDMA_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//cdma2000 频段
	for( iBand_Idx = 0; iBand_Idx < C2K_BC_SIZE; iBand_Idx++ )
	{
		//清空REF频段名和使能标识
		oj_str_Ref_Data_Band.strr_C2K_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//清空REF值
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_C2K_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_C2K_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//TD-SCDMA 频段
	for( iBand_Idx = 0; iBand_Idx < TD_BAND_SIZE; iBand_Idx++ )
	{
		//清空REF频段名和使能标识
		oj_str_Ref_Data_Band.strr_TD_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//清空REF值
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_TD_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_TD_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//LTE 频段
	for( iBand_Idx = 0; iBand_Idx < L_BAND_SIZE; iBand_Idx++ )
	{
		//清空REF频段名和使能标识
		oj_str_Ref_Data_Band.strr_LTE_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//清空REF值
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_LTE_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_LTE_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//NB-IoT 频段
	for( iBand_Idx = 0; iBand_Idx < NB_IDX_B_SIZE; iBand_Idx++ )
	{
		//清空REF频段名和使能标识
		oj_str_Ref_Data_Band.strr_NBIoT_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//清空REF值
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_NBIoT_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_NBIoT_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//NR_5G_SUB6 频段
	for( iBand_Idx = 0; iBand_Idx < NR5GSUB6_N_SIZE; iBand_Idx++ )
	{
		//清空REF频段名和使能标识
		oj_str_Ref_Data_Band.strr_NR_5G_SUB6_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//清空REF值
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_NR_5G_SUB6_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_NR_5G_SUB6_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//C-V2X
	for( iBand_Idx = 0; iBand_Idx < CV2X_BAND_SIZE; iBand_Idx++ )
	{
		//清空REF频段名和使能标识
		oj_str_Ref_Data_Band.strr_CV2X_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//清空REF值
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_CV2X_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_CV2X_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//GPS
	//清空REF频段名和使能标识
	oj_str_Ref_Data_Band.str_GPS_REF_Data.i_Band_Name_And_Enable = -1;

	//清空REF值
	for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
	{
		oj_str_Ref_Data_Band.str_GPS_REF_Data.dr_Tx_Power[iLMH_Idx] = -1.0;
		oj_str_Ref_Data_Band.str_GPS_REF_Data.dr_Rx_Level[iLMH_Idx] = -1.0;
	}


	//WiFi
	for( iBand_Idx = 0; iBand_Idx < WIFI_FB_COUNT; iBand_Idx++ )
	{
		//清空REF频段名和使能标识
		oj_str_Ref_Data_Band.strr_WiFi_REF_Data[iBand_Idx].i_Band_Name_And_Enable = -1;

		//清空REF值
		for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
		{
			oj_str_Ref_Data_Band.strr_WiFi_REF_Data[iBand_Idx].dr_Tx_Power[iLMH_Idx] = -1.0;
			oj_str_Ref_Data_Band.strr_WiFi_REF_Data[iBand_Idx].dr_Rx_Level[iLMH_Idx] = -1.0;
		}
	}


	//蓝牙
	//清空REF频段名和使能标识
	oj_str_Ref_Data_Band.str_Bluetooth_REF_Data.i_Band_Name_And_Enable = -1;

	//清空REF值
	for( iLMH_Idx = 0; iLMH_Idx < CH_LMH_COUNT; iLMH_Idx++ )
	{
		oj_str_Ref_Data_Band.str_Bluetooth_REF_Data.dr_Tx_Power[iLMH_Idx] = -1.0;
		oj_str_Ref_Data_Band.str_Bluetooth_REF_Data.dr_Rx_Level[iLMH_Idx] = -1.0;
	}
}


void CGFC::Copy_Struct_ANT_CFG_ALL_S( ANT_CFG_ALL_S & oj_str_ANT_CFG_Dest, ANT_CFG_ALL_S str_ANT_CFG_Source )
{
	int iAnt_Idx;


	//Copy“ANT天线列表”结构体
	this->Copy_Struct_ANT_LIST_S( oj_str_ANT_CFG_Dest.str_ANT_List, str_ANT_CFG_Source.str_ANT_List );


	//Copy“线损”数据
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_IDX_SIZE; iAnt_Idx++ )
	{
		//Copy“线损 V2”结构体
		this->Copy_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_CFG_Dest.strr_ANT_RF_Loss[iAnt_Idx], str_ANT_CFG_Source.strr_ANT_RF_Loss[iAnt_Idx] );
	}
}


void CGFC::Copy_Struct_ANT_LIST_S( ANT_LIST_S & oj_str_ANT_List_Dest, ANT_LIST_S str_ANT_List_Source )
{
	int iAnt_Idx;

	//赋值“天线列表”的数量
	oj_str_ANT_List_Dest.i_ANT_Quan = str_ANT_List_Source.i_ANT_Quan;

	//赋值“天线列表”的天线数据
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++ )
	{
		//天线属性数组
		oj_str_ANT_List_Dest.strr_ANT_Prop_List[iAnt_Idx].eu_ANT_HW_Type_Name = str_ANT_List_Source.strr_ANT_Prop_List[iAnt_Idx].eu_ANT_HW_Type_Name;
		oj_str_ANT_List_Dest.strr_ANT_Prop_List[iAnt_Idx].i_ANT_SW_Numb = str_ANT_List_Source.strr_ANT_Prop_List[iAnt_Idx].i_ANT_SW_Numb;
	}
}


void CGFC::Copy_Struct_TESTER_RF_OFFSET_V2_S( TESTER_RF_OFFSET_V2_S & oj_str_RF_Loss_Dest, TESTER_RF_OFFSET_V2_S str_RF_Loss_Source )
{
	int iBuff_Idx, iQuan;


	//GSM
	iQuan = G_BAND_SIZE * 6;
	//GSM 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_GSM_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_GSM_RF_Loss[iBuff_Idx];
	}

	//WCDMA
	iQuan = W_BAND_SIZE * 6;
	//WCDMA 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_W_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_W_RF_Loss[iBuff_Idx];
	}

	//TD-SCDMA
	iQuan = TD_BAND_SIZE * 6;
	//TD-SCDMA 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_TD_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_TD_RF_Loss[iBuff_Idx];
	}

	//cdma2000
	iQuan = C2K_BC_SIZE * 6;
	//cdma2000 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_C2K_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_C2K_RF_Loss[iBuff_Idx];
	}

	//LTE
	iQuan = L_BAND_SIZE * 6;
	//LTE 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_LTE_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_LTE_RF_Loss[iBuff_Idx];
	}

	//NB-IoT
	iQuan = NB_IDX_B_SIZE * 6;
	//NB-IoT 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_NBIoT_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_NBIoT_RF_Loss[iBuff_Idx];
	}

	//NR 5G SUB6
	iQuan = NR5GSUB6_N_SIZE * 6;
	//NR 5G SUB6 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_NR_5G_SUB6_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_NR_5G_SUB6_RF_Loss[iBuff_Idx];
	}

	//GPS
	iQuan = 2;
	//GPS 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_GPS_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_GPS_RF_Loss[iBuff_Idx];
	}

	//WiFi
	iQuan = WIFI_FB_COUNT * 6;
	//WiFi 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_WiFi_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_WiFi_RF_Loss[iBuff_Idx];
	}

	//Bluetooth
	iQuan = 2;
	//Bluetooth 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_BLU_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_BLU_RF_Loss[iBuff_Idx];
	}

	//CV2X
	iQuan = CV2X_BAND_SIZE * 6;
	//CV2X 赋值
	for( iBuff_Idx = 0; iBuff_Idx < iQuan; iBuff_Idx++ )
	{
		oj_str_RF_Loss_Dest.dr_CV2X_RF_Loss[iBuff_Idx] = str_RF_Loss_Source.dr_CV2X_RF_Loss[iBuff_Idx];
	}
}


void CGFC::Copy_Struct_TESTER_RF_OFFSET_V2_S_To_TESTER_RF_OFFSET_S( TESTER_RF_OFFSET_S & oj_str_RF_Loss_V1_Dest, TESTER_RF_OFFSET_V2_S str_RF_Loss_V2_Source )
{
	int iBand_Idx, iQuan;


	//线损
	//GSM 线损
	iQuan = G_BAND_SIZE * 6;
	//GSM
	for( iBand_Idx = 0; iBand_Idx < iQuan; iBand_Idx++ )
	{
		oj_str_RF_Loss_V1_Dest.dr_GSM_RF_Loss[iBand_Idx] = str_RF_Loss_V2_Source.dr_GSM_RF_Loss[iBand_Idx];
	}

	//WCDMA 线损
	for( iBand_Idx = 0; iBand_Idx < W_BAND_SIZE; iBand_Idx++ )
	{
		//TX
		oj_str_RF_Loss_V1_Dest.dr_W_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT] = str_RF_Loss_V2_Source.dr_W_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + CH_MID];
		//RX
		oj_str_RF_Loss_V1_Dest.dr_W_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT] = str_RF_Loss_V2_Source.dr_W_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + CH_MID];
	}

	//TD 线损
	for( iBand_Idx = 0; iBand_Idx < TD_BAND_SIZE; iBand_Idx++ )
	{
		//TX
		oj_str_RF_Loss_V1_Dest.dr_TD_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT] = str_RF_Loss_V2_Source.dr_TD_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + CH_MID];
		//RX
		oj_str_RF_Loss_V1_Dest.dr_TD_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT] = str_RF_Loss_V2_Source.dr_TD_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + CH_MID];
	}

	//cdma2000 线损
	for( iBand_Idx = 0; iBand_Idx < C2K_BC_SIZE; iBand_Idx++ )
	{
		//TX
		oj_str_RF_Loss_V1_Dest.dr_C2K_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT] = str_RF_Loss_V2_Source.dr_C2K_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + CH_MID];
		//RX
		oj_str_RF_Loss_V1_Dest.dr_C2K_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT] = str_RF_Loss_V2_Source.dr_C2K_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + CH_MID];
	}

	//LTE 线损
	for( iBand_Idx = 0; iBand_Idx < L_BAND_SIZE; iBand_Idx++ )
	{
		//TX
		oj_str_RF_Loss_V1_Dest.dr_LTE_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT] = str_RF_Loss_V2_Source.dr_LTE_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + CH_MID];
		//RX
		oj_str_RF_Loss_V1_Dest.dr_LTE_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT] = str_RF_Loss_V2_Source.dr_LTE_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + CH_MID];
	}

	//NB-IoT 线损
	for( iBand_Idx = 0; iBand_Idx < NB_IDX_B_SIZE; iBand_Idx++ )
	{
		//TX
		oj_str_RF_Loss_V1_Dest.dr_NBIoT_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT] = str_RF_Loss_V2_Source.dr_NBIoT_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + CH_MID];
		//RX
		oj_str_RF_Loss_V1_Dest.dr_NBIoT_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT] = str_RF_Loss_V2_Source.dr_NBIoT_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + CH_MID];
	}
}


void CGFC::Copy_Struct_TESTER_RF_OFFSET_S_To_TESTER_RF_OFFSET_V2_S( TESTER_RF_OFFSET_V2_S & oj_str_RF_Loss_V2_Dest, TESTER_RF_OFFSET_S str_RF_Loss_V1_Source )
{
	int iBand_Idx, iQuan, iLMH_Idx;


	//线损
	//GSM 线损
	iQuan = G_BAND_SIZE * 6;
	//GSM
	for( iBand_Idx = 0; iBand_Idx < iQuan; iBand_Idx++ )
	{
		oj_str_RF_Loss_V2_Dest.dr_GSM_RF_Loss[iBand_Idx] = str_RF_Loss_V1_Source.dr_GSM_RF_Loss[iBand_Idx];
	}

	//WCDMA 线损
	for( iBand_Idx = 0; iBand_Idx < W_BAND_SIZE; iBand_Idx++ )
	{
		for( iLMH_Idx = CH_LOW; iLMH_Idx <= CH_HIGH; iLMH_Idx++ )
		{
			//TX
			oj_str_RF_Loss_V2_Dest.dr_W_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_W_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT];
			//RX
			oj_str_RF_Loss_V2_Dest.dr_W_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_W_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT];
		}
	}

	//TD 线损
	for( iBand_Idx = 0; iBand_Idx < TD_BAND_SIZE; iBand_Idx++ )
	{
		for( iLMH_Idx = CH_LOW; iLMH_Idx <= CH_HIGH; iLMH_Idx++ )
		{
			//TX
			oj_str_RF_Loss_V2_Dest.dr_TD_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_TD_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT];
			//RX
			oj_str_RF_Loss_V2_Dest.dr_TD_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_TD_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT];
		}
	}

	//cdma2000 线损
	for( iBand_Idx = 0; iBand_Idx < C2K_BC_SIZE; iBand_Idx++ )
	{
		for( iLMH_Idx = CH_LOW; iLMH_Idx <= CH_HIGH; iLMH_Idx++ )
		{
			//TX
			oj_str_RF_Loss_V2_Dest.dr_C2K_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_C2K_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT];
			//RX
			oj_str_RF_Loss_V2_Dest.dr_C2K_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_C2K_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT];
		}
	}

	//LTE 线损
	for( iBand_Idx = 0; iBand_Idx < L_BAND_SIZE; iBand_Idx++ )
	{
		for( iLMH_Idx = CH_LOW; iLMH_Idx <= CH_HIGH; iLMH_Idx++ )
		{
			//TX
			oj_str_RF_Loss_V2_Dest.dr_LTE_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_LTE_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT];
			//RX
			oj_str_RF_Loss_V2_Dest.dr_LTE_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_LTE_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT];
		}
	}

	//NB-IoT 线损
	for( iBand_Idx = 0; iBand_Idx < NB_IDX_B_SIZE; iBand_Idx++ )
	{
		for( iLMH_Idx = CH_LOW; iLMH_Idx <= CH_HIGH; iLMH_Idx++ )
		{
			//TX
			oj_str_RF_Loss_V2_Dest.dr_NBIoT_RF_Loss[iBand_Idx * 6 + RF_TX_UL_INPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_NBIoT_RF_Loss[iBand_Idx * 2 + RF_TX_UL_INPUT];
			//RX
			oj_str_RF_Loss_V2_Dest.dr_NBIoT_RF_Loss[iBand_Idx * 6 + RF_RX_DL_OUTPUT * 3 + iLMH_Idx] = str_RF_Loss_V1_Source.dr_NBIoT_RF_Loss[iBand_Idx * 2 + RF_RX_DL_OUTPUT];
		}
	}
}


bool CGFC::Get_RF_Key_Type_Name( const EU_RF_KEY_TYPE_INDEX eu_RF_Key_Type_Index, CString & oj_cs_RF_Key_Type_Name )
{
	//RF Key Type index号 转化为 RF Key Type Name 字串

	//
	try
	{
		switch( eu_RF_Key_Type_Index )
		{
		case RF_KEY_TYPE_IDX_PXA1802S_RF858:	//ASR平台RF Type:"1802S+RF858"
			oj_cs_RF_Key_Type_Name.Format( _T( "1802S+RF858" ) );
			break;
		case RF_KEY_TYPE_IDX_PAX1908_RF858:		//ASR平台RF Type:"1802+RF858"
			oj_cs_RF_Key_Type_Name.Format( _T( "1802+RF858" ) );
			break;
		case RF_KEY_TYPE_IDX_RF866:				//ASR平台RF Type:"1826+RF866"
			oj_cs_RF_Key_Type_Name.Format( _T( "1826+RF866" ) );
			break;
		case RF_KEY_TYPE_IDX_RF859C:			//ASR平台RF Type:"1802S+RF859C"
			oj_cs_RF_Key_Type_Name.Format( _T( "1802S+RF859C" ) );
			break;
		case RF_KEY_TYPE_IDX_1601_3601:			//ASR平台RF Type:"3601_1601"
			oj_cs_RF_Key_Type_Name.Format( _T( "3601_1601" ) );
			break;
		case RF_KEY_TYPE_IDX_1603:			    //ASR平台RF Type:"1603"
			oj_cs_RF_Key_Type_Name.Format( _T( "1603" ) );
			break;
		case RF_KEY_TYPE_IDX_3603:			    //ASR平台RF Type:"3603"
			oj_cs_RF_Key_Type_Name.Format( _T( "3603" ) );
			break;
		case RF_KEY_TYPE_IDX_1803_FAICON:		//ASR平台RF Type:"1803"
			oj_cs_RF_Key_Type_Name.Format( _T( "1803" ) );
			break;
		case RF_KEY_TYPE_IDX_1606:				//ASR平台RF Type:"1606"
			oj_cs_RF_Key_Type_Name.Format( _T( "1606" ) );
			break;
		case RF_KEY_TYPE_IDX_1828_RF866:		//ASR平台RF Type:"1828+RF866"
			oj_cs_RF_Key_Type_Name.Format( _T( "1828+RF866" ) );
			break;
		default:
			oj_cs_RF_Key_Type_Name.Format( _T( "NDF" ) );
			//fail
			throw(0);
		}//switch( eu_RF_Key_Type_Index
	}
	catch( ... )
	{
		//fail信息
		this->m_csError_Info.Format( _T( "ERROR, Get_RF_Key_Type_Name, RF_Key_Type_Index(%d) is Wrong!" ), eu_RF_Key_Type_Index );

		//fail
		return false;
	}

	//pass
	return true;
}


void CGFC::Unicode_To_UTF8( const wchar_t * wcp_Unicode_Str, CStringA & oj_csa_UTF_8_Str )
{
	//Unicode，wchar_t* ---> utf8编码，char*

	int iLen = WideCharToMultiByte( CP_UTF8, 0, wcp_Unicode_Str, -1, NULL, 0, NULL, NULL );

	//分配内存
	char * cpUtf8 = (char *)malloc( iLen + 8 );

	//清空
	memset( cpUtf8, NULL, (iLen + 8) );

	//转换
	WideCharToMultiByte( CP_UTF8, 0, wcp_Unicode_Str, -1, cpUtf8, iLen, NULL, NULL );

	//赋值
	oj_csa_UTF_8_Str.Format( "%s", cpUtf8 );

	//释放内存
	free( cpUtf8 );
}


bool CGFC::Get_CV2X_Band_Index( int i_CV2X_Band_Name, int * ip_CV2X_Band_Index )
{
	//CV2X频段名 转化 CV2X频段index号
	//CV2X新加频段代码位置

	//
	switch( i_CV2X_Band_Name )
	{
	case 46://CV2X_BAND_46
		*ip_CV2X_Band_Index = CV2X_BAND_46;
		break;
	case 47://CV2X_BAND_47
		*ip_CV2X_Band_Index = CV2X_BAND_47;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_CV2X_Band_Index, C-V2X_Band_Name(%d) is wrong!" ), i_CV2X_Band_Name );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_CV2X_Band_Name( int i_CV2X_Band_Index, int * ip_CV2X_Band_Name )
{
	//CV2X频段index号 转化 CV2X频段名
	//CV2X新加频段代码位置

	//
	switch( i_CV2X_Band_Index )
	{
	case CV2X_BAND_46:
		*ip_CV2X_Band_Name = 46;
		break;
	case CV2X_BAND_47:
		*ip_CV2X_Band_Name = 47;
		break;
	default:
		this->m_csError_Info.Format( _T( "ERROR, Get_CV2X_Band_Name, CV2X_Band_Index(%d) is wrong!" ), i_CV2X_Band_Index );
		::AfxMessageBox( this->m_csError_Info );
		//fail
		return false;
	}

	//pass
	return true;
}


void CGFC::Copy_Struct_BAND_ENABLE_S( BAND_ENABLE_S & oj_str_Band_Enable_Dest, BAND_ENABLE_S str_Band_Enable_Source )
{
	int iBuff_Idx;


	//GSM
	for( iBuff_Idx = 0; iBuff_Idx < G_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_GSM_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_GSM_Band_Enable[iBuff_Idx];
	}

	//WCDMA
	for( iBuff_Idx = 0; iBuff_Idx < W_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_WCDMA_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_WCDMA_Band_Enable[iBuff_Idx];
	}

	//cdma2000
	for( iBuff_Idx = 0; iBuff_Idx < C2K_BC_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_C2K_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_C2K_Band_Enable[iBuff_Idx];
	}

	//TD
	for( iBuff_Idx = 0; iBuff_Idx < TD_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_TD_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_TD_Band_Enable[iBuff_Idx];
	}

	//LTE
	for( iBuff_Idx = 0; iBuff_Idx < L_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_LTE_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_LTE_Band_Enable[iBuff_Idx];
	}

	//NB-IoT
	for( iBuff_Idx = 0; iBuff_Idx < NB_IDX_B_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_NBIoT_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_NBIoT_Band_Enable[iBuff_Idx];
	}

	//NR 5G SUB6
	for( iBuff_Idx = 0; iBuff_Idx < NR5GSUB6_N_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_NR_5G_SUB6_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_NR_5G_SUB6_Band_Enable[iBuff_Idx];
	}

	//C-V2X
	for( iBuff_Idx = 0; iBuff_Idx < CV2X_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable_Dest.ir_CV2X_Band_Enable[iBuff_Idx] = str_Band_Enable_Source.ir_CV2X_Band_Enable[iBuff_Idx];
	}
}


void CGFC::Assign_Struct_BAND_ENABLE_S( BAND_ENABLE_S & oj_str_Band_Enable, int i_Value_Data )
{
	int iBuff_Idx;


	//GSM
	for( iBuff_Idx = 0; iBuff_Idx < G_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_GSM_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//WCDMA
	for( iBuff_Idx = 0; iBuff_Idx < W_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_WCDMA_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//cdma2000
	for( iBuff_Idx = 0; iBuff_Idx < C2K_BC_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_C2K_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//TD
	for( iBuff_Idx = 0; iBuff_Idx < TD_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_TD_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//LTE
	for( iBuff_Idx = 0; iBuff_Idx < L_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_LTE_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//NB-IoT
	for( iBuff_Idx = 0; iBuff_Idx < NB_IDX_B_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_NBIoT_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//NR 5G SUB6
	for( iBuff_Idx = 0; iBuff_Idx < NR5GSUB6_N_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_NR_5G_SUB6_Band_Enable[iBuff_Idx] = i_Value_Data;
	}

	//C-V2X
	for( iBuff_Idx = 0; iBuff_Idx < CV2X_BAND_SIZE; iBuff_Idx++ )
	{
		oj_str_Band_Enable.ir_CV2X_Band_Enable[iBuff_Idx] = i_Value_Data;
	}
}


bool CGFC::Get_NB_SCSP_Name( int i_S3K75_S15K, CString & oj_cs_NB_SCSP_Name )
{
	try
	{
		switch( i_S3K75_S15K )
		{
		case NB_SCSP_S3K75://3.75 kHz
			oj_cs_NB_SCSP_Name.Format( _T( "S3K75" ) );
			break;
		case NB_SCSP_S15K://15 kHz
			oj_cs_NB_SCSP_Name.Format( _T( "S15K" ) );
			break;
		default:
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		//fail信息
		this->m_csError_Info.Format( _T( "ERROR, Get_NB_SCSP_Name, S3K75_S15K(%d) is Wrong!" ), i_S3K75_S15K );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NB_Mod_Scheme_Name( int i_BPSK_QPSK, CString & oj_cs_NB_Mod_Scheme_Name )
{
	try
	{
		switch( i_BPSK_QPSK )
		{
		case NB_MSCH_BPSK://0 = BPSK
			oj_cs_NB_Mod_Scheme_Name.Format( _T( "BPSK" ) );
			break;
		case NB_MSCH_QPSK://1 = QPSK
			oj_cs_NB_Mod_Scheme_Name.Format( _T( "QPSK" ) );
			break;
		default:
			//fail
			throw(0);
		}
	}
	catch( ... )
	{
		//fail信息
		this->m_csError_Info.Format( _T( "ERROR, Get_NB_Mod_Scheme_Name, BPSK_QPSK(%d) is Wrong!" ), i_BPSK_QPSK );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NR_5G_MMW_Band_Index( int i_Band_Name, int & oj_i_Band_Index )
{
	try
	{
		switch( i_Band_Name )//5G mmW: n257/n258/n260/n261
		{
		case 257://NR5GMMW_N_257
			oj_i_Band_Index = NR5GMMW_N_257;
			break;
		case 258://NR5GMMW_N_258
			oj_i_Band_Index = NR5GMMW_N_258;
			break;
		case 260://NR5GMMW_N_260
			oj_i_Band_Index = NR5GMMW_N_260;
			break;
		case 261://NR5GMMW_N_261
			oj_i_Band_Index = NR5GMMW_N_261;
			break;
		default:
			//fail
			throw(0);
		}//switch( i_Band_Name
	}
	catch( ... )
	{
		//fail信息
		this->m_csError_Info.Format( _T( "ERROR, Get_NR_5G_MMW_Band_Index, Band_Name(%d) is Wrong!" ), i_Band_Name );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_NR_5G_MMW_Band_Name( int i_Band_Index, int & oj_i_Band_Name )
{
	try
	{
		switch( i_Band_Index )//5G mmW: n257/n258/n260/n261
		{
		case NR5GMMW_N_257:
			oj_i_Band_Name = 257;
			break;
		case NR5GMMW_N_258:
			oj_i_Band_Name = 258;
			break;
		case NR5GMMW_N_260:
			oj_i_Band_Name = 260;
			break;
		case NR5GMMW_N_261:
			oj_i_Band_Name = 261;
			break;
		default:
			//fail
			throw(0);
		}//switch( i_Band_Index

	}
	catch( ... )
	{
		//fail信息
		this->m_csError_Info.Format( _T( "ERROR, Get_NR_5G_MMW_Band_Name, Band_Index(%d) is Wrong!" ), i_Band_Index );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Get_ANT_Type_Index( const wchar_t * wcp_Ant_Type_Name, EU_ANT_TYPE_INDEX & oj_eu_Ant_Type_Idx )
{
	if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MAIN_1" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MAIN_1;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_DIV_1" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_DIV_1;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_GPS" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_GPS;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_WIFI_BLU" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_WIFI_BLU;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_DIV_GPS" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_DIV_GPS;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_DG" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_DG;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_M" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_M;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_M1" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_M1;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_M2" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_M2;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_1" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_1;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_2" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_2;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_3" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_3;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_4" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_4;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_5" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_5;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_6" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_6;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_7" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_7;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_8" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_8;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MAIN_2" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MAIN_2;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_DIV_2" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_DIV_2;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_M3" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_M3;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_MULTI_M4" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_MULTI_M4;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_WIFI_MIMO" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_WIFI_MIMO;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_V2X_MAIN" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_V2X_MAIN;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_V2X_DIV" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_V2X_DIV;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_DSDA_MAIN" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_DSDA_MAIN;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_DSDA_DIV" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_DSDA_DIV;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_V2X_TRX0" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_V2X_TRX0;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_V2X_TRX1" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_V2X_TRX1;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_9" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_9;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_10" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_10;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_11" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_11;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_NR_5G_SUB6_12" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_NR5GSUB6_12;
	}
	else if( wcscmp( wcp_Ant_Type_Name, _T( "ANT_WIFI_BLU_GPS" ) ) == 0 )
	{
		oj_eu_Ant_Type_Idx = ANT_TYPE_IDX_WIFI_BLU_GPS;
	}
	else
	{
		//显示异常
		this->m_csError_Info.Format( _T( "ERROR, Get_ANT_Type_Index(), \"%s\" is Wrong!" ), wcp_Ant_Type_Name );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}

//比较当前时间和给定时间比较
//_T("2018-05-18 18:32:44")
int CGFC::TimeCmp(CString strTimeSrc)
{
	CString strRep(_T("")),strDst(_T("")),csTr(_T(""));
	double dwSrc(0),dwDst(0);

	for (int i=0;i<strTimeSrc.GetLength();i++)
	{
		csTr = strTimeSrc.GetAt(i);
		if ((csTr >=_T("0")) && (csTr <=_T("9")))
		{
			strRep+=csTr;
		}
		else
		{
			continue;
		}

	}
	dwSrc= _wtof(strRep);
#if 0
	CTime aTime = CTime::GetCurrentTime();
	int y = aTime.GetYear();
	int m = aTime.GetMonth();
	int d = aTime.GetDay();
	int h = aTime.GetHour();
	int min = aTime.GetMinute();
	int s = aTime.GetSecond();

	strDst.Format(_T("%04d%02d%02d%02d%02d%02d"),y,m,d,h,min,s);
	dwDst= _wtof(strDst);
#endif
	dwDst=20180601000000;

	if (dwSrc>dwDst)
	{
		//当前的时间比给定的时间要新
		return 1;
	} 
	else
	{
		return -1;
	}
	

}

void CGFC::Get_RF_Instru_Name( int i_Instru_Index, CString & oj_cs_Instru_Name )
{
	//仪器宏定义转仪器字串名
	switch( i_Instru_Index )
	{
	case CMU200:
		oj_cs_Instru_Name.Format( _T( "CMU200" ) );
		break;

	case AG_E1960:
	case AG_E1968:
	case AG_E1987A:
	case AG_E1962B:
		oj_cs_Instru_Name.Format( _T( "8960" ) );
		break;

	case CMW500:
		oj_cs_Instru_Name.Format( _T( "CMW500" ) );
		break;

	case LP_IQXS:
		oj_cs_Instru_Name.Format( _T( "IQxstream" ) );
		break;

	case CMW100:
		oj_cs_Instru_Name.Format( _T( "CMW100" ) );
		break;

	case LP_IQXM:
		oj_cs_Instru_Name.Format( _T( "IQxstream-M" ) );
		break;

	case LP_5G_IQXS:
		oj_cs_Instru_Name.Format( _T( "IQxstream-5G" ) );
		break;

	case CMP180:
		oj_cs_Instru_Name.Format( _T( "CMP180" ) );
		break;

	default:
		oj_cs_Instru_Name.Format( _T( "NDF" ) );
		break;
	}//switch( i_Instru_Index
}

bool CGFC::Get_Locoal_IP_Mac(CString  cs_PC_IP, CString & cs_PC_Mac)
{
	//vector<MacIp> &macip;
	CString myIP, myMAC;
	bool bNetReady = false;
	ULONG outBufLen = 0;
	DWORD dwRetVal = 0;

	PIP_ADAPTER_INFO pAadpterInfo;
	PIP_ADAPTER_INFO pAadpterInfoTmp = NULL;

	//MacIp tmpmacip;

	pAadpterInfo = (IP_ADAPTER_INFO*)GlobalAlloc(GMEM_ZEROINIT, sizeof(IP_ADAPTER_INFO));

	if (GetAdaptersInfo(pAadpterInfo, &outBufLen) != ERROR_SUCCESS)
	{
		GlobalFree(pAadpterInfo);
		pAadpterInfo = (IP_ADAPTER_INFO*)GlobalAlloc(GMEM_ZEROINIT, outBufLen);
	}

	if ((dwRetVal = GetAdaptersInfo(pAadpterInfo, &outBufLen)) == NO_ERROR)
	{
		pAadpterInfoTmp = pAadpterInfo;
		myIP = "";

		while (pAadpterInfoTmp)
		{
			// 			if (pAadpterInfoTmp->Type == MIB_IF_TYPE_ETHERNET)
			// 			{
			if (pAadpterInfoTmp->CurrentIpAddress)
			{
				myIP = pAadpterInfoTmp->CurrentIpAddress->IpAddress.String;
			}
			else
			{
				myIP = pAadpterInfoTmp->IpAddressList.IpAddress.String;
			}
			//			}
			OutputDebugString(L"IP:" + myIP);
			if (!myIP.IsEmpty() && (myIP.CompareNoCase(_T("0.0.0.0")) != 0))
			{
				myMAC.Format(_T("%02X%02X%02X%02X%02X%02X"),
					pAadpterInfoTmp->Address[0],
					pAadpterInfoTmp->Address[1],
					pAadpterInfoTmp->Address[2],
					pAadpterInfoTmp->Address[3],
					pAadpterInfoTmp->Address[4],
					pAadpterInfoTmp->Address[5]);
				//tmpmacip.ip = myIP;
				//tmpmacip.mac = myMAC;
				//				macip.push_back(tmpmacip);
				OutputDebugString(L"MAC:" + myMAC);
				//break;
				if (myIP == cs_PC_IP)
				{
					bNetReady = true;
					cs_PC_Mac = myMAC;
					break;
				}
			}

			pAadpterInfoTmp = pAadpterInfoTmp->Next;
		}
	}
	GlobalFree(pAadpterInfo);

	return bNetReady;
}



void CGFC::Empty_Struct_REF_DATA_ALL_V2_S( REF_DATA_ALL_V2_S & oj_str_Ref_Data_All_V2 )
{
	int iAnt_Idx;

	//清空“天线列表”的结构体
	this->Empty_Struct_ANT_LIST_S( oj_str_Ref_Data_All_V2.str_Ref_Ant_List );

	//清空“金板”数据
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++ )
	{
		this->Empty_Struct_REF_DATA_BAND_S( oj_str_Ref_Data_All_V2.strr_Ref_Data_List[iAnt_Idx] );
	}
}


bool CGFC::Get_ANT_Type_List_Buff_Index( ANT_LIST_S str_Ant_List, EU_ANT_TYPE_INDEX eu_Ant_Type_Idx, int & oj_i_Ant_Buff_Index )
{
	//提取当前处理的ANT天线的结构体存储Index值

	int iBuff_Idx;

	//循环扫描ANT天线列表
	for( iBuff_Idx = 0; iBuff_Idx < str_Ant_List.i_ANT_Quan; iBuff_Idx++ )
	{
		if( eu_Ant_Type_Idx == str_Ant_List.strr_ANT_Prop_List[iBuff_Idx].eu_ANT_HW_Type_Name )
		{
			//找到了目标ANT了，此处返回pass
			//赋值返回数据
			oj_i_Ant_Buff_Index = iBuff_Idx;
			//pass
			return true;
		}
	}//for( iBuff_Idx = 0; iBuff_Idx < str_Ant_List.i_ANT_Quan; iBuff_Idx++


	//显示异常
	this->m_csError_Info.Format( _T( "ERROR, Get_ANT_Type_List_Buff_Index(), No ANT(%d) In List!" ), eu_Ant_Type_Idx );
	::AfxMessageBox( this->m_csError_Info );

	//搜索不到匹配天线，便返回fail
	return false;
}


void CGFC::Empty_Struct_ANT_CFG_ALL_V2_S( ANT_CFG_ALL_V2_S & oj_str_ANT_Config_All )
{
	int iAnt_Idx;


	//清空“天线列表”结构体
	this->Empty_Struct_ANT_LIST_S( oj_str_ANT_Config_All.str_ANT_List );


	//清空“线损”数据
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++ )
	{
		this->Empty_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_Config_All.strr_ANT_RF_Loss_List[iAnt_Idx] );
	}
}


void CGFC::Copy_Struct_ANT_CFG_ALL_V2_S( ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_Dest, ANT_CFG_ALL_V2_S str_ANT_CFG_Source )
{
	int iAnt_Idx;


	//Copy“ANT天线列表”结构体
	this->Copy_Struct_ANT_LIST_S( oj_str_ANT_CFG_Dest.str_ANT_List, str_ANT_CFG_Source.str_ANT_List );


	//Copy“线损”数据
	for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++ )
	{
		//Copy“线损 V2”结构体
		this->Copy_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_CFG_Dest.strr_ANT_RF_Loss_List[iAnt_Idx], str_ANT_CFG_Source.strr_ANT_RF_Loss_List[iAnt_Idx] );

	}//for( iAnt_Idx = 0; iAnt_Idx < ANT_TYPE_BUFF_SIZE; iAnt_Idx++
}


void CGFC::Copy_Struct_ANT_CFG_ALL_V2_S_To_ANT_CFG_ALL_S( ANT_CFG_ALL_S & oj_str_ANT_CFG_V1_Dest, const ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_V2_Source )
{
	//把“ANT_CFG_ALL_V2_S”结构体 Copy赋值 到“ANT_CFG_ALL_S”结构体中

	int iAnt_Idx;
	EU_ANT_TYPE_INDEX euANT_Type_Name;


	//Copy“ANT天线列表”结构体
	this->Copy_Struct_ANT_LIST_S( oj_str_ANT_CFG_V1_Dest.str_ANT_List, oj_str_ANT_CFG_V2_Source.str_ANT_List );


	//Copy“线损”数据
	for( iAnt_Idx = 0; iAnt_Idx < oj_str_ANT_CFG_V2_Source.str_ANT_List.i_ANT_Quan; iAnt_Idx++ )
	{
		//获取ANT天线名
		euANT_Type_Name = oj_str_ANT_CFG_V2_Source.str_ANT_List.strr_ANT_Prop_List[iAnt_Idx].eu_ANT_HW_Type_Name;

		//判断
		if( (euANT_Type_Name < 0) || (euANT_Type_Name >= ANT_TYPE_IDX_COUNT) )
		{
			CString csTemp;
			//显示异常
			csTemp.Format( _T( "ERROR, Copy_Struct_ANT_CFG_ALL_V2_S_To_ANT_CFG_ALL_S, ANT_Type_Name(%d) is wrong!" ), euANT_Type_Name );
			::AfxMessageBox( csTemp );
			//fail
			return;
		}

		//Copy“线损”结构体
		//把“V2 ANT天线的线损List的线损结构体（ANT天线列表Index号排序存储）”Copy到“V1 ANT天线的线损结构体（ANT天线名存储）”中
		this->Copy_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_CFG_V1_Dest.strr_ANT_RF_Loss[euANT_Type_Name], oj_str_ANT_CFG_V2_Source.strr_ANT_RF_Loss_List[iAnt_Idx] );

	}//for( iAnt_Idx = 0; iAnt_Idx < oj_str_ANT_CFG_V2_Source.str_ANT_List.i_ANT_Quan; iAnt_Idx++
}


void CGFC::Copy_Struct_ANT_CFG_ALL_S_To_ANT_CFG_ALL_V2_S( ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_V2_Dest, const ANT_CFG_ALL_S & oj_str_ANT_CFG_V1_Source )
{
	//把“ANT_CFG_ALL_S”结构体 Copy赋值 到“ANT_CFG_ALL_V2_S”结构体中

	int iAnt_Idx;
	EU_ANT_TYPE_INDEX euANT_Type_Name;


	//Copy“ANT天线列表”结构体
	this->Copy_Struct_ANT_LIST_S( oj_str_ANT_CFG_V2_Dest.str_ANT_List, oj_str_ANT_CFG_V1_Source.str_ANT_List );


	//Copy“线损”数据
	for( iAnt_Idx = 0; iAnt_Idx < oj_str_ANT_CFG_V1_Source.str_ANT_List.i_ANT_Quan; iAnt_Idx++ )
	{
		//获取ANT天线名
		euANT_Type_Name = oj_str_ANT_CFG_V1_Source.str_ANT_List.strr_ANT_Prop_List[iAnt_Idx].eu_ANT_HW_Type_Name;

		//判断
		if( (euANT_Type_Name < 0) || (euANT_Type_Name >= ANT_TYPE_IDX_COUNT) )
		{
			CString csTemp;
			//显示异常
			csTemp.Format( _T( "ERROR, Copy_Struct_ANT_CFG_ALL_S_To_ANT_CFG_ALL_V2_S, ANT_Type_Name(%d) is wrong!" ), euANT_Type_Name );
			::AfxMessageBox( csTemp );
			//fail
			return;
		}

		//Copy“线损”结构体
		//把“V1 ANT天线的线损结构体（ANT天线名存储）”Copy到“V2 ANT天线的线损List的线损结构体（ANT天线列表Index号排序存储）”中
		this->Copy_Struct_TESTER_RF_OFFSET_V2_S( oj_str_ANT_CFG_V2_Dest.strr_ANT_RF_Loss_List[iAnt_Idx], oj_str_ANT_CFG_V1_Source.strr_ANT_RF_Loss[euANT_Type_Name] );

	}//for( iAnt_Idx = 0; iAnt_Idx < oj_str_ANT_CFG_V1_Source.str_ANT_List.i_ANT_Quan; iAnt_Idx++
}


bool CGFC::Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S( const ANT_CFG_ALL_V2_S & oj_str_ANT_CFG_V2, EU_ANT_TYPE_INDEX eu_Ant_Type_Idx, int i_Format_Network, int i_Band_Name, int i_L_M_H, double & oj_d_RF_Loss_Tx_Input, double & oj_d_RF_Loss_Rx_Output )
{
	int iANT_Buff_Index = 0;//ANT天线的结构体存储index
	int iBand_Index = 0;//Band 频段的 index 号

	//
	try
	{
		//检查L/M/H值
		if( (i_L_M_H < CH_LOW) || (i_L_M_H > CH_HIGH) )
		{
			this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, L_M_H(%d) is Wrong!" ), i_L_M_H );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			throw(0);
		}


		//提取当前处理的ANT天线的结构体存储index
		if( this->Get_ANT_Type_List_Buff_Index( oj_str_ANT_CFG_V2.str_ANT_List, eu_Ant_Type_Idx, iANT_Buff_Index ) == false )
		{
			//显示错误信息
			this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No_ANT_%d_Buff_Index!" ), eu_Ant_Type_Idx );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			throw(0);
		}


		//网络制式switch
		switch( i_Format_Network )//网络制式switch
		{
		case FORMAT_GSM_S:
		case FORMAT_GSM_NS:
			{
				//2G的 i_Band_Name 就是index号
				iBand_Index = i_Band_Name;

				//提取、赋值所需要的线损值
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_GSM_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_GSM_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_WCDMA_S:
		case FORMAT_WCDMA_NS:
			{
				//频段名 转 频段index
				if( this->Get_WCDMA_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//显示错误信息
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No WCDMA Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//提取、赋值所需要的线损值
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_W_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_W_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_TD_NS:
			{
				//频段名 转 频段index
				if( this->Get_TD_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//显示错误信息
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No TD-SCDMA Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//提取、赋值所需要的线损值
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_TD_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_TD_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_C2K_S:
		case FORMAT_C2K_NS:
			{
				//频段名 转 频段index
				if( this->Get_CDMA2000_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//显示错误信息
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No cdma2000 Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//提取、赋值所需要的线损值
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_C2K_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_C2K_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_LTE_NS:
			{
				//频段名 转 频段index
				if( this->Get_LTE_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//显示错误信息
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No LTE Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//提取、赋值所需要的线损值
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_LTE_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_LTE_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_GPS_NS:
			{
				//提取、赋值所需要的线损值
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_GPS_RF_Loss[RF_TX_UL_INPUT];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_GPS_RF_Loss[RF_RX_DL_OUTPUT];
			}
			//
			break;

		case FORMAT_WIFI_NS:
			{
				//频段名 转 频段index
				if( this->Get_WiFi_Standard_FreqBand( i_Band_Name, &iBand_Index ) == false )
				{
					//显示错误信息
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No WiFi_Standard_FreqBand(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//提取、赋值所需要的线损值
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_WiFi_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_WiFi_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_BLUETOOTH_NS:
			{
				//提取、赋值所需要的线损值
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_BLU_RF_Loss[RF_TX_UL_INPUT];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_BLU_RF_Loss[RF_RX_DL_OUTPUT];
			}
			//
			break;

		case FORMAT_NBIOT_S:
		case FORMAT_NBIOT_NS:
			{
				//频段名 转 频段index
				if( this->Get_NBIoT_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//显示错误信息
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No NB-IoT Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//提取、赋值所需要的线损值
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_NBIoT_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_NBIoT_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_CV2X_NS:
			{
				//频段名 转 频段index
				if( this->Get_CV2X_Band_Index( i_Band_Name, &iBand_Index ) == false )
				{
					//显示错误信息
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No C-V2X Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//提取、赋值所需要的线损值
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_CV2X_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_CV2X_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		case FORMAT_NR5GSUB6_NS:
			{
				//频段名 转 频段index
				if( this->Get_NR_5G_SUB6_Band_Index( i_Band_Name, iBand_Index ) == false )
				{
					//显示错误信息
					this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No NR_5G_SUB6 Band(%d)!" ), i_Band_Name );
					::AfxMessageBox( this->m_csError_Info );
					//fail
					throw(0);
				}

				//提取、赋值所需要的线损值
				oj_d_RF_Loss_Tx_Input = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_NR_5G_SUB6_RF_Loss[iBand_Index * 6 + RF_TX_UL_INPUT * 3 + i_L_M_H];
				oj_d_RF_Loss_Rx_Output = oj_str_ANT_CFG_V2.strr_ANT_RF_Loss_List[iANT_Buff_Index].dr_NR_5G_SUB6_RF_Loss[iBand_Index * 6 + RF_RX_DL_OUTPUT * 3 + i_L_M_H];
			}
			//
			break;

		default:
			//显示错误信息
			this->m_csError_Info.Format( _T( "ERROR, Get_RF_Loss_Data_From_ANT_CFG_ALL_V2_S, No Format_Network(%d)!" ), i_Format_Network );
			::AfxMessageBox( this->m_csError_Info );
			//fail
			throw(0);
		}//switch( i_Format_Network

	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

bool CGFC::Get_Rx_Chain_Type_Name( int i_Rx_Chain_Type, CString & oj_cs_Rx_Chain_Type_Name )
{
	//Rx_Chain类型 转 Rx_Chain名

	//
	try
	{
		switch( i_Rx_Chain_Type )
		{
		case RX_CHAIN_MAIN:
			oj_cs_Rx_Chain_Type_Name.Format( _T( "MAIN" ) );
			break;
		case RX_CHAIN_DIV:
			oj_cs_Rx_Chain_Type_Name.Format( _T( "DIV" ) );
			break;
		case RX_CHAIN_GPS:
			oj_cs_Rx_Chain_Type_Name.Format( _T( "GPS" ) );
			break;
		case RX_CHAIN_MAIN_DIV:
			oj_cs_Rx_Chain_Type_Name.Format( _T( "MAIN_DIV" ) );
			break;
		default:
			//fail
			throw(0);
		}//switch( i_Rx_Chain_Type
	}
	catch( ... )
	{
		//fail信息
		this->m_csError_Info.Format( _T( "ERROR, Get_Rx_Chain_Type_Name, Rx_Chain_Type(%d) is Wrong!" ), i_Rx_Chain_Type );
		::AfxMessageBox( this->m_csError_Info );

		//fail
		return false;
	}

	//pass
	return true;
}


bool CGFC::Determine_Type_Of_Plan_Connect_ANT_And_RF_Port( const ANT_CFG_ALL_V2_S str_ANT_CFG_ALL_V2, int & oj_i_Plan_Connect_ANT_And_RF_Port )
{
	//判定“ANT天线组合与仪器端RF口”接线方案

	//
	try
	{
		//默认RF口连接方案
		oj_i_Plan_Connect_ANT_And_RF_Port = PLAN_ANT4_MAIN1_DIV1_GPS1_WIFI1;//传统的单Main主天线的方案


		//判断是否是其他RF口连接方案
		if( (4 == str_ANT_CFG_ALL_V2.str_ANT_List.i_ANT_Quan) &&
			(ANT_TYPE_IDX_MAIN_1 == str_ANT_CFG_ALL_V2.str_ANT_List.strr_ANT_Prop_List[0].eu_ANT_HW_Type_Name) &&
			(ANT_TYPE_IDX_MAIN_2 == str_ANT_CFG_ALL_V2.str_ANT_List.strr_ANT_Prop_List[1].eu_ANT_HW_Type_Name) &&
			(ANT_TYPE_IDX_DIV_1 == str_ANT_CFG_ALL_V2.str_ANT_List.strr_ANT_Prop_List[2].eu_ANT_HW_Type_Name) &&
			(ANT_TYPE_IDX_DIV_2 == str_ANT_CFG_ALL_V2.str_ANT_List.strr_ANT_Prop_List[3].eu_ANT_HW_Type_Name) )
		{
			//确定是4根ANT天线, “2根MAIN主天线 + 2根DIV分集天线”
			oj_i_Plan_Connect_ANT_And_RF_Port = PLAN_ANT4_MAIN2_DIV2;//2根MAIN主天线 + 2根DIV分集天线
		}

	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

//查找某进程（process）
bool CGFC::Find_Process( const wchar_t * wcp_Process_Name )
{
	//

	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;

	PROCESSENTRY32 strProcess_Info;
	CString csCheck, csTgt_Name;
	BOOL bStart= FALSE;

	BOOL bFind = FALSE;

	g_del_exe_Ctrl.Lock(); //

	try
	{
		//给系统内所有的进程拍个快照
		hProcessSnap = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

		if( hProcessSnap == INVALID_HANDLE_VALUE )
		{
			throw(0);
		}


		//载入目标名
		csTgt_Name.Format( _T( "%s" ), wcp_Process_Name );
		csTgt_Name.MakeLower();


		//在使用这个结构前，先设置它的大小
		strProcess_Info.dwSize = sizeof( PROCESSENTRY32 );

		//遍历当前的进程列表，寻找是否有用户输入的进程
		//遍历进程快照，轮流显示每个进程的信息
		bStart = Process32First( hProcessSnap, &strProcess_Info );

		while( bStart )
		{
			//获得进程列表中的进程名
			csCheck.Format( _T( "%s" ), strProcess_Info.szExeFile );
			csCheck.MakeLower();

			if( csCheck.Compare( csTgt_Name ) == 0 )//Zero if the strings are identical
			{
			   	bFind = TRUE;
				break;
			}

			//找下一个
			bStart = Process32Next( hProcessSnap, &strProcess_Info );
		}//while(bStart

		//注意：关闭这个句柄的方式还有争议
		CloseHandle( hProcessSnap );

		if (TRUE == bFind)
		{
			g_del_exe_Ctrl.Unlock();
			return true;
		}
		else
		{
			g_del_exe_Ctrl.Unlock();
			return false;
		}
	}
	catch( ... )
	{
		g_del_exe_Ctrl.Unlock();
		return false;
	}

	g_del_exe_Ctrl.Unlock();
	return true;
}
