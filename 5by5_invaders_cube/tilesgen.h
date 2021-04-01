#pragma once
class TilesGen : public Generator {


public:

  TilesGen(uint16_t** tiles, uint8_t max_tiles, uint8_t zoom) :
    _tiles(tiles),
    _max_tiles(max_tiles),
    _zoom(zoom),
    _seed(0), 
    _rows(0), 
    _cols(0), 
    _tile_map(0) 
  { 
    // no implementation
  }

    
  ~TilesGen() 
  {
    if (_tile_map != 0) {
      delete[] _tile_map;
    }
  }


  void init(uint64_t seed, uint16_t width, uint16_t height) 
  {
    _seed = seed;
    
    _rows = height / (_zoom*TILE_Y);
    _cols = width / (_zoom*TILE_X);
    
    if (_tile_map != 0) {
      delete[] _tile_map;
    }
    _tile_map = new uint8_t[_cols * _rows];
  }


  uint8_t generate(uint16_t x, uint16_t y)
  {
    uint8_t cell_x = x / (_zoom*TILE_X);
    uint8_t cell_y = y / (_zoom*TILE_Y);

    uint8_t cell_index = cell_x+cell_y*_cols;

    if ((x%(_zoom*TILE_X) == 0) && (y%(_zoom*TILE_Y) == 0)) {
      _tile_map[cell_index] = random(_max_tiles);
    }

    uint16_t* tile = _tiles[_tile_map[cell_index]];
    uint8_t bit = (pgm_read_word(&tile[(y/_zoom)%TILE_Y]) >> ((x/_zoom)%TILE_X)) & 0x1;
     
    return (bit == 1) ? 0 : 1;
  }


private:
  uint16_t** _tiles;
  uint8_t _max_tiles;
  uint8_t _zoom;
  uint64_t _seed;
  uint8_t _rows;
  uint8_t _cols;
  uint8_t*_tile_map;

};
