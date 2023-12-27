/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */
/// @defgroup CALIB CALIBration Driver
///  @{
///

#ifndef _CALIB_M_H_
#define _CALIB_M_H_


#include "cs_types.h"

// =============================================================================
//  MACROS
// =============================================================================
/// Version of the CALIBration stub and code (on 8 bits).
#define CALIB_NB_MAJ_VERSION                       (2)
#define CALIB_NB_MIN_VERSION                       (7)
#define CALIB_NB_MARK_VERSION                      (0XCA1B0000)
#define CALIB_NB_VERSION_NUMBER                    (( CALIB_NB_MARK_VERSION | CALIB_NB_MAJ_VERSION << 8 | CALIB_NB_MIN_VERSION ))
#define CALIB_NB_STUB_VERSION                      (0XCA5B0001)
/// Number of CALIB timings for the transceiver.
#define CALIB_NB_XCV_TIME_QTY                      (20)
/// Number of CALIB timings for PAL.
#define CALIB_NB_PAL_TIME_QTY                      (20)
/// Number of generic parameters for the transceiver.
#define CALIB_NB_XCV_PARAM_QTY                     (40)
/// Mask for transceiver RF name.
#define CALIB_NB_XCV_MASK                          ((1 << 24))
/// Mask for PA RF name.
#define CALIB_NB_PA_MASK                           ((1 << 25))
/// Mask for switch RF name.
#define CALIB_NB_SW_MASK                           ((1 << 26))
/// Unrealistic values meaning that the power measure is not complete yet.
#define CALIB_NB_STUB_SEARCH_POWER                 (0X0)
/// Unrealistic values meaning that the FOf measure is not complete yet.
#define CALIB_NB_STUB_SEARCH_FOF                   (-2000000)
/// For communication between CALIB Stub and CALIBration tools.
#define CALIB_NB_STUB_XTAL_IDLE                    (-2000001)
/// For communication between CALIB Stub and CALIBration tools.
#define CALIB_NB_STUB_PA_PROF_IDLE                 (-2000002)
/// For communication between CALIB Stub and CALIBration tools.
#define CALIB_NB_STUB_ILOSS_IDLE                   (-128)
/// For communication between CALIB Stub and CALIBration tools.
#define CALIB_NB_STUB_DCO_IDLE                     (-32768)
/// For communication between CALIB Stub and CALIBration tools.
#define CALIB_NB_STUB_DCO_ERROR                    (-32767)
/// For communication between CALIB Stub and CALIBration tools.
#define CALIB_NB_STUB_GPADC_ERROR                  (0XFFFF)
/// For communication between CALIB Stub and CALIBration tools.
#define CALIB_NB_STUB_GPADC_IDLE                   (0XFFFE)



// ============================================================================
// CALIB_NB_GLOBALS_T
// =============================================================================
typedef struct {
} CALIB_NB_GLOBALS_T; //Size : 0x0

// ============================================================================
// CALIB_NB_ARFCN_T
// =============================================================================
typedef INT16 CALIB_NB_ARFCN_T[2];


// ============================================================================
//                          CALIB_NB_STUB_BAND_T
// -----------------------------------------------------------------------------
// =============================================================================
typedef enum {
    CALIB_NB_STUB_BAND_1                            = 0x00000001,
    CALIB_NB_STUB_BAND_2                            = 0x00000002,
    CALIB_NB_STUB_BAND_3                            = 0x00000003,
    CALIB_NB_STUB_BAND_5                            = 0x00000005,
    CALIB_NB_STUB_BAND_8                            = 0x00000008,
    CALIB_NB_STUB_BAND_20                           = 0x00000014,
    CALIB_NB_STUB_BAND_QTY                          = 0x00000006
} CALIB_NB_STUB_BAND_T;

/// This magiv tag is used as a parameter to the boot loader to force is to run the
/// CALIBration stub
#define CALIB_NB_MAGIC_TAG                        (0XDB1ADB1A)

