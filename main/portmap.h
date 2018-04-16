#ifndef PORTMAP_H
#define PORTMAP_H

#include <Arduino.h>

//Digital Ports
const static uint8_t kMUX[] = {3, 2, 1, 0};
const static uint8_t kTrigger[] = {4, 5, 6, 7, 8, 9};

//Analog ports
const static uint8_t kCoinTrigger = A0;
const static uint8_t kCoin[] = {A1, A2, A3};
const static uint8_t kOptionalSwitch = A4;
const static uint8_t kLEDFeedback = A5;
const static uint8_t kMuxSig = A6;

//Multiplexer ports
const static uint8_t kFlexMUX[] = {9, 10, 11, 12, 13, 14};
const static uint8_t kTemp[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

#endif //PORTMAP_H
