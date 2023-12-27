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

#ifndef FCS_H
#define FCS_H

#include "fplink_export.h"

struct FCSContext {
    unsigned int remainder;
};

FPLINK_API void FCSInit(struct FCSContext* context);
FPLINK_API void FCSUpdate(struct FCSContext* context, unsigned char* buffer,int len);
FPLINK_API void FCSFinal(struct FCSContext* context,unsigned char fcs[3]);
FPLINK_API unsigned int FCSCalculate(unsigned char* buffer,int len);

#endif
