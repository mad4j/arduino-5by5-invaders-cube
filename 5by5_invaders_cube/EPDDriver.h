#pragma once

/**
 * Waveshare 1.54 E-Ink diplay V2
 * ---------------------------------
 * 
 * Connection Table
 * ---------------------------------
 * E-Paper        Arduino UNO / Nano
 *  3.3V              3V3
 *  GND               GND
 *  DIN               D11
 *  CLK               D13
 *  CS                D10
 *  DC                D9
 *  RST               D8
 *  BUSY              D7 
 */

//TBV display rotation a driver level

#include <SPI.h>


// Pin definition
static const uint8_t DC_PIN   = 9;
static const uint8_t CS_PIN   = 10;
static const uint8_t BUSY_PIN = 7;
static const uint8_t RST_PIN  = 8;

// Display resolution
static const uint16_t EPD_WIDTH = 200;
static const uint16_t EPD_HEIGHT = 200;

// Commands
static const uint8_t CMD_DRIVER_OUTPUT_CONTROL                = 0x01;
static const uint8_t CMD_DATA_ENTRY_MODE_SETTINGS             = 0x11;
static const uint8_t CMD_SW_RESET                             = 0x12;
static const uint8_t CMD_MASTER_ACTIVATION                    = 0x20;
static const uint8_t CMD_DISPLAY_UPDATE_CONTROL_1             = 0x21;
static const uint8_t CMD_DISPLAY_UPDATE_CONTROL_2             = 0x22;
static const uint8_t CMD_BORDER_WAVEFORM_CONTROL              = 0x3C; 
static const uint8_t CMD_SET_RAM_X_ADDRESS_START_END_POSITION = 0x44;
static const uint8_t CMD_SET_RAM_Y_ADDRESS_START_END_POSITION = 0x45;
static const uint8_t CMD_SET_RAM_X_ADDRESS_COUNTER            = 0x4E;
static const uint8_t CMD_SET_RAM_Y_ADDRESS_COUNTER            = 0x4F; 


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

  sendCommand(CMD_SW_RESET);
  waitUntilIdle();

  sendCommand(CMD_DRIVER_OUTPUT_CONTROL);
  sendData(0xC7);
  sendData(0x00);
  sendData(0x01);

  sendCommand(CMD_DATA_ENTRY_MODE_SETTINGS);
  sendData(0x01);

  sendCommand(CMD_SET_RAM_X_ADDRESS_START_END_POSITION); //set Ram-X address start/end position
  sendData(0x00);
  sendData(0x18);    //0x0C-->(18+1)*8=200

  sendCommand(CMD_SET_RAM_Y_ADDRESS_START_END_POSITION); //set Ram-Y address start/end position
  sendData(0xC7);   //0xC7-->(199+1)=200
  sendData(0x00);
  sendData(0x00);
  sendData(0x00);

  sendCommand(CMD_BORDER_WAVEFORM_CONTROL); //BorderWavefrom
  sendData(0x01);

  sendCommand(0x18); //Read built-in temperature sensor
  sendData(0x80);

  sendCommand(CMD_DISPLAY_UPDATE_CONTROL_2); 
  sendData(0XB1);  //Load Temperature and waveform setting.
  
  sendCommand(CMD_MASTER_ACTIVATION);

  sendCommand(CMD_SET_RAM_X_ADDRESS_COUNTER);   // set RAM x address count to 0;
  sendData(0x00);
  
  sendCommand(CMD_SET_RAM_Y_ADDRESS_COUNTER);   // set RAM y address count to 0X199;
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
  sendCommand(CMD_DISPLAY_UPDATE_CONTROL_2);
  sendData(0xC7);
  sendCommand(CMD_MASTER_ACTIVATION);
  waitUntilIdle();
}


void EPDDriver::display(const uint8_t* frame_buffer)
{
  int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
  int h = EPD_HEIGHT;

  if (frame_buffer != NULL) {
    sendCommand(0x24);
    for (auto j = 0; j < h; j++) {
      for (auto i = 0; i < w; i++) {
        sendData(pgm_read_byte(&frame_buffer[i + j * w]));
      }
    }
  }

  //DISPLAY REFRESH
  sendCommand(CMD_DISPLAY_UPDATE_CONTROL_2);
  sendData(0xF7);
  sendCommand(CMD_MASTER_ACTIVATION);
  waitUntilIdle();
}


void EPDDriver::displayGenerator(Generator& gen, uint64_t seed)
{
  gen.init(seed, EPD_WIDTH, EPD_HEIGHT);

  sendCommand(0x24);
  for (auto j = 0; j < EPD_HEIGHT; j++) {
    for (auto i = 0; i < EPD_WIDTH; i+=8) {

      uint8_t bits = 0;
      for (auto k=0; k<8; k++) {
#ifdef DISPLAY_ROTATION
        auto b = gen.generate(j, i+k);
#else
        auto b = gen.generate(i+k, j);
#endif
        bits <<= 1;
        bits |= (b&0x01);
      }
  
      sendData(bits);
    }
  }
  
  //DISPLAY REFRESH
  sendCommand(CMD_DISPLAY_UPDATE_CONTROL_2);
  sendData(0xC7);
  sendCommand(CMD_MASTER_ACTIVATION);
  waitUntilIdle();
}


void EPDDriver::sleep()
{
  sendCommand(0x10); //enter deep sleep
  sendData(0x01);
  delay(200);

  digitalWrite(RST_PIN, LOW);
}
