//单片机头文件
#include "stm32f10x.h"

#include "Data.h"
#include "usart.h"
#include "Led.h"
#include "AutoControl.h"
#include "Stepper.h"
#include "LED.h"
#include "Delay.h"
#include "Lcd.h"
#include "Beep.h"

//网络设备
#include "esp8266.h"
//C库
#include <string.h>
#include <stdio.h>
#include "cJSON.h"

//接收数据
void Data_Recieve(void)
{
    unsigned char *dataPtr = NULL;
    char len[lenSize]; // 长度
    
    dataPtr = (unsigned char *)ESP8266_GetIPD(3,len);
    unsigned short dataLen =(unsigned short)atoi(len);
    
    if(dataPtr != NULL)
    {
        UsartPrintf(USART_DEBUG,(char *)dataPtr);
        //UsartPrintf(USART_DEBUG," %d",dataLen);
        Data_RevPro(dataPtr,dataLen);
    }
}


//==========================================================
//	函数名称：	Data_RevPro
//
//	函数功能：	返回数据检测
//
//==========================================================
void Data_RevPro(unsigned char *req, unsigned short req_len) // 将json进行处理，并执行相应的操作
{
    char *req_payload = (char *)req;
    
    //unsigned short req_len = 0;
    int value1, value2, value3;
    
    cJSON *json, *json_value;//定义cJSON
	
    //解析json格式req_payload的数据包
        json = cJSON_Parse(req_payload);
        if(!json) UsartPrintf(USART_DEBUG,"Error before: [%s]\n",cJSON_GetErrorPtr());
        else
        {
            json_value = cJSON_GetObjectItem(json, "TEM1");
            if (json_value) // 设置温度阈值1
            {
                 value1 = atoi(json_value->valuestring);
                if (value1)
                {
                    UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value1);
                    tempControl.TEM_FirstMaxValue = value1;
                }
            }
            
            json_value = cJSON_GetObjectItem(json, "TEM2");
            if (json_value) // 设置温度阈值2
            {
                 value2 = atoi(json_value->valuestring);
                if (value2)
                {
                    UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value2);
                    tempControl.TEM_SecondMaxValue = value2;
                }
            }
            
            json_value = cJSON_GetObjectItem(json, "LIGHT");
            if (json_value) // 设置光照阈值
            {
                 value3 = atoi(json_value->valuestring);
                if (value3)
                {
                    UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value3);
                    lightControl.ENV_MaxValue = value3;
                    
                }
            }
           
            // 更新阈值
            value_change(value1,value2,value3);
            
            json_value = cJSON_GetObjectItem(json, "LED1");
            if(json_value)
            {
                int value = atoi(json_value->valuestring);
                if (value)
                {
                    UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value);
                    LED1_ON();//打开LED1
                    
                }
                else 
                {
                    LED1_OFF();//关闭LED1
                }
            }
            
            json_value = cJSON_GetObjectItem(json, "AutoControl");
            if(json_value)
            {
                int value = atoi(json_value->valuestring);
                if (value)
                {
                    UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value);
                    AutoControl_mode = 1; // 自动模式
                }
             
            }
           
            if (AutoControl_mode == 0)
            {
                //可调灯 LEDlevel
                json_value = cJSON_GetObjectItem(json, "LEDlevel");
                if(json_value)
                {
                    int value = atoi(json_value->valuestring);
                    if (value)
                    {
                        UsartPrintf(USART_DEBUG, "json_value: [%d]\n", value);
                        TIM_SetCompare2(TIM3,value);
                    }
                 
                }
            }
      
            
        }
        cJSON_Delete(json);//删cJSON包
        
        ESP8266_Clear();//清空缓存
        
        //dataPtr = strchr(req_payload, '}');					//搜索'}'

//    if(dataPtr != NULL && result != -1)					//如果找到了
//    {
//        dataPtr++;
//        
//        while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
//        {
//            numBuf[num++] = *dataPtr++;
//        }
//        
//        num = atoi((const char *)numBuf);				//转为数值形式
//        
//    }
}