// ============================================================================
// CALIB_NB_VERSION_TAG_T
// =============================================================================
typedef UINT32 CALIB_NB_VERSION_TAG_T;

// ============================================================================
// CALIB_NB_METHOD_T
// -----------------------------------------------------------------------------
/// CALIB process method type.
// =============================================================================
typedef enum {
    CALIB_NB_METH_DEFAULT                          = 0xCA11E700,
    CALIB_NB_METH_COMPILATION                      = 0xCA11E701,
    CALIB_NB_METH_MANUAL                           = 0xCA11E702,
    CALIB_NB_METH_AUTOMATIC                        = 0xCA11E703,
    CALIB_NB_METH_SIMULATION                       = 0xCA11E704
} CALIB_NB_METHOD_T;

// ============================================================================
// CALIB_NB_OP_INFO_T
// -----------------------------------------------------------------------------
/// CALIB process method and date type.
// =============================================================================
typedef struct {
    UINT32                            date;                         //0x00000000
    CALIB_NB_METHOD_T                 method;                       //0x00000004
} CALIB_NB_OP_INFO_T; //Size : 0x8


// ============================================================================
// CALIB_NB_XCV_TIMES_T
// =============================================================================
typedef INT16 CALIB_NB_XCV_TIMES_T[CALIB_NB_XCV_TIME_QTY];

// ============================================================================
// CALIB_NB_XCV_PARAM_T
// =============================================================================
typedef INT32 CALIB_NB_XCV_PARAM_T[CALIB_NB_XCV_PARAM_QTY];

// ============================================================================
// CALIB_PAL_TIMES_T
// =============================================================================
typedef INT16 CALIB_NB_PAL_TIMES_T[CALIB_NB_PAL_TIME_QTY];

// ============================================================================
// CALIB_NB_XCV_PALCUST
// =============================================================================
typedef UINT32 CALIB_NB_RF_CHIP_NAME_T;                               //Chip name
typedef INT16 CALIB_NB_RXTX_FREQ_OFFSET_T[CALIB_NB_STUB_BAND_QTY];    // CALIB_NB_RXTX_FREQ_OFFSET_T
typedef INT16 CALIB_NB_RXTX_TIME_OFFSET_T;                            // CALIB_NB_RXTX_TIME_OFFSET_T
typedef INT16 CALIB_NB_RXTX_IQ_TIME_OFFSET_T;                         // CALIB_NB_RXTX_IQ_TIME_OFFSET_T
typedef INT16 CALIB_NB_DCO_CAL_TIME_T;                                // CALIB_NB_DCO_CAL_TIME_T

typedef struct {
    CALIB_NB_RF_CHIP_NAME_T           name;                         //0x00000000
    CALIB_NB_RXTX_FREQ_OFFSET_T       rxTxFreqOffset;               //0x00000004
    CALIB_NB_RXTX_TIME_OFFSET_T       rxTxTimeOffset;               //0x0000000C
    CALIB_NB_RXTX_IQ_TIME_OFFSET_T    rxIqTimeOffset;               //0x0000000E
    CALIB_NB_RXTX_IQ_TIME_OFFSET_T    txIqTimeOffset;               //0x00000010
    CALIB_NB_DCO_CAL_TIME_T           dcoCalTime;                   //0x00000012
    INT32                          spare[7];                     //0x00000014
} CALIB_NB_XCV_PALCUST_T; //Size : 0x30

// ============================================================================
// CALIB_NB_XCV_T
// -----------------------------------------------------------------------------
/// Transceiver CALIBration.
// =============================================================================
typedef struct {
    CALIB_NB_XCV_TIMES_T              times;                        //0x00000000
    CALIB_NB_XCV_PARAM_T              param;                        //0x00000028
    CALIB_NB_XCV_PALCUST_T            palcust;                      //0x00000078
} CALIB_NB_XCV_T; //Size : 0xA8

