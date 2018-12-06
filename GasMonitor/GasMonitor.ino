/*********************************************************************
 * WaterMonitor.ino
 *
 * Copyright (C)    2017   [DFRobot](http://www.dfrobot.com)
 * GitHub Link :https://github.com/DFRobot/watermonitor
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Description:
 * This sample code is mainly used to monitor water quality
 * including ph, temperature, dissolved oxygen, ec and orp,etc.
 *
 * Software Environment: Arduino IDE 1.8.2
 * Software download link: https://www.arduino.cc/en/Main/Software
 *
 * Install the library file：
 * Copy the files from the github repository folder libraries to the libraries
 * in the Arduino IDE 1.8.2 installation directory
 *
 * Hardware platform   : Arduino M0 Or Arduino Mega2560
 * Sensor pin:
 * EC  : A1
 * PH  : A2
 * ORP : A3
 * RTC : I2C
 * DO  : Serial port Rx(0),Tx(1)
 * GravityDO：A4
 * temperature:D5
 *
 * SD card attached to SPI bus as follows:
 * Mega:  MOSI - pin 51, MISO - pin 50, CLK - pin 52, CS - pin 53
 * and pin #53 (SS) must be an output
 * M0:   Onboard SPI pin,CS - pin 4 (CS pin can be changed)
 *
 * author  :  Jason(jason.ling@dfrobot.com)
 * version :  V1.0
 * date    :  2017-04-06
 **********************************************************************/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "GravitySensorHub.h"
#include "GravityRtc.h"
//#include "OneWire.h"
#include "SdService.h"
#include "Debug.h"
//#include <SoftwareSerial.h>
//#include <DFRobot_SIM7000.h>
//#include <EEPROM.h>
#include "LiquidCrystal_I2C.h"

// clock module
GravityRtc rtc;
#define RelayAir 32
#define RelayOffgas 36
#define RelayPump 39

// sensor monitor
GravitySensorHub sensorHub;
SdService sdService = SdService(sensorHub.sensors);

//IOT的设置
//SoftwareSerial mySerial(8, 7);
//DFRobot_SIM7000 sim7000;
//OneWire ds(2);

//#define serverIP "iot.dfrobot.com.cn"
//#define IOT_CLIENT "195ff91766ff7bb0"
//#define IOT_USERNAME "H1W0p7j5Rz"
//#define IOT_KEY "B1GATmi5Rz"
//#define IOT_TEMPERATURE "B1Ti7Zg87"
//#define IOT_PH "rJUPQZxIm"
//#define IOT_EC "rJWumWx8Q"

//#define PH_PIN A1
//#define EC_PIN A2
//boolean flag = 0;
//float voltage, phValue, ecValue, temperature = 25;
//DFRobot_PH ph;
//DFRobot_EC ec;

// test函数的设置
unsigned long delayTime = 0; //test模式超时时间
unsigned long testTime = 0;  //校准预热时间
int time = 0;				 //180s
int testflag = 0;
float O2 = 21;
float CO2 = 400;
char num = '9';
//年月日修改设置
uint16_t year = 2018;
uint8_t month = 12;
uint8_t day = 30;
uint8_t week = 7;
uint8_t hour = 12;
uint8_t minute = 0;
uint8_t second = 0;
uint16_t temp1 = 0;
uint8_t temp2 = 0;

//loop函数中的变量
unsigned long updateTime = 0;
unsigned int flag = 0;
unsigned int flag0 = 0;
unsigned long lcdTime = 0;
//unsigned long firstTime = 600000;
//unsigned long secondTime = 1200000;
unsigned long firstTime = 6000;
unsigned long secondTime = 12000;
String SerTest = "";

//lcd屏幕显示的变量
LiquidCrystal_I2C lcd(0x27, 16, 2);
float MoleRatio;

