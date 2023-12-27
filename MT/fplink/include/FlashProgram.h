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

#ifndef __FLASH_PROGRAM_H_INCLUDED__
#define __FLASH_PROGRAM_H_INCLUDED__

#include "fplink_export.h"
#include "TargetConnection.h"
#include "ChipInfo.h"
//#include <QObject>
#include "libusb-1.0\msvc\stdint.h"

class LodData;

#ifndef FLASH_PROGRAM_INTERNAL
typedef void FLASH_PROG_MAP_ACCESS_T;
typedef void FPC_COMMAND_DATA_T;
#endif

typedef void (*ProcessPercent)(int,void *user);

class FPLINK_API FlashProgram
{

public:
    FlashProgram(TargetConnection* conn, ChipInfo* chip);
    ~FlashProgram();

    int checkStart();
    int run(LodData* lod, bool checkBootFlag, bool NeedLodEnd, bool flashtest=false);
    int program(LodData* lod);
    int verify(LodData* lod, bool checkBootFlag, bool NeedLodEnd = true);
    int erase(uint32_t flashAddress, uint32_t size);
    int overwrite(uint32_t flashAddress, uint8_t* data, uint32_t size);
    int checkFcs(uint32_t flashAddress, uint32_t size, uint32_t fcs);
    int checkFcs(int num, uint32_t* flashAddress, uint32_t* size, uint32_t* fcs);

    int factRead(uint32_t flashAddress, void* data);
    int factWrite(uint32_t flashAddress, void* data);
    void stop() {
        mProgramStopReq = true;
    }

    void setWaitEventTimeout (uint32_t waitEventTimeout);
    void setPreferEraseProgramTime (uint32_t eraseProgramTime);

    int shutDownChip();
    int restartChip();
    int imageInitial(uint32_t uartID, uint32_t totalSize,bool needClockOut,uint32_t poweronMode=0);
    int imageSendData(char *fname,uint32_t timeout = 30000 );
    int imageProgram(uint32_t timeout = 50000);
    int imageVerify(char *fname);

//Q_SIGNALS:
    //void programProgress(int percent);
    //void eraseProgramTimeTooLong();
    //void eraseProgramTime(int time, bool sectorsize);
    int program(LodData* lod,ProcessPercent fp,void *user);

private:
    int getAccess();
    int lodProgram();
    int lodProgram(ProcessPercent fp,void *user);
    int flashtestlodProgram();
    int lodVerify();
    int lodEnd();
    int checkBootMagic();
    int sendFpdCommand(FPC_COMMAND_DATA_T* cmd);
    int waitFpcCommandDone();

    LodData* mLod; // not hold by me
    TargetConnection* mConn; // not hold by me
    ChipInfo* mChip; // not hold by me

    target_address_t mAccessAddress;
    FLASH_PROG_MAP_ACCESS_T* mAccess;
    void* mFF2K;
    bool mUidEventAfterReady;
    uint32_t mEncryptUid;
    uint32_t mWaitStartTimeout;
    uint32_t mWaitEventTimeout;
    uint32_t mPreferEraseProgramTime;
    std::list<uint32_t> mStartEvents;
    std::list<uint32_t> mReadyEvents[2];

    int mCmdIndex;
    int mCmdPending;
    uint64_t mStartMsec;

    volatile bool mProgramStopReq;
};

#endif
