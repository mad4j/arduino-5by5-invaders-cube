
#define DEBUG
#define DISPLAY_ROTATION

#include "qrlogo.h"
#include "tiles.h"

#include "generators.h"
#include "invadergen.h"
#include "tilegen.h"

#include "EPDDriver.h"


static const uint8_t UPDATE_EVERY_MININUTES = 20;
static const uint8_t QRCODE_FREQUENCY       = 25;

static const uint8_t MAX_GENERATORS         = 3;


//cool pattern generators

InvadersGen invGen;
TileGen mazeGen(MAZE_TILESET , MAZE_TILESET_SIZE, 1);
//TileGen waveGen(WAVE_TILESET , WAVE_TILESET_SIZE, 2);

Generator* generators[] = {
  &invGen,
  &mazeGen,
//  &waveGen
};

void setup() 
{
  Serial.begin(9600);
  
  //reinit random generator
  randomSeed(analogRead(0));
}


void loop() 
{   
  //reinit display
  EPDDriver::init();
  //EPDDriver::clear();
  
  //draw something cool
  if (random(QRCODE_FREQUENCY) == 0) {
    
    //display QR code
    EPDDriver::display(QR_IMAGE);
    
  } else {
    
    //display a random pattern
    uint64_t seed = random(INT32_MAX);
    EPDDriver::displayGenerator(*generators[random(MAX_GENERATORS)], seed);
  }

  //go slepping
  EPDDriver::sleep();

  delay(60000);

}
