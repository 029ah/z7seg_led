#include <z7seg_led.h>
#include <SPI.h>

/*
 * Control 7-segment display connected via 74HC4094 IC
 *
 * Connection:
 *      Arduino           74HC4094
 *      MOSI (11)  <--->  D    ( 2)
 *      SCK  (13)  <--->  CLK  ( 3)
 *      OE   (9*)  <--->  OE   (15)    * - PWM pin
 *      CE   ( 8)  <--->  STR  ( 1)
 *      VCC        <--->  MR   (10)
 */
const int _CE = 8;
const int _OE = 9;

z7seg_led display(_OE, _CE);

void setup() {
    SPI.begin();
    display.begin();
    display.set_type(z7seg_led_74hc4094);
}

void loop()
{
    static int i = 0;

    display.begin_transfer();
    display.send_digit(i++);
    display.end_transfer();

    if (i >= 10) {
        i = 0;
    }

    delay(250);
}

