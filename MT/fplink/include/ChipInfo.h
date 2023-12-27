/* Copyright (C) 2015 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef __CHIP_INFO_H_INCLUDED__
#define __CHIP_INFO_H_INCLUDED__

#include "fplink_export.h"
#include "TargetConnection.h"
#include "FlashInfo.h"

// This class contains various chip information and operations.

enum {
    CHIP_ID_TYPE_8809   = 0x01,
    CHIP_ID_TYPE_8826   = 0x02,
    CHIP_ID_TYPE_8809z  = 0x03,
    CHIP_ID_TYPE_8809NEZ  = 0x04
};

class FPLINK_API ChipInfo
{
public:
    ChipInfo(TargetConnection* conn);
    ~ChipInfo();

    static int ReadChipId(TargetConnection* conn, uint32_t& chipId);
    static bool Is8809(uint32_t chipId);
    static bool Is8826(uint32_t chipId);
    static bool Is8809z(uint32_t chipId);
    static bool Is8809zU01(uint32_t chipId);
    static bool Is8809NEZ(uint32_t chipId);
    static bool Is8809NEZU01(uint32_t chipId);
    static std::string RamrunFname(uint32_t connMode, uint32_t chipId, char* flashModel);

    static void SetSupportedChipIdTypes(uint32_t types);
    static bool IsChipIdSupported(uint32_t chipId);

    // Probe device.
    // Usally, will try to read ChipID from device, and it will be regarded
    // there exists a device when read ChipID successed. However, don't
    // depend on the probing mechanism.
    int probeDevice();

    // Some information will be cached. In case of device changed or forcedly
    // device access is needed, it is needed to clear cache.
    void clearCache();

    // GET the chip id. After success read, the value will be cached.
    int getChipId(uint32_t& chipId);

    // GET the flash infomation. After success read, the value will be cached.
    int getFlashInfo(FlashInfo& flashInfo);

    // Optional, flash information can be set manually.
    int setFlashInfo(const FlashInfo& flashInfo);

    // The first word of boot FLASH is a magic number.
    bool isBootMagic(uint32_t code);

    // The first 16 bytes of boot FLASH are fixed.
    // It should be only used to guess flash size.
    bool isBootMagicHeader(uint32_t code[4]);

    bool isKseg0(uint32_t targetAddress);
    bool isKseg1(uint32_t targetAddress);
    uint32_t physicalAddress(uint32_t targetAddress);

    uint32_t flashOffset(uint32_t targetAddress);
    uint32_t flashPhysicalAddress(uint32_t offset);
    bool isFlashBootAddress(uint32_t targetAddress);

    uint32_t bootSectorStructPtrAddress();
    uint32_t mapTablePtrAddress();
    uint32_t bootHostXCtxAddress();
    uint32_t bootUsbMonitorPtrAddress();

    uint32_t SPI_FLASH_BASE_ADDRESS();
    uint32_t SPI_FLASH_CMD_REG();
    uint32_t SPI_FLASH_STATUS_REG();

    uint32_t SYS_CTRL_BASE_ADDRESS();
    uint32_t SYS_CTRL_RESET_CAUSE_REG();
    uint32_t SYS_CTRL_CS_TIME_WRITE_REG();
    uint32_t SYS_CTRL_CS_CONFIG1_MODE_REG();
    uint32_t SYS_CTRL_REG_DBG();
    uint32_t SYS_CTRL_WAKE_UP();

    uint32_t INT_DEBUG_HOST_BASE_ADDRESS();
    uint32_t INT_DEBUG_HOST_CTRL_SET_REG();
    uint32_t INT_DEBUG_HOST_CTRL_CLR_REG();
    uint32_t INT_DEBUG_HOST_CFG_REG();
    uint32_t INT_DEBUG_HOST_H2P_STATUS_REG();
    uint32_t INT_DEBUG_HOST_P2H_STATUS_REG();

    uint32_t TIMER_BASE_ADDRESS();
    uint32_t TIMER_WDTIMER_CTRL_REG() ;

    uint32_t CHIP_ID_REG();

    uint32_t XCPU_BASE_ADDRESS();
    uint32_t XCPU_PC_REG();

    uint32_t INT_SRAM_BASE_ADDRESS();
    uint32_t INT_ROM_BASE_ADDRESS();
    uint32_t PSRAM_BASE_ADDRESS() ;// CS1
    uint32_t FLASH_BASE_ADDRESS(); // CS0

    bool isSram(uint32_t targetAddress);
    bool isPsram(uint32_t targetAddress);
    bool isFlash(uint32_t targetAddress);

    int xfbp();
    int xrbp();
    int disableWatchdog();

    int hostPrepareRamRun(bool bWaitVersion = false,bool bWaitMemd=true);
    int waitMemdMapReady(uint32_t timeout = 5000);
    int waitMmiMapReady(uint32_t timeout = 4000);
    int enableDebugHost();
    int chipRestart();
    int enterHostMonitor();
    int enterCalib();
    int NB_enterCalib();
    int configEbcRam();
    int targetExecuteAndJump(uint32_t pc, uint32_t sp, uint32_t param);

    int getFlashAccessAddress(target_address_t& addr);
    int getStackAccessAddress(target_address_t& addr);

    int getCalibVersionAddress(target_address_t& addr);
    int getCalibAccessAddress(target_address_t& addr);
    int NB_getCalibVersionAddress(target_address_t& addr);
    int NB_getCalibAccessAddress(target_address_t& addr);

    int getMapMemdVersionAddress(target_address_t& addr);
    int getMapMemdAccessAddress(target_address_t& addr);

    int getMapBtdVersionAddress(target_address_t& addr);
    int getMapBtdAccessAddress(target_address_t& addr);

    int getMapMmiVersionAddress(target_address_t &addr);
    int getMapMmiAccessAddress(target_address_t &addr);

    int getVersion(std::string &strVersion);
    bool Is8809NEZ();
    int creconnect();

    uint32_t MMI_PWR_ENABLE_ADDRESS();
    uint32_t FLASH_PROG_ACCESS_PTR_OFFSET;
    uint32_t CALIB_VERSION_PTR_OFFSET;
    uint32_t CALIB_ACCESS_PTR_OFFSET;
    uint32_t STACK_ACCESS_PTR_OFFSET;
    uint32_t NB_CALIB_VERSION_PTR_OFFSET;
    uint32_t NB_CALIB_ACCESS_PTR_OFFSET;

private:
    TargetConnection* mConn; // not hold by me

    uint32_t mChipId;
    uint32_t mFlashSize;
    FlashInfo mFlashInfo;
    int mFlashInfoState;
    int mChipIdState;
    uint32_t mWaitEventTimeout;

    static uint32_t mSupportedChipIdTypes;
};

#endif