void test()
{
	//加入一个flag，当test后flag为1，回到Loop中不需要判断test直接进入，直到test输入0时flag为0退出
	Serial.println("1、CAL：O2");
	Serial.println("2、CAL：CO2");
	Serial.println("3、TIME");
	Serial.println("4、DATE");
	Serial.println("5、TIMER");
	Serial.println("6、normal");
	Serial.println("0、exit");
	Serial.println("please input a number");
	// if (Serial.available())
	// {
	// 	num = Serial.read();
	// }
	while (num == '9')
	{
		//	num = Serial.read();
		if (Serial.available())
		{
			num = Serial.read();
		}
		switch (num)
		{
		case '0': //exit
			testflag = 0;
			num = '8';
			Serial.println("exit test");
			break;
		case '1': //O2
			//串口输入校准值
			Serial.println("input O2 calibration value");
			while (1)
			{
				if (Serial.available())
				{
					O2 = Serial.read();
					break;
				}
			}
			time = 180;
			Serial.print(time);
			Serial.println("s  O2 start");
			testTime = millis();
			for (time = 180; time > 0;)
			{
				//if (millis() - testTime > 10000)
				if (millis() - testTime > 1000)
				{
					testTime = millis();
					time = time - 10;
					Serial.print(time);
					Serial.println("s");
				}
			}
			testTime = millis();

			while (1)
			{
				if (millis() - testTime > 3000)
				{
					testTime = millis();
					sensorHub.update();
					Serial.print("O2:");
					Serial.print(O2);
					//Serial.print(sensorHub.getValueBySensorNumber(1));
					Serial.println("%");
					Serial.println("if you want to stop, input 'OK'");
				}
				if (Serial.available())
				{
					//wchar_t valOK = Serial.read();
					char valOK = Serial.read();
					if (valOK == 'O')
					{
						Serial.print("O2 CAL:");
						Serial.print(O2);
						Serial.println("ppm");
						Serial.println("CAL O2 OK");
						break;
					}
				}
			}
			//串口输出数据，待稳定后，串口输入OK后，完成校准。
			num = '8';
			break;
		case '2': //CO2
			//串口输入校准值
			Serial.println("input CO2 calibration value");
			while (1)
			{
				if (Serial.available())
				{
					CO2 = Serial.read();
					break;
				}
			}
			time = 180;
			Serial.print(time);
			Serial.println("s  CO2 start"); //显示一次180s
			testTime = millis();
			for (time = 180; time > 0;)
			{
				//if (millis() - testTime > 10000)
				if (millis() - testTime > 1000)
				{
					testTime = millis();
					time = time - 10;
					Serial.print(time);
					Serial.println("s");
				}
			}
			testTime = millis();
			while (1)
			{
				if (millis() - testTime > 3000)
				{
					testTime = millis();
					sensorHub.update();
					Serial.print("CO2:");
					Serial.print(sensorHub.getValueBySensorNumber(1));
					Serial.println("ppm");
					Serial.println("if you want to stop, input 'OK'");
				}
				if (Serial.available())
				{
					//wchar_t valOK = Serial.read();
					char valOK = Serial.read();
					if (valOK == 'O')
					{
						Serial.print("CO2 CAL:");
						Serial.print(CO2);
						Serial.println("ppm");
						Serial.println("CAL CO2 OK");
						break;
					}
				}
			}
			//串口输出数据，待稳定后，串口输入OK后，完成校准。
			num = '8';
			break;
		case '3': //time
			Serial.println("please input hour, like 12 (0~23)");
			while (1)
			{
				if (Serial.available())
				{
					while (Serial.available() > 0)
					{							   // 串口收到字符数大于零。
						temp2 = Serial.parseInt(); // 在串口数据流中查找一个有效整数。
						hour = temp2;			   //打印接收到的数字
					}
					break;
				}
			}

			Serial.println("please input minute, like 45 (0~59)");
			while (1)
			{
				if (Serial.available())
				{
					while (Serial.available() > 0)
					{							   // 串口收到字符数大于零。
						temp2 = Serial.parseInt(); // 在串口数据流中查找一个有效整数。
						minute = temp2;			   //打印接收到的数字
					}
					break;
				}
			}
			rtc.adjustRtc(year, month, day, week, hour, minute, second);
			Serial.print("now time is");
			Serial.print(hour);
			Serial.print("/");
			Serial.print(minute);
			Serial.println("/0");
			num = '8';
			break;
		case '4': //date
			Serial.println("please input year, like 2018 (2001~2099)");
			while (1)
			{
				if (Serial.available())
				{
					while (Serial.available() > 0)
					{							   // 串口收到字符数大于零。
						temp1 = Serial.parseInt(); // 在串口数据流中查找一个有效整数。
						year = temp1;			   //打印接收到的数字
					}
					break;
				}
			}
			Serial.println("please input month, like 12 (1~12)");
			while (1)
			{
				if (Serial.available())
				{
					while (Serial.available() > 0)
					{							   // 串口收到字符数大于零。
						temp2 = Serial.parseInt(); // 在串口数据流中查找一个有效整数。
						month = temp2;			   //打印接收到的数字
					}
					break;
				}
			}
			Serial.println("please input day, like 7 (1~31)");
			while (1)
			{
				if (Serial.available())
				{
					while (Serial.available() > 0)
					{							   // 串口收到字符数大于零。
						temp2 = Serial.parseInt(); // 在串口数据流中查找一个有效整数。
						day = temp2;			   //打印接收到的数字
					}
					break;
				}
			}
			Serial.println("please input week, like 7 (1~7)");
			while (1)
			{
				if (Serial.available())
				{
					while (Serial.available() > 0)
					{							   // 串口收到字符数大于零。
						temp2 = Serial.parseInt(); // 在串口数据流中查找一个有效整数。
						week = temp2;			   //打印接收到的数字
					}
					break;
				}
			}
			rtc.adjustRtc(year, month, day, week, hour, minute, second);
			Serial.print("now time is");
			Serial.print(year);
			Serial.print("/");
			Serial.print(month);
			Serial.print("/");
			Serial.print(day);
			Serial.print("/");
			Serial.println(week);
			num = '8';
			break;
		case '5': //timer
				  //间隔怎么设置？第一段第二段第三段时间的设置？
			Serial.println("input the air sensor's timer(ms)");
			while (1)
			{
				if (Serial.available())
				{
					while (Serial.available() > 0)
					{								   // 串口收到字符数大于零。
						firstTime = Serial.parseInt(); // 在串口数据流中查找一个有效整数。
					}
					break;
				}
			}
			Serial.println("input the offgas sensor's timer(ms)");
			while (1)
			{
				if (Serial.available())
				{
					while (Serial.available() > 0)
					{									// 串口收到字符数大于零。
						secondTime = Serial.parseInt(); // 在串口数据流中查找一个有效整数。
					}
					break;
				}
			}
			Serial.println("timer is ok");
			Serial.print(firstTime);
			Serial.print("and");
			Serial.println(secondTime);
			num = '8';
			break;
		case '6': //normal
				  //输入6为默认设置，哪些默认设置
			Serial.println("normal");
			firstTime = 600000;
			secondTime = 1200000;
			rtc.adjustRtc(2018, 12, 30, 7, 12, 0, 0);
			O2 = 21;
			CO2 = 400;
			num = '8';
			break;
		default: // 可选的
			num = '9';
		}
	}
}

