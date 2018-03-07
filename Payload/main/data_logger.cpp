#include "data_logger.h"
#include <SD.h>

DataLogger::DataLogger(Data *data, uint8_t sd_port, String file_name)
:sd_init(false){
  this->data = data;
  this->file_name = file_name;
  if(!SD.begin()){
    Serial.println("SD CARD NOT FOUND! DATA WILL BE LOGGED TO THE SERIAL MONITOR!");
    return;  
  }

  sd_init = true;

  String temp_file = file_name;
  int counter = 0;
  while(SD.exists(temp_file)){
    temp_file = file_name.substring(0, file_name.indexOf(".csv")) + String(++counter) + ".csv";
  }
  file_name = temp_file;

  Serial.print("Logging data to file: ");
  Serial.print(file_name);
  Serial.println();

  this->file_name = temp_file;
  
  file = SD.open(this->file_name, FILE_WRITE);
  file.println(data->getHeader());
  file.close();
}

DataLogger::~DataLogger(){
}

void DataLogger::writeToSD(){
  if(sd_init){
    file = SD.open(this->file_name, FILE_WRITE);
    file.println(data->getDataRow());
    file.close();
  }else
    Serial.println(data->getDataRow());
}
