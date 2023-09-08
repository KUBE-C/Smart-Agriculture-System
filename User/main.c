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
#include "Data.h"
#include "Stmflash.h"
#include "Rtc.h"
#include "Sr501.h"
#include "Norflash.h"
#include "DS18B20.h"
#include "Rc522.h"

//网络设备
#include "esp8266.h"

//C库
#include <string.h>
#include <stdio.h> 



uint16_t humidityH,humidityL,temperatureH,temperatureL,Light;
char PUB_BUF[256];//上传数据的拼接字符串
#define flashsize sizeof(PUB_BUF)
char flashtemp[flashsize];

int main(void)
{
    uint8_t key;
    char time[256];
    uint16_t id = 0;
    int i = 0,j = 0, t = 0;
    uint8_t ucStatusReturn;    //返回状态  
    u8 ucArray_ID [ 4 ]; // 卡的ID存储，32位，4字节
    char cStr [ 30 ];
    
    Key_Init();
    Usart1_Init(115200);
    LED_Init();
    TIM3_PWM_Init(100,0);//ARR为100，CCR为0
    //TIM2_Int_Init(); 
    TIM4_Int_Init(); //1ms中断：1s传数据
    lsens_init();
    Control_Init();
    lcd_init();
    Stepper_GPIOInit();
    uart3_init(115200);
    //Usart2_Init(9600);
    RTC_Init();
    //Sr501_Init();//人体红外感应
    Norflash_Init();
    //RC522_Init(); // RFID
    ESP8266_Init();					//初始化ESP8266

    
    UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
    
    id = Norflash_read_id();    /* 读取FLASH ID */
    UsartPrintf(USART_DEBUG," DEVICEID:0x%X",id);
    
    lcd_show_string(30, 20, 200, 16, 16, "Humidity: 00 %", BLUE);
    lcd_show_string(30+18*8, 20, 200, 16, 16, "normal", BLUE);
    lcd_show_string(30, 40, 200, 16, 16, "Temparate: 00 C", BLUE);
    
    lcd_show_string(30, 60, 200, 16, 16, "Light: ", BLUE);
    
    lcd_show_string(30, 80, 200, 16, 16, "Mode: ", BLUE);
    lcd_show_string(30 + 6 * 8, 80, 200, 16, 16,"Manual mode",BLUE);
    //lcd_show_string(30, 100, 200, 16, 16,"Fault State: ",BLUE);
    
    lcd_show_string(30, 120, 200, 16, 16,"Date:xxxx-xx-xx",BLUE);
    lcd_show_string(30, 140, 200, 16, 16,"Time:xx:xx:xx",BLUE);
    
//    while(ds18b20_init())	//DS18B20初始化	
//	{
//		lcd_show_string(30,250,200,16,16,"DS18B20 Error",RED);
//		
//	}								   
//	lcd_show_string(30,270,200,16,16,"DS18B20 OK", BLUE);
     
    

    while ((id == 0) || (id == 0XFFFF)) /* 检测不到FLASH芯片 */
    {
        lcd_show_string(30, 200, 200, 16, 16, "FLASH Check Failed!", RED);
    }

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
        
        lcd_show_xnum(30 , 100, tempControl.TEM_FirstMaxValue, 2, 16, 0, BLUE);   // 阈值
        lcd_show_xnum(30+3*8 , 100, tempControl.TEM_SecondMaxValue, 2, 16, 0, BLUE);   // 阈值
        lcd_show_xnum(30+6*8 , 100, lightControl.ENV_MaxValue, 2, 16, 0, BLUE);   // 阈值
        
        // 实时时钟
        RTC_ReadTime();
		
		lcd_show_xnum(30+5*8, 120,RTC_Time[0],4,16,0,BLUE);
		lcd_show_xnum(30+10*8, 120,RTC_Time[1],2,16,0,BLUE);
		lcd_show_xnum(30+13*8, 120,RTC_Time[2],2,16,0,BLUE);
        
		lcd_show_xnum(30+5*8, 140,RTC_Time[3],2,16,0,BLUE);
		lcd_show_xnum(30+8*8, 140,RTC_Time[4],2,16,0,BLUE);
		lcd_show_xnum(30+11*8, 140,RTC_Time[5],2,16,0,BLUE);
        
        // RFID
//        if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )
//			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		   

//		if ( ucStatusReturn == MI_OK  )
//		{
//			/* 防冲突操作，被选中的卡片序列传入数组ucArray_ID中 */
//			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )
//			{
//				sprintf ( cStr, "The Card ID is: %02X%02X%02X%02X", ucArray_ID [ 0 ], ucArray_ID [ 1 ], ucArray_ID [ 2 ], ucArray_ID [ 3 ] );				
//				UsartPrintf(USART_DEBUG," %s\r\n",cStr ); 
//			}
//		}

        
        sprintf(time,"%d-%d-%d %d:%d:%d",RTC_Time[0],RTC_Time[1],RTC_Time[2],RTC_Time[3],RTC_Time[4],RTC_Time[5]);
        
        
        //自动控制数据赋值
        tempControl.TEM_value = temperatureH;
        lightControl.ENV_value = Light;
       
        Control_Execute();
        
        Data_Recieve(); // 接收数据
        
        sprintf(PUB_BUF,"{\"Hum\":\"%d\",\"Temp\":\"%d\",\"TheLight\":\"%d\",\"AutoControl\":\"%d\",\"TEM1\":\"%d\",\"TEM2\":\"%d\",\"LIGHT\":\"%d\",\"TEM_Status\":\"%d\",\"Light_Status\":\"%d\",\"Time\":\"%s\"}"
         ,humidityH,temperatureH,Light,AutoControl_mode,tempControl.TEM_FirstMaxValue,tempControl.TEM_SecondMaxValue,lightControl.ENV_MaxValue,tempControl.TEM_Status,lightControl.ENV_Status,time);

      
        if(!ESP8266_SendCmd("AT+CIPSTATUS\r\n", "STATUS:3")) //与服务器连接
        {
//            UsartPrintf(USART_DEBUG,"  11connect\r\n");
           if(timed8266[0] > timed8266[1]) // 2s检查
            {
                UsartPrintf(USART_DEBUG,"  send ok\r\n");
                // 实时数据
                ESP8266_SendData((unsigned char *)PUB_BUF, (unsigned short)strlen(PUB_BUF));
                //UsartPrintf(USART_DEBUG,PUB_BUF);
                timed8266[0] = 0;
            }
            //norflash数据
//            UsartPrintf(USART_DEBUG," j:%d",j);
//            UsartPrintf(USART_DEBUG," j:%d",i);
            if(timeflash[0] > timeflash[1]) // 1.5s检查
            {
               if(j <= i && i != 0)
               {
                    Norflash_Read(flashsize * j,(u8 *)flashtemp ,flashsize);
                    ESP8266_SendData((unsigned char *)flashtemp, (unsigned short)strlen(flashtemp));
                    //UsartPrintf(USART_DEBUG," norflash is %s\r\n",(char *)flashtemp);
                   UsartPrintf(USART_DEBUG," send norflash ok");
                    j++;
                   
               }
               timeflash[0]= 0;
           }
         
            
        }
         else   //if(!ESP8266_SendCmd("AT+CIPSTATUS\r\n", "STATUS:4"))  //与服务器断开连接
        {
            //UsartPrintf(USART_DEBUG," don't connect\r\n");
            
            //还要继续尝试连接
            if(!ESP8266_SendCmd(ESP8266_TCP1_INFO, "OK"))
            {
                 UsartPrintf(USART_DEBUG,"  connect\r\n");
            
            }
            else
              {
                                        
                    Norflash_Write(flashsize * i, (u8*)(PUB_BUF),flashsize);
                    UsartPrintf(USART_DEBUG," norflash ok");
                  
                    i++;
              }
              
              //读norflash
              for(t = 0; t<i; t++)
              {
                 Norflash_Read(flashsize * t,(u8 *)flashtemp ,flashsize);
                    
                  UsartPrintf(USART_DEBUG," norflash is %s\r\n",(char *)flashtemp);
              }

        }
    
        
        
        
    }
} 










