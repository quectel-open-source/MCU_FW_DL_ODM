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
  #define ABOOT_API __attribute__ ((visibility("default")))
#endif

/**
 * Callback events to upper layer
 */
typedef enum {
    ABOOT_EVENT_SYSTEM_LOG,
    ABOOT_EVENT_DOWNLOAD_INIT,
    ABOOT_EVENT_DOWNLOAD_START,
    ABOOT_EVENT_DOWNLOAD_STOP,
    ABOOT_EVENT_DOWNLOAD_GET_TRAININGS,
    ABOOT_EVENT_DEVICE_NEW,
    ABOOT_EVENT_DEVICE_CHANGE,
    ABOOT_EVENT_RELEASE_LIST_PRODUCTS,
    ABOOT_EVENT_RELEASE_GENERATE,
    ABOOT_EVENT_PRODMODE_START,
    ABOOT_EVENT_PRODMODE_STOP,
    ABOOT_EVENT_RELEASE_KEYGEN,
    ABOOT_EVENT_RELEASE_SIGN,
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
int ABOOT_API aboot_engine_init(void);

/**
 * Cleanup aboot engine.
 */
void ABOOT_API aboot_engine_exit(void);

/**
 * Register engine call back.
 *
 * @param cb callback function pointer
 * @param ctx user data pointer
 */
void ABOOT_API aboot_engine_register_cb(aboot_callback_func_t cb, void *ctx);

/**
 * Unregister engine call back.
 *
 */
void ABOOT_API aboot_engine_unregister_cb(void);

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
int ABOOT_API aboot_download_init(const char *json);

/**
 * Configure USB port training data.
 *
 * @param json USB port training configuration data in json format\n
 * [\n
 *   {\n
 *     "locationInfo": string, // port location information\n
 *     "order": integer        // device order allocated\n
 *   },\n
 *   {\n
 *     "locationInfo": string, // port location information\n
 *     "order": integer        // device order allocated\n
 *   }\n
 * ]
 *
 * @return 0 on success, otherwise failed
 */
int ABOOT_API aboot_download_config_trainings(const char *json);

/**
 * Get USB port training data.
 *
 * @return 0 on success, otherwise failed
 */
int ABOOT_API aboot_download_get_trainings(void);

/**
 * Start aboot download module.
 *
 * @param json startup options in json format\n
 * {\n
 *   "baudRate": integer, // baud rate setting for UART \n
 *   "usbOnly": bool, // only download for USB device \n
 *   "usbAutoEnable": bool, // USB device auto start\n
 *   "enableOnce": bool, // enable device only once\n
 *   "disabledGroups": [ "1", "2" ] // array of disabled downloading groups\n
 * }
 * @return 0 on success, otherwise failed
 */
int ABOOT_API aboot_download_start(const char *json);

/**
 * Stop aboot download module.
 */
void ABOOT_API aboot_download_stop(void);

/**
 * Enable aboot device for downloading.
 *
 * @param json device info in json format\n
 * {\n
 *   "path": string // COMM device name\n
 * }
 * @return 0 on success, otherwise failed
 */
int ABOOT_API aboot_download_enable_device(const char *json);

/**
 * List supported release products
 *
 * @param json specify the configuration base directory in json format\n
 * {\n
 *   "path": string\n
 * }
 * @return 0 on success, otherwise failed
 */
int ABOOT_API aboot_release_list_products(const char *json);

/**
 * Generate a release package
 * @param json specify the options in json format\n
 * {\n
 *   "path": string, // configuration base directory\n
 *   "imageBase": string, // use new image base directory\n
 *   "product": string, // product name\n
 *   "variant": string, // variant name\n
 *   "output": string, output file name\n
 *   "custom": { "cp": string, "dsp": string }\n
 *   "eraseAll": boolean, if generate eraseall command\n
 *   "eraseAllOnly": boolean, if generate eraseall package only\n
 *   "fuseOnly": boolean, if generate fuse only package\n
 *   "uploadOnly": boolean, if generate upload only package\n
 *   "stage1": boolean, if running in stage1\n
 *   "stage2": boolean, if running in stage2\n
 *   "stageDir": string, stage intermediate directory\n
 * }
 * @return 0 on success, otherwise failed
 */
int ABOOT_API aboot_release_generate(const char *json);

/**
 * Generate a suite security keys
 * @param json specify the options in json format\n
 * {\n
 *   "path": string, // configuration base directory\n
 *   "keyAlg": string, // key algorithm name\n
 * }
 * @return 0 on success, otherwise failed
 */
int ABOOT_API aboot_release_keygen(const char *json);

/**
 * Sign one image only
 * @param json specify the options in json format\n
 * {\n
 *   "path": string, // configuration base directory\n
 *   "product": string, // product name\n
 *   "variant": string, // variant name\n
 *   "input": string, // source file name\n
 *   "output": string, output file name\n
 *   "stage1": boolean, if running in stage1\n
 *   "stage2": boolean, if running in stage2\n
 *   "stageDir": string, stage intermediate directory\n
 * }
 * @return 0 on success, otherwise failed
 */
int ABOOT_API aboot_release_sign(const char *json);

/**
 * Start aboot production mode module.
 *
 * @return 0 on success, otherwise failed
 */
int ABOOT_API aboot_prodmode_start(void);

/**
 * Stop aboot production mode module.
 */
void ABOOT_API aboot_prodmode_stop(void);

#ifdef  __cplusplus
}
#endif

#endif
