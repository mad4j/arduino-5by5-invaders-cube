#pragma once

class Generator {

  public:
    Generator() { };
    virtual ~Generator() { };

    virtual void init(uint64_t seed, uint16_t width, uint16_t height) = 0;
    virtual uint8_t generate(uint16_t x, uint16_t y) = 0;
  
};
