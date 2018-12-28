# Oxygen station

Knowflow-based framework for the framework of sludge exhaust oxygen detection.
# 概述
上海电力学院环境工程蒋路漫老师为了进行活性污泥法中微孔曝气系统的供氧效率，需要监测离开曝气池的尾气中的氧浓度，需要通过技术手段来完成一系列的控制和监测工作。

![图片](https://images-cdn.shimo.im/7uJlYSknIL8LUQXg/15397701854391.jpg!thumbnail)

在污水处理中，一般常规的处理方法是活性污泥法。活性污泥法需要通过向污水中泵入空气，提供给菌胶团生长和代谢的氧气，通过菌胶团的代谢来分解水中含有的有机物，将这些有机物分解成二氧化碳和水等无机物。装在反应池底部的曝气装置通过鼓风机向混合液鼓入空气，一部分溶解于水中成为溶解氧（DO）被微生物利用，一部分来不及溶解而直接从污水表面逸散。使更多的氧气溶解在混合液里是曝气系统的终极目标。目前常用的微孔曝气器通过减小产生气泡的体积，增加气泡在水里的停留时间，从而增加氧气在水中的溶解量
目前国外团队开发的尾气分析仪装置示意图如下所示：
1、加州大学团队
![图片](https://images-cdn.shimo.im/l2p6XZNVqRcBV0Yy/image.png!thumbnail)
![图片](https://images-cdn.shimo.im/pJTk12M0YlAOVait/image.png!thumbnail)
![图片](https://images-cdn.shimo.im/Lpb3psRHLPotIZwx/image.png!thumbnail)
2、波兰团队
氧传感器采用AMI 65/O2 zirconium probe
![图片](https://images-cdn.shimo.im/wspKHJ9MKuMBv7Y9/image.png!thumbnail)

3、Mazzei AirJection公司的装置：
氧传感器：a Teledyne Analytical Instruments Model 320P/D Portable Oxygen Analyzer

4、比利时团队
![图片](https://images-cdn.shimo.im/2Q1Xw8ANuN8qUcYK/image.png!thumbnail)
氧传感器：AMI Model 65，Advanced Micro Instruments，USA
CO2含量测定：光声红外气体分析仪（X-Stream，Emerson）

5、中国人民大学团队
![图片](https://images-cdn.shimo.im/K4lThXThACwoTb2d/image.png!thumbnail)
气体分析仪包括温度传感器6、流量计及控制阀7 和8、二氧化碳和水蒸气的吹扫柱9、氧传感器10、干燥柱11、二氧化碳红外分析仪12 和数据分析仪及显示面板13 等。氧传感器、温度传感器和二氧化碳红外分析仪可以在线测定O2、温度和CO2，并将测定结果传送到数据分析仪，通过数据分析可以直接在仪器面板上显示OTE 的值。

在活性污泥法中，供气是整个运行中的相当重要的能源消耗部分。如何更有效率的给活性污泥供气是研究的一个重要方向。
蒋路漫老师团队准备利用测量供气和尾气中的氧气含量来研究各种曝气装置的曝气效率。
因此需要一套控制装置能够进行：
1. 测量尾气/空气中的氧含量（A）
2. 测量取样管进气的流量（A）
3. 测量尾气中二氧化碳的含量（A）
4. 测量环境BME280温度、湿度、气压（I2C）
5. 本地数据SD卡存储（SPI）
6. 上传物联网（串口）
7. 控制空气和尾气取样电磁阀（D）
8. 控制取样泵启动与关闭（D）
9. 在地的LCD显示（I2C）
# 方案说明
方案采用开源硬件Arduino Mega作为主控。采用单路传感器输入，两路数据保存的模型进行处理。
![图片](https://images-cdn.shimo.im/nytnBpa7lJct5LVh/15397703138400.jpg!thumbnail)

其中一路作为在地的数据保存和显示，一路作为网络数据传输。
![图片](https://images-cdn.shimo.im/qMbbWfxWOiMFFCVy/image.png!thumbnail)
氧气传感器、二氧化碳传感器、气体流量传感器、环境传感器BME280均通过扩展板连接到arduino Mega上，其中氧传感器通过变送器（该变送器采用24v供电）采用0-5v信号输出。并且通过GPRS模块将数据传输到iot.dfrobot.com.cn上进行存储。
![图片](https://images-cdn.shimo.im/tpwCK2qjbjUy0xTD/15407904694211.jpg!thumbnail)

两路电磁阀控制信号也由这块控制器进行控制进行每个小时的取样。
将所有的运行状态用SD卡进行保存，数据依照RTC时钟数据作为区隔。以便后续的数据处理。
采用24v供电，直接给氧传感器变送器供电，然后用电压转换模块将电压从24v直接转换为7.4v以便供给主控器等其他部件。

湿式真空泵抽取尾气，气管直径0.5cm，气体流量计0~10 L/min，具体抽取气量根据实验现场情况而定（约5~6 L/min）。将气体通过填充有干燥剂的柱子除去水气。干燥后气体通过CO2分析仪，然后通过填充有氢氧化钠（负载在PVC颗粒内）的柱子除去二氧化碳。处理后气体准备进入尾气阀门。

## 线路初步设计
![图片](https://images-cdn.shimo.im/1K2LFnSssvsDEfdV/image.png!thumbnail)
![图片](https://images-cdn.shimo.im/yRyt3c2UsRU5sUxH/image.png!thumbnail)
![图片](https://images-cdn.shimo.im/SRMGiSqLWSgAyZxF/15422468718168.jpg!thumbnail)
# 数据估算
## 功耗估算
主板功耗：500ma
传感器功耗：500ma
通讯模块功耗：100ma
共计：1100ma

## 流量估算
每天24次数据传输，每次传输数据0.2k，共计4.8k。
进行18个月即18*30=540天
共消耗流量540×4.8k=2592k字节

## IOT节点估算
每天24个数据节点，每次上传数据为7个，18个月运行，共90720个数据。
# 软件需求简述
IO口设计：
| 序号   | 名称   | IO口   | 数量   | 说明   | 备注   | 
|:----|:----|:----|:----|:----|:----|
| 1   | 氧气   | 模拟口   | 1   | 4~20ma   |    | 
| 2   | 二氧化碳   | 模拟口   | 1   |    |    | 
| 3   | 环境   | I2C   | 1   | 温度、湿度、气压   |    | 
| 4   | 继电器   | 数字口   | 1   | 2路电磁阀控制   | 一路继电器接常闭，一路接常开   | 
| 5   | 继电器   | 数字口   | 1   | 真空泵控制   |    | 
| 6   | 通讯   | 串口   | 1   | GPRS   |    | 
| 7   | LCD   | I2C   | 1   |    |    | 
| 8   | SD   | SPI   | 1   |    |    | 
| 9   | RTC   | I2C   | 1   |    |    | 


## 控制部分：

整点开启采样泵，开启空气阀门，关闭尾气阀门，将稳定5min后的空气数据采集5min并进行平均后存储。
开启尾气阀门，关闭空气阀门，将稳定15min后的尾气数据采集5min并进行平均后存储。（氧传感器的响应时间为<5s）
开启空气阀门，关闭尾气阀门，将稳定5min后的空气数据采集5min并进行平均后存储。
关闭采样泵。

## 数据传输部分：
将每小时采集的数据通过GPRS依次传回IOT网站（空气氧含量、尾气氧含量、二氧化碳含量、温度、湿度、压力）
将数据存储在SD卡上，用RTC数据打点。
| 时间   | 空气氧含量（%）   | 尾气氧含量（%）   | 摩尔体积分数（%）   | 二氧化碳含量（PPM）   | 温度  （℃）   | 湿度  （%）   | 压力  （KPa）   | 电磁阀状态  （空1/尾0）   | 
|:----|:----|:----|:----|:----|:----|:----|:----|:----|
| 2019-12-23   | 20.5   | 19.4   | 94.63   | 400   | 23.5   | 80   | 101   | 1   | 

## 校准
供电： 10-30Vdc； 接24V和0V； 输出信号： 0-5V（实际有偏差）；接OuT和GND； 通氮气，此时输出的电压值就是零点的信号值； 通空气，此时输出的信号就是20.95%氧气输出的信号； 两点成一线性就就可以测量了. 
## 正常模式
上电后，按照时间正常操作。
显示屏显示当前测量数据，每三秒钟更新下一项数据内容。
按控制部分要求运行。
屏幕上部显示相对量
屏幕下部显示

## 调试模式
二期：
按下按钮后，显示调试菜单。
菜单目录同串口菜单。

---
一期：
通过串口输入：test后进入调试模式。
并显示调试菜单
1、CAL：O2
2、CAL：CO2
3、TIME
4、DATE
5、TIMER
0、normal

串口输入1后进入氧气校准
校准氧气（标准空气21%）
校准氧气时时
预热3min倒计时
180s
179s
178s
...
...
3s
2s
1s
校准氧气时，串口输出数据，待稳定后，串口输入OK后，完成校准。
串口输出校准值以及实际测量值。
O2 CAL XXX， O2 XXX
CAL O2 OK

串口输入2后进入二氧化碳校准
校准二氧化碳
校准二氧化碳时
预热3min倒计时
180s
179s
178s
...
...
3s
2s
1s
串口输出相应数值，待数据稳定后，串口输入OK后，完成校准。
串口输出校准值以及实际测量值。
CO2 CAL XXX， CO2 XXX
CAL CO2 OK

串口输入3后可设定当前时间。
串口按格式输入HHMM
串口输出当前时间。

串口输入4后可设定当前日期。
串口按格式输入YYMMDD
串口输出当前日期。

串口输入5后可设定定时间隔。
串口按格式输入MM
串口输出当前设定时间间隔。
如输入0则显示当前设定时间间隔。

![图片](https://images-cdn.shimo.im/NY1NA59WAn01z5um/image.png!thumbnail)
## 编程部分：
![图片](https://images-cdn.shimo.im/DdCHBcUjBzI7sulh/knowflow软件需求.jpg!thumbnail)
### Control部分：
* 输入：mills()
* 输出： 3个继电器(高开低关)
* 控制函数：
* （1）mills()；

（2）digitalWrite( );
* 异常：无法检测到时钟模块
* 异常处理：维持继电器Air、Offgas一开一闭，继电器Pump关闭的状态
### SensorHub部分：
* 输入：环境传感器、O2传感器、CO2传感器、RTC
* 输出：温度、湿度、气压、O2、CO2、时钟时间
* 控制函数：

（1）RTC.h；
（2）环境传感器.h（待封装）；
（3）O2传感器.h（待封装）；
（4）CO2传感器.h（待封装）；
* 异常：无时钟模块；检测不到传感器数据
* 异常处理：Record部分记录异常，不操作传感器；connectString(0)；
### Record（SD卡部分）：
* 输入：DATA、RTC
* 输出： SD
* 控制函数：

（1）SDService.h；
（2）RTC.H
* 异常：SD卡部分的异常参照SDService.h
*  异常处理：SD卡部分的异常参照SDService.h
### Record（IOT部分）：
* 输入：DATA
* 输出：IOT
* 控制函数：

（1）DFRobot_SIM7000.h
* 异常：IOT部分的异常参照DFRobot_SIM7000.h
* 异常处理：IOT部分的异常参照DFRobot_SIM7000.h
* 备注：IOT部分需要账号及SIM卡

# 物料清单
| 序号   | 名称   | 数量   | 单价   | 总价   | 链接   | 
|:----|:----|:----|:----|:----|:----|
| 1   | 氧气传感器   | 1   |    |    | A01T   | 
| 2   | CO2传感器   | 1   | ~~599~~  378   | ~~599~~  378   | [http://www.dfrobot.com.cn/goods-1360.html](http://www.dfrobot.com.cn/goods-1360.html)  [http://www.dfrobot.com.cn/goods-1348.html](http://www.dfrobot.com.cn/goods-1348.html)   | 
| 3   | 环境传感器   | 1   | 99   | 99   | [http://www.dfrobot.com.cn/goods-1410.html](http://www.dfrobot.com.cn/goods-1410.html)   | 
| 4   | 继电器模块   | 3   | 40   | 120   | [http://www.dfrobot.com.cn/goods-1370.html](http://www.dfrobot.com.cn/goods-1370.html)   | 
| 5   | Maga   | 1   | 121   | 121   | [http://www.dfrobot.com.cn/goods-504.html](http://www.dfrobot.com.cn/goods-504.html)   | 
| 6   | 扩展板   | 1   | 98   | 98   | [http://www.dfrobot.com.cn/goods-195.html](http://www.dfrobot.com.cn/goods-195.html)   | 
| 7   | 通讯扩展板   | 1   | 295   | 295   | [http://www.dfrobot.com.cn/goods-1645.html](http://www.dfrobot.com.cn/goods-1645.html)   | 
| 8   | LCD   | 1   | 45   | 45   | [http://www.dfrobot.com.cn/goods-1696.html](http://www.dfrobot.com.cn/goods-1696.html)   | 
| 9   | SD卡   | 1   | 59   | 59   | [http://www.dfrobot.com.cn/goods-1037.html](http://www.dfrobot.com.cn/goods-1037.html)   | 
| 10   | 防水外壳   | 1   | 50   | 50   |    | 
| 11   | 时钟模块   | 1   | 60   | 60   | [http://www.dfrobot.com.cn/goods-1405.html](http://www.dfrobot.com.cn/goods-1405.html)   | 
| 12 | DC24V转DC7.5V 5A 电源转换器 | 1   | 30   | 58   | [http://www.dfrobot.com.cn/goods-905.html](http://www.dfrobot.com.cn/goods-905.html)  https://item.taobao.com/item.htm?spm=a1z0d.6639537.1997196601.4.20947484sCGZI1&id=546075182288   | 
| 13   | 24v电源适配器   | 1   |    | 37   | [https://detail.tmall.com/item.htm?spm=a230r.1.14.6.718d3629ToLxsr&id=573953258723&cm_id=140105335569ed55e27b&abbucket=1](https://detail.tmall.com/item.htm?spm=a230r.1.14.6.718d3629ToLxsr&id=573953258723&cm_id=140105335569ed55e27b&abbucket=1)   | 
| 14   | 真空泵   | 2   | 160   | 320   | [https://detail.tmall.com/item.htm?spm=a1z10.5-b-s.w4011-15999798332.84.740774b26441NY&id=547164502105&rn=ca68d85f4d7e04f141f62d6ec96c9955&abbucket=5&skuId=3476195390859](https://detail.tmall.com/item.htm?spm=a1z10.5-b-s.w4011-15999798332.84.740774b26441NY&id=547164502105&rn=ca68d85f4d7e04f141f62d6ec96c9955&abbucket=5&skuId=3476195390859)   | 
| 15   | 电磁阀   | 2   | 22   | 44   | [https://detail.tmall.com/item.htm?spm=a1z10.3-b-s.w4011-15999798336.25.f4295c77xnUTmh&id=552684251829&rn=bf65a3f6f477522fd8978fc308c3ea77&abbucket=5&skuId=3600600436037](https://detail.tmall.com/item.htm?spm=a1z10.3-b-s.w4011-15999798336.25.f4295c77xnUTmh&id=552684251829&rn=bf65a3f6f477522fd8978fc308c3ea77&abbucket=5&skuId=3600600436037)   | 
| 16   | 电池   | 1   |    |    |    | 
| 17   | 管道及配件   |    |    | 100   |    | 
| 18   | 设备总价   |    |    |    | 未完全计算   | 
| 19   | 人工成本   |    |    | 5000   |    | 

# 风险
~~在这个项目中，氧气传感器的稳定性和精确度是至关重要的，但是我们现在并没有一个很确切的氧气传感器可供选择。~~

可行的方案是利用现有的市售氧含量传感器读取相关数据并进行读取。是否能读取市售传感器数据目前还没有经过测试。

现在已经选用A01T氧气传感器，经过了解，该传感器采用24v供电变送器进行取样和输出。
# 项目管理
## 开发人员职责
夏青--总协调
项目管理
硬件设计和搭建
项目总结
梁优优--主工程师
软件架构
软件代码审核
苏木--助理工程师
软件实现
软件文档
路漫--项目主管
项目验收
项目需求整理
项目合同撰写
陈--
尾气收集与化学预处理部分的设计组装
辅助硬件搭建
现场调试
调试报告撰写
## 开发时间
硬件搭建5天
软件工程10天
调试5天
调整3天
预计总计18天
![图片](https://images-cdn.shimo.im/Wp7vGPZ2cgo6jr1E/image.png!thumbnail)
# TODO list
- [x] 设备采购（DFRobot部分）——﻿@微笑的Rockets﻿ 
- [x] 设备采购（非DFRobot部分）——
    - [ ] 太阳能板
    - [ ] 太阳能管理模块
    - [ ] 电源
- [ ] 箱体设计——
- [ ] 布局设计——
- [x] 软件设计——﻿@微笑的Rockets﻿ 
- [x] control部分代码——
- [x] SensorHub部分代码
    - [x] 环境部分代码封装——﻿@Xiao Liang﻿ 
- [x] Record部分代码
- [x] 总体组装——@苏木
- [x] IOT部分代码﻿@苏木﻿
- [ ] 设备调试
- [ ] 软件文档——@苏木
# 知识产权
建议该方案内容采用开源协议CC-BY-NC（署名-非商业性使用）。
![图片](https://licensebuttons.net/l/by-nc/4.0/88x31.png)
后期我们将此设计发布在publiclab的网站及DFRobot网站。

```
<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="知识共享许可协议" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/80x15.png" /></a><br />本作品采用<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">知识共享署名-非商业性使用 4.0 国际许可协议</a>进行许可。
```

开源协议参考链接：[https://www.cnblogs.com/huaxia283611/p/LicenseIndex.html](https://www.cnblogs.com/huaxia283611/p/LicenseIndex.html)
参考资料：
![图片](https://images-cdn.shimo.im/YQDXFTOq6qsHRKBS/image.png!thumbnail)![图片](https://images-cdn.shimo.im/gJsL5NFvWe8ym64a/image.png!thumbnail)

代码参考：
[https://github.com/KnowFlow/KnowFlow_AWM](https://github.com/KnowFlow/KnowFlow_AWM)

sim7000参考代码：
```
#include <Wire.h>
#include <DFRobot_SIM7000.h>
#include <OneWire.h>
#include "DFRobot_PH.h"
#include "DFRobot_EC.h"
#include <EEPROM.h>

SoftwareSerial          mySerial(8,7);
DFRobot_SIM7000         sim7000;
OneWire ds(2);

#define serverIP        "iot.dfrobot.com.cn"
#define IOT_CLIENT      "195ff91766ff7bb0"
#define IOT_USERNAME    "H1W0p7j5Rz"
#define IOT_KEY         "B1GATmi5Rz"
#define IOT_TEMPERATURE "B1Ti7Zg87"
#define IOT_PH          "rJUPQZxIm"
#define IOT_EC          "rJWumWx8Q"

#define PH_PIN A1
#define EC_PIN A2
boolean flag = 0;
float voltage,phValue,ecValue,temperature = 25;
DFRobot_PH ph;
DFRobot_EC ec;

void setup() 
{
    Serial.begin(115200);
    ph.begin();
    ec.begin();
    while(!Serial);
    if(SIM7000_Begin()==0)
    {
       Serial.println(F("Init SIM7000 Successful"));
       flag = 1;
    }
}

void loop() 
{
     static unsigned long printTimepoint = millis();
    if(millis()-printTimepoint>2000U)
    {
       printTimepoint = millis();
        temperature = readTemperature();
        Serial.print(F("temperature:"));
        Serial.print(temperature);
        
      voltage = analogRead(PH_PIN)/1024.0*5000;  // read the voltage
      phValue = ph.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
      Serial.print("^C  pH:");
      Serial.print(phValue,2);

      voltage = analogRead(EC_PIN)/1024.0*5000;  // read the voltage
      ecValue =  ec.readEC(voltage,temperature);  // convert voltage to EC with temperature compensation
      Serial.print("  EC:");
      Serial.print(ecValue,2);
      Serial.println("ms/cm");
     }
    
    static unsigned long iotTimepoint = millis();
    if((flag)&&(millis()-iotTimepoint>180000U))
    {
       iotTimepoint = millis();
       SIM7000_Send(phValue,ecValue,temperature);
     }
}


int SIM7000_Begin()
{  
    int count;
    sim7000.begin(mySerial);
    Serial.println(F("Turn ON SIM7000......"));
    if(sim7000.turnON())
    {                                        //Turn ON SIM7000
        Serial.println(F("Turn ON !"));
    }
    else{
           Serial.println(F("Turn ON FAILED!"));
           return -1;
      }
    Serial.println(F("Set baud rate......"));
    count = 5;
    while(1)
    {
        if(sim7000.setBaudRate(19200))
        {                          //Set SIM7000 baud rate from 115200 to 19200 reduce the baud rate to avoid distortion
            Serial.println(F("Set baud rate:19200 Successful"));
            break;
        }else{
            Serial.println(F("Faile to set baud rate"));
            delay(1000);
            count--;
            if(count<=0)
              return -1;
        }
    }

    Serial.println(F("Check SIM card......"));
    if(sim7000.checkSIMStatus()){                                //Check SIM card
        Serial.println(F("SIM card READY"));
    }else{
        Serial.println(F("SIM card ERROR, Check if you have insert SIM card and restart SIM7000"));
        return -1;
    }

    count = 5;
    Serial.println(F("Set net mode......"));
    while(1){
        if(sim7000.setNetMode(GPRS)){                              //Set net mod NB-IOT
            Serial.println(F("Set NB-IOT mode successful"));
            break;
        }else{
            Serial.println(F("Fail to set mode"));
            delay(1000);
             count--;
            if(count<=0)
              return -1;
        }
    }

    /*Serial.println("Get signal quality......");
    delay(500);
    signalStrength=sim7000.checkSignalQuality();                 //Check signal quality from (0-30)
    Serial.print("signalStrength =");
    Serial.println(signalStrength);
    delay(500);
    */
    count = 5;
    Serial.println(F("Attaching service......"));
    while(1){
        if(sim7000.attacthService()){                            //Open the connection
            Serial.println(F("Attach service"));
            break;
        }else{
            Serial.println(F("Fail to Attach service"));
            delay(1000);
             count--;
            if(count<=0)
              return -1;
        }
    }
    return 0;
}

int SIM7000_Send(float _ph, float _ec, float _temperature)
{
    Serial.print(F("Connect to :"));
    Serial.println(F(serverIP));
    if(sim7000.openNetwork(TCP,serverIP,1883)){                  //Connect to server
        Serial.println(F("Connected !"));
    }else{
        Serial.println(F("Failed to connect"));
        return -1;
    }
    delay(200);

    Serial.print(F("Connect to : "));
    Serial.println(F(IOT_USERNAME));
    if(sim7000.mqttConnect(IOT_CLIENT,IOT_USERNAME,IOT_KEY)){    //MQTT connect request
        Serial.println(F("Connected !"));
    }else{
        Serial.println(F("Failed to connect"));
        return -1;
    }
    delay(200);
    
    Serial.println(F("Send temperature data... "));
    if(sim7000.mqttPublish(IOT_TEMPERATURE,(String)_temperature)){
        Serial.println(F("temperature send OK"));
    }else{
        Serial.println(F("Failed to send temperature"));
        return -1;
    }
    delay(200);

     Serial.println(F("Send pH data... "));
    if(sim7000.mqttPublish(IOT_PH,(String)_ph)){
        Serial.println(F("pH send OK"));
    }else{
        Serial.println(F("Failed to send pH"));
        return -1;
    }
    delay(200);

     Serial.println(F("Send EC data... "));
    if(sim7000.mqttPublish(IOT_EC,(String)_ec)){
        Serial.println(F("EC send OK"));
    }else{
        Serial.println(F("Failed to send EC"));
        return -1;
    }
    delay(200);

    Serial.println(F("Close connection......"));
    if(sim7000.closeNetwork()){                                  //Close connection
        Serial.println(F("Close connection !"));
    }else{
        Serial.println(F("Fail to close connection !"));
        return -1;
    }
    delay(2000);
    return 0;
}

float readTemperature() //read temperature from ds18b20
{
  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println(F("CRC is not valid!"));
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print(F("Device is not recognized"));
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  delay(800);
  
  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  return TemperatureSum;
}
```
校准程序参考：[https://item.taobao.com/item.htm?spm=a230r.1.14.252.e6cf32b73uhk0M&id=543378947206&ns=1&abbucket=1#detail](https://item.taobao.com/item.htm?spm=a230r.1.14.252.e6cf32b73uhk0M&id=543378947206&ns=1&abbucket=1#detail)