// ============================================================================
// CALIB_NB_BB_T
// =============================================================================
typedef struct
{
    CALIB_NB_PAL_TIMES_T            times;                        //0x00000000
} CALIB_NB_BB_T;

// ============================================================================
// CALIB_NB_RF_CRC_T
// =============================================================================
typedef UINT32 CALIB_NB_RF_CRC_T;

// ============================================================================
//
//                            CALIB_NB_BUFFER_T
//
// =============================================================================
typedef struct {
    /// Information about this CALIB buffer.
    CALIB_NB_VERSION_TAG_T            versionTag;                   //0x00000000
    /// CALIB process method and date type.
    CALIB_NB_OP_INFO_T                opInfo;                       //0x00000004
    /// Transceiver CALIBration.
    CALIB_NB_XCV_T                    xcv;                          //0x0000000C
    ///Baseband calibration
    CALIB_NB_BB_T                      bb;                           //0x00001360
    /// Reserved for future use.
    UINT32                          reserved[2];                     //0x00001CEC
    /// CRC value for RF CALIBration data.
    CALIB_NB_RF_CRC_T                 rfCrc;                        //0x00001CF8
} CALIB_NB_BUFFER_T; //Size : 0x1CFC


// ============================================================================
// CALIB_NB_PARAM_STATUS_T
// -----------------------------------------------------------------------------
/// CALIBration parameter type identifier.
// =============================================================================
typedef enum {
    CALIB_NB_PARAM_DEFAULT                         = 0xCA11B042,
    CALIB_NB_PARAM_DEFAULT_RF_MIS                  = 0xCA11B043,
    CALIB_NB_PARAM_INIT_ERROR                      = 0xCA10DEAD,
    CALIB_NB_PARAM_RF_CALIBRATED                   = 0x007F0011,
    CALIB_NB_PARAM_RF_MIS                          = 0x00A0D043,
    CALIB_NB_PARAM_CALIBRATED                      = 0x00DEF011
} CALIB_NB_PARAM_STATUS_T;


// ============================================================================
// CALIB_NB_COMMAND_T
// =============================================================================
typedef enum {
/// Command used by the CALIBration Embedded Stub to inform HST that the command
/// is done.
    CALIB_NB_CMD_DONE                              = 0xCA11B042,
    CALIB_NB_CMD_NOT_ACCESSIBLE                    = 0xCA11B043,
    CALIB_NB_CMD_UPDATE                            = 0xCA11B044,
    CALIB_NB_CMD_UPDATE_ERROR                      = 0xCA11B045,
    CALIB_NB_CMD_RF_FLASH_BURN                     = 0xCA11B048,
    CALIB_NB_CMD_FLASH_ERASE                       = 0xCA11B049,
    CALIB_NB_CMD_FLASH_ERROR                       = 0xCA11B04A,
    CALIB_NB_CMD_RF_RESET                          = 0xCA11B04B,
    CALIB_NB_CMD_RESET                             = 0xCA11B04D,
    CALIB_NB_CMD_FLASH_BURN                        = 0xCA11B04F,
    CALIB_NB_CMD_CFP_BURN                          = 0xCA11B050
} CALIB_NB_COMMAND_T;

// ============================================================================
// CALIB_NB_STUB_CMDS_T
// -----------------------------------------------------------------------------
/// Values used to define the contexts of the CALIB Stub.
// =============================================================================
typedef enum {
    CALIB_NB_STUB_INIT_STATE                         = 0x00000000,
    CALIB_NB_STUB_CS_STATE                           = 0x00000001,
    CALIB_NB_STUB_MIB_STATE                          = 0x00000002,
    CALIB_NB_STUB_SIB1_STATE                         = 0x00000003,
    CALIB_NB_STUB_FOF_STATE                          = 0x00000004,
    CALIB_NB_STUB_TX_STATE                           = 0x00000005,
    CALIB_NB_STUB_RX_STATE                           = 0x00000006,
    CALIB_NB_STUB_IDLE_SIDE                          = 0x00000007,
    CALIB_NB_STUB_NST_STATE                          = 0x00000008,
    CALIB_NB_STUB_NSTREQ_STATE                       = 0x00000009,
} CALIB_NB_STUB_CMDS_T;

