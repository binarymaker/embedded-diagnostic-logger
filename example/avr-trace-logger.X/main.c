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

/* Includes ------------------------------------------------------------------*/
#include "mcu.h"
#include "log.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
LOG_MODULE_DEFINE( main );
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int
main(void)
{
  uint8_t count_u8;
  MCU_Init();
  MCU_INTERRUPT_ENABLE();
  LOG_Config(USART_Write);
  LOG_Restart();
  while(1)
  {
    LOG_Trace(Super_loop_EVENT);
    LOG_Info(Enter_loop_EVENT);
    LOG_Warning(count_value_is_multiple_of_5_EVENT, (count_u8%5 == 0));
    LOG_Error(count_value_is_multiple_of_10_EVENT, LOG_DATA_8, &count_u8, (count_u8%10 == 0));
    LOG_Fatal(Counter_overflow_EVENT, count_u8 % 100 == 0);
    count_u8++;
    DELAY_sec(1);
  }
}