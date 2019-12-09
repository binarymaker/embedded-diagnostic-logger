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

/**
 * \file complier-cfg.h
 * \author Binary Maker (https://github.com/binarymaker)
 * \brief complier relative macros for optimization
 * \date 2019-10-16
 */

#ifndef COMPLIER_e5a3b768_f000_11e9_95e9_705a0f25cb51
#define COMPLIER_e5a3b768_f000_11e9_95e9_705a0f25cb51

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"

#define _ASM_            __asm                 /*!< asm keyword for target Compiler   */
#define _INLINE_         inline                /*!< inline keyword for target Compiler*/
#define _STATIC_INLINE_  static inline         /*!< static inline combination for \
                                                                inline function */

#define NULL    0
     
typedef uint8_t sfr_t;

#ifdef __cplusplus
}
#endif

#endif /* COMPLIER_e5a3b768_f000_11e9_95e9_705a0f25cb51 */
