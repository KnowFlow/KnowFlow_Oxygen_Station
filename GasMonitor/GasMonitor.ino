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
#include "SdService.h"
#include "Debug.h"
#include <SoftwareSerial.h>
#include "DFRobot_SIM7000.h"
#include <EEPROM.h>
#include <OneWire.h>

//iot
SoftwareSerial mySerial(10, 11);   //关键语句
DFRobot_SIM7000 sim7000;
OneWire ds(2);
#define serverIP "iot.dfrobot.com.cn"
#define IOT_CLIENT "37386488df2beb56"
#define IOT_USERNAME "H1-QCEdJxN"
#define IOT_KEY "rJG70VOJxN"
#define IOT_TEMP "H1b1Sd1e4"
#define IOT_HUMI "SkCGH_yeE"
#define IOT_PRESS "HJwEr_1gN"
#define IOT_CO2 "Bk-BSuye4"
#define IOT_O2 "ryRHSukeE"
#define IOT_TEMP "H1b1Sd1e4"
#define IOT_HUMI "SkCGH_yeE"
#define IOT_PRESS "HJwEr_1gN"
#define IOT_CO2 "Bk-BSuye4"
#define IOT_O2 "ryRHSukeE"
boolean flag = 0;

// clock module
GravityRtc rtc;

// sensor monitor
GravitySensorHub sensorHub;
SdService sdService = SdService(sensorHub.sensors);

//loop函数中的变量
unsigned long updateTime = 0;
//unsigned int flag = 0;
unsigned int flag0 = 0;
unsigned long lcdTime = 0;
//unsigned long firstTime = 600000;
//unsigned long secondTime = 1200000;
unsigned long firstTime = 6000;
unsigned long secondTime = 12000;
String SerTest = "";

//lcd屏幕显示的变量
//LiquidCrystal_I2C lcd(0x27, 16, 2);
float MoleRatio;

void setup()
{
  Serial.begin(115200);
  rtc.setup();
  sensorHub.setup();
  sdService.setup();

  //rtc.adjustRtc(2018,12,8,6,14,1,0);

  //lcd初始化
  //lcd.init();
  //lcd.backlight();

  //iot
  //Serial.begin(115200);
  while (!Serial)
    ;
  if (SIM7000_Begin() == 0)
  {
    Serial.println(F("Init SIM7000 Successful"));
    flag = 1;
  }
}

void loop()
{
  rtc.update();
  sensorHub.update();
  sdService.update();

  if (millis() - updateTime > 1000)
  {
    updateTime = millis();
    sensorHub.getValueBySensorNumber(0);
    Serial.print(F("temperature:"));
    Serial.print(sensorHub.value[0]);
    Serial.print(F("  ,humidity:"));
    Serial.print(sensorHub.value[1]);
    Serial.print(F("  ,pressure:"));
    Serial.print(sensorHub.value[2]);
    Serial.print(F("  CO2:"));
    Serial.print(sensorHub.getValueBySensorNumber(1));
    Serial.print(F("  O2:"));
    Serial.println(sensorHub.getValueBySensorNumber(2));
    SIM7000_Send(sensorHub.value[0], sensorHub.value[1], sensorHub.value[2], sensorHub.getValueBySensorNumber(1), sensorHub.getValueBySensorNumber(2));

    Serial.print("   Year = "); //year
    Serial.print(rtc.year);
    Serial.print("   Month = "); //month
    Serial.print(rtc.month);
    Serial.print("   Day = "); //day
    Serial.print(rtc.day);
    Serial.print("   Week = "); //week
    Serial.print(rtc.week);
    Serial.print("   Hour = "); //hour
    Serial.print(rtc.hour);
    Serial.print("   Minute = "); //minute
    Serial.print(rtc.minute);
    Serial.print("   Second = "); //second
    Serial.println(rtc.second);
  }
}

