#pragma once

/**
 * Connection Table
 * --------------------------
 * E-Paper        Arduino UNO 
 *  3.3V              3V3
 *  GND               GND
 *  DIN               D11
 *  CLK               D13
 *  CS                D10
 *  DC                D9
 *  RST               D8
 *  BUSY              D7 
 */


#include <SPI.h>


// Pin definition
static const uint8_t DC_PIN = 9;
static const uint8_t CS_PIN = 10;
static const uint8_t BUSY_PIN = 7;
static const uint8_t RST_PIN = 8;

// Display resolution
static const uint16_t EPD_WIDTH = 200;
static const uint16_t EPD_HEIGHT = 200;


class EPDDriver 
{
public:
  EPDDriver();
  virtual ~EPDDriver();

  static void init();
  static void spiTransfer(uint8_t data);

  static void sendCommand(uint8_t command);
  static void sendData(uint8_t data);

  static void display(const uint8_t* frame_buffer);
  static void displayGenerator(Generator& gen, uint64_t seed);
  
  static void waitUntilIdle();
  static void reset();
  static void clear();
  static void sleep();
};

EPDDriver::EPDDriver()
{
  
}


EPDDriver::~EPDDriver() 
{
  
}


void EPDDriver::init() 
{
  pinMode(CS_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(DC_PIN, OUTPUT);
  pinMode(BUSY_PIN, INPUT); 

  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

  /* EPD hardware init start */
  reset();
  waitUntilIdle();

  sendCommand(0x12);  //SWRESET
  waitUntilIdle();

  sendCommand(0x01); //Driver output control
  sendData(0xC7);
  sendData(0x00);
  sendData(0x01);

  sendCommand(0x11); //data entry mode
  sendData(0x01);

  sendCommand(0x44); //set Ram-X address start/end position
  sendData(0x00);
  sendData(0x18);    //0x0C-->(18+1)*8=200

  sendCommand(0x45); //set Ram-Y address start/end position
  sendData(0xC7);   //0xC7-->(199+1)=200
  sendData(0x00);
  sendData(0x00);
  sendData(0x00);

  sendCommand(0x3C); //BorderWavefrom
  sendData(0x01);

  sendCommand(0x18);
  sendData(0x80);

  sendCommand(0x22); // //Load Temperature and waveform setting.
  sendData(0XB1);
  sendCommand(0x20);

  sendCommand(0x4E);   // set RAM x address count to 0;
  sendData(0x00);
  sendCommand(0x4F);   // set RAM y address count to 0X199;
  sendData(0xC7);
  sendData(0x00);
  waitUntilIdle();
  /* EPD hardware init end */
}


void EPDDriver::spiTransfer(uint8_t data) 
{
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(data);
  digitalWrite(CS_PIN, HIGH);
}


void EPDDriver::sendCommand(uint8_t command)
{
  digitalWrite(DC_PIN, LOW);
  spiTransfer(command);
}


void EPDDriver::sendData(uint8_t data)
{
  digitalWrite(DC_PIN, HIGH);
  spiTransfer(data);
}


void EPDDriver::waitUntilIdle(void)
{
  while(digitalRead(BUSY_PIN) == 1) {      //LOW: idle, HIGH: busy
    delay(100);
  }
  delay(200);
}

void EPDDriver::reset()
{
  digitalWrite(RST_PIN, HIGH);
  delay(200);
  digitalWrite(RST_PIN, LOW);                //module reset
  delay(20);
  digitalWrite(RST_PIN, HIGH);
  delay(200);
}


void EPDDriver::clear()
{
  sendCommand(0x24);
  for (int j = 0; j < EPD_HEIGHT; j++) {
    for (int i = 0; i < EPD_WIDTH; i+=8) {
      sendData(0xFF);
    }
  }

  //DISPLAY REFRESH
  sendCommand(0x22);
  sendData(0xC7);
  sendCommand(0x20);
  waitUntilIdle();
}


void EPDDriver::display(const uint8_t* frame_buffer)
{
  int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
  int h = EPD_HEIGHT;

  if (frame_buffer != NULL) {
    sendCommand(0x24);
    for (int j = 0; j < h; j++) {
      for (int i = 0; i < w; i++) {
        sendData(pgm_read_byte(&frame_buffer[i + j * w]));
      }
    }
  }

  //DISPLAY REFRESH
  sendCommand(0x22);
  sendData(0xF7);
  sendCommand(0x20);
  waitUntilIdle();
}


void EPDDriver::displayGenerator(Generator& gen, uint64_t seed)
{
  gen.init(seed, EPD_WIDTH, EPD_HEIGHT);

  sendCommand(0x24);
  for (int j = 0; j < EPD_HEIGHT; j++) {
    for (int i = 0; i < EPD_WIDTH; i+=8) {
      if (gen.generate(i, j) == 0) {
        sendData(0x00);
      } else {
        sendData(0xFF);
      }
    }
  }
  
  //DISPLAY REFRESH
  sendCommand(0x22);
  sendData(0xC7);
  sendCommand(0x20);
  waitUntilIdle();
}


void EPDDriver::sleep()
{
  sendCommand(0x10); //enter deep sleep
  sendData(0x01);
  delay(200);

  digitalWrite(RST_PIN, LOW);
}
