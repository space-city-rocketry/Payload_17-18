#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include <stdint.h>

class Multiplexer{

public:
  Multiplexer(int s0, int s1, int s2, int s3);
  ~Multiplexer();
  int multiplexerRead(int analogPin, uint8_t muxPin);  

private:
  void selectAddress(uint8_t address);
  int _s0,_s1,_s2,_s3;

};

#endif //MULTIPLEXER_H
