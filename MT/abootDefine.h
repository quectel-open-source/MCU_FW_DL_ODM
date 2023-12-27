/**
 * @file
 * @author  Jinhua Huang <jinhuahuang@asrmicro.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The time class represents a moment of time.
 */
#ifndef ABOOT_H
#define ABOOT_H

#ifdef  __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
  #if defined(BUILD_DLL)
    #define ABOOT_API __declspec(dllexport)
  #elif defined(USE_DLL)
    #define ABOOT_API __declspec(dllimport)
  #else
    #define ABOOT_API
  #endif
#else
  #define ABOOT_API
#endif

/**
 * Callback events to upper layer
 */
typedef enum {
    ABOOT_EVENT_SYSTEM_LOG,
    ABOOT_EVENT_DOWNLOAD_INIT,
    ABOOT_EVENT_DOWNLOAD_START,
    ABOOT_EVENT_DOWNLOAD_STOP,
    ABOOT_EVENT_DEVICE_NEW,
    ABOOT_EVENT_DEVICE_CHANGE,
    ABOOT_EVENT_RELEASE_LIST_PRODUCTS,
    ABOOT_EVENT_RELEASE_GENERATE,
} aboot_callback_event_t;

/**
 * Callback function pointer type
 *
 * @param json callback info
 * @param ctx user data pointer
 */
typedef void (*aboot_callback_func_t)(const char *json, void *ctx);

/**
 * Initialize aboot engine.
 *
 * @return 0 on success, otherwise failed
 */
typedef int (*aboot_engine_init_func_t)(void);

/**
 * Cleanup aboot engine.
 */
typedef void(*aboot_engine_exit_func_t)(void);


/**
 * Register engine call back.
 *
 * @param cb callback function pointer
 * @param ctx user data pointer
 */
typedef void (*aboot_engine_register_cb_func_t)(aboot_callback_func_t cb, void *ctx);

/**
 * Unregister engine call back.
 *
 */
typedef void(*aboot_engine_unregister_cb_func_t)(void);


/**
 * Initialize aboot download module.
 *
 * @param json config release package info in json format\n
 * {\n
 *   "path": string,     // release package file path\n
 *   "address": integer, // or release package in memory address\n
 *   "size": integer     // with size\n
 * }
 * @return 0 on success, otherwise failed
 */
typedef int (*aboot_download_init_func_t)(const char *json);

/**
 * Start aboot download module.
 *
 * @param json startup options in json format\n
 * {\n
 *   "baudRate": integer, // baud rate setting for UART \n
 *   "usbOnly": bool, // only download for USB device \n
 *   "usbAutoEnable": bool, // USB device auto start\n
 *   "disabledGroups": [ "1", "2" ] // array of disabled downloading groups\n
 * }
 * @return 0 on success, otherwise failed
 */
typedef int (*aboot_download_start_func_t)(const char *json);

/**
 * Stop aboot download module.
 */
typedef void(*aboot_download_stop_func_t)(void);

/**
 * Enable aboot device for downloading.
 *
 * @param json device info in json format\n
 * {\n
 *   "path": string // COMM device name\n
 * }
 * @return 0 on success, otherwise failed
 */
typedef int (*aboot_download_enable_device_func_t)(const char *json);

/**
 * List supported release products
 *
 * @param json specify the configuration base directory in json format\n
 * {\n
 *   "path": string\n
 * }
 * @return 0 on success, otherwise failed
 */
typedef int (*aboot_release_list_products_func_t)(const char *json);

/**
 * Generate a release package
 * @param json specify the options in json format\n
 * {\n
 *   "path": string, // configuration base directory\n
 *   "product": string, // product name\n
 *   "output": string, output file name\n
 *   "custom": { "seagull": string, "msa": string }\n
 * }
 * @return 0 on success, otherwise failed
 */
typedef int (*aboot_release_generate_func_t)(const char *json);

#ifdef  __cplusplus
}
#endif

#endif
