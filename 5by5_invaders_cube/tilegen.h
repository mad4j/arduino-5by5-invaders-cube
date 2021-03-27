#pragma once

class TileGen : public Generator {

public:


  TileGen(uint8_t** tiles, uint8_t max_tiles) :
    _tiles(tiles),
    _max_tiles(max_tiles), 
    _seed(0), 
    _rows(0), 
    _cols(0), 
    _tile_map(0) 
  { 
    // no implementation
  }

    
  ~TileGen() 
  {
    if (_tile_map != 0) {
      delete[] _tile_map;
    }
  }


  void init(uint64_t seed, uint16_t width, uint16_t height) 
  {
    _seed = seed;
    
    _rows = height / TILE_Y;
    _cols = width / TILE_X;
    
    if (_tile_map != 0) {
      delete[] _tile_map;
    }
    _tile_map = new uint8_t[_cols * _rows];
  }


  uint8_t generate(uint16_t x, uint16_t y)
  {
    uint8_t cell_x = x / TILE_X;
    uint8_t cell_y = y / TILE_Y;

    uint8_t cell_index = cell_x+cell_y*_cols;

    if ((x%TILE_X == 0) && (y%TILE_Y == 0)) {
      _tile_map[cell_index] = random(_max_tiles);
    }

    uint8_t* tile = _tiles[_tile_map[cell_index]];
    uint8_t bit = (pgm_read_byte(&tile[y%8]) >> (x%8)) & 0x1; 
    return (bit == 1) ? 0 : 1;
  }


private:
  uint8_t** _tiles;
  uint8_t _max_tiles;
  uint64_t _seed;
  uint8_t _rows;
  uint8_t _cols;
  uint8_t*_tile_map;

};
