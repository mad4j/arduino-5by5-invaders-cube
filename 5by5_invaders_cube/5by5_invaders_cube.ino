
#define DEBUG
#define DISPLAY_ROTATION

#include <Wire.h>
#include <avr/sleep.h>

#include "DS3231.h"

#include "Generators.h"
#include "EPDDriver.h"

#include "qrlogo.h"


static const uint8_t UPDATE_EVERY_MININUTES = 20;
static const uint8_t QRCODE_FREQUENCY       = 25;


//cool pattern generator
InvadersGen invGen;

//access to real-time clock functions
DS3231 RTC;


void initRTC(byte control, bool which)
{
  Wire.beginTransmission(0x68);

  Wire.write((which) ? 0x0f : 0x0e); 
  Wire.write(control);
  
  Wire.endTransmission();
}

void wakeup()
{
  //wakeup and disable interrupt to avoid loops
  sleep_disable();
}

void gosleep()
{
  sleep_enable();
  delay(100);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();
  sleep_bod_disable();
  sei();
  sleep_cpu();

  // *** WAKE HAPPENS HERE *** ///

  RTC.clearAlarm1();
}


void setup() 
{
  Serial.begin(9600);
  
  //reinit random generator
  randomSeed(analogRead(0));

  //DS3231 library initialization
  Wire.begin();
  RTC.begin();
  initRTC(0b11110111, false);

  RTC.armAlarm1(false);
  RTC.armAlarm2(false);
  RTC.clearAlarm1();
  RTC.clearAlarm2();

  //seup wakeup interrupt
  pinMode(2, INPUT);
  attachInterrupt(0, wakeup, FALLING);

  //reinit display
  EPDDriver::init();
}


void loop() 
{ 

  Serial.println("LOOP");
  
  // clear display
  EPDDriver::clear();
  
  //draw something cool
  if (random(QRCODE_FREQUENCY) == 0) {
    
    //display QR code
    EPDDriver::display(QR_IMAGE);
    
  } else {
    
    //display a random pattern
    uint64_t seed = random(INT32_MAX);
    EPDDriver::displayGenerator(invGen, seed);
  }

  //go slepping
  EPDDriver::sleep();

  // set and arm alarm1 on RTC
  RTC.setDateTime(2021, 3, 14, 0, 0, 0);
  RTC.setAlarm1(0, UPDATE_EVERY_MININUTES / 60, UPDATE_EVERY_MININUTES % 60, 0, DS3231_MATCH_M_S);
  delay(100);

  Serial.flush();
  Serial.end();

  gosleep();

  /// *** RESUME LOOP FROM HERE *** ///

  Serial.begin(9600);
}
