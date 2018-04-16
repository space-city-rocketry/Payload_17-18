#include "multiplexer.h"
#include <Arduino.h>

Multiplexer::Multiplexer(int s0, int s1, int s2, int s3)
:_s0(s0), _s1(s1), _s2(s2), _s3(s3){
  pinMode(_s0, OUTPUT);
  pinMode(_s1, OUTPUT);
  pinMode(_s2, OUTPUT);
  pinMode(_s3, OUTPUT);
}

Multiplexer::~Multiplexer(){
  
}
  
int Multiplexer::multiplexerRead(int analogPin, uint8_t muxPin){
  
  selectAddress(muxPin);

  return analogRead(analogPin);
}

void Multiplexer::selectAddress(uint8_t address){
  int p0, p1, p2, p3;

  p0 = address >> 3;
  p1 = (address >> 2) & 0x00000001;
  p2 = (address >> 1) & 0x00000001;
  p3 = address & 0x000000001;
  digitalWrite(_s0, p0);
  digitalWrite(_s1, p1);
  digitalWrite(_s2, p2);
  digitalWrite(_s3, p3);
}