// ============================================================================
// CALIB_NB_STUB_CTX_T
// -----------------------------------------------------------------------------
/// Used to send CALIBration context change requests from the remote CALIBration
/// tools to the CALIB Stub.
// =============================================================================
typedef struct {
    /// Commands from the remote CALIBration tools.
    VOLATILE CALIB_NB_STUB_CMDS_T     state;                        //0x00000000
    VOLATILE BOOL                  firstFint;                    //0x00000004
    /// Subcommands from the remote CALIBration tools.
    VOLATILE INT32                 setXtalFreqOffset;            //0x00000008
    VOLATILE UINT8                 setAfcBound;                  //0x0000000C
    VOLATILE UINT8                 setChangeDACAfcBound;         //0x0000000D
    VOLATILE INT32                 setChangeDACAfcFreqOffset;    //0x00000010
    VOLATILE INT32                 setChangeDACAfcFreq;          //0x00000014
    VOLATILE INT32                 setAfcFreqOffset;             //0x00000018
    VOLATILE INT8                  setILossRxOffset;               //0x0000001C
    VOLATILE INT16                 setMeaseTxPower;               //0x00000020
    VOLATILE INT8                  setILossTxOffest;               //0x00000020
    VOLATILE INT8                  setCALIBUpdate;               //0x00000024
    /// Cells information
    VOLATILE UINT16                eArfcn;                       //0x00000026
    VOLATILE UINT8                 band;                         //0x0000003A
    VOLATILE INT16                 cellPower;                    //0x0000003C
    VOLATILE UINT16                cellId;                        //0x00000040
    VOLATILE UINT8                 antPortNum;                     //0x00000041
    VOLATILE UINT8                 RAN88ScrChqEn;                     //0x00000041
    VOLATILE UINT16                crnti;                         //0x00000042
    VOLATILE INT32                 tOf;                          //0x00000048
    VOLATILE INT32                 fOf;                          //0x0000004C
    VOLATILE UINT16                snR;                          //0x00000050
    VOLATILE INT32                 meanFOf;                      //0x00000058
    /// This is initialized by HST
    VOLATILE UINT8                 xtalCALIBDone;                //0x0000005C
    /// This is initialized by HST
    VOLATILE UINT8                 txPowerCALIBDone;                //0x0000005C
    /// Status of the iloss CALIBration porcess. This is initialized by HST.
    UINT8                          iLossCalibDone;               //0x000000E8
    /// ARFCN for which to measure the insertion loss. This is not initialized.
    UINT16                         iLossNextArfcn;               //0x000000EA
    INT16                         iTXLossNextPower;               //0x000000EA
    /// Parameters for Tx commands from the remote CALIBration tools.
    VOLATILE UINT16                txArfcn;                      //0x000004F6
    VOLATILE INT16                 txPower;                        //0x000004F8
    VOLATILE UINT8                 txBand;                       //0x000004FC
    VOLATILE INT8                 apcAnaGain;                      //0x000004FC
    VOLATILE INT16                 measRssi;                      //0x000004FC
} CALIB_NB_STUB_CTX_T; //Size : 0x518


// ============================================================================
// NSTARFRSSI_T_T
// =============================================================================
typedef struct {
    VOLATILE UINT16                Arfcn;                        //0x00000000
    VOLATILE UINT16                Rssi;                         //0x00000002
} NSTNbARFRSSI_T; //Size : 0x4

// ============================================================================
// NSTPOWERREQ_T_T
// -----------------------------------------------------------------------------
// =============================================================================
typedef struct {
    VOLATILE UINT16                Nb;                           //0x00000000
    VOLATILE UINT16                Index;                        //0x00000002
    VOLATILE NSTNbARFRSSI_T        List[548];                    //0x00000004
} NSTNbPOWERREQ_T; //Size : 0x894

