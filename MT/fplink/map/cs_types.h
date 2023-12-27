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

#ifndef _CS_TYPES_H_
#define _CS_TYPES_H_

#include <stdint.h>

#define INT8    int8_t
#define INT16   int16_t
#define INT32   int32_t
#define UINT8   uint8_t
#define UINT16  uint16_t
#define UINT32  uint32_t

#define BOOL    uint8_t
#define VOID    void

#define VOLATILE
#define PUBLIC

#ifdef CONST
#undef CONST
#endif
#define CONST

#endif

