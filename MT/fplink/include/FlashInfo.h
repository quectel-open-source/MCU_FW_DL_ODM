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

#ifndef __FLASH_INFO_H_INCLUDED__
#define __FLASH_INFO_H_INCLUDED__

#include "fplink_export.h"
#include "TargetConnection.h"
#include <map>
#include <string>

// There are several use models:
// * When flash info is stored in configuration file, it can be load
//   from configuration file.
// * When (memd_version.version == 0), we can get partial information
//   from the connected device.
// * When (memd_version.version == 1), we can get all information
//   from the connected device.
//
// NOTE: It is possible that information is unavailable (UNKNOWN_VALUE).
//       Caller should take care about this case.

class ChipInfo;
class LodHeader;

class FPLINK_API FlashInfo
{
public:
    FlashInfo();
    ~FlashInfo();

    FlashInfo& operator =(const FlashInfo& o);

    struct EraseSector {
        uint32_t base;
        uint32_t size;
    };

    void clear();

    int loadFromTarget(TargetConnection* conn, ChipInfo* chip);
    int loadFromLodHeader(const LodHeader& header);

    uint32_t flashSize;
    uint32_t ramPhySize;
    uint32_t userDataBase;
    uint32_t userDataSize;
    uint32_t fsRootBase;
    uint32_t fsRootSize;
    uint32_t calibBase;
    uint32_t calibSize;
    uint32_t calibRfBase;
    uint32_t nbcalibRfBase;
    uint32_t nbcalibRfSize;
    uint32_t factBase;
    uint32_t factSize;
    std::string flashModel;
    std::string xcvModel;

private:
    int loadFromTargetV0(TargetConnection* conn, ChipInfo* chip);
    int loadFromTargetV1(TargetConnection* conn, ChipInfo* chip);
    int loadFromTargetV2(TargetConnection* conn, ChipInfo* chip);
};

#endif