void setup()
{
	Serial.begin(115200);
	rtc.setup();
	sensorHub.setup();
	sdService.setup();

	//三个继电器的初始化
	pinMode(RelayAir, OUTPUT);
	pinMode(RelayOffgas, OUTPUT);
	pinMode(RelayPump, OUTPUT);
	digitalWrite(RelayAir, HIGH);
	digitalWrite(RelayOffgas, LOW);
	digitalWrite(RelayPump, HIGH);

	//IOT的初始化
	// while(!Serial);
	// if(SIM7000_Begin()==0)
	// {
	//    Serial.println(F("Init SIM7000 Successful"));
	//    flag = 1;
	// }

	//lcd初始化
	lcd.init();
	lcd.backlight();
}

//********************************************************************************************
// function name: sensorHub.getValueBySensorNumber (0)
// Function Description: Get the sensor's values, and the different parameters represent the acquisition of different sensor data
// Parameters: 0 ph value
// Parameters: 1 temperature value
// Parameters: 2 Dissolved Oxygen
// Parameters: 3 Conductivity
// Parameters: 4 Redox potential
// return value: returns a double type of data
//********************************************************************************************

void loop()
{
	rtc.update();

	//test模式进入
	if (Serial.available())
	{
		//wchar_t val = Serial.read();
		while (Serial.available() > 0)
		{
			SerTest += char(Serial.read()); //每次读一个char字符，并相加
			delay(2);
		}
		//String val = Serial.read();
		Serial.println(SerTest);
		if (SerTest == "test" && testflag == 0)
		{
			testflag = 1;
			SerTest = "";
			test();
		}
	}
	if (testflag == 1)
	{
		num = '9';
		test();
	}

	//lcd屏幕显示
	lcdTime = millis();
	if (millis() - lcdTime > 3000)
	{
		sensorHub.update();
		lcd.setCursor(0, 0);
		lcd.print("MoleRatio：96");
		//lcd.print(MoleRatio);
		lcd.setCursor(0, 1);
		
		switch (flag0)
	{
		case 0: //**:00~**:10
		sensorHub.update();
		sensorHub.getValueBySensorNumber(0);
		lcd.print("temp:");
		lcd.print(sensorHub.value[0]);
		flag0=flag0+1;
		break;
	case 1: //**:10~**:30
		sensorHub.update();
		sensorHub.getValueBySensorNumber(0);
		lcd.print("hum:");
		lcd.print(sensorHub.value[1]);
		flag0=flag0+1;
		break;
	case 2: //**:30~**:40
		sensorHub.update();
		sensorHub.getValueBySensorNumber(0);
		lcd.print("press:");
		lcd.print(sensorHub.value[2]);
		flag0=flag0+1;
		break;
	case 3: //**:40~**:00
		sensorHub.update();
		lcd.print("CO2:");
		lcd.print(sensorHub.getValueBySensorNumber(1));
		flag0=0;
		break;
	default: // 可选的
		Serial.print("LCD ERROR");
	}
	}

	//分时间段显示
	switch (flag)
	{
	case 0: //**:00~**:10
		digitalWrite(RelayPump, HIGH);
		if (millis() - updateTime > firstTime)
		{
			updateTime = millis();
			rtc.update();
			sdService.update();
			flag++;
			Serial.println("00~10,pump high");
		}

		break;
	case 1: //**:10~**:30
		digitalWrite(RelayAir, LOW);
		digitalWrite(RelayOffgas, HIGH);
		if (millis() - updateTime > secondTime)
		{
			updateTime = millis();
			rtc.update();
			sdService.update();
			flag++;
			Serial.println("10~30,pump high air low gas high");
		}

		break;
	case 2: //**:30~**:40
		digitalWrite(RelayAir, HIGH);
		digitalWrite(RelayOffgas, LOW);
		if (millis() - updateTime > firstTime)
		{
			updateTime = millis();
			rtc.update();
			sdService.update();
			flag++;
			Serial.println("30~40,pump high air high gas low");
		}

		break;
	case 3: //**:40~**:00
		digitalWrite(RelayPump, LOW);
		if (millis() - updateTime > secondTime)
		{
			updateTime = millis();
			rtc.update();
			flag = 0;
			Serial.println("40~00,pump low");
		}

		break;
	default: // 可选的
		Serial.print("flag default");
	}
}
// rtc.update();
// sensorHub.update();
// sdService.update();

