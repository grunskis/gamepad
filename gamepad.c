/* Keyboard example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_keyboard.html
 * Copyright (c) 2008 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "usb_gamepad.h"

#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD &= ~(1<<6))
#define LED_OFF		(PORTD |= (1<<6))

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

#define B0              (1 << 0)
#define B1              (1 << 1)

int main(void) {
  uint8_t b;

  // set for 16 MHz clock
  CPU_PRESCALE(0);
  LED_CONFIG;
  LED_OFF;

  DDRB  = B0;
  PORTB = B0;

  // Initialize the USB, and then wait for the host to set configuration.
  // If the Teensy is powered without a PC connected to the USB port,
  // this will wait forever.
  usb_init();
  while (!usb_configured()) /* wait */ ;

  // Wait an extra second for the PC's operating system to load drivers
  // and do whatever it does to actually be ready for input
  _delay_ms(1000);

  while (1) {
    b = PINB;

   if (b & B0) {
      LED_ON;
      usb_joystick_action(128, 128, 0, 0);
    } else {
      LED_OFF;
      usb_joystick_action(128, 128, 1, 0);
    }
  }
}
 
