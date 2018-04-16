#include "data_logger.h"
#include "status_led.h"
#include <SD.h>

DataLogger::DataLogger(Data *data, uint8_t sd_port, String file_name)
:_data(data), _file_name(file_name), _sd_init(false){
  if(!SD.begin()){
    Serial.println("SD CARD NOT FOUND! DATA WILL BE LOGGED TO THE SERIAL MONITOR!");
    //StatusLed::push_error(PreLaunchError::SD_FAILURE);
    return;  
  }

  _sd_init = true;

  String temp_file = _file_name;
  int counter = 0;
  while(SD.exists(temp_file)){
    temp_file = _file_name.substring(0, _file_name.indexOf(".csv")) + String(++counter) + ".csv";
  }
  _file_name = temp_file;

  Serial.print("Logging data to file: ");
  Serial.print(_file_name);
  Serial.println();

  this->_file_name = temp_file;
  
  _file = SD.open(this->_file_name, FILE_WRITE);
  _file.println(_data->getHeader());
  _file.close();
}

DataLogger::~DataLogger(){
}

void DataLogger::writeToSD(){
  if(_sd_init){
    _file = SD.open(this->_file_name, FILE_WRITE);
    _file.println(_data->getDataRow());
    _file.close();
  }else
    Serial.println(_data->getDataRow());
}
