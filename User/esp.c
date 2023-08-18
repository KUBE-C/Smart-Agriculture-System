#include "stm32f10x.h"                  // Device header
#include <string.h>
#include "Serial.h"
#include "Servo.h"
#include "Delay.h"
#include "String.h"
#include "Oled.h"
#include "LED.h"
#include "esp.h"
#include "Motor.h"
#include "AD.h"

char LED1, LED2;
const char* WIFI ="dooth";
const char* WIFIASSWORD="wh741852963"; 
const char* ClintID="h6t7GurN1TW.znwl02|securemode=2\\,signmethod=hmacsha256\\,timestamp=1671445344622|";
const char* username="znwl02&h6t7GurN1TW";
const char* passwd="0ece4b6a846a1108890f93d5360b7910d3ed3117138baf2edf0bfa38ae02f106";
const char* Url="iot-06z00fw34djtzfc.mqtt.iothub.aliyuncs.com";
const char* pubtopic="/sys/h6t7GurN1TW/znwl02/thing/event/property/post";
const char* subtopic="/sys/h6t7GurN1TW/znwl02/thing/event/property/post_reply";
const char* func1="temperature";
const char* func2="LightLux";
const char* func3="MotionAlarmState";
const char* func4="LEDSwitch";
const char* func5="DoorSwitch";

//注意ClintID逗号前面加\\

/**
  * @brief  初始化阿里云物联网云平台
  * @param  无
  * @retval None
  */
char esp_Init(void)
{
	
	memset(RECS,0,sizeof(RECS));//把接收数据的数组进行清0
	printf("AT+RST\r\n");  //重启
	Delay_ms(2000);
	
	memset(RECS,0,sizeof(RECS));
	printf("ATE0\r\n");    //关闭回显
	Delay_ms(10);
	if(strcmp(RECS,"OK"))
		return 1;
	
	printf("AT+CWMODE=1\r\n"); //Station模式
	Delay_ms(1000);
	if(strcmp(RECS,"OK"))
		return 2;
	
	
	memset(RECS,0,sizeof(RECS));
	printf("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n"); //配置时区
	Delay_ms(1000);
	if(strcmp(RECS,"OK"))
		return 3;
	
	memset(RECS,0,sizeof(RECS));
	printf("AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI,WIFIASSWORD); //连接热点
	Delay_ms(2000);
	if(strcmp(RECS,"OK"))
		return 4;
	

	memset(RECS,0,sizeof(RECS));
	printf("AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",ClintID,username,passwd);//用户信息配置
	Delay_ms(10);
	if(strcmp(RECS,"OK"))
		return 4;
	
	
	memset(RECS,0,sizeof(RECS));
	printf("AT+MQTTCONN=0,\"%s\",1883,1\r\n",Url); //连接服务器
	Delay_ms(1000);
	if(strcmp(RECS,"OK"))
		return 6;
	
	printf("AT+MQTTSUB=0,\"%s\",1\r\n",subtopic); //订阅消息
	Delay_ms(500);
	if(strcmp(RECS,"OK"))
		return 7;
	memset(RECS,0,sizeof(RECS));
	return 0;
}


//功能：esp发送消息
//参数：无
//返回值：0：发送成功；1：发送失败
char Esp_PUB(void)
{
	memset(RECS,0,sizeof(RECS));
//	printf("AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d}}\",0,0\r\n",pubtopic,func1,Temperature,func2,Shidu,func3,Lock,func4,Switch2);
	//while(RECS[0]);//等待ESP返回数据
	printf("AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%d\\,\\\"%s\\\":%d\\,\\\"%s\\\":%d}}\",0,0\r\n",pubtopic,func1,AD_Value[0],func2,AD_Value[1],func3,InfraredSwitch);
	Delay_ms(200);//延时等待数据接收完成
	if(strcmp(RECS,"ERROR")==0)
		return 1;
	return 0;
}


//功能：esp发送消息
//参数：无
//返回值：0：发送成功；1：发送失败
char Esp_PUB1(void)
{
	memset(RECS,0,sizeof(RECS));
	printf("AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%d\\,\\\"%s\\\":%d\\}}\",0,0\r\n",pubtopic,func4,LED1_flag,func5,Servo_flag);
	while(RECS[0]);//等待ESP返回数据
	Delay_ms(200);//延时等待数据接收完成
	if(strcmp(RECS,"ERROR")==0)
		return 1;
	return 0;
}


/**
  * @brief 接收阿里云发送的内容并进行解析
  * @param  无
  * @retval None
  */
void CommandAnalyse(void)
{
	if(strncmp(RECS,"+MQTTSUBRECV:",13)==0)
	{
		uint8_t i=0;
		while(RECS[i++] != '\0')             
		{
			if(strncmp((RECS+i),func4,4)==0)
			{
				while(RECS[i++] != ':');
				LED1=RECS[i];
				if(LED1 == '0')
				{
					LED1_OFF();
				}
				if(LED1 == '1')
				{
					LED1_ON();
				}
			}
			if(strncmp((RECS+i),func5,5)==0)
			{
				while(RECS[i++] != ':');       
				LED2=RECS[i];
				if(LED1 == '0')
				{
					Servo_close();
				}
				if(LED1 == '1')
				{
					Servo_open();
				}
			}
		}
	}
}
