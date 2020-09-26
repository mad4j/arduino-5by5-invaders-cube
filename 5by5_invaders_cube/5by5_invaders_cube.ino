
#define DEBUG
#define DISPLAY_ROTATION

#include "Generators.h"
#include "EPDDriver.h"

#include "qrlogo.h"


static const uint8_t UPDATE_EVERY_MININUTES = 10;
static const uint8_t QRCODE_FREQUENCY       = 10;


//cool pattern generator
InvadersGen invGen;


void setup() 
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("SETUP");
#endif
}


void loop() 
{ 
#ifdef DEBUG
  Serial.println("LOOP");
#endif

  //reint random generator
  randomSeed(analogRead(0));

  //reinit display
  EPDDriver::init();
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
  delay(UPDATE_EVERY_MININUTES*60*1000L);
}
