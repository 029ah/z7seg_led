#include <z7seg_led.h>
#include <SPI.h>

/*
 * Control 7-segment displays connected via 74hc595 IC
 *
 * Connection:
 *      Arduino           74HC595
 *      MOSI (11)  <--->  DS   (14)
 *      SCK  (13)  <--->  SHCP (12)
 *      _OE  (9*)  <--->  _OE  (13)    * - PWM pin
 *      _CE  ( 8)  <--->  STCP (12)
 *      VCC        <--->  MR   (10)
 */
const int _CE = 8;
const int _OE = 9;

z7seg_led display(_OE, _CE);

void setup() {
    SPI.begin();
    display.begin();
    display.set_pattern("AFEDCBG."); // Mirror digits so '3' will be 'E'
}

void loop()
{
    char s[16];

    sprintf(s, "%08d", millis())
    display.print(s);

    delay(250);
}

