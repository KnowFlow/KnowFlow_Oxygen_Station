#pragma once

#include "ISensor.h"
#include "Arduino.h"

class GravityO2 : public ISensor
{
public:
	GravityO2();
	~GravityO2();

public:
	void setup();
	void update();
	double getValue();
	void getValue(double dataValue[]){};
  char* getName();
  
	int pin;
	double o2Value;
};
