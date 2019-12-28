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
#include "mcu-cfg.h"
#include "mcu.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
enum intSense_e
{
INT_SENSE_LOW     ,
INT_SENSE_CHANGE  ,
INT_SENSE_FALLING ,
INT_SENSE_RISING
};
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* ########################################################################## */
/*                        GPIO INITIALIZATION SETTINGS                        */
/* ########################################################################## */

void
GPIO_Config()
{
  /**
   * Port Direction ------------------------------------------------------------
   * _O_ - Output
   * _I_ - Input      
   */
  DDRB    =    _O_(7)| _O_(6)| _O_(5)| _O_(4)| _O_(3)| _O_(2)| _O_(1)| _O_(0)  ; 
  DDRC    =    _O_(7)| _O_(6)| _O_(5)| _O_(4)| _O_(3)| _O_(2)| _O_(1)| _O_(0)  ; 
  DDRD    =    _O_(7)| _O_(6)| _O_(5)| _O_(4)| _O_(3)| _O_(2)| _O_(1)| _O_(0)  ;
  
  /**
   * Port value ----------------------------------------------------------------
   *       OUTPUT | INPUT
   * _L_ - Low      Open drain
   * _H_ - High     PullUp
   */
  PORTB   =    _L_(7)| _L_(6)| _L_(5)| _L_(4)| _L_(3)| _L_(2)| _L_(1)| _L_(0)  ;
  PORTC   =    _L_(7)| _L_(6)| _L_(5)| _L_(4)| _L_(3)| _L_(2)| _L_(1)| _L_(0)  ;
  PORTD   =    _L_(7)| _L_(6)| _L_(5)| _L_(4)| _L_(3)| _L_(2)| _L_(1)| _L_(0)  ;
}

/* ########################################################################## */
/*                  EXTERNAL AND PIN CHANGE INTERRUPT SETTING                 */
/* ########################################################################## */

void
EXTERNAL_INTERRUPT_config(void)
{
  /**
   * External interrupts ------------------------------------------------------
   * EIMSK
   *   _L_ - Disable
   *   _H_ - Enable
   * EICRC
   *   INT_SENSE_LOW_LEVEL
   *   INT_SENSE_ANY_EDGE
   *   INT_SENSE_FALLING_EDGE
   *   INT_SENSE_RISING_EDGE
   *
   *                    INT1                |         INT0                      
   */
  EIMSK   =            _L_(1)               |        _L_(0)                    ;
  EICRA   =    (INT_SENSE_RISING << 2)      |    INT_SENSE_RISING              ; 

  /**
   * Pin change interrupt ------------------------------------------------------
   * PCIRC
   *   _L_ - Disable
   *   _H_ - Enable
   * 
   *              PCIE2      |     PCIE1      |     PCIE0                       
   */
  PCICR   =       _L_(2)     |     _L_(1)     |     _L_(0)                     ;
  
  /**
   * Pin change interrupt channels ---------------------------------------------
   * _L_ - Disable particular pin
   * _H_ - Enable particular pin
   * 
   * PCINT0->|   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
   * PCINT1->|       |   14  |   13  |   12  |   11  |   10  |   9   |   8   |
   * PCINT2->|   23  |   22  |   21  |   20  |   19  |   18  |   17  |   16  |
   */
  PCMSK0  =    _L_(7)| _L_(6)| _L_(5)| _L_(4)| _L_(3)| _L_(2)| _L_(1)| _L_(0)  ;
  PCMSK1  =            _L_(6)| _L_(5)| _L_(4)| _L_(3)| _L_(2)| _L_(1)| _L_(0)  ;
  PCMSK2  =    _L_(7)| _L_(6)| _L_(5)| _L_(4)| _L_(3)| _L_(2)| _L_(1)| _L_(0)  ;
}

/* ########################################################################## */
/*                         ADC INITIALIZATION SETTING                         */
/* ########################################################################## */

