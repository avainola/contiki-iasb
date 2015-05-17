/*
 * @author    dresden elektronik ingenieurtechnik gmbh: http://www.dresden-elektronik.de
 * @author    Support email: wireless@dresden-elektronik.de
 *
 * Copyright (c) 2011, dresden elektronik ingenieurtechnik gmbh. All rights reserved.
 *
 * Licensed under dresden elektronik's Limited License Agreement --> deEULA.txt
 */

/* === INCLUDES ============================================================ */
#include <avr/io.h>           // include io definitions
#include <util/delay.h>       // include delay definitions
#include <stdio.h>            // include standard io definitions
#include <stdbool.h>          // include bool definition

#include "usb.h"              // include USB specific definitions

/* === TYPES =============================================================== */

/* === MACROS / DEFINES ==================================================== */

/* === PROTOTYPES ========================================================== */

uint8_t read_usb_byte(void);
void write_usb_byte(uint8_t data);

/* === GLOBALS ============================================================= */

// Setup a File stream with putchar() and getchar() functionality over USB
FILE usb_stream = FDEV_SETUP_STREAM(usb_putc_std, usb_getc_std,_FDEV_SETUP_RW);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief USB interface initialization function.
 *
 * This function initializes the USB hardware.
 * This is only working if platform is Sensor Terminal Board or deRFnode.
 */
void usb_io_init(void)
{
   /* Make sure USB_RXF and USB_TXE are inputs */
   USB_TXE_DDR  &= ~_BV(USB_TXE_PIN);
   USB_TXE_PORT |=  _BV(USB_TXE_PIN);

   USB_RXF_DDR  &= ~_BV(USB_RXF_PIN);
   USB_RXF_PORT |=  _BV(USB_RXF_PIN);

   stdout = &usb_stream;                            // init standard output over USB
   //stdin  = &usb_stream;                            // init standard input over USB
}

/**
 * @brief Checks if a new character received.
 *
 * This is only working if platform is Sensor Terminal Board or deRFnode.
 *
 * @return  Returns '1' if a new character is available on the FTDI USB chip
 *
 */
uint8_t usb_keypressed(void)
{
   return (!(USB_RXF_PINS & _BV(USB_RXF_PIN))) ? 1 : 0;
}

/**
 * @brief Reads a character via USB.
 *
 * This function waits for character available on the USB chip and gives it back.
 * Blocking function.
 * This function is the standard I/O implementation which can be used for std. I/O facilities.
 *
 * This is only working if platform is Sensor Terminal Board or deRFnode.
 *
 * @param     dummy_file  not used
 * @return    character on the USB chip
 *
 */
int usb_getc_std(FILE* dummy_file)
{
   /* wait until new char received */
   while (USB_RXF_PINS & _BV(USB_RXF_PIN)){;}
   /* return the data */
   return(read_usb_byte());
}

/**
 * @brief Writes a character via USB.
 *
 * This function waits for empty buffer and then writes a character to USB.
 * This function is the standard I/O implementation which can be used for std. I/O facilities.
 *
 * This is only working if platform is Sensor Terminal Board or deRFnode.
 *
 * @param     c           the character to write
 * @param     dummy_file  not used
 *
 * @return    char: character written to USB
 *
 */
int usb_putc_std(char c, FILE* dummy_file)
{
   /* Wait for empty transmit buffer */
   while (USB_TXE_PINS & _BV(USB_TXE_PIN)){;}
   /* write the byte into the USB FIFO */
   write_usb_byte(c);
   /* return the char */
   return(c);
}

/**
 * @brief Helper for usb byte input.
 *
 * This function reads a byte from USB interface.
 * This is only working if platform is Sensor Terminal Board or deRFnode.
 *
 * @return    the character read from USB
 *
 */
