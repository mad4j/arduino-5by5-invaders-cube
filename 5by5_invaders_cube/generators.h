#pragma once

class Generator {

  public:
    Generator() { };
    virtual ~Generator() { };

    virtual void init(uint16_t seed) = 0;
    virtual uint8_t generate(uint8_t x, uint8_t y) = 0;
  
};


class InvadersGen : public Generator {

public:

  InvadersGen() : _seed(0) { }

  ~InvadersGen() { }

  void init(uint16_t seed) { _seed = seed; };

  uint8_t generate(uint8_t x, uint8_t y)
  {

    uint8_t i = (x/40) / 5;
    uint8_t j = y/40;

    uint8_t bit = 3*j + ((i < 3) ? i : 4-i);

    return ((_seed >> bit) & 0x1 > 0) ? 1 : 0;
  }

private:

  uint16_t _seed;
  
};
