#include "AutoControl.h"
#include "Stepper.h"
#include "LED.h"
#include "Timer.h"
#include "usart.h"
#include "Delay.h"
#include "Lcd.h"


//0Ϊ�ر��Զ�ģʽ��1Ϊ�����Զ�ģʽ
uint8_t AutoControl_mode = 0; 
uint8_t led_flag = 0;
uint8_t motorStatus1 = 0, motorStatus2 = 0;


struct TEM_Struct tempControl;//�¶Ƚṹ��
struct ENV_Struct lightControl;//����ǿ�ȿ��ƽṹ��

void Control_Init(void)
{
    //�¶ȳ�ʼ��
    tempControl.TEM_free = 0;
    tempControl.TEM_FirstMaxValue = 29;
    tempControl.TEM_SecondMaxValue = 32;
    tempControl.TEM_MiniValue = 10;


    //����ǿ�ȳ�ʼ��
    lightControl.ENV_free = 0;
    lightControl.ENV_MaxValue = 50;
    lightControl.ENV_MiniValue = 10;
    
    
}

void Control_Execute(void)
{
    //�ж��Ƿ����Զ�ģʽ
    if(AutoControl_mode == 1)
    {
        //����ǿ���ж�
        if(lightControl.ENV_free == 0)
        {
            ledlevel_change();
            if(lightControl.ENV_value > lightControl.ENV_MaxValue) // ������ֵ
            {      
                if (motorStatus1 == 0)
                {
                   
                    // �Զ����Ʋ��������ת
                    Stepper_RotateByLoop(Reversal,5000,2);
                    motorStatus1 = 1;
                }
                 lightControl.ENV_Status=1; //����
                // �Զ����ƿɵ��ƹر�
                led_flag = 0;
            }
            else
            {
                lightControl.ENV_Status = 0;
                if (motorStatus2 == 0)
                {
                  
                    // �Զ����Ʋ����������
                    Stepper_RotateByLoop(Forward,5000,2);
                    motorStatus2 = 1;
                }
                lightControl.ENV_Status=0;
                // �Զ����ƿɵ��ƴ�
                led_flag = 1;
            }
            
        }
        else
        {
            lightControl.ENV_free--;
        }
        
        //�¶��ж�
        if(tempControl.TEM_free == 0)
        {
            if(tempControl.TEM_value > tempControl.TEM_FirstMaxValue && tempControl.TEM_value <= tempControl.TEM_SecondMaxValue)
            {
    //				relay1_ON();//�򿪷���
    //				Buzzer_ON();//������
                tempControl.TEM_Status=0;//������
                UsartPrintf(USART_DEBUG, " First word");
                LED1_ON();

                
            }
            else if(tempControl.TEM_value > tempControl.TEM_SecondMaxValue)
            {
                tempControl.TEM_Status=0;//������
                UsartPrintf(USART_DEBUG, " Second word");
                LED1_Turn();

            }
            else
            {
                tempControl.TEM_Status=1;//����
    //				Buzzer_OFF();
    //				relay1_OFF();//�رշ���
                LED1_OFF();
            }
        }
        else
        {
            tempControl.TEM_free--;
        }
    }
    else //�ֶ�
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
        if((lightControl.ENV_value>0)&&(lightControl.ENV_value<=10))         //��ǰ��������С��10���ƹ����ȵ�����ߵĵ��嵵
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
    if(lightControl.ENV_value > lightControl.ENV_MaxValue) // ������ֵ
        {      
            
            lightControl.ENV_Status=1; //����
        }
        else
        {
            lightControl.ENV_Status = 0;
        }
    
    //�¶��ж�
        if(tempControl.TEM_value > tempControl.TEM_FirstMaxValue && tempControl.TEM_value < tempControl.TEM_SecondMaxValue)
        {
            tempControl.TEM_Status=0;//������
        }
        else if(tempControl.TEM_value > tempControl.TEM_SecondMaxValue)
        {
            tempControl.TEM_Status=0;//������
        }
        else
        {
            tempControl.TEM_Status=1;//����
        }
        
}


