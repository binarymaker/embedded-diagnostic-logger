/**\cond
  ******************************************************************************
  * ______  _                             ___  ___        _               
  * | ___ \(_)                            |  \/  |       | |              
  * | |_/ / _  _ __    __ _  _ __  _   _  | .  . |  __ _ | | __ ___  _ __ 
  * | ___ \| || '_ \  / _` || '__|| | | | | |\/| | / _` || |/ // _ \| '__|
  * | |_/ /| || | | || (_| || |   | |_| | | |  | || (_| ||   <|  __/| |   
  * \____/ |_||_| |_| \__,_||_|    \__, | \_|  |_/ \__,_||_|\_\\___||_|   
  *                                 __/ |                                 
  *                                |___/                                  
  *                                                                       
  * Copyright (C) 2019 Binary Maker - All Rights Reserved
  *
  * This program and the accompanying materials are made available
  * under the terms described in the LICENSE file which accompanies
  * this distribution.
  * Written by Binary Maker <https://github.com/binarymaker>
  ******************************************************************************
  \endcond*/

#ifndef BM_97cd7597_d9c7_11e9_8377_705a0f25cb51
#define BM_97cd7597_d9c7_11e9_8377_705a0f25cb51

#include <stdint.h>
#include "log-cfg.h"

#ifndef TEST
#include "log_modules.h"
#endif

#define LOG_DATA_8                                                        (0x01)
#define LOG_DATA_16                                                       (0x02)
#define LOG_DATA_32                                                       (0x04)
#define LOG_DATA_SIGN                                                     (0x10)
#define LOG_FRMT_FLT                                                      (0x20)
#define LOG_FRMT_CHAR                                                     (0x40)
#define LOG_FRMT_HEX                                                      (0x80)

typedef enum
{
  TRACE_ID,
  DEBUG_ID,
  INFO_ID,
  WARNING_ID,
  ERROR_ID,
  FATAL_ID,
  RESTART_ID,
  FILELOG_ID
}logId_et;

#if (_LOG_TRACE == ENABLE)   || \
    (_LOG_DEBUG == ENABLE)   || \
    (_LOG_INFO == ENABLE)    || \
    (_LOG_WARNING == ENABLE) || \
    (_LOG_ERROR == ENABLE)   || \
    (_LOG_FATAL == ENABLE)

#define LOG_MODULE_DEFINE( name )  static uint8_t log_module = LOG_MODULE_##name

#else

#define LOG_MODULE_DEFINE( name )

#endif


#if _LOG_TRACE == ENABLE
#define LOG_Trace(msg_event)                                                   \
  (_LOG_BasicFrame(TRACE_ID, msg_event, log_module))
#else
#define LOG_Trace(msg_event) ((void)0U)
#endif

#if _LOG_INFO == ENABLE
#define LOG_Info(msg_event)                                                    \
  (_LOG_BasicFrame(INFO_ID, msg_event, log_module))
#else
#define LOG_Info(msg_event) ((void)0U)
#endif

#if _LOG_DEBUG == ENABLE
#define LOG_Debug(msg_event, datatypes, data)                                  \
  (_LOG_DebugFrame(DEBUG_ID, msg_event, log_module, datatypes, (uint32_t)data))
#else
#define LOG_Debug(msg_event, datatypes, data) ((void)0U)
#endif

#if _LOG_WARNING == ENABLE
#define LOG_Warning(msg_event, condition)                                      \
  ((condition) ? _LOG_BasicFrame(WARNING_ID, msg_event, log_module) : 0)
#else
#define LOG_Warning(msg_event, condition) ((void)0U)
#endif

#if _LOG_ERROR == ENABLE
#define LOG_Error(msg_event, datatypes, data, condition)                       \
  ((condition) ? _LOG_DebugFrame(ERROR_ID, msg_event, log_module, datatypes, (uint32_t)data) : 0)
#else
#define LOG_Error(msg_event, datatypes, data, condition)   ((void)0U)
#endif

#if _LOG_FATAL == ENABLE
#define LOG_Fatal(msg_event, condition)                                        \
  ((condition) ? _LOG_BasicFrame(FATAL_ID, msg_event, log_module) : 0)
#else
#define LOG_Fatal(msg_event, condition) (void)0U
#endif

#define LOG_Restart() _LOG_BasicFrame(RESTART_ID, 0, 0)

void
LOG_Config(void (*tx_function_handler)(uint8_t));

void
_LOG_BasicFrame(logId_et log_ev, uint8_t msg_event_8u, uint8_t log_module_8u);

void
_LOG_DebugFrame(logId_et log_ev, uint8_t msg_event_8u, uint8_t log_module_8u,
                uint8_t datatypeflag_8u, uint32_t data_32u);

#endif /* BM_97cd7597_d9c7_11e9_8377_705a0f25cb51 */
