
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "Generators.h"
#include "EPDDriver.h"

#include "LowPower.h"

static const uint8_t UPDATE_EVERY_MININUTES = 10;

InvadersGen invGen;

static uint16_t counter = 0;


void setup() 
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("START");
#endif
  
  randomSeed(analogRead(0));

//  sleep_enable();
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
 
}


void loop() 
{ 
  if ((counter++ % 10) == 0)  {
#ifdef DEBUG    
    Serial.println("Clear");
#endif
    EPDDriver::init();
    EPDDriver::clear();
  }
  
  uint64_t seed = random(INT32_MAX);
  
#ifdef DEBUG
  Serial.print("Draw: ");
  Serial.println((long)seed, 16);
#endif
  
  EPDDriver::init();
  EPDDriver::displayGenerator(invGen, seed);
  
#ifdef DEBUG  
  Serial.println("Sleep");
#endif 

  EPDDriver::sleep();
/*
  for (auto i=0; i<(UPDATE_EVERY_MININUTES*60)/8; i++) {
    Serial.println(i);
    wdt_enable(WDTO_8S);
    sleep_cpu();
  }
*/
  delay(UPDATE_EVERY_MININUTES*60*1000L);
}
