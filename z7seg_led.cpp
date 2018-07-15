/*
 *    z7seg_led.cpp - A library for controlling 7-segment displays
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

#include <SPI.h>
#include "z7seg_led.h"


// Public functions
z7seg_led::z7seg_led(uint8_t output_enable_pin, uint8_t chip_select_pin) {
    _oe_pin = output_enable_pin;
    _cs_pin = chip_select_pin;
    _brightness = 255;
    _use_pattern = 0;

    // Pattern map contains output bit masks for segments
    // from 'A' (0) through 'G' (6) and dot symbol (7)
    _pattern_map[7] = 0b10000000; // dot bits pattern

    // Set default type
    this->set_type(z7seg_led_74hc595);
}


void z7seg_led::set_type(uint8_t type) {
    switch (type) {
        case z7seg_led_74hc4094:
            _cs_level = 1;
            _oe_level = 1;
            break;
        case z7seg_led_74hc595:
            _cs_level = 0;
            _oe_level = 0;
            break;
    };
}


void z7seg_led::begin() {
    pinMode(_oe_pin, OUTPUT);
    pinMode(_cs_pin, OUTPUT);
    digitalWrite(_oe_pin, !_oe_level);
    digitalWrite(_cs_pin, !_cs_level);
}


void z7seg_led::begin_transfer() {
    digitalWrite(_cs_pin, _cs_level);
    digitalWrite(_oe_pin, !_oe_level);
}


void z7seg_led::end_transfer() {
    digitalWrite(_cs_pin, !_cs_level);
    this->_apply_brightness();
}


void z7seg_led::set_brightness(uint8_t value) {
    _brightness = value;
    this->_apply_brightness();
}


void z7seg_led::send_digit(uint8_t value) {
    send_digit(value, false);
}


void z7seg_led::send_digit(uint8_t value, boolean dp) {
    if (value > 15) return;
    uint8_t v = _get_char_bits(value);
    if (dp) v |= _pattern_map[7];
    SPI.transfer(v);
}


void z7seg_led::send_char(char value) {
    this->send_char(value, false);
}


void z7seg_led::send_char(char value, boolean dp) {
    if (value > 127) value = 32;
    uint8_t v = _get_char_bits(value);
    if (dp) v |= _pattern_map[7];
    SPI.transfer(v);
}


void z7seg_led::print(char *c) {
    this->begin_transfer();

    while (*c) {
        if (*(c+1) != '.') {
            this->send_char(*c, false);
        } else {
            this->send_char(*c, true);
            c++;
            if (!*c) break;
        }
        c++;
    }

    this->end_transfer();
}


// Change default pattern
// Example: display.set_pattern("AFEDCBG.");
void z7seg_led::set_pattern(char pattern[8]) {
    // Clean current pattern map
    for (uint8_t i = 0; i < 8; i++) {
        _pattern_map[i] = 0;
    }

    // Map symbols to bits
    for (uint8_t i = 0; i < 8; i++) {
        if (pattern[i] >= 'A' && pattern[i] <= 'G') {
            _pattern_map[pattern[i] - 'A'] |= 1 << i;

        } else if (pattern[i] >= 'a' && pattern[i] <= 'g') {
            _pattern_map[pattern[i] - 'a'] |= 1 << i;

        } else if (pattern[i] == '.') {
            _pattern_map[7] |= 1 << i;
        }
    }

    // Compare pattern map with common ABCDEFG.
    _use_pattern = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (_pattern_map[i] != (1 << i)) {
            _use_pattern = 1;
        }
    }
}


// Private functions
void z7seg_led::_apply_brightness() {
    analogWrite(_oe_pin, (_oe_level) ? _brightness : (255 - _brightness));
}


uint8_t z7seg_led::_get_char_bits(char value) {
    uint8_t v = pgm_read_byte_near(z7seg_led_char_table + value);
    if (_use_pattern == 0) {
        return v;

    } else {
        uint8_t result = 0;

        for (uint8_t i = 0; i < 8; i++) {
            if (v & (1 << i)) {
                result |= _pattern_map[i];
            }
        }

        return result;
    }
}

