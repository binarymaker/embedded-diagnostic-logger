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

#include "crc8-ccitt.h"

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
test_log_formater_flag()
{
    /* Ensure known test state */

    /* Setup expected function mock */

    /* Function under test */

    /* Verify test results */
    TEST_ASSERT_EQUAL( 0x01, LOG_DATA_8);
    TEST_ASSERT_EQUAL( 0x02, LOG_DATA_16);
    TEST_ASSERT_EQUAL( 0x04, LOG_DATA_32);
    TEST_ASSERT_EQUAL( 0x10, LOG_DATA_SIGN);
    TEST_ASSERT_EQUAL( 0x20, LOG_FRMT_FLT);
    TEST_ASSERT_EQUAL( 0x40, LOG_FRMT_CHAR);
    TEST_ASSERT_EQUAL( 0x80, LOG_FRMT_HEX);
}

void
test_module_name_define_variable_should_create()
{
    /* Ensure known test state */
    #define LOG_MODULE_main 0xA5
    
    /* Setup expected function mock */

    /* Function under test */
    LOG_MODULE_DEFINE(main);

    /* Verify test results */
    TEST_ASSERT_EQUAL(0xA5, log_module);
}

#define BUFFER_SIZE 32

uint8_t log_buffer[BUFFER_SIZE];
uint8_t buffer_index_u8;

void
log_buffer_write(uint8_t ch)
{
  buffer_index_u8 %= BUFFER_SIZE;
  log_buffer[buffer_index_u8] = ch;
  buffer_index_u8++;
}

void
log_buffer_clear()
{
  for (uint8_t i_u8 = 0; i_u8 < BUFFER_SIZE; i_u8++)
  {
    log_buffer[i_u8] = 0;
  }
  buffer_index_u8 = 0;
}

void
test_logger_core_function_LOG_BasicFrame()
{
  /* Ensure known test state */
  uint8_t LOG_MODULE_binary = 0xA5;
  uint8_t Logger_EVENT      = 0x01;
  
  LOG_Config(log_buffer_write);
  log_buffer_clear();
  /* Setup expected function mock */
  

  /* Function under test */
  LOG_BasicFrame(TRACE_ID, LOG_MODULE_binary, Logger_EVENT);

  /* Verify test results */
  uint8_t expected_transfer[] = {'#', 0x00, 0xA5, 0x01, 0x5E,'\n'};
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_transfer, log_buffer, 
                                sizeof(expected_transfer));
}

void
test_logger_core_function_LOG_DebugFrame()
{
  /* Ensure known test state */
  uint8_t LOG_MODULE_binary = 0xA5;
  uint8_t Logger_EVENT      = 0x01;
  uint32_t data_u32 = 0x0BADF00D;

  LOG_Config(log_buffer_write);
  log_buffer_clear();
  /* Setup expected function mock */
  

  /* Function under test */
  LOG_DebugFrame(DEBUG_ID, LOG_MODULE_binary, Logger_EVENT, LOG_DATA_32, 0x01, &data_u32);

  /* Verify test results */
  uint8_t expected_transfer[] = {'#', 0x01, 0xA5, 0x01, 0x04, 0x01,
                                  0x0D, 0xF0, 0xAD, 0x0B, 
                                  0xBC,'\n'
                                };
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_transfer, log_buffer, 
                                sizeof(expected_transfer));
}

void
test_logger_core_function_LOG_DebugFrame_multi_length()
{
  /* Ensure known test state */
  uint8_t LOG_MODULE_binary = 0xA5;
  uint8_t Logger_EVENT      = 0x01;
  uint8_t data_u8arr[] = {0x10, 0x20, 0x30};

  LOG_Config(log_buffer_write);
  log_buffer_clear();
  /* Setup expected function mock */
  

  /* Function under test */
  LOG_DebugFrame(DEBUG_ID, LOG_MODULE_binary, Logger_EVENT, LOG_DATA_8, 0x03, &data_u8arr);

  /* Verify test results */
  uint8_t expected_transfer[] = {'#', 0x01, 0xA5, 0x01, 0x01, 0x03,
                                  0x10, 0x20, 0x30,
                                  0x7F,'\n'
                                };
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_transfer, log_buffer, 
                                sizeof(expected_transfer));
}

