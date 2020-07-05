
#include <avr/sleep.h>

#include "Generators.h"
#include "EPDDriver.h"

static const uint8_t UPDATE_EVERY_MININUTES = 5;

InvadersGen invGen;

void setup() 
{
  Serial.begin(115200);
  Serial.println("START");
  randomSeed(analogRead(0));
}


void loop() 
{
  uint16_t counter = 0;
  
  if (counter++ % 100 == 0)  {
    Serial.println("Clear");
    EPDDriver::init();
    EPDDriver::clear();
  }
  
  uint64_t seed = random(INT32_MAX);
  Serial.print("Draw: ");
  Serial.println((long)seed, 16);
  
  EPDDriver::init();
  EPDDriver::displayGenerator(invGen, seed);
  
  Serial.println("Sleep");
  EPDDriver::sleep();

  delay(UPDATE_EVERY_MININUTES*60*1000L);
}
