#include "GravityO2.h"
#include "Config.h"

extern uint16_t readMedianValue(int* dataArray, uint16_t arrayLength);

GravityO2::GravityO2():pin(O2PIN)
{
}


GravityO2::~GravityO2()
{
}

void GravityO2::setup()
{
	pinMode(pin, INPUT);
}


void GravityO2::update()
{
	int o2ValueBuffer[ARRAYLENGTH];
	float averageVoltage;
	for (uint8_t i = 0; i < ARRAYLENGTH; i++)
	{
		o2ValueBuffer[i] = analogRead(this->pin);
		delay(10);
	}
	averageVoltage = readMedianValue(o2ValueBuffer, ARRAYLENGTH)/1024.0*5000.0;
	this->o2Value = averageVoltage;
}

double GravityO2::getValue()
{
	return this->o2Value;
}

char* GravityO2::getName()
{
  char* sensorName = "O2";
  return sensorName;  
}
