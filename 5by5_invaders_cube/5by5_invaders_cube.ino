
#include <avr/sleep.h>

#include "generators.h"

#include "EPDDriver.h"


InvadersGen invGen;

void setup() 
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
}


void loop() 
{
  Serial.println("Reset and Init");
  EPDDriver::init();
  
  uint16_t seed = random(1<<15);
  Serial.println(seed);
  
  EPDDriver::displayGenerator(invGen, seed);

  Serial.println("Sleep");
  EPDDriver::sleep();

  delay(5*60*1000L);
}
