#include <Arduino.h>

#ifndef BATTERY_WATCH_H
#define BATTERY_WATCH_H

struct VoltageData;

class BatteryWatch{

  public:
    BatteryWatch(VoltageData *voltages, const uint8_t flex_ports[6], const uint8_t coin_port);

    void start();
    void update();
    void stop();

   static constexpr float kFlexCutoff = 3.0f;
   static constexpr float kCoinCutoff = 3.0f;

  private:
    VoltageData *_voltages;
    uint8_t _flex_ports[6];
    uint8_t _coin_port;
  
};

#endif //BATTERY_WATCH_H