#ifdef hide
void
test_loggerLOG_Trace()
{
    /* Ensure known test state */
    uint8_t LOG_MODULE_binary = 0xA5;
    uint8_t Logger_EVENT      = 0x01;
    LOG_MODULE_DEFINE(binary);
    
    LOG_Config(log_buffer_write);
    log_buffer_clear();
    /* Setup expected function mock */
    

    /* Function under test */
    LOG_Trace(Logger_EVENT);

    /* Verify test results */
    uint8_t expected_transfer[] = {0, 100, 1};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_transfer, log_buffer, 
                                  sizeof(expected_transfer));
}

void
test_loggerLOG_Info()
{
    /* Ensure known test state */
    uint8_t LOG_MODULE_binary = 0xA5;
    uint8_t Logger_EVENT      = 0x01;
    LOG_MODULE_DEFINE(binary);
    
    LOG_Config(log_buffer_write);
    log_buffer_clear();
    /* Setup expected function mock */

    /* Function under test */
    LOG_Info(Logger_EVENT);

    /* Verify test results */
    uint8_t expected_transfer[] = {2, 100, 1};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_transfer, log_buffer, 
                                  sizeof(expected_transfer));
}

void
test_loggerLOG_Debug()
{
    /* Ensure known test state */
    uint8_t LOG_MODULE_binary = 0xA5;
    uint8_t Logger_EVENT      = 0x01;
    LOG_MODULE_DEFINE(binary);
    
    LOG_Config(log_buffer_write);
    log_buffer_clear();
    /* Setup expected function mock */

    /* Function under test */
    LOG_Debug(Logger_EVENT, LOG_DATA_SIGN | LOG_DATA_16, 0xF00D);

    /* Verify test results */
    uint8_t expected_transfer[] = {1, 100, 1, 0x12, 0x0D, 0xF0};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_transfer, log_buffer, 
                                  sizeof(expected_transfer));
}

void
test_loggerLOG_Warning()
{
    /* Ensure known test state */
    uint8_t LOG_MODULE_binary = 0xA5;
    uint8_t Logger_EVENT      = 0x01;
    LOG_MODULE_DEFINE(binary);
    
    LOG_Config(log_buffer_write);
    log_buffer_clear();
    /* Setup expected function mock */
    

    /* Function under test */
    LOG_Warning(Logger_EVENT, 100 > 10);

    /* Verify test results */
    uint8_t expected_transfer[] = {3, 100, 1};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_transfer, log_buffer, 
                                  sizeof(expected_transfer));
}

void
test_loggerLOG_Error()
{
    /* Ensure known test state */
    uint8_t LOG_MODULE_binary = 0xA5;
    uint8_t Logger_EVENT      = 0x01;
    LOG_MODULE_DEFINE(binary);
    
    LOG_Config(log_buffer_write);
    log_buffer_clear();
    /* Setup expected function mock */

    /* Function under test */
    LOG_Error(Logger_EVENT, LOG_DATA_8, 10, 100 > 10);

    /* Verify test results */
    uint8_t expected_transfer[] = {4, 100, 1, 0x01, 10};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_transfer, log_buffer, 
                                  sizeof(expected_transfer));
}

void
test_loggerLOG_Fatal()
{
    /* Ensure known test state */
    uint8_t LOG_MODULE_binary = 0xA5;
    uint8_t Logger_EVENT      = 0x01;
    LOG_MODULE_DEFINE(binary);
    
    LOG_Config(log_buffer_write);
    log_buffer_clear();
    /* Setup expected function mock */

    /* Function under test */
    LOG_Fatal(Logger_EVENT, 100 > 10);

    /* Verify test results */
    uint8_t expected_transfer[] = {5, 100, 1};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_transfer, log_buffer, 
                                  sizeof(expected_transfer));
}

void
test_loggerLOG_Restart()
{
    /* Ensure known test state */
    LOG_Config(log_buffer_write);
    log_buffer_clear();

    /* Setup expected function mock */

    /* Function under test */
    LOG_Restart();

    /* Verify test results */
    uint8_t expected_transfer[] = {6, 0, 0};
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_transfer, log_buffer, 
                                  sizeof(expected_transfer));
}

#endif // hide