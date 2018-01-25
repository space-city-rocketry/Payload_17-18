#include "multiplexer.h"
#include <Arduino.h>

Multiplexer::Multiplexer(int s0, int s1, int s2, int s3)
:s0(s0), s1(s1), s2(s2), s3(s3){
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
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
  digitalWrite(s0, p0);
  digitalWrite(s1, p1);
  digitalWrite(s2, p2);
  digitalWrite(s3, p3);
}

