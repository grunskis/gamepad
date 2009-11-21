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
#define LED_OFF		(PORTD &= ~(1<<6))
#define LED_ON		(PORTD |= (1<<6))

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

#define BUTTON1 (1 << 0)
#define BUTTON2 (1 << 1)

#define JOYSTICK_RIGHT (1 << 2)
#define JOYSTICK_LEFT  (1 << 3)
#define JOYSTICK_UP    (1 << 4)
#define JOYSTICK_DOWN  (1 << 5)

#define BUTTON_PINS   (BUTTON1 | BUTTON2)
#define JOYSTICK_PINS (JOYSTICK_LEFT | JOYSTICK_RIGHT | JOYSTICK_UP | JOYSTICK_DOWN)

int main(void) {
  uint8_t b, x, y;

  // set for 16 MHz clock
  CPU_PRESCALE(0);
  LED_CONFIG;
  LED_ON; // power up led on startup for 1 sec

  PORTB = (BUTTON_PINS | JOYSTICK_PINS); // enable pull-ups on button & joystick pins

  // Initialize the USB, and then wait for the host to set configuration.
  // If the Teensy is powered without a PC connected to the USB port,
  // this will wait forever.
  usb_init();
  while (!usb_configured()) /* wait */ ;

  // Wait an extra second for the PC's operating system to load drivers
  // and do whatever it does to actually be ready for input
  _delay_ms(1000);

  while (1) {
    b = 0; // assume no buttons pressed
    x = y = 128; // assume no joystick movement
    
    if ((PINB & JOYSTICK_LEFT) == 0) {
      x = 0;
    } else if ((PINB & JOYSTICK_RIGHT) == 0) {
      x = 255;
    }

    if ((PINB & JOYSTICK_UP) == 0) {
      y = 0;
    } else if ((PINB & JOYSTICK_DOWN) == 0) {
      y = 255;
    }

    if ((PINB & BUTTON1) == 0) {
      b = BUTTON1;
    }
    if ((PINB & BUTTON2) == 0) {
      b |= BUTTON2;
    }

    usb_joystick_action(x, y, b);

    // debug message
    if (x != 128 || y != 128 || b != 0) {
      LED_ON;
    } else {
      LED_OFF;
    }
  }
}
 
