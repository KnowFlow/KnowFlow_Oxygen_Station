#pragma once
#include "ISensor.h"
#include "Arduino.h"

class GravityCo2 : public ISensor
{
public:
	GravityCo2();
	~GravityCo2();

public:
	void setup();
	void update();
	double getValue();
  void getValue(double dataValue[]){};

	int pin;
	double co2Value;
};