void
ADC_Config()
{
  /**
   * ADC module activate -------------------------------------------------------
   *            
   *            ADEN 
   * Enable  -  _H_
   * Disable -  _L_
   */
  ADCSRA    =    _H_(ADEN)                                                     ;
  
  /**
   * ADC prescaler selection ---------------------------------------------------
   *  clk
   * Divider       ADPS2   |   ADPS1  |   ADPS0
   *   2     -      _L_    |   _L_    |    _L_
   *   2     -      _L_    |   _L_    |    _H_
   *   4     -      _L_    |   _H_    |    _L_
   *   8     -      _L_    |   _H_    |    _H_
   *  16     -      _H_    |   _L_    |    _L_
   *  32     -      _H_    |   _L_    |    _H_
   *  64     -      _H_    |   _H_    |    _L_
   * 128     -      _H_    |   _H_    |    _H_
   */
  ADCSRA   |=    _L_(ADPS2) |   _L_(ADPS1)   |   _L_(ADPS0)                    ;
  
  /**
   * ADC result data justify ---------------------------------------------------
   *          
   *          ADLAR                  ADCH    ADCL
   * Right  -  _L_             ex: 000000DD DDDDDDDD
   * Left   -  _H_             ex: DDDDDDDD DD000000
   */
  ADMUX     =    _L_(ADLAR)                                                    ;
  
  /**
   * ADC Trigger mode ----------------------------------------------------------
   * 
   *           ADATE 
   * Enable  -  _H_
   * Disable -  _L_
   */
  ADCSRA   |=    _L_(ADATE)                                                    ;
  
  /**
   * ADC Trigger source selection
   *                      ADTS2   |   ADTS1  |   ADTS0
   * Free run       -      _L_    |   _L_    |    _L_
   * Analog cmpr    -      _L_    |   _L_    |    _H_
   * Ext Intrpt 0   -      _L_    |   _H_    |    _L_
   * Tim0 cmpr A    -      _L_    |   _H_    |    _H_
   * Tim0 overflow  -      _H_    |   _L_    |    _L_
   * Tim1 cmpr B    -      _H_    |   _L_    |    _H_
   * Tim1 overflow  -      _H_    |   _H_    |    _L_
   * Tim1 capture   -      _H_    |   _H_    |    _H_
   */
  ADCSRB   |=    _L_(ADTS2)  |   _L_(ADTS1)   |   _L_(ADTS0)                   ;
  
  /**
   * ADC Channel enable / Digital pin disable ----------------------------------
   *              
   * Analog pin   -   _H_
   * Digital pin  -   _L_ 
   */
  DIDR0     =   _L_(5) | _L_(4) | _L_(3) | _L_(2) | _L_(1) | _H_(0)            ;

  /**
   * ADC Interrupt activate ----------------------------------------------------
   * 
   *            ADIE 
   * Enable  -  _H_
   * Disable -  _L_
   */
  ADCSRA   |=    _L_(ADIE)                                                     ;
}

/* ########################################################################## */
/*                        USART INITIALIZATION SETTING                        */
/* ########################################################################## */

void
USART_Config()
{
  /**
   * Baud rate register value --------------------------------------------------
   * 
   * use USART_BAUD_REG_CALC() for normal speed
   * use USART_BAUD_REG_2X_CALC for 2x speed
   */
  UBRR0     =    USART_BAUD_REG_CALC(9600UL)                                   ;
  
  /**
   * Baud rate speed increase by 2
   * 
   *            U2X0
   * Enable  -  _H_ 
   * Disable -  _L_ 
   */
  UCSR0A    =    _L_(U2X0)                                                     ;
  
  /**
   * USART transmit and receive activation -------------------------------------
   * 
   *          Receive | Transmit
   *           RXEN0  |  TXEN0
   * Enable  -  _H_   |   _H_
   * Disable -  _L_   |   _L_
   */
  UCSR0B    =     _L_(RXEN0)    |     _H_(TXEN0)                               ;
  UCSR0B   |=     _L_(RXCIE0)   |     _L_(TXCIE0)     |   _L_(UDRIE0)          ;
  
  /**
   * Frame Character size ------------------------------------------------------
   * 
   *         UCSZ02 |  UCSZ01 |  UCSZ00
   * 5 bit -   _L_  |   _L_   |   _L_
   * 6 bit -   _L_  |   _L_   |   _H_
   * 7 bit -   _L_  |   _H_   |   _L_
   * 8 bit -   _L_  |   _H_   |   _H_
   * 9 bit -   _H_  |   _H_   |   _H_
   */
  UCSR0C    =     _H_(UCSZ01)   |     _H_(UCSZ00)                              ;
  UCSR0B   |=     _L_(UCSZ02)                                                  ;
  
  /**
   * Frame Stop bit size -------------------------------------------------------
   * 
   *         USBS0
   * 1 bit -  _L_
   * 2 nit -  _H_
   */
  UCSR0C   |=     _L_(USBS0)                                                   ;
  
  /**
   * Frame Parity bit setting --------------------------------------------------
   * 
   *            UMP01  |  UMP00
   * Disable -   _L_   |   _L_
   * Even    -   _H_   |   _L_
   * Odd     -   _H_   |   _H_
   */
  
  UCSR0C   |=     _L_(UMP01)    |    _L_(UMP01)                                ;
}

/* ########################################################################## */
/*                       I2C/TWI INITIALIZATION SETTING                       */
/* ########################################################################## */

