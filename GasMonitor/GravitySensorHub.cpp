/*********************************************************************************************
* GravitySensorHub.cpp
*
* Copyright (C)    2017   [DFRobot](http://www.dfrobot.com),
* GitHub Link :https://github.com/DFRobot/watermonitor
* This Library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Description:
*
* author  :  Jason(jason.ling@dfrobot.com)
* version :  V1.0
* date    :  2017-04-12
*********************************************************************************************/


#include "GravitySensorHub.h"
#include "GravityBme280.h"
#include "GravityCo2.h"
#include "GravityO2.h"
#include "Config.h"

//********************************************************************************************
// function name: sensors []
// Function Description: Store the array of sensors
// Parameters: 0  BME280
// Parameters: 1  CO2
// Parameters: 2 
// Parameters: 3 
// Parameters: 4 
//********************************************************************************************

GravitySensorHub::GravitySensorHub()
{
	for (size_t i = 0; i < this->SensorCount; i++)
	{
		this->sensors[i] = NULL;
	}
	this->sensors[bme280Sensor] = new GravityBme280();
  this->sensors[co2Sensor] = new GravityCo2();
  this->sensors[o2Sensor] = new GravityO2();
}

//********************************************************************************************
// function name: ~ GravitySensorHub ()
// Function Description: Destructor, frees all sensors
//********************************************************************************************
GravitySensorHub::~GravitySensorHub()
{
	for (size_t i = 0; i < SensorCount; i++)
	{
		if (this->sensors[i])
		{
			delete this->sensors[i];
		}
	}
}


//********************************************************************************************
// function name: setup ()
// Function Description: Initializes all sensors
//********************************************************************************************
void GravitySensorHub::setup()
{
	for (size_t i = 0; i < SensorCount; i++)
	{
		if (this->sensors[i])
		{
			this->sensors[i]->setup();
		}
	}
}


//********************************************************************************************
// function name: update ()
// Function Description: Updates all sensor values
//********************************************************************************************
void GravitySensorHub::update()
{
	for (size_t i = 0; i < SensorCount; i++)
	{
		if (this->sensors[i])
		{
			this->sensors[i]->update();
		}
	}
}

//********************************************************************************************
// function name: getValueBySensorNumber ()
// Function Description: Get the sensor data
// Parameters: 0 bme280
// Parameters: 1 
// Parameters: 2 
// Parameters: 3 
// Parameters: 4 
// Return Value: Returns the acquired sensor data
//********************************************************************************************
double GravitySensorHub::getValueBySensorNumber(int num)
{
	if (num >= SensorCount)
	{
		return 0;
	}
	if (num == 0) //for BME280 sensor
  {
	  this->sensors[num]->getValue(value);
    return 0;
  }
  else
   return this->sensors[num]->getValue();
}
