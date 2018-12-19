/*********************************************************************
* SdService.cpp
*
* Copyright (C)    2017   [DFRobot](http://www.dfrobot.com),
* GitHub Link :https://github.com/DFRobot/watermonitor
* This Library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Description:SD card datalogger,Data write format:
* "Year,Month,Day,Hour,Minues,Second,pH,temp(C),DO(mg/l0,ec(s/m),orp(mv)"
*
* Product Links:http://www.dfrobot.com.cn/goods-1142.html
*
* SD card attached to SPI bus as follows:
* UNO:  MOSI - pin 11, MISO - pin 12, CLK - pin 13, CS - pin 4 (CS pin can be changed)
* and pin #10 (SS) must be an output
* Mega:  MOSI - pin 51, MISO - pin 50, CLK - pin 52, CS - pin 53
* and pin #53 (SS) must be an output
* M0:   Onboard SPI pin,CS - pin 4 (CS pin can be changed)
*
* author  :  Jason(jason.ling@dfrobot.com)
* version :  V1.0
* date    :  2017-04-19
**********************************************************************/

// SD card select pin
//#if defined(__SAMD21G18A__)
#if defined(__AVR_ATmega2560__)

const int CsPin = 53;

#else

const int CsPin = 4;

#endif

// #define SDUPDATEDATATIME 30000  //去除了定时写入的时间变量

#include "SdService.h"
#include <SPI.h>
#include "Debug.h"
#include "GravityRtc.h"
#include "config.h"
#include "Debug.h"

extern GravityRtc rtc;

//Initialize variable to hold data
String dataString = "";
char storeArrayChar[20];

SdService ::SdService(ISensor *gravitySensor[]) : chipSelect(CsPin), sdDataUpdateTime(0)
{
	this->gravitySensor = gravitySensor;
}

SdService ::~SdService()
{
}

//********************************************************************************************
// function name: setup ()
// Function Description: Initialize the SD card
//********************************************************************************************
void SdService::setup()
{
	Debug::println(F("Initializing SD card..."));

	pinMode(SS, OUTPUT);

	if (!SD.begin(chipSelect))
	{
		Debug::println(F("SD card failed, or not present"));
		// don't do anything more:
		return;
	}
	sdReady = true;
	Debug::println(F("SD card initialized."));

	// write the file header
	dataFile = SD.open("sensor.csv", FILE_WRITE);
	if (dataFile && dataFile.position() == 0)
	{
		Debug::println(F("Writing file header to SD"));
		//dataFile.println(F("Year,Month,Day,Hour,Minues,Second,pH,temp(C),DO(mg/l),ec(s/m),orp(mv)"));
		//dataFile.println(F("Year,Month,Day,Hour,Minues,Second,O2inAir(%),O2inOffair(%),MoleRatio(%),Co2(PPM),Temp(C),Humi(%),Press(KPa),Air/Offgas(1/0)"));
		// dataFile.println(F("date,pH,temp(C),DO(mg/l),ec(s/m),orp(mv)"));
		//dataFile.println(F("date,O2inAir(%),O2inOffair(%),MoleRatio(%),Co2(PPM),Temp(C),Humi(%),Press(KPa),Air/Offgas(1/0)"));
		dataFile.println(F("date,time,temperature(^C),humidity(%),pressure(kPa),CO2(ppm),O2(ppm)"));
		dataFile.close();
	}
}

//********************************************************************************************
// function name: update ()
// Function Description: Update the data in the SD card
//********************************************************************************************
void SdService::update()
{
	 if (sdReady && millis() - sdDataUpdateTime > SDUPDATEDATATIME)
	{
	Debug::println(F("Prepare writing time to SD card"));
	//Update time from clock module
   rtc.update();
	dataString = "";
	// Year Month Day Hours Minute Seconds
	dataString += String(rtc.year, 10);
	dataString += "/";
	dataString += String(rtc.month, 10);
	dataString += "/";
	dataString += String(rtc.day, 10);
	dataString += ",";
	dataString += String(rtc.hour, 10);
	dataString += ":";
	dataString += String(rtc.minute, 10);
	dataString += ":";
	dataString += String(rtc.second, 10);
	dataString += ",";

	// write time to SD card, write in two operations to keep write size small
	dataFile = SD.open("sensor.csv", FILE_WRITE);
	if (dataFile)
	{
		Debug::print(F("Writing Time data to card:"));
    Debug::println(dataString);
    
		dataFile.print(dataString);
		dataFile.close();
		
	}
  
	dataString = "";
	//此处为写入传感器数据
  Debug::println(F("Prepare writing sensor data to SD card"));

  //BME280输出三种参数，做特殊处理
	if (this->gravitySensor[0] != NULL)
	{
		this->gravitySensor[0]->update();
		this->gravitySensor[0]->getValue(dataValues);
    Debug::print(this->gravitySensor[0]->getName());
		Debug::print(F(" data[0]:"));
		Debug::print(dataValues[0]);
    Debug::print(F(", data[1]:"));
		Debug::print(dataValues[1]);
    Debug::print(F(", data[2]:"));
		Debug::println(dataValues[2]);
    
		/*connectString(dataValues[0]);
		connectString(dataValues[1]);
		connectString(dataValues[2]);*/
    //two decimal digits
    dtostrf(dataValues[0],2,2,storeArrayChar);
    connectString(storeArrayChar);
    dtostrf(dataValues[1],2,2,storeArrayChar);
    connectString(storeArrayChar);
    dtostrf(dataValues[2],2,2,storeArrayChar);
    connectString(storeArrayChar);
	}
	else
	{
		connectString(0.0);
		connectString(0.0);
		connectString(0.0);
    Debug::print(this->gravitySensor[0]->getName());
    Debug::println(F(" sensor Error"));
	}

  /*
	//CO2
	if (this->gravitySensor[1] != NULL)
	{
		//connectString(this->gravitySensor[1]->getValue());
    this->gravitySensor[1]->update();
    dtostrf(this->gravitySensor[1]->getValue(),2,2,storeArrayChar);
    connectString(storeArrayChar);
		Debug::print(F("CO2 data:"));
		Debug::println(this->gravitySensor[1]->getValue());
	}
	else
  {
		connectString(0.0);
    Debug::println(F("CO2 sensor Error"));
  }*/

  //一个传感器输出一种参数，做通用处理
    for (int8_t i = 1; i < SENSORCOUNT; i++)
    {
      if (this->gravitySensor[i] != NULL)
      {
        this->gravitySensor[i]->update();
        dtostrf(this->gravitySensor[i]->getValue(),2,2,storeArrayChar);
        connectString(storeArrayChar);
        Debug::print(this->gravitySensor[i]->getName());
        Debug::print(F(" data:"));
        Debug::println(this->gravitySensor[i]->getValue());
      }    
      else
      {
        connectString(0.0);
        Debug::print(this->gravitySensor[i]->getName());
        Debug::println(F(" sensor Error"));
      }
    }

	// write SD card
	dataFile = SD.open("sensor.csv", FILE_WRITE);
	if (dataFile)
	{
		 Debug::print(F("Writing sensor data to SD card:"));
     Debug::println(dataString);
		dataFile.println(dataString);
		dataFile.close();
	}
	
	 sdDataUpdateTime = millis();
	}
}

//********************************************************************************************
// function name: connectString ()
// Function Description: Connects the string data
//********************************************************************************************
void SdService::connectString(double value)
{
  dataString += String(value, 10);
  dataString += ",";
}


void SdService::connectString(char valueChar[])
{
  dataString += String(valueChar);
  dataString += ",";
}
