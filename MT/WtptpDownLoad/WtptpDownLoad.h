#pragma once

#include "ParaDefine.h"
/*******************************************************************************************************************

 (C) Copyright  Marvell Semiconductor Hefei branch Ltd. All Rights Reserved  

 *******************************************************************************************************************/

#ifdef WTPTPDOWNLOAD_DYNAMIC
#ifdef WTPTPDOWNLOAD_EXPORTS
#define WTPTPDOWNLOAD_FUNC    extern "C"  __declspec(dllexport)
#else
#define WTPTPDOWNLOAD_FUNC    extern "C" __declspec(dllimport)
#endif
#else
#define WTPTPDOWNLOAD_FUNC
#endif

/*! @defgroup group Functions list
* @{
*/
/// =========================================
/// @fn			InitializeBL(PInstanceParams pInstParam)
/// @brief		This function is used to initialize download parameter and waiting for USB device arrived
/// @param[in]	PInstParam including download file list and DKB files name and other configurations.                                                                 
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		please input the parameter of  pInstParam from IFL_SWD function output \n
///
/// =========================================
WTPTPDOWNLOAD_FUNC BOOL  InitializeBL(PInstanceParams pInstParam);

/// =========================================
/// @fn			TerminateBL()
/// @brief		Terminate download process                                                                    
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		This function is used to stop all download process \n
///
/// =========================================
WTPTPDOWNLOAD_FUNC BOOL  TerminateBL();
///@}		//@defgroup group Functions list






