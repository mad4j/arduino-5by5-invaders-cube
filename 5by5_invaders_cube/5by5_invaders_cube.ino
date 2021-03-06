
#define DEBUG
#define DISPLAY_ROTATION

#include "qrlogo.h"
#include "tiles.h"

#include "generators.h"
#include "invadergen.h"
#include "tilesgen.h"
#include "linesgen.h"
#include "fracgen.h"
#include "icongen.h"

#include "EPDDriver.h"


static const uint8_t DONE_PIN               = 2;

static const uint8_t UPDATE_EVERY_MININUTES = 20;
static const uint8_t QRCODE_FREQUENCY       = 25;

static const uint8_t MAX_GENERATORS         = 5;


// cool pattern generators

InvadersGen invGen;
FracGen fracGen;
IconGen iconGen;
TilesGen mazeGen(MAZE_TILESET , MAZE_TILESET_SIZE, 1);
TilesGen waveGen(WAVE_TILESET , WAVE_TILESET_SIZE, 1);

Generator* generators[] = {
  &invGen,
  &fracGen,
  &mazeGen,
  &waveGen,
  &iconGen
};

void setup() 
{
  // initialize serial port for debug
  Serial.begin(9600);
  
  // reinit random generator
  randomSeed(analogRead(0));

  // DONE pin on low-power timer
  pinMode(DONE_PIN, OUTPUT);
}


void loop() 
{   
  // reinit and clear display
  EPDDriver::init();

  // draw something cool
  if (random(QRCODE_FREQUENCY) == 0) {
    
    // display QR code
    EPDDriver::display(QR_IMAGE);
    
  } else {
    
    // display a random pattern
    uint64_t seed = random(INT32_MAX);
    EPDDriver::displayGenerator(*generators[random(MAX_GENERATORS)], seed);
  }

  // go slepping
  EPDDriver::sleep();

  // wait a while
  delay(500);

  // switch power off
  digitalWrite(DONE_PIN, HIGH);

  // do nothing while switching
  delay(60000);

}
