#include "GravityCo2.h"
#include "Config.h"

extern uint16_t readMedianValue(int* dataArray, uint16_t arrayLength);

GravityCo2::GravityCo2():pin(CO2PIN)
{
}


GravityCo2::~GravityCo2()
{
}

void GravityCo2::setup()
{
	pinMode(pin, INPUT);
}


void GravityCo2::update()
{
	int co2ValueBuffer[ARRAYLENGTH];
	float averageVoltage;
	for (uint8_t i = 0; i < ARRAYLENGTH; i++)
	{
		co2ValueBuffer[i] = analogRead(this->pin);
		delay(10);
	}
	averageVoltage = readMedianValue(co2ValueBuffer, ARRAYLENGTH)/1024.0*5000.0;
	this->co2Value = (averageVoltage - 400)*50.0/16.0;
}

double GravityCo2::getValue()
{
	return this->co2Value;
}

char* GravityCo2::getName()
{
    char* sensorName = "CO2";
    return sensorName;  
}
