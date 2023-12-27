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

#ifndef __PORT_SETTING_H_INCLUDED__
#define __PORT_SETTING_H_INCLUDED__

#include "fplink_export.h"
#include <QList>
#include <stdint.h>

#define VIDPID(vid, pid)    (((vid) << 16) | (pid))
#define VIDPID_INVALID      0xFFFFFFFF
#define VIDPID_UNKNOWN      0

class SettingLoader;
class FPLINK_API PortSetting
{
public:
    static PortSetting* Instance();

    std::list<uint32_t> usb0DeviceIDs;
    std::list<uint32_t> usb1DeviceIDs;
    uint32_t usbReopenDisappearTimeout;
    uint32_t usbReopenReappearTimeout;
    uint32_t usbReopenStableTime;
    uint32_t usbReopenPollInterval;
    bool usbSupportBackgroundPolling;
    bool usbVerboseScanInfo;

private:
    PortSetting();
    ~PortSetting() {}

    void loadStore(SettingLoader* loader);
};

#endif
