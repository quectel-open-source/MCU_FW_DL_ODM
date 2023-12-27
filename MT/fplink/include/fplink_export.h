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

#ifndef __FP_LINK_EXPORT_H_INCLUDED__
#define __FP_LINK_EXPORT_H_INCLUDED__

#ifdef _MSC_VER
#ifdef FPLINK_EXPORTS
#define FPLINK_API __declspec(dllexport)
#else
#define FPLINK_API __declspec(dllimport)
#endif
#else
#define FPLINK_API
#endif

#ifdef _MSC_VER
#ifdef FPLINK_EXPORTS
#define FPLINK_CAPI extern "C" __declspec(dllexport)
#else
#define FPLINK_CAPI extern "C" __declspec(dllimport)
#endif
#else
#define FPLINK_CAPI extern "C"
#endif

#endif
