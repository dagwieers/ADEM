/*
 * This file is part of the ADEM project.
 *
 * ADEM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,·
 * (at your option) any later version.
 *
 * ADEM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ADEM.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Koen Verstringhe
 *
 */

#ifndef _I2CGps_h
#define _I2CGps_h
#include <Arduino.h>
#include <Sensor.h>
#include <ArduinoJson.h>
#include <I2Cdev.h>

#include "i2c_gps_registers.h"                 //Register definitions

#define GPS_ADDRESS 0x20

struct I2CGPSData : sensorData{
  uint8_t day = 0;
  uint8_t month = 0;
  uint8_t year = 0;
  uint32_t time = 0;
  GPS_COORDINATES location = GPS_COORDINATES{0,0};
  uint8_t satellites = 0;
  uint16_t altitude = 0;
  uint16_t speed = 0;
};

//abstract class Sensor
class I2CGps : public Sensor {
public:
  I2CGps(int address);
  I2CGps();
  ~I2CGps();

  //virtual function must be implemented
  virtual void begin();
  virtual void end();
  virtual void read();
  virtual void write();
  virtual void process();
  virtual String report();
  virtual size_t dataBufferSize();  
  virtual uint8_t * dataToBuffer();
  String bufferedReport(uint8_t* buffer);
  virtual String buildReport(sensorData *sData);

  boolean ready = false;
  
private:
  StaticJsonBuffer<200> jsonBuffer;

  I2CGPSData measuredData;
  String FormatDateTime(I2CGPSData *data);
  int i2cGpsAddress = GPS_ADDRESS;
  int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
};

#endif
