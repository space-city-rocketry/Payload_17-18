#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <stdint.h>
#include <SD.h>

#include "data_types.h"

class DataLogger{

public:
  DataLogger(Data* data, uint8_t sd_port, String file_name);
  ~DataLogger();
  void writeToSD(); 

private:
  Data* data;
  File file;
  String file_name;
  bool sd_init;
};

#endif //DATA_LOGGER_H
