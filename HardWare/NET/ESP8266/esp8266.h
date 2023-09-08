#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "stm32f10x.h"


#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"TP-LINK_4B5DA4\",\"\"\r\n"
#define ESP8266_WIFI_INFO		"AT+CWJAP=\"Xiaomi 12X\",\"abc123123\"\r\n"
//#define ESP8266_WIFI_INFO		"AT+CWJAP=\"L\",\"wendy147369\"\r\n"

#define ESP8266_TCP1_INFO		"AT+CIPSTART=\"TCP\",\"192.168.5.220\",8800\r\n"

#define ESP8266_TCP2_INFO		"AT+CIPSTART=\"TCP\",\"10.101.129.102\",8800\r\n"

#define ESP8266_TCP3_INFO		"AT+CIPSTART=\"TCP\",\"192.168.5.219\",1234\r\n"

extern uint16_t lenSize;

void ESP8266_Init(void);

void ESP8266_Clear(void);

void ESP8266_SendData(unsigned char *data, unsigned short len);

unsigned char *ESP8266_GetIPD(unsigned short timeOut, char *len);

_Bool ESP8266_SendCmd(char *cmd, char *res);


#endif