// // ************************* Serial debugging ******************
// if(millis() - updateTime > 2000)
// {
// 	updateTime = millis();
// 	Serial.print(F("ph= "));
// 	Serial.print(sensorHub.getValueBySensorNumber(0));
// 	Serial.print(F("  Temp= "));
// 	Serial.print(sensorHub.getValueBySensorNumber(1));
// 	Serial.print(F("  Do= "));
// 	Serial.print(sensorHub.getValueBySensorNumber(2));
// 	Serial.print(F("  Ec= "));
// 	Serial.print(sensorHub.getValueBySensorNumber(3));
// 	Serial.print(F("  Orp= "));
// 	Serial.println(sensorHub.getValueBySensorNumber(4));
// }

//* ***************************** Print the relevant debugging information ************** ************ * /
// Note: Arduino M0 need to replace Serial with SerialUSB when printing debugging information

// ************************* Serial debugging ******************
//Serial.print("ph= ");
//Serial.print(sensorHub.getValueBySensorNumber(0));
//Serial.print("  Temp= ");
//Serial.print(sensorHub.getValueBySensorNumber(1));
//Serial.print("  Orp= ");
//Serial.println(sensorHub.getValueBySensorNumber(4));
//Serial.print("  EC= ");
//Serial.println(sensorHub.getValueBySensorNumber(3));

// ************************************************************ time ********************** **********
//Serial.print("   Year = ");//year
//Serial.print(rtc.year);
//Serial.print("   Month = ");//month
//Serial.print(rtc.month);
//Serial.print("   Day = ");//day
//Serial.print(rtc.day);
//Serial.print("   Week = ");//week
//Serial.print(rtc.week);
//Serial.print("   Hour = ");//hour
//Serial.print(rtc.hour);
//Serial.print("   Minute = ");//minute
//Serial.print(rtc.minute);
//Serial.print("   Second = ");//second
//Serial.println(rtc.second);