// ============================================================================
// NST_MEASCFG_T_T
// -----------------------------------------------------------------------------
// =============================================================================
typedef struct {
    VOLATILE UINT8                 NbMeas;                       //0x00000000
    VOLATILE UINT8                 NbMeasSub;                    //0x00000001
    VOLATILE UINT16                MeasCumul;                    //0x00000002
    VOLATILE UINT16                MeasCumulSub;                 //0x00000004
    VOLATILE UINT16                TotalBit;                     //0x00000006
    VOLATILE UINT16                TotalBitSub;                  //0x00000008
    VOLATILE UINT16                WrongBitCumul;                //0x0000000A
    VOLATILE UINT16                WrongBitCumulSub;             //0x0000000C
    VOLATILE UINT8                 RxLevFull;                    //0x0000000E
    VOLATILE UINT8                 RxLevSub;                     //0x0000000F
    VOLATILE UINT8                 RlaVal;                       //0x00000010
    VOLATILE UINT8                 RxQualFull;                   //0x00000011
    VOLATILE UINT8                 RxQualSub;                    //0x00000012
} NST_NB_MEASCFG_T; //Size : 0x14

// ============================================================================
// CALIB_NB_STUB_NST_CTX_T
// =============================================================================
typedef struct {
    VOLATILE UINT32                nststate;                        //0x00000000
    VOLATILE UINT32                nstreqstate;                  //0x00000004
    VOLATILE UINT32                nstreqcurrstate;              //0x00000008
    VOLATILE UINT8                 modeId;                       //0x0000000C
    VOLATILE UINT8                 loopback;                     //0x0000000D
    VOLATILE UINT8                 syncPhase;                    //0x0000000E
    VOLATILE UINT8                 codec;                        //0x0000000F
    VOLATILE UINT8                 order;                        //0x00000010
    VOLATILE UINT8                 logEnable;                    //0x00000011
    VOLATILE UINT8                 num_samples;                  //0x00000012
    VOLATILE UINT32                num_PDCCH;                 //0x00000014
    VOLATILE UINT32                num_PDSCH;                     //0x00000018
    VOLATILE UINT32                num_errors;                   //0x0000001C
    VOLATILE UINT8                 pn_type;                      //0x00000020
    VOLATILE UINT8                 ber_mode;                     //0x00000021
    VOLATILE UINT8                 invert;                       //0x00000022
    VOLATILE UINT8                 pad;                          //0x00000023
    VOLATILE UINT16                acq_threshold;                //0x00000024
    VOLATILE UINT16                acq_lost_threshold;           //0x00000026
    VOLATILE UINT8                 rspcId;                       //0x00000028
    VOLATILE UINT8                 status;                       //0x00000029
    VOLATILE UINT8                 band;                         //0x0000002E
    VOLATILE UINT8                 monPower;                     //0x0000002F
    VOLATILE UINT8                 nbPower;                      //0x00000030
    VOLATILE UINT8                 txPower;                        //0x00000031
} CALIB_NB_STUB_NST_CTX_T; //Size : 0xB60


// ============================================================================
// CALIB_NB_CALIBRATION_T
// -----------------------------------------------------------------------------
/// This struct will contain pointers to the CALIBration info and to the struct where
/// to put the CALIBration context requests. It also contains the address of the
/// CALIBration sector in flash.
// =============================================================================
typedef struct {
    CALIB_NB_VERSION_TAG_T            codeVersion;                  //0x00000000
    CALIB_NB_PARAM_STATUS_T           paramStatus;                  //0x00000004
    CALIB_NB_COMMAND_T                command;                      //0x00000008
    CALIB_NB_OP_INFO_T*               opInfo;                       //0x0000000C
    CALIB_NB_XCV_T*                   xcv;                          //0x00000010
    VOLATILE CALIB_NB_OP_INFO_T*      hstOpInfo;                    //0x00000014
    VOLATILE CALIB_NB_XCV_T*          hstXcv;                       //0x00000018
    VOLATILE CALIB_NB_STUB_CTX_T*     stubCtx;                      //0x0000001C
    CONST CALIB_NB_BUFFER_T*          flash;                        //0x00000020
    VOLATILE CALIB_NB_STUB_NST_CTX_T* stubNsftCtx;                  //0x00000024
} CALIB_NB_CALIBRATION_T; //Size : 0x24


