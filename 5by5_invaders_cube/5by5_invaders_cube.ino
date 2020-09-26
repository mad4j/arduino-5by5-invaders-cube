
#undef DEBUG
#define DISPLAY_ROTATION

#include "Generators.h"
#include "EPDDriver.h"

#include "qrlogo.h"


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
}


void loop() 
{ 


  EPDDriver::init();
  
  if ((counter++ % 10) == 0)  {
#ifdef DEBUG    
    Serial.println("Clear");
#endif  
    EPDDriver::clear();
  }
  
  uint64_t seed = random(INT32_MAX);
  
#ifdef DEBUG
  Serial.print("Draw: ");
  Serial.println((long)seed, 16);
#endif

  if (random(10) == 0) {
    EPDDriver::display(QR_IMAGE);
  } else {
    EPDDriver::displayGenerator(invGen, seed);
  }
  
#ifdef DEBUG  
  Serial.println("Sleep");
#endif 

  EPDDriver::sleep();

  delay(UPDATE_EVERY_MININUTES*60*1000L);
}