int SIM7000_Begin()
{
  int count;
  sim7000.begin(mySerial);
  Serial.println(F("Turn ON SIM7000......"));
  if (sim7000.turnON())
  { //Turn ON SIM7000
    Serial.println(F("Turn ON !"));
  }
  else
  {
    Serial.println(F("Turn ON FAILED!"));
    return -1;
  }
  Serial.println(F("Set baud rate......"));
  count = 5;
  while (1)
  {
    if (sim7000.setBaudRate(19200))
    { //Set SIM7000 baud rate from 115200 to 19200 reduce the baud rate to avoid distortion
      Serial.println(F("Set baud rate:19200 Successful"));
      break;
    }
    else
    {
      Serial.println(F("Faile to set baud rate"));
      delay(1000);
      count--;
      if (count <= 0)
        return -1;
    }
  }

  Serial.println(F("Check SIM card......"));
  if (sim7000.checkSIMStatus())
  { //Check SIM card
    Serial.println(F("SIM card READY"));
  }
  else
  {
    Serial.println(F("SIM card ERROR, Check if you have insert SIM card and restart SIM7000"));
    return -1;
  }

  count = 5;
  Serial.println(F("Set net mode......"));
  while (1)
  {
    if (sim7000.setNetMode(GPRS))
    { //Set net mod NB-IOT
      Serial.println(F("Set GPRS mode successful"));
      break;
    }
    else
    {
      Serial.println(F("Fail to set mode"));
      delay(1000);
      count--;
      if (count <= 0)
        return -1;
    }
  }

  /*Serial.println("Get signal quality......");
    delay(500);
    signalStrength=sim7000.checkSignalQuality();                 //Check signal quality from (0-30)
    Serial.print("signalStrength =");
    Serial.println(signalStrength);
    delay(500);
    */
  count = 5;
  Serial.println(F("Attaching service......"));
  while (1)
  {
    if (sim7000.attacthService())
    { //Open the connection
      Serial.println(F("Attach service"));
      break;
    }
    else
    {
      Serial.println(F("Fail to Attach service"));
      delay(1000);
      count--;
      if (count <= 0)
        return -1;
    }
  }
  return 0;
}



int SIM7000_Send(float _temp, float _humi, float _press,float _CO2,float _O2)
{
  Serial.print(F("Connect to :"));
  Serial.println(F(serverIP));
  if (sim7000.openNetwork(TCP, serverIP, 1883))
  { //Connect to server
    Serial.println(F("Connected !"));
  }
  else
  {
    Serial.println(F("Failed to connect"));
    return -1;
  }
  delay(200);

  Serial.print(F("Connect to : "));
  Serial.println(F(IOT_USERNAME));
  if (sim7000.mqttConnect(IOT_CLIENT, IOT_USERNAME, IOT_KEY))
  { //MQTT connect request
    Serial.println(F("Connected !"));
  }
  else
  {
    Serial.println(F("Failed to connect"));
    return -1;
  }
  delay(200);

  Serial.println(F("Send temperature data... "));
  if (sim7000.mqttPublish(IOT_TEMP, (String)_temp))
  {
    Serial.println(F("temperature send OK"));
  }
  else
  {
    Serial.println(F("Failed to send temperature"));
    return -1;
  }
  delay(200);

  Serial.println(F("Send HUMI data... "));
  if (sim7000.mqttPublish(IOT_HUMI, (String)_humi))
  {
    Serial.println(F("HUMI send OK"));
  }
  else
  {
    Serial.println(F("Failed to send HUMI"));
    return -1;
  }
  delay(200);

  Serial.println(F("Send PRESS data... "));
  if (sim7000.mqttPublish(IOT_PRESS, (String)_press))
  {
    Serial.println(F("PRESS send OK"));
  }
  else
  {
    Serial.println(F("Failed to send PRESS"));
    return -1;
  }
  delay(200);

  Serial.println(F("Send CO2 data... "));
  if (sim7000.mqttPublish(IOT_CO2, (String)_CO2))
  {
    Serial.println(F("CO2 send OK"));
  }
  else
  {
    Serial.println(F("Failed to send CO2"));
    return -1;
  }
  delay(200);
  
  Serial.println(F("Send O2 data... "));
  if (sim7000.mqttPublish(IOT_O2, (String)_O2))
  {
    Serial.println(F("O2 send OK"));
  }
  else
  {
    Serial.println(F("Failed to send O2"));
    return -1;
  }
  delay(200);

  Serial.println(F("Close connection......"));
  if (sim7000.closeNetwork())
  { //Close connection
    Serial.println(F("Close connection !"));
  }
  else
  {
    Serial.println(F("Fail to close connection !"));
    return -1;
  }
  delay(2000);
  return 0;
}