// ============================================================================
// CALIB_NB_H_ENUM_0_T
// -----------------------------------------------------------------------------
/// Used for autonomous CALIB processes, results from CALIB Stub to remote.
// =============================================================================
typedef enum {
    CALIB_NB_PROCESS_STOP                          = 0x00000000,
    CALIB_NB_PROCESS_CONTINUE                      = 0x00000001,
    CALIB_NB_PROCESS_PENDING                       = 0x00000002,
    CALIB_NB_PROCESS_NEED_CALM                     = 0x00000003,
    CALIB_NB_PROCESS_ERR_BAD_POW                   = 0x000000F0,
    CALIB_NB_PROCESS_ERR_NO_MONO_POW               = 0x000000F1,
    CALIB_NB_PROCESS_ERR_ZERO_DAC                  = 0x000000F2,
    CALIB_NB_PROCESS_ERROR                         = 0x000000FF
} CALIB_NB_H_ENUM_0_T;


// ============================================================================
// CALIB_NB_NST_STAT_T
// -----------------------------------------------------------------------------
// =============================================================================
typedef enum {
    NST_NO_STATE                                = 0x00000000,
    NST_IDLE_STATE                              = 0x00000001,
    NST_FREQSCAN_STATE                          = 0x00000002,
    NST_CELLSEARCH_STATE                        = 0x00000003,
    NST_MIB_STATE                               = 0x00000004,
    NST_SIB1_STATE                              = 0x00000005,
    NST_CONNECTED_STATE                         = 0x00000006,
    NST_CACULATION_RX_STATE                     = 0x00000007,
    NST_CACULATION_TX_STATE                     = 0x00000008,
    NST_STOP_RX_SYNC_STATE                      = 0x00000009,
    NST_SIMU_LOG_STATE                          = 0x0000000A,
    NST_DISCONN_STATE                           = 0x0000000B
} CALIB_NB_NST_STAT_T;


// ============================================================================
// CALIB_NB_STUB_AFC_BOUND_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum {
    CALIB_NB_STUB_AFC_BOUND_CENTER                 = 0x00000000,
    CALIB_NB_STUB_AFC_BOUND_NEG_FREQ               = 0x00000001,
    CALIB_NB_STUB_AFC_BOUND_POS_FREQ               = 0x00000002,
    CALIB_NB_STUB_AFC_BOUND_NO                     = 0x00000003
} CALIB_NB_STUB_AFC_BOUND_T;

/// Value that defines the number of measure to do before the DC offset average is
/// considered valid.
#define CALIB_NB_DCO_ACC_COUNT                     (32)

// ============================================================================
// CALIB_NB_BITINFO_T_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef struct {
    VOLATILE UINT16                num_bits;                     //0x00000000
    VOLATILE UINT16                num_errors;                   //0x00000002
} CALIB_NB_BITINFO_T_T; //Size : 0x4


//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define CALIB_NB_MAJ_MASK(x) ((x >> 8) & 0xFF)
#define CALIB_NB_MIN_MASK(x) (x & 0xFF)
#define CALIB_NB_MARK_MASK(x) (x & 0xFFFF0000)
#define CALIB_NB_VERSION(maj, min) (CALIB_NB_MARK_VERSION | maj << 8 | min << 0)

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

