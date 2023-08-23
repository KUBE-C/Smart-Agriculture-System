//单片机头文件
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//硬件驱动

#include "Timer.h"
#include "key.h"
#include "Led.h"
#include "usart.h"
#include "DHT11.h"
#include "Lsens.h"
#include "AD.h"
#include "Lcd.h"
#include "AutoControl.h"
#include "Stepper.h"

//网络设备
#include "esp8266.h"

//C库
#include <string.h>
#include <stdio.h> 



uint16_t humidityH,humidityL,temperatureH,temperatureL,Light,LED1_Status,LED2_Status,Buzz_Status,soilHumidity,Servo,Fan,WaterPump;
char PUB_BUF[256];//上传数据的拼接字符串
unsigned char *dataPtr = NULL;

int main(void)
{
    uint8_t key;
    
    //MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};
    
    Key_Init();
    Usart1_Init(115200);
    LED_Init();
    TIM3_PWM_Init(100,0);//ARR为100，CCR为0
    //TIM2_Int_Init(); 
    //TIM4_Int_Init(); //1ms中断：1s传数据
    lsens_init();
    Control_Init();
    lcd_init();
    Stepper_GPIOInit();
    uart3_init(115200);
    ESP8266_Init();					//初始化ESP8266

    
    UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
    
    
    lcd_show_string(30, 20, 200, 16, 16, "Humidity: 00 %", BLUE);
    lcd_show_string(30+18*8, 20, 200, 16, 16, "normal", BLUE);
    lcd_show_string(30, 40, 200, 16, 16, "Temparate: 00 C", BLUE);
    
    lcd_show_string(30, 60, 200, 16, 16, "Light: ", BLUE);
    
    lcd_show_string(30, 80, 200, 16, 16, "Mode: ", BLUE);
    lcd_show_string(30 + 6 * 8, 80, 200, 16, 16,"Manual mode",BLUE);
    //lcd_show_string(30, 100, 200, 16, 16,"Fault State: ",BLUE);

    while(1)
    {
        //按键 手动
        key = Key_GetNum();
        Key_Execute(key);
        
        //光照强度
        Light = lsens_get_val();

        //温湿度
        if(DHT_Get_Temp_Humi_Data(DHT_Buffer))
        {
            humidityH = DHT_Buffer[0];
            //humidityL = DHT_Buffer[1];
            temperatureH = DHT_Buffer[2];
            //temperatureL = DHT_Buffer[3];
        }
      
        
        lcd_show_xnum(30 + 10 * 8, 20, humidityH, 2, 16, 0, BLUE);    /* 显示整数部分 */
        //lcd_show_xnum(30 + 13 * 8, 20, humidityL, 2, 16, 0X80, BLUE); /* 显示小数部分 */
        
        lcd_show_xnum(30 + 11 * 8, 40, temperatureH, 2, 16, 0, BLUE);    /* 显示整数部分 */
        //lcd_show_xnum(30 + 14 * 8, 40, temperatureL, 2, 16, 0X80, BLUE); /* 显示小数部分 */
        lcd_show_status(30+18*8, 40,tempControl.TEM_Status);
            
        lcd_show_xnum(30 + 7 * 8, 60, Light, 2, 16, 0, BLUE);   
        lcd_show_status(30+18*8, 60,lightControl.ENV_Status);
        
        
        //自动控制数据赋值
        tempControl.TEM_value = temperatureH;
        lightControl.ENV_value = Light;
       
        Control_Execute();
            
            sprintf(PUB_BUF,"{\"Hum\":\"%d\",\"Temp\":\"%d\",\"Light\":\"%d\",\"AutoControl\":\"%d\",\"TEM_FirstMaxValue\":\"%d\",\"TEM_SecondMaxValue\":\"%d\",\"LIGHT_MaxValue\":\"%d\",\"TEM_Status\":\"%d\",\"Light_Status\":\"%d\"}"
        ,humidityH,temperatureH,Light,AutoControl_mode,tempControl.TEM_FirstMaxValue,tempControl.TEM_SecondMaxValue,lightControl.ENV_MaxValue,tempControl.TEM_Status,lightControl.ENV_Status);

        //UsartPrintf(USART_DEBUG,PUB_BUF);
        
        // 数据发送 1s
        if(timed8266[0] > timed8266[1])
		{
            ESP8266_SendData((unsigned char *)PUB_BUF, (unsigned short)strlen(PUB_BUF));
            timed8266[0] = 0;
            ESP8266_Clear();
        }
        
        //接收数据
        dataPtr = (unsigned char *)ESP8266_GetIPD(3);
        if(dataPtr != NULL)
		{
			UsartPrintf(USART_DEBUG,(char *)dataPtr);
            ESP8266_Clear();
            if(strcmp((char *)dataPtr,"0")==0)
            {
                UsartPrintf(USART_DEBUG,"OK");
                LED0_Turn();
            }
            else
            {
                LED1_Turn();
            }
		}
        
    }
} 










