#pragma once

class Generator {

  public:
    Generator() { };
    virtual ~Generator() { };

    virtual void init(uint64_t seed, uint16_t width, uint16_t height) = 0;
    virtual uint8_t generate(uint16_t x, uint16_t y) = 0;
  
};


class InvadersGen : public Generator {

public:

  InvadersGen() : _seed(0), _dx(1), _dy(1) { }
  ~InvadersGen() { }

  void init(uint64_t seed, uint16_t width, uint16_t height) 
  {
    _seed = seed % INT16_MAX;
    
    _dx = width / 5;
    _dy = height / 5; 
   }

  uint8_t generate(uint16_t x, uint16_t y)
  {
    uint8_t i = x / _dx;
    uint8_t j = y / _dy;

    uint8_t bit = 3*j + ((i < 3) ? i : 4-i);
    
    return ((_seed >> bit) & 0x1 > 0) ? 1 : 0;
  }

private:
  uint64_t _seed;
  uint16_t _dx;
  uint16_t _dy;
};
