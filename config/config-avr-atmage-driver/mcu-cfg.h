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

#ifndef MCU_a1c651d2_fb96_11e9_9e08_0c5b8f279a64
#define MCU_a1c651d2_fb96_11e9_9e08_0c5b8f279a64

#ifdef __cplusplus
 extern "C" {
#endif

#define F_CPU                                                       (16000000UL)

#define MCU_GPIO_DRIVER                                                  ENABLE 
#define MCU_EXTERNAL_INTERRUPT_DRIVER                                    DISABLE
#define MCU_ADC_DRIVER                                                   DISABLE
#define MCU_USART_DRIVER                                                 ENABLE
#define MCU_I2C_DRIVER                                                   DISABLE
#define MCU_SPI_DRIVER                                                   DISABLE
#define MCU_TIMER0_DRIVER                                                ENABLE
#define MCU_TIMER1_DRIVER                                                DISABLE
#define MCU_TIMER2_DRIVER                                                DISABLE

#define IS_DRIVER_ENABLE(module)               (MCU_##module##_DRIVER == ENABLE)

/* ############################# SYSTIMER DRIVER ############################ */

#define SYSTIMER_PRESCALLER                                                (64U)
#define SYSTIMER_MAX_COUNT                                                (256U)
#define MICROSECONDS_PER_SYSTIMER_OVERFLOW                                     \
           CLOCK_CYCLE_TO_MICROSECONDS(SYSTIMER_PRESCALLER * SYSTIMER_MAX_COUNT)

/* ############################### GPIO DRIVER ############################## */

#if IS_DRIVER_ENABLE(GPIO)

#define GPIO_PIN_PER_PORT                                                   (8U)

void
GPIO_Config();

#endif

/* ################ EXTERNAL AND PIN CHANGE INTERRUPT DRIVER ################ */

#if IS_DRIVER_ENABLE(EXTERNAL_INTERRUPT)

void
EXTERNAL_INTERRUPT_config(void);

#endif

/* ############################### ADC DRIVER ############################### */

#if IS_DRIVER_ENABLE(ADC)

void
ADC_Config();

#endif

/* ############################## USART DRIVER ############################## */

#if IS_DRIVER_ENABLE(USART)

#define USART_BAUD_REG_CALC(baud)        (uint16_t)((F_CPU / (16 * (baud))) - 1)
#define USART_BAUD_REG_2X_CALC(baud)      (uint16_t)((F_CPU / (8 * (baud))) - 1)

void
USART_Config();

#endif

/* ############################### I2C DRIVER ############################### */

#if IS_DRIVER_ENABLE(I2C)

#define I2C_STATUS_RETURN                                                DISABLE

#define I2C_CLOCK_BITRATE_REG_CALC(i2cClkFreq, i2cPrescale)                    \
                            ((F_CPU / ((i2cClkFreq) * (i2cPrescale)) - 16 ) / 2)
void
I2C_Config();

#endif

/* ############################## TIMER0 DRIVER ############################# */

#if IS_DRIVER_ENABLE(TIMER0)

void
TIMER0_Config();

#endif

/* ########################################################################## */


#ifdef __cplusplus
}
#endif

#endif /* MCU_a1c651d2_fb96_11e9_9e08_0c5b8f279a64 */

