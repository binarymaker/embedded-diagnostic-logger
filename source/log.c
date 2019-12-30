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

#include "log.h"

void (*log_tx_handler)(uint8_t);

void
LOG_Config(void (*tx_function)(uint8_t))
{
  log_tx_handler = tx_function;
}

void
LOG_BasicFrame(logId_et log_ev, uint8_t msg_event_u8, uint8_t log_module_u8)
{
  log_tx_handler(log_ev);
  log_tx_handler(log_module_u8);
  log_tx_handler(msg_event_u8);
}

void
LOG_DebugFrame(logId_et log_ev, uint8_t msg_event_u8, uint8_t log_module_u8,
                uint8_t datatypeflag_u8, uint32_t data_32u)
{
  uint8_t i_u8;
  uint8_t byte_len_u8;

  byte_len_u8 = (datatypeflag_u8 & 0x07);

  log_tx_handler(log_ev);
  log_tx_handler(log_module_u8);
  log_tx_handler(msg_event_u8);
  log_tx_handler(datatypeflag_u8);

  for(i_u8 = 0; i_u8 < byte_len_u8; i_u8++)
  {
    log_tx_handler((data_32u >> (i_u8 * 8)) & 0x000000FF);
  }
}
