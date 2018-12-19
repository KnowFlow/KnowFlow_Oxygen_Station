#pragma once

//Serial print switch
#define DEBUG_AVR
//#define DEBUG_M0

//The maximum length of the sensor filter array
#define ARRAYLENGTH 10

//SD card update data time, 60,000 is 1 minute
#define SDUPDATEDATATIME 60000

//Sensor pin settings
#define CO2PIN  A0
#define O2PIN A1

//The number of sensors
#define SENSORCOUNT 3

//The sensor corresponds to the array number, ph=0, temperature=1..., the maximum number is SENSORCOUNT-1
enum SensorNumber
{
	bme280Sensor = 0, //I2C
	co2Sensor,
	o2Sensor
};
