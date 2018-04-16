#include "status_led.h"
#include "portmap.h"

StatusLed::StatusLed(){
  pinMode(kLEDFeedback, OUTPUT);
}

void StatusLed::push_error(PreLaunchError errono){
  //errors.push_back(errono);
}

void StatusLed::update(int cycles){

  /*
  if(errors.size() > 0){
    switch(placeholder){
      case PreLaunchError::MPU6050_FAILURE:
      break;

      case PreLaunchError::SD_FAILURE:
      break;
    }
  }else{*/
    if(cycles % 5 == 0){
      digitalWrite(kLEDFeedback, _led_state?HIGH:LOW);
      _led_state = !_led_state;
    }
  //}

  
}

bool StatusLed::mpu6050_failure_handler(){
  return true;
}

bool StatusLed::sd_failure_handler(){
  return true;
}

