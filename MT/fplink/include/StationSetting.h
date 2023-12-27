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

#ifndef __STATION_SETTING_H_INCLUDED__
#define __STATION_SETTING_H_INCLUDED__

#include "fplink_export.h"
#include <stdint.h>
#include <string>
#include <vector>
#include <list>

class FPLINK_API StationSetting
{
public:
    static StationSetting* Instance();

    enum {
        VERSION2 = 0xFAC00200
    };

    bool isVersion2() const;

    int getStationIndex(const std::string& name);

    // return the station names, and empty names are stripped.
    std::vector<std::string> getStationNames();
    // return the station names, including empty names.
    std::vector<std::string> getStationList() ;

    bool valid;

private:
    StationSetting();
    ~StationSetting() {}

    bool init();

    uint32_t mVersion;
    std::vector<std::string> mStationNames;
};

#endif
