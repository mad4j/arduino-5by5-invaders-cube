#pragma once

static const uint8_t TILE_X = 8;
static const uint8_t TILE_Y = 8;

static const unsigned char SLASH_TILE[TILE_X] PROGMEM = {
  0b00000011,
  0b00000111,
  0b00001110,
  0b00011100,
  0b00111000,
  0b01110000,
  0b11100000,
  0b11000000
}; 

static const unsigned char BACKSLASH_TILE[TILE_X] PROGMEM = {
  0b11000000,
  0b11100000,
  0b01110000,
  0b00111000,
  0b00011100,
  0b00001110,
  0b00000111,
  0b00000011
}; 
/*
static const unsigned char BACKSLASH_TILE[TILE_X] PROGMEM = {
  0b00000000,
  0b00000000,
  0b00000011,
  0b00000011,
  0b00001100,
  0b00001100,
  0b00110000,
  0b00110000
}; 
*/
static const uint8_t MAZE_TILESET_SIZE = 2;
static const uint8_t* MAZE_TILESET[] = {
  SLASH_TILE,
  BACKSLASH_TILE
};
