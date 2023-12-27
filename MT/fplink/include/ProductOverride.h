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

#ifndef __PRODUCT_OVERRIDE_H_INCLUDED__
#define __PRODUCT_OVERRIDE_H_INCLUDED__

#include "fplink_export.h"
#include <QString>
#include <QStringList>
#include <stdint.h>

class FPLINK_API ProductOverride
{
public:
    static ProductOverride* Instance();

private:
    ProductOverride();
    ~ProductOverride() {}

    void init();
    std::string mFlashModel;
    std::string mFlashSize;
    std::string mCalibBase;
    std::string mCalibRfBase;
    std::string mFactBase;
};

#endif
