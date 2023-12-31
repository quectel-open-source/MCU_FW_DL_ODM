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
/// This file contains the portion of the module's memory map that will be accessible
/// through CoolWatcher (or any CoolXml tool). It is also used for the get version
/// mechanism.

#ifndef _STACK_MAP_H_
#define _STACK_MAP_H_



// =============================================================================
//  MACROS
// =============================================================================
/// Validity pattern
#define STACK_AUTO_CALL_VALID                   (0XADF1900D)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// STACK_MAP_GLOBALS_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef struct {
} STACK_MAP_GLOBALS_T; //Size : 0x0



// ============================================================================
// STACK_AUTO_CALL_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef struct {
    /// valid if it equals STACK_AUTO_CALL_VALID
    UINT32                         validity;                     //0x00000000
    /// auto call instruction bitset
    UINT32                         flag;                         //0x00000004
} STACK_AUTO_CALL_T; //Size : 0x8



// ============================================================================
// STACK_PROFILE_ON_BUFFER_T
// -----------------------------------------------------------------------------
/// This is used only when the code is compiled with STACK_PROFILE_ON_BUFFER_T in
/// debug.
// =============================================================================
typedef struct {
    /// pxts buffer start address
    UINT32                         buffer_start;                 //0x00000000
    /// pxts buffer current position
    UINT32                         buffer_pos;                   //0x00000004
    /// pxts buffer size
    UINT32                         buffer_size;                  //0x00000008
} STACK_PROFILE_ON_BUFFER_T; //Size : 0xC



// ============================================================================
// STACK_MAP_ACCESS_T
// -----------------------------------------------------------------------------
/// Type used to define the accessible structures of the module.
// =============================================================================
typedef struct {
    STACK_AUTO_CALL_T*             autoCallStructPtr;            //0x00000000
    /// This is used only when the code is compiled with STACK_PROFILE_ON_BUFFER_T
    /// in debug.
    STACK_PROFILE_ON_BUFFER_T      ProfileOnBuffer;              //0x00000004
} STACK_MAP_ACCESS_T; //Size : 0x10






// =============================================================================
// stack_GetAutoCallStruct
// -----------------------------------------------------------------------------
/// This function get the auto call structure pointer from the map accessor.
// =============================================================================
PUBLIC STACK_AUTO_CALL_T* stack_GetAutoCallStruct(VOID);


// =============================================================================
// stack_RegisterYourself
// -----------------------------------------------------------------------------
/// This function registers the module itself to HAL so that the version and
/// the map accessor are filled. Then, the CSW get version function and the
/// CoolWatcher get version command will work.
// =============================================================================
PUBLIC VOID stack_RegisterYourself(VOID);



#endif

