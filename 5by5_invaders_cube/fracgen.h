#pragma once
class FracGen : public Generator {


public:

  FracGen() :
    _seed(0),
    _dx(0),
    _dy(0)
  { 
    // no implementation
  }

    
  ~FracGen() 
  {
    // no implementation
  }


  void init(uint64_t seed, uint16_t width, uint16_t height) 
  {
    _seed = seed;

    _dx = 3;
    while (width > _dx) {
      _dx *= 3;
    }

    _dy = 3;
    while (height > _dy) {
      _dy *= 3;
    }

    Serial.println(_dx);
    Serial.println(_dy);
    
    _dx = (_dx - width) / 2;
    _dy = (_dy - height) / 2;
  }


  uint8_t generate(uint16_t x, uint16_t y)
  {

    x += _dx;
    y += _dy;

    // if you write coordinates of any point on 
    // the carpet in base 3, the pixel is blank 
    // if and only if any matching pair of digits 
    // are (1, 1)
    while ((x>0) && (y>0)) {
      if ((x%3==1) && (y%3==1)) {
        return 1;
      }

      x /= 3;
      y /= 3;
    }

    return 0;
  }

private:
  uint64_t _seed;
  int16_t _dx;
  int16_t _dy;
};
