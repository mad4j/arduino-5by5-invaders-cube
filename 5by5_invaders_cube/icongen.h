#pragma once

class IconGen : public Generator {

public:

  IconGen() :
    _seed(0),
    _dx(0),
    _dy(0),
    _w(0),
    _h(0)
  { 
    // no implementation
  }

    
  ~IconGen() 
  {
    // no implementation
  }


  void init(uint64_t seed, uint16_t width, uint16_t height) 
  {
    _seed = seed;
    if (_seed & 0x01FFFFFF == 0) {
      _seed = 0xAAAAAAAA;
    }
    _dx = width / 10;
    _dy = height / 10;
    _w = width / _dx - 1;
    _h = height / _dy - 1;
  }

 
  uint8_t generate(uint16_t x, uint16_t y)
  {
    uint16_t tx = x / _dx;
    uint16_t ty = y / _dy;
    
    if (tx > (_w / 2)) tx = _w-tx;
    if (ty > (_h / 2)) ty = _h-ty;
    
    return (_seed >> ((ty*5)+tx)) & 0x1;
  }


private:
  uint64_t _seed;
  int16_t _dx;
  int16_t _dy;
  int16_t _w;
  int16_t _h;
};