//=============================================================================
// CALIB_NB_DaemonInit
//-----------------------------------------------------------------------------
/// At boot time, if the CALIB_NB_TAG_CALIBRATED is in the validTag field of
/// the CALIBration flash sector, CALIB uses the CALIBrated parameters
/// (i.e. the parameters present in the CALIB flash sector), else CALIB
/// uses the default parameters.
/// @par
/// If the code has been compiled with the CALIBration mode flag set, at boot
/// time, this function also uses the remote parameters RAM buffer and copy
/// the default parameters in it.
/// @par
/// This must be called in the PAL initialization, after the initialization
/// of the RF drivers.
//=============================================================================
PUBLIC VOID nCALIB_DaemonInit(VOID);


//=============================================================================
// CALIB_NB_DaemonProcess
//-----------------------------------------------------------------------------
/// Execute the CALIBration commands received by the remote CALIB tools. This
/// will, usually, update the CALIBration buffers with new values provided by
/// the user controlling the CALIB tools.
/// @par
/// This must be called in the PAL start frame.
//=============================================================================
PUBLIC VOID nCALIB_DaemonProcess(VOID);


//=============================================================================
// CALIB_NB_GetPointers
//-----------------------------------------------------------------------------
/// Access to the CALIBration parameters.
///
/// @return The global pointers used by the code to access the CALIBrated
/// parameters. When the target's flash does not contain a flash sector,
/// the CALIB parameters are initialized with default parameters during
/// the CALIBration init. Also returns the pointers used by the remote
/// CALIBration tools to get the version number of the CALIBration and to
/// find the CALIBration buffers.
//=============================================================================
PUBLIC CALIB_NB_CALIBRATION_T* nCALIB_GetPointers(VOID);


//=============================================================================
// CALIB_NB_SetStubContext
//-----------------------------------------------------------------------------
/// Used by the CALIB Stub to define the CALIBration context structure that
/// the remote CALIBration tools must use to communicate with the CALIB Stub.
///
/// @param ctx Pointer to the context where the remote CALIB tools must put
/// its CALIBration command to be executed by the CALIB Stub.
//=============================================================================
PUBLIC VOID nCALIB_SetStubContext(CONST CALIB_NB_STUB_CTX_T *ctx);


//=============================================================================
// CALIB_NB_SetXcvDefaults
//-----------------------------------------------------------------------------
/// Used by the transceiver RF driver to set its default CALIBration parameters
/// (see #CALIB_NB_DaemonInit to know when they can be used). It must be called
/// during the RF transceiver driver initialization.
///
/// @param param Pointer to a structure containing the default values to be
/// used by this transceiver model.
//=============================================================================
PUBLIC VOID nCALIB_SetXcvDefaults(CONST CALIB_NB_XCV_T* param);


//=============================================================================
// CALIB_NB_DaemonTracePrint
//-----------------------------------------------------------------------------
/// Display in the trace the current CALIBration status, used for debug.
//=============================================================================
PUBLIC VOID nCALIB_DaemonTracePrint(VOID);


// =============================================================================
// CALIB_NB_DaemonSetContext
// -----------------------------------------------------------------------------
/// Defines the context structure that HST and CES must use.
/// @param ctx Pointer to the context to use.
// =============================================================================
PUBLIC VOID nCALIB_DaemonSetContext(VOID* ctx);


// =============================================================================
// CALIB_NB_DaemonUsrDataProcess
// -----------------------------------------------------------------------------
/// Apply and burn HST CALIBration buffer.
/// @return TRUE if success, and FALSE otherwise.
// =============================================================================
PUBLIC BOOL nCALIB_DaemonUsrDataProcess(VOID);


// =============================================================================
// CALIB_NB_GetRfCALIBStatus
// -----------------------------------------------------------------------------
/// Get the status of RF CALIBration data.
/// @return 0 if CALIBrated with good CRC, 1 if CALIBrated with bad CRC,
///         2 if not CALIBrated.
// =============================================================================
PUBLIC UINT8 nCALIB_GetRfCALIBStatus(VOID);


#endif

