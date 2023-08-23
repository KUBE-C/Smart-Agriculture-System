#include "AutoControl.h"
#include "Stepper.h"
#include "LED.h"
#include "Timer.h"
#include "usart.h"
#include "Delay.h"
#include "Lcd.h"


//0为关闭自动模式，1为开启自动模式
uint8_t AutoControl_mode = 0; 
uint8_t led_flag = 0;
uint8_t motorStatus1 = 0, motorStatus2 = 0;


struct TEM_Struct tempControl;//温度结构体
struct ENV_Struct lightControl;//光照强度控制结构体

void Control_Init(void)
{
    //温度初始化
    tempControl.TEM_free = 0;
    tempControl.TEM_FirstMaxValue = 29;
    tempControl.TEM_SecondMaxValue = 32;
    tempControl.TEM_MiniValue = 10;


    //光照强度初始化
    lightControl.ENV_free = 0;
    lightControl.ENV_MaxValue = 50;
    lightControl.ENV_MiniValue = 10;
    
    
}

void Control_Execute(void)
{
    //判断是否开启自动模式
    if(AutoControl_mode == 1)
    {
        //光照强度判断
        if(lightControl.ENV_free == 0)
        {
            ledlevel_change();
            if(lightControl.ENV_value > lightControl.ENV_MaxValue) // 高于阈值
            {      
                if (motorStatus1 == 0)
                {
                   
                    // 自动控制步进电机反转
                    Stepper_RotateByLoop(Reversal,5000,2);
                    motorStatus1 = 1;
                }
                 lightControl.ENV_Status=1; //正常
                // 自动控制可调灯关闭
                led_flag = 0;
            }
            else
            {
                lightControl.ENV_Status = 0;
                if (motorStatus2 == 0)
                {
                  
                    // 自动控制步进电机正传
                    Stepper_RotateByLoop(Forward,5000,2);
                    motorStatus2 = 1;
                }
                lightControl.ENV_Status=0;
                // 自动控制可调灯打开
                led_flag = 1;
            }
            
        }
        else
        {
            lightControl.ENV_free--;
        }
        
        //温度判断
        if(tempControl.TEM_free == 0)
        {
            if(tempControl.TEM_value > tempControl.TEM_FirstMaxValue && tempControl.TEM_value <= tempControl.TEM_SecondMaxValue)
            {
    //				relay1_ON();//打开风扇
    //				Buzzer_ON();//蜂鸣器
                tempControl.TEM_Status=0;//不正常
                UsartPrintf(USART_DEBUG, " First word");
                LED1_ON();

                
            }
            else if(tempControl.TEM_value > tempControl.TEM_SecondMaxValue)
            {
                tempControl.TEM_Status=0;//不正常
                UsartPrintf(USART_DEBUG, " Second word");
                LED1_Turn();

            }
            else
            {
                tempControl.TEM_Status=1;//正常
    //				Buzzer_OFF();
    //				relay1_OFF();//关闭风扇
                LED1_OFF();
            }
        }
        else
        {
            tempControl.TEM_free--;
        }
    }
    else //手动
    {
        manual_control();
        motorStatus1 = 0;
        motorStatus2 = 0;
    }
        
}

void ledlevel_change(void)
{
    if (led_flag == 1)
    {
        if((lightControl.ENV_value>0)&&(lightControl.ENV_value<=10))         //当前环境亮度小于10，灯光亮度调至最高的第五档
        {
            TIM_SetCompare2(TIM3,100);
    //            LEDlevel=100;
        }
        else if((lightControl.ENV_value>10)&&(lightControl.ENV_value<=20))        
        {
            TIM_SetCompare2(TIM3,80);
    //            LEDlevel=80;
        }
       else if((lightControl.ENV_value>20)&&(lightControl.ENV_value<=30))     
        {
            TIM_SetCompare2(TIM3,60);
    //            LEDlevel=60;
        }
        else if ((lightControl.ENV_value > 30) && (lightControl.ENV_value <= 40))
        {        
            TIM_SetCompare2(TIM3, 40);
        }
        else if ((lightControl.ENV_value > 40) && (lightControl.ENV_value <= 50))
        {        
            TIM_SetCompare2(TIM3, 20);
        }
        
        else if (lightControl.ENV_value > 50)
        {        
            TIM_SetCompare2(TIM3, 0);
        }
    }
}

void manual_control(void)
{
     led_flag = 0;
    TIM_SetCompare2(TIM3, 0);
    LED1_OFF();
    if(lightControl.ENV_value > lightControl.ENV_MaxValue) // 高于阈值
        {      
            
            lightControl.ENV_Status=1; //正常
        }
        else
        {
            lightControl.ENV_Status = 0;
        }
    
    //温度判断
        if(tempControl.TEM_value > tempControl.TEM_FirstMaxValue && tempControl.TEM_value < tempControl.TEM_SecondMaxValue)
        {
            tempControl.TEM_Status=0;//不正常
        }
        else if(tempControl.TEM_value > tempControl.TEM_SecondMaxValue)
        {
            tempControl.TEM_Status=0;//不正常
        }
        else
        {
            tempControl.TEM_Status=1;//正常
        }
        
}



