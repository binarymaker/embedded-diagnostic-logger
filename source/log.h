/**
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
  */

#ifndef BM_97cd7597_d9c7_11e9_8377_705a0f25cb51
#define BM_97cd7597_d9c7_11e9_8377_705a0f25cb51

#include <stdint.h>
#include "log-cfg.h"
#include "log_modules.h"

#define DATA_8        (0x01)
#define DATA_16       (0x02)
#define DATA_32       (0x04)
#define DATA_SIGN     (0x10)
#define FRMT_FLT      (0x20)
#define FRMT_CHAR     (0x40)
#define FRMT_HEX      (0x80)

typedef enum
{
  TRACE_ID = 0,
  DEBUG_ID,
  INFO_ID,
  WARNING_ID,
  ERROR_ID,
  FATAL_ID
}logId_t;

#if defined(LOG_TRACE_ENABLE)   || \
    defined(LOG_DEBUG_ENABLE)   || \
    defined(LOG_INFO_ENABLE)    || \
    defined(LOG_WARNING_ENABLE) || \
    defined(LOG_ERROR_ENABLE)   || \
    defined(LOG_FATAL_ENABLE)

#define LOG_MODULE_DEFINE( _name )  static uint32_t log_module = LOG_MODULE_##_name

#else

#define LOG_MODULE_DEFINE( _name )

#endif


#if defined(LOG_TRACE_ENABLE)

#define LOG_Trace(msg_event)                              \
  (_LOG_BasicFrame(TRACE_ID, msg_event, log_module))

#else

#define LOG_Trace(msg_event) ((void)0U)

#endif

#if defined(LOG_DEBUG_ENABLE)

#define LOG_Debug(msg_event, datatypes, data)             \
  (_LOG_DebugFrame(DEBUG_ID, msg_event, log_module, datatypes, data))

#else

#define LOG_Debug(msg_event, datatypes, data) ((void)0U)

#endif

#if defined(LOG_ERROR_ENABLE)

#define LOG_Error(msg_event, datatypes, data, condition)  \
  ((condition) ? _LOG_DebugFrame(ERROR_ID, msg_event, log_module, datatypes, data) : 0)

#else

#define LOG_Error(msg_event, datatypes, data, condition)   ((void)0U)

#endif

#if defined(LOG_INFO_ENABLE)

#define LOG_Info(msg_event)                               \
  (_LOG_BasicFrame(INFO_ID, msg_event, log_module))

#else

#define LOG_Info(msg_event) ((void)0U)

#endif

#if defined(LOG_WARNING_ENABLE)

#define LOG_Warning(msg_event, condition)                 \
  ((condition) ? _LOG_BasicFrame(WARNING_ID, msg_event, log_module) : 0)

#else

#define LOG_Warning(msg_event, condition) ((void)0U)

#endif

#if defined(LOG_WARNING_ENABLE)

#define LOG_Fatal(msg_event, condition)                   \
  ((condition) ? _LOG_BasicFrame(FATAL_ID, msg_event, log_module) : 0)

#else

#define LOG_Fatal(msg_event, condition) (void)0U

#endif


void
LOG_Config(void (*tx_function_handler)(uint8_t));

void
_LOG_BasicFrame(logId_t log_e, uint8_t msg_event_8u, uint8_t log_module_8u);

void
_LOG_DebugFrame(logId_t log_e, uint8_t msg_event_8u, uint8_t log_module_8u,
                uint8_t datatypes_8u, uint32_t data_32u);

#endif /* BM_97cd7597_d9c7_11e9_8377_705a0f25cb51 */
