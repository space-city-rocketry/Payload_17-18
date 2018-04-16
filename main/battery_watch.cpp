#include "battery_watch.h"
#include "data_types.h"

BatteryWatch::BatteryWatch(VoltageData *voltages, const uint8_t flex_ports[6], const uint8_t coin_port)
: _voltages(voltages), _coin_port(coin_port){

  for(int i = 0; i < 6; i++){
    _flex_ports[i] = flex_ports[i];
    pinMode(_flex_ports[i], OUTPUT);
  }

  pinMode(_coin_port, OUTPUT);
  
}

void BatteryWatch::start(){
  for(int i = 0; i < 6; i++)
    digitalWrite(_flex_ports[i], HIGH);

  pinMode(_coin_port, HIGH);
}

void BatteryWatch::update(){

  for(int i = 0; i < 6; i++){
    if(_voltages->voltage_flex[i]<=kFlexCutoff)
      digitalWrite(_flex_ports[i], LOW);
  }

  if(_voltages->voltage_coin[0]<=kCoinCutoff || _voltages->voltage_coin[1]<=kCoinCutoff || _voltages->voltage_coin[2]<=kCoinCutoff)
    pinMode(_coin_port, LOW);
  
}

void BatteryWatch::stop(){
  for(int i = 0; i < 6; i++)
    digitalWrite(_flex_ports[i], LOW);

  pinMode(_coin_port, LOW);
}

