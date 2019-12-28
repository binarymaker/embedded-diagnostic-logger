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

#include "unity.h"
#include "log-cfg.h"
#include "log.h"

void
setUp()
{

}

void
tearDown()
{

}

/* -------------------------------------------------------------------------- */
void
test_log_id_number()
{
    /* Ensure known test state */

    /* Setup expected function mock */

    /* Function under test */

    /* Verify test results */
    TEST_ASSERT_EQUAL( 0, TRACE_ID);
    TEST_ASSERT_EQUAL( 1, DEBUG_ID);
    TEST_ASSERT_EQUAL( 2, INFO_ID);
    TEST_ASSERT_EQUAL( 3, WARNING_ID);
    TEST_ASSERT_EQUAL( 4, ERROR_ID);
    TEST_ASSERT_EQUAL( 5, FATAL_ID);
    TEST_ASSERT_EQUAL( 6, RESTART_ID);
    TEST_ASSERT_EQUAL( 7, FILELOG_ID);
}

void
test_module_name_define_variable_should_create()
{
    /* Ensure known test state */
    #define LOG_MODULE_binary 100
    
    /* Setup expected function mock */

    /* Function under test */
    LOG_MODULE_DEFINE(binary);

    /* Verify test results */
    TEST_ASSERT_EQUAL(100, log_module);
}

#define BUFFER_SIZE 32

uint8_t log_buffer[BUFFER_SIZE];
uint8_t buffer_index_8u;

void
log_buffer_write(uint8_t ch)
{
  buffer_index_8u %= BUFFER_SIZE;
  log_buffer[buffer_index_8u] = ch;
  buffer_index_8u++;
}

void 
log_buffer_clear()
{
  for (int8_t i_8u = 0; i_8u < BUFFER_SIZE; i_8u++)
  {
    log_buffer[i_8u] = 0;
  }
  buffer_index_8u = 0;
}

void
test_logger_core_function__LOG_BasicFrame()
{
    /* Ensure known test state */
    #define LOG_MODULE_binary 100
    #define logger_1_EVENT    1
    LOG_MODULE_DEFINE(binary);
    
    log_buffer_clear();
    /* Setup expected function mock */
    LOG_Config(log_buffer_write);

    /* Function under test */
    _LOG_BasicFrame(TRACE_ID, logger_1_EVENT, log_module);

    /* Verify test results */
    uint8_t expected_transfer[] = {0, 100, 1};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_transfer, log_buffer, 
                                  sizeof(expected_transfer));
}

void
test_logger_core_function__LOG_DebugFrame()
{
    /* Ensure known test state */
    #define LOG_MODULE_binary 100
    #define logger_1_EVENT    1
    LOG_MODULE_DEFINE(binary);
    
    log_buffer_clear();
    /* Setup expected function mock */
    LOG_Config(log_buffer_write);

    /* Function under test */
    _LOG_DebugFrame(DEBUG_ID, logger_1_EVENT, log_module, LOG_DATA_32, 0x0BADF00D);

    /* Verify test results */
    uint8_t expected_transfer[] = {1, 100, 1, 4, 0x0D, 0xF0, 0xAD, 0X0B};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_transfer, log_buffer, 
                                  sizeof(expected_transfer));
}