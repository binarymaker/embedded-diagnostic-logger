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
#include "crc8-ccitt.h"

#define LOG_START_ID                                              ((uint8_t)'#')
#define LOG_END_ID                                               ((uint8_t)'\n')

void (*log_tx_handler)(uint8_t);

void
LOG_Config(void (*tx_function)(uint8_t))
{
  log_tx_handler = tx_function;
}

void
LOG_BasicFrame(logId_et log_ev, uint8_t log_module_u8, uint8_t msg_event_u8)
{
  uint8_t crc_u8;
  uint8_t data_u8arr[3] = {log_ev, log_module_u8, msg_event_u8};
  
  crc_u8 = CRC8_Calculate(data_u8arr, 3);

  log_tx_handler(LOG_START_ID);
  log_tx_handler(log_ev);
  log_tx_handler(log_module_u8);
  log_tx_handler(msg_event_u8);
  log_tx_handler(crc_u8);
  log_tx_handler(LOG_END_ID);
}

void
LOG_DebugFrame(logId_et log_ev, uint8_t log_module_u8, uint8_t msg_event_u8, 
                uint8_t datatypeflag_u8, uint8_t length_u8, const void * data_ptr)
{
  uint8_t byte_len_u8;
  uint8_t crc_u8;
  uint8_t * datapos_u8ptr = (uint8_t *) data_ptr;
  uint8_t data_u8arr[] = {log_ev, log_module_u8, msg_event_u8,
                          datatypeflag_u8, length_u8};

  byte_len_u8 = (datatypeflag_u8 & 0x07) * length_u8;
  
  crc_u8 = CRC8_Calculate(data_u8arr, 5);

  log_tx_handler(LOG_START_ID);
  log_tx_handler(log_ev);
  log_tx_handler(log_module_u8);
  log_tx_handler(msg_event_u8);
  log_tx_handler(datatypeflag_u8);
  log_tx_handler(length_u8);
 
  for(uint8_t i_u8 = 0; i_u8 < byte_len_u8; i_u8++)
  {
    crc_u8 = CRC8_Table(crc_u8 ^ *datapos_u8ptr);
    log_tx_handler(*datapos_u8ptr);
    datapos_u8ptr++;
  }

  log_tx_handler(crc_u8);
  log_tx_handler(LOG_END_ID);
}
