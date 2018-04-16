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
  Data* _data;
  File _file;
  String _file_name;
  bool _sd_init;
};

#endif //DATA_LOGGER_H
