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

#ifndef __RAM_RUN_H_INCLUDED__
#define __RAM_RUN_H_INCLUDED__

#include "fplink_export.h"
#include "TargetConnection.h"
#include "ChipInfo.h"
#include <QList>
#include "libusb-1.0\msvc\stdint.h"

class LodData;

class FPLINK_API RamRun
{
public:
    RamRun();
    ~RamRun();

    bool init(ChipInfo* chip, LodData* lod, TargetConnection* conn);
    int run();

private:
    int enterHostMonitorMode();
    int enterUsbMonitorMode();
    int configEbcRam();

    LodData* mLod; // not hold by me
    TargetConnection* mConn; // not hold by me
    ChipInfo* mChip; // not hold by me

    uint32_t mWaitEventTimeout;
};

#endif
