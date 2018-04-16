#ifndef DATA_TYPES_H
#define DATA_TYPES_H

struct Data{
  virtual String getHeader() = 0;
  virtual String getDataRow() = 0;
};

struct Timestamp : public Data{
  unsigned long time;

  String getHeader(){
    return "Timestamp";
  }

  String getDataRow(){
    /*int hours = (time) / (60 * 60 * 1000);  
    int minutes = (time % (60 * 60 * 1000)) / 60000;
    int seconds = ((time % (60 * 60 * 1000)) % 60000) / 1000;
    int ms = ((time % (60 * 60 * 1000)) % 60000) % 1000;
    return String(hours) + ":" + String(minutes) + ":" + String(seconds) + ":" + String(ms);*/
    return String(time);
  }
  
};

struct VoltageData : public Data{
  float voltage_flex[6];
  float voltage_coin[3];

  String getHeader(){
    String header = "";
    for(int j = 1; j <=3; j++){
      header.concat("Coin Cell Battery ");
      header.concat(j);
      header.concat(" (V)");
      header.concat(",");
    }
    for(int i = 1; i <= 6; i++){
      header.concat("Flexible Battery ");
      header.concat(i);
      header.concat(" (V)");
      if(i < 6) header.concat(",");
    }
    return header;
  }
  
  String getDataRow(){
    String data_row = "";
    for(int j = 0; j < 3; j++){
      data_row.concat(voltage_coin[j]);
      data_row.concat(",");
    }
    for(int i = 0; i < 6; i++){
      data_row.concat(voltage_flex[i]);
      if(i < 2) data_row.concat(",");
    }
    return data_row;
  } 
};

struct TemperatureData : public Data{
  float temperature[9];

  String getHeader(){
    String header = "";
    for(int i = 1; i <= 9; i++){
      header.concat("Temperature ");
      header.concat(i);
      if (i < 9) header.concat(",");
    }
    return header;
  }
  
  String getDataRow(){
    String data_row = "";
    for(int i = 0; i < 9; i++){
      data_row.concat(temperature[i]);
      if (i < 8) data_row.concat(",");
    }
    return data_row;
  } 
};

struct AccelerationData : public Data{
  float axis[3]; //X = 0, Y = 1, Z = 2

  String getHeader(){
    return "Acceleration X (m/s^2), Acceleration Y (m/s^2), Acceleration Z (m/s^2)";
  }
  
  String getDataRow(){
    String data_row = "";
    for(int i = 0; i < 3; i++){
      data_row.concat(axis[i]);
      if (i < 2) data_row.concat(",");
    }
    return data_row;
  }

};

struct BMP180Data : public Data{
  float pressure;
  float altitude;

  String getHeader(){
    return "Pressure (millibar), Altitude (m)";
  }

  String getDataRow(){
    return String(pressure) + "," + String(altitude);
  }
};

struct PayloadData : public Data{
  Timestamp timestamp;
  VoltageData voltages;
  TemperatureData temperatures;
  AccelerationData accelerations;
  BMP180Data bmp;
  
  String getHeader(){
    return timestamp.getHeader() + "," + voltages.getHeader() + "," + temperatures.getHeader() + "," + accelerations.getHeader() + "," + bmp.getHeader(); 
  }
  String getDataRow(){
    return timestamp.getDataRow() + "," + voltages.getDataRow() + "," + temperatures.getDataRow() + "," + accelerations.getDataRow() + "," + bmp.getDataRow();
  }
};

#endif //DATA_TYPES_H