void
I2C_Config()
{
  /**
   * I2C Prescaler -------------------------------------------------------------
   * 
   * Prescale    TWPS1   |  TWPS0
   *    1     -   _L_    |   _L_
   *    4     -   _L_    |   _H_
   *   16     -   _H_    |   _L_
   *   64     -   _H_    |   _H_
   */
  TWSR     =    _L_(TWPS1)    |    _L_(TWPS0)                                  ;
  
  /**
   * I2C clock frequency -------------------------------------------------------
   * Argument 1 - I2C clock frequency
   * Argument 2 - Prescale value
   */
  TWBR     =    I2C_CLOCK_BITRATE_REG_CALC(400000UL, 1UL)                      ;
}

/* ########################################################################## */
/*                        TIMER0 INITIALIZATION SETTING                       */
/* ########################################################################## */

void
TIMER0_Config()
{
  /**
   * Timer clock selection -----------------------------------------------------
   * 
   * clk div           CS02    |   CS01    |   CS00
   * timer off       -  _L_    |    _L_    |    _L_
   * 1               -  _L_    |    _L_    |    _H_
   * 8               -  _L_    |    _H_    |    _L_
   * 64              -  _L_    |    _H_    |    _H_
   * 256             -  _H_    |    _L_    |    _L_
   * 1024            -  _H_    |    _L_    |    _H_
   * Ext T0 falling  -  _H_    |    _H_    |    _L_
   * Ext T0 rising   -  _H_    |    _H_    |    _H_     
   */
  TCCR0B    =     _L_(CS02)    |    _H_(CS01)    |    _H_(CS00)                ;
  
  /**
   * Timer0 count value --------------------------------------------------------
   */
  TCNT0     =   0x00                                                           ;
  
  /**
   * Timer0 Output compare register
   */
  OCR0A     =   0xff                                                           ;
  OCR0B     =   0xff                                                           ;
  
 /**
   * Waveform generation mode --------------------------------------------------
   * Common for OC0A and OC0B
   * 
   * Mode | TimerMode  | TOP       WGM02   |   WGM01   |   WGM00
   *  0   | Normal     | 0xFF    -  _L_    |    _L_    |    _L_
   *  1   | Phase PWM  | 0xFF    -  _L_    |    _L_    |    _H_
   *  2   | CTC(1)     | OCRA    -  _L_    |    _H_    |    _L_
   *  3   | Fast PWM   | 0xFF    -  _L_    |    _H_    |    _H_
   *  4   | Reserved   |         -  _H_    |    _L_    |    _L_
   *  5   | Phase PWM  | OCRA    -  _H_    |    _L_    |    _H_
   *  6   | Reserved   |         -  _H_    |    _H_    |    _L_
   *  7   | Fast PWM   | OCRA    -  _H_    |    _H_    |    _H_
   * 
   * (1) Clear timer on match
   */
  TCCR0A    =     _L_(WGM01)     |    _L_(WGM00)                               ;
  TCCR0B   |=     _L_(WGM02)                                                   ;
  
  /**
   * OC0A - Compare match output mode ------------------------------------------
   *                               Phase
   * non-PWM    |   Fast-PWM   | Correct-PWM    COM0A1   |   COM0A0
   * Disconnect |  Disconnect  |  Disconnect  -   _L_    |    _L_
   * Toggle     |  Toggle(1)   |  Toggle(1)   -   _L_    |    _H_
   * Clear      |  Normal PWM  |  Invert PWM  -   _H_    |    _L_
   * Set        |  Invert PWM  |  Normal PWM  -   _H_    |    _H_  
   * 
   * (1) Toggle function in WGM02 = 1, Disconnect in WGM02 = 0 
   */
  TCCR0A   |=     _L_(COM0A1)    |    _L_(COM0A0)                              ;
  
  /**
   * OC0B - Compare match output mode ------------------------------------------
   *                              Phase
   * non-PWM    |   Fast-PWM   | Correct-PWM    COM0B1   |   COM0B0
   * Disconnect |  Disconnect  |  Disconnect  -   _L_    |    _L_
   * Toggle     |  Reserved    |  Reserved    -   _L_    |    _H_
   * Clear      |  Normal PWM  |  Invert PWM  -   _H_    |    _L_
   * Set        |  Invert PWM  |  Normal PWM  -   _H_    |    _H_  
   */
  TCCR0A   |=     _L_(COM0B1)    |    _L_(COM0B0)                              ;
    
  /**
   * Timer0 interrupt activation -----------------------------------------------
   *           
   *             overflow  |  match A  |  match B
   *              TOIE0    |  OCIE0A   |  OCIE0B
   * Enable    -   _H_     |   _H_     |   _H_
   * Disable   -   _L_     |   _L_     |   _L_
   */
   TIMSK0    =     _H_(TOIE0)     |     _L_(OCIE0A)     |     _L_(OCIE0B)      ;
}