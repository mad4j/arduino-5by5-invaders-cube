#pragma once

class LinesGen : public Generator {

public:

  static const uint8_t MAX_LINES = 8;

  LinesGen() :
    _seed(0)
  { 
  }

    
  ~LinesGen() 
  {
  }


  void init(uint64_t seed, uint16_t width, uint16_t height) 
  {
    _seed = seed;

    for (uint8_t i=0; i<MAX_LINES; i++) {
      _hlines[i] = random(256);
      _vlines[i] = random(256);
    }
  }


  uint8_t generate(uint16_t x, uint16_t y)
  {
     
    return 1;
  }


private:
  uint64_t _seed;
  uint8_t _hlines[MAX_LINES];
  uint8_t _vlines[MAX_LINES];
};
