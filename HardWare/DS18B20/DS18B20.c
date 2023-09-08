
#include "Delay.h"
#include "DS18B20.h"


/**
 * @brief       ��λDS18B20
 * @param       data: Ҫд�������
 * @retval      ��
 */
static void ds18b20_reset(void)
{
    DS18B20_DQ_OUT(0);  /* ����DQ,��λ */
    Delay_us(750);      /* ����750us */
    DS18B20_DQ_OUT(1);  /* DQ=1, �ͷŸ�λ */
    Delay_us(15);       /* �ӳ�15US */
}

/**
 * @brief       �ȴ�DS18B20�Ļ�Ӧ
 * @param       ��
 * @retval      0, DS18B20����
 *              1, DS18B20�쳣/������
 */
uint8_t ds18b20_check(void)
{
    uint8_t retry = 0;
    uint8_t rval = 0;

    while (DS18B20_DQ_IN && retry < 200)    /* �ȴ�DQ���, �ȴ�200us */
    {
        retry++;
        Delay_us(1);
    }

    if (retry >= 200)
    {
        rval = 1;
    }
    else
    {
        retry = 0;

        while (!DS18B20_DQ_IN && retry < 240)   /* �ȴ�DQ���, �ȴ�240us */
        {
            retry++;
            Delay_us(1);
        }

        if (retry >= 240) rval = 1;
    }

    return rval;
}

/**
 * @brief       ��DS18B20��ȡһ��λ
 * @param       ��
 * @retval      ��ȡ����λֵ: 0 / 1
 */
static uint8_t ds18b20_read_bit(void)
{
    uint8_t data = 0;
    DS18B20_DQ_OUT(0);
    Delay_us(2);
    DS18B20_DQ_OUT(1);
    Delay_us(12);

    if (DS18B20_DQ_IN)
    {
        data = 1;
    }

    Delay_us(50);
    return data;
}

/**
 * @brief       ��DS18B20��ȡһ���ֽ�
 * @param       ��
 * @retval      ����������
 */
static uint8_t ds18b20_read_byte(void)
{
    uint8_t i, b, data = 0;

    for (i = 0; i < 8; i++)
    {
        b = ds18b20_read_bit(); /* DS18B20�������λ���� ,��λ���ݺ���� */
        
        data |= b << i;         /* ���data��ÿһλ */ 
    }

    return data;
}

/**
 * @brief       дһ���ֽڵ�DS18B20
 * @param       data: Ҫд����ֽ�
 * @retval      ��
 */
static void ds18b20_write_byte(uint8_t data)
{
    uint8_t j;

    for (j = 1; j <= 8; j++)
    {
        if (data & 0x01)
        {
            DS18B20_DQ_OUT(0);  /*  Write 1 */
            Delay_us(2);
            DS18B20_DQ_OUT(1);
            Delay_us(60);
        }
        else
        {
            DS18B20_DQ_OUT(0);  /*  Write 0 */
            Delay_us(60);
            DS18B20_DQ_OUT(1);
            Delay_us(2);
        }

        data >>= 1;             /* ����,��ȡ��һλ���� */
    }
}

/**
 * @brief       ��ʼ�¶�ת��
 * @param       ��
 * @retval      ��
 */
static void ds18b20_start(void)
{
    ds18b20_reset();
    ds18b20_check();
    ds18b20_write_byte(0xcc);   /*  skip rom */
    ds18b20_write_byte(0x44);   /*  convert */
}

/**
 * @brief       ��ʼ��DS18B20��IO�� DQ ͬʱ���DS18B20�Ĵ���
 * @param       ��
 * @retval      0, ����
 *              1, ������/������
 */
uint8_t ds18b20_init(void)
{
    RCC->APB2ENR |= 1 << 8;
    GPIOG->CRH &= 0XFFFF0FFF;
    GPIOG->CRH |= 0X00005000; //PG11 ��©��� 0101
    
    ds18b20_reset();
    
    return ds18b20_check();
}

/**
 * @brief       ��ds18b20�õ��¶�ֵ(���ȣ�0.1C)
 * @param       ��
 * @retval      �¶�ֵ ��-550~1250��
 *   @note      ���ص��¶�ֵ�Ŵ���10��.
 *              ʵ��ʹ�õ�ʱ��,Ҫ����10����ʵ���¶�.
 */
short ds18b20_get_temperature(void)
{
    uint8_t flag = 1;           /* Ĭ���¶�Ϊ���� */
    uint8_t TL, TH;
    short temp;
    
    ds18b20_start();            /*  ds1820 start convert */
    ds18b20_reset();
    ds18b20_check();
    ds18b20_write_byte(0xcc);   /*  skip rom */
    ds18b20_write_byte(0xbe);   /*  convert */
    TL = ds18b20_read_byte();   /*  LSB */
    TH = ds18b20_read_byte();   /*  MSB */

    if (TH > 7)
    {/* �¶�Ϊ�����鿴DS18B20���¶ȱ�ʾ���������洢�������ݵ�ԭ��һ�£�
        ��������Ϊ�Ĵ����洢��������������������Ϊ�Ĵ����洢ֵ��λȡ����+1
        ��������ֱ��ȡ��ʵ�ʵĸ������֣��������Ĳ���Ϊȡ�����һ�������ǵ���λ����+1���н�λ�ʹ������࣬
        ������������ʱû����+1�Ĵ�����������Ҫ���� */
        TH = ~TH;
        TL = ~TL;
        flag = 0;   /* �¶�Ϊ�� */
    }

    temp = TH;      /* ��ø߰�λ */
    temp <<= 8;
    temp += TL;     /* ��õװ�λ */
   
    /* ת����ʵ���¶� */
    if (flag == 0)
    {   /* ���¶�ת���ɸ��¶ȣ������+1�ο�ǰ���˵�� */
        temp = (double)(temp+1) * 0.625;
        temp = -temp;   
    }
    else
    {
        temp = (double)temp * 0.625;
    }
    
    return temp;
}