uint8_t read_usb_byte(void)
{
   uint8_t data = 0;

   /* set Data Directions to input */
   USB_D0_DDR &= ~_BV(USB_D0_PIN);
   USB_D1_DDR &= ~_BV(USB_D1_PIN);
   USB_D2_DDR &= ~_BV(USB_D2_PIN);
   USB_D3_DDR &= ~_BV(USB_D3_PIN);
   USB_D4_DDR &= ~_BV(USB_D4_PIN);
   USB_D5_DDR &= ~_BV(USB_D5_PIN);
   USB_D6_DDR &= ~_BV(USB_D6_PIN);
   USB_D7_DDR &= ~_BV(USB_D7_PIN);

   /*
    * RD and WR are low active
    */
   /* make sure WR pin is high, to enable correct latch direction */
   USB_WR_PORT |= _BV(USB_WR_PIN);
   USB_WR_DDR  |= _BV(USB_WR_PIN);

   /* set RD pin active -> low */
   USB_RD_PORT &= ~_BV(USB_RD_PIN);
   USB_RD_DDR  |=  _BV(USB_RD_PIN);

   /* give chip a chance to put out the data */
   _delay_us(5);

   /* read data */
   if(USB_D0_PINS & _BV(USB_D0_PIN)){ data |= _BV(USB_D0_BIT);} else{ data &= ~_BV(USB_D0_BIT);}
   if(USB_D1_PINS & _BV(USB_D1_PIN)){ data |= _BV(USB_D1_BIT);} else{ data &= ~_BV(USB_D1_BIT);}
   if(USB_D2_PINS & _BV(USB_D2_PIN)){ data |= _BV(USB_D2_BIT);} else{ data &= ~_BV(USB_D2_BIT);}
   if(USB_D3_PINS & _BV(USB_D3_PIN)){ data |= _BV(USB_D3_BIT);} else{ data &= ~_BV(USB_D3_BIT);}
   if(USB_D4_PINS & _BV(USB_D4_PIN)){ data |= _BV(USB_D4_BIT);} else{ data &= ~_BV(USB_D4_BIT);}
   if(USB_D5_PINS & _BV(USB_D5_PIN)){ data |= _BV(USB_D5_BIT);} else{ data &= ~_BV(USB_D5_BIT);}
   if(USB_D6_PINS & _BV(USB_D6_PIN)){ data |= _BV(USB_D6_BIT);} else{ data &= ~_BV(USB_D6_BIT);}
   if(USB_D7_PINS & _BV(USB_D7_PIN)){ data |= _BV(USB_D7_BIT);} else{ data &= ~_BV(USB_D7_BIT);}

   /* set RD pin inactive -> high */
   USB_RD_PORT |= _BV(USB_RD_PIN);

   return data;
}


/**
 * @brief Helper for usb byte output.
 *
 * This function writes a byte to USB interface.
 * This is only working if platform is Sensor Terminal Board or deRFnode.
 *
 * @param   data  data which should be written
 *
 */
void write_usb_byte(uint8_t data)
{
   /* set data pins, depending on character */
   if(data & _BV(USB_D0_BIT)){ USB_D0_PORT |= _BV(USB_D0_PIN);} else{ USB_D0_PORT &= ~_BV(USB_D0_PIN);}
   if(data & _BV(USB_D1_BIT)){ USB_D1_PORT |= _BV(USB_D1_PIN);} else{ USB_D1_PORT &= ~_BV(USB_D1_PIN);}
   if(data & _BV(USB_D2_BIT)){ USB_D2_PORT |= _BV(USB_D2_PIN);} else{ USB_D2_PORT &= ~_BV(USB_D2_PIN);}
   if(data & _BV(USB_D3_BIT)){ USB_D3_PORT |= _BV(USB_D3_PIN);} else{ USB_D3_PORT &= ~_BV(USB_D3_PIN);}
   if(data & _BV(USB_D4_BIT)){ USB_D4_PORT |= _BV(USB_D4_PIN);} else{ USB_D4_PORT &= ~_BV(USB_D4_PIN);}
   if(data & _BV(USB_D5_BIT)){ USB_D5_PORT |= _BV(USB_D5_PIN);} else{ USB_D5_PORT &= ~_BV(USB_D5_PIN);}
   if(data & _BV(USB_D6_BIT)){ USB_D6_PORT |= _BV(USB_D6_PIN);} else{ USB_D6_PORT &= ~_BV(USB_D6_PIN);}
   if(data & _BV(USB_D7_BIT)){ USB_D7_PORT |= _BV(USB_D7_PIN);} else{ USB_D7_PORT &= ~_BV(USB_D7_PIN);}

   /* set Data Directions to output */
   USB_D0_DDR |= _BV(USB_D0_PIN);
   USB_D1_DDR |= _BV(USB_D1_PIN);
   USB_D2_DDR |= _BV(USB_D2_PIN);
   USB_D3_DDR |= _BV(USB_D3_PIN);
   USB_D4_DDR |= _BV(USB_D4_PIN);
   USB_D5_DDR |= _BV(USB_D5_PIN);
   USB_D6_DDR |= _BV(USB_D6_PIN);
   USB_D7_DDR |= _BV(USB_D7_PIN);

   /*
    * RD and WR are low active
    */
   /* make sure RD pin is inactive -> high */
   USB_RD_PORT |= _BV(USB_RD_PIN);
   USB_RD_DDR  |= _BV(USB_RD_PIN);

   /* set WR pin active -> low */
   USB_WR_PORT &= ~_BV(USB_WR_PIN);
   USB_WR_DDR  |=  _BV(USB_WR_PIN);

   /* set WR pin inactive -> high */
   USB_WR_PORT |= _BV(USB_WR_PIN);

   /* --> data has been written */
}

