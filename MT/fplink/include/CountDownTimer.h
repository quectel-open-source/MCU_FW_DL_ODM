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

#ifndef __COUNT_DOWN_TIMER_H_INCLUDED__
#define __COUNT_DOWN_TIMER_H_INCLUDED__

#include "fplink_export.h"
#include <QtGlobal>
#include <stdint.h>

class FPLINK_API CountDownTimer
{
public:
    CountDownTimer(uint32_t ms);
    ~CountDownTimer() {}

    void reset(uint32_t ms);
    bool expired() const;
    uint32_t remained() const;
    uint32_t elapsed() const;

private:
    qint64 mStart;
    qint64 mTarget;
};

#endif
