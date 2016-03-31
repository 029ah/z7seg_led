/*
 *    z7seg_led.h - A library for controlling 7-segment displays
 *    using shift registers connected to SPI
 *    Copyright (c) 2016 Sergey Pershin
 *
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 *
 *    This permission notice shall be included in all copies or
 *    substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _z7seg_led_h
#define _z7seg_led_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include <avr/pgmspace.h>


/*
 * There are only a few characters that make sense here :
 *      '0','1','2','3','4','5','6','7','8','9','0',
 *  'A','b','c','d','E','F','H','L','P',
 *  '.','-','_',' '
 */
const static uint8_t z7seg_led_char_table [] PROGMEM = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111,
    0b01111111, 0b01101111, 0b01110111, 0b01111100, 0b01011000, 0b01011110, 0b01111001, 0b01110001,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b01000000, 0b10000000, 0b00000000,
    0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111,
    0b01111111, 0b01101111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b01110111, 0b01111100, 0b01011000, 0b01011110, 0b01111001, 0b01110001, 0b00000000,
    0b01110110, 0b00000000, 0b00000000, 0b00000000, 0b00111000, 0b00000000, 0b00000000, 0b00000000,
    0b01110011, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000,
    0b00000000, 0b01110111, 0b01111100, 0b01011000, 0b01011110, 0b01111001, 0b01110001, 0b00000000,
    0b01110110, 0b00000000, 0b00000000, 0b00000000, 0b00111000, 0b00000000, 0b01010100, 0b01011100,
    0b01110011, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000
};

/* Shift registers type */
enum z7seg_led_type_e {
    z7seg_led_74hc595 = 0,
    z7seg_led_74hc4094
};


class z7seg_led {
    public:
        /* Create new controller, default shift register type is z7seg_led_74hc595 */
        z7seg_led(uint8_t output_enable_pin, uint8_t chip_select_pin);
        void begin();

        /* Set shift register type provided by z7seg_led_type_e enum */
        void set_type(uint8_t type);

        /* 
         * Set custom pattern for shift register outputs.
         * Default is "ABCDEFG." (output Q0 - 'A', ... Q6 - 'G', Q7 - '.')
         */
        void set_pattern(char pattern[8]);
        
        /* Set display brightness by setting PWM on output_enable_pin */
        void set_brightness(uint8_t value);

        /*
         * Shift out char string.
         * Use '.' char to set decimal point
         * Example: "3.14159" string will turn on decimal point on first display
         */
        void print(char *c);


        /* Prepare shift register for receiving data for send_digit and send_char methods */
        void begin_transfer();
        void end_transfer();

        /* Shift out one digit or char */
        void send_digit(uint8_t value);
        void send_char(char value);

        /* Shift out ont digit or char, dp sets decimal point */
        void send_digit(uint8_t value, boolean dp);
        void send_char(char value, boolean dp);

    private:
        void _apply_brightness();
        uint8_t _get_char_bits(char value);

        uint8_t _oe_pin, _cs_pin, _cs_level, _oe_level;
        uint8_t _brightness;
        uint8_t _pattern_map[8];
        bool _use_pattern;
};

#endif

