#ifndef STATUS_LED_H
#define STATUS_LED_H

enum PreLaunchError{
  MPU6050_FAILURE,
  SD_FAILURE
};

class StatusLed{

  public:
    StatusLed();
    void push_error(PreLaunchError errono);
    void update(int cycles);
    
  private:
    bool mpu6050_failure_handler();
    bool sd_failure_handler();
    bool _led_state;

  
};

#endif // STATUS_LED_H
