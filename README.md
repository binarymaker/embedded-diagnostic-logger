[![Build Status](https://travis-ci.org/binarymaker/embedded-diagnostic-logger.svg?branch=develop)](https://travis-ci.org/binarymaker/embedded-diagnostic-logger) [![codecov](https://codecov.io/gh/binarymaker/embedded-diagnostic-logger/branch/develop/graph/badge.svg)](https://codecov.io/gh/binarymaker/embedded-diagnostic-logger)

# Embedded diagnostic logger

Logging system for small microcontroller projects.


# Usage

```c
File: main.c

#include "log.h"
#include "uart.h" /* user develop library for serial uart */


LOG_MODULE_DEFINE(main); /* module name should same as file name */

void
main(void)
{
  uint16_t debug_data_xyz_u16 = 100u;

  UART_init();

  /* Replace UART_SendChar to user defined function which is send a byte thru uart*/
  LOG_Config(UART_SendChar);
  
  LOG_Restart();
  
  LOG_Trace(Application_Trace_message_EVENT);

  LOG_Info(Application_Info_massage_EVENT);
  
  LOG_Debug(Application_debug_message_EVENT, LOG_DATA_16, &debug_data_xyz_u16);
  
  /* condition true means warning will go */
  LOG_Warning(Application_Warning_message_EVENT, debug_data_xyz_u16 > 10u);
  
  while(1)
  {

  }
}

```

output in logger-client-console.py application

```
10:54:41    0.008 main    11  TRACE --> Application Trace message
10:54:41    0.010 main    13  INFO  --> Application Info massage
10:54:41    0.012 main    15  DEBUG --> Application debug message   [100]
10:54:41    0.015 main    17  WARN  --> Application Warning message
```

Actual data transfer from micrcontroller to client app
```
0x24 0x06 0x00 0x00 0x00 -> for LOG_Restart()
0x24 0x00 0x00 0x00 0x00 -> for LOG_Trace(Application_Trace_message_EVENT);
0x24 0x01 0x00 0x01 0x6B -> for LOG_Info(Application_Info_massage_EVENT);
0x24 0x02 0x00 0x02 0x02 0x01 0x64 0x00 0xB9 -> 
 for LOG_Debug(Application_debug_message_EVENT, LOG_DATA_16, &debug_data_xyz_u16);
0x24 0x03 0x00 0x03 0xB4 -> 
 for LOG_Warning(Application_Warning_message_EVENT, debug_data_xyz_u16 > 10u);
```
