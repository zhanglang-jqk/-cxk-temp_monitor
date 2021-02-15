/***********************************************************************
 * @file 	:	max31865.cpp
 * @author	:	ch
 * @brief	:
 * @version:	v1.0
 * @Copyright (C)  2021-02-13  .cdWFVCEL. all right reserved
***********************************************************************/

/* ����ͷ�ļ� -------------------------------------------------------------------*/
#include "max31865.h"
/* �궨�� -----------------------------------------------------------------------*/
#define MAX31856_CONFIG_REG 0x00
#define MAX31856_CONFIG_BIAS 0x80
#define MAX31856_CONFIG_MODEAUTO 0x40
#define MAX31856_CONFIG_MODEOFF 0x00
#define MAX31856_CONFIG_1SHOT 0x20
#define MAX31856_CONFIG_3WIRE 0x10
#define MAX31856_CONFIG_24WIRE 0x00
#define MAX31856_CONFIG_FAULTSTAT 0x02
#define MAX31856_CONFIG_FILT50HZ 0x01
#define MAX31856_CONFIG_FILT60HZ 0x00

#define MAX31856_RTDMSB_REG 0x01
#define MAX31856_RTDLSB_REG 0x02
#define MAX31856_HFAULTMSB_REG 0x03
#define MAX31856_HFAULTLSB_REG 0x04
#define MAX31856_LFAULTMSB_REG 0x05
#define MAX31856_LFAULTLSB_REG 0x06
#define MAX31856_FAULTSTAT_REG 0x07

#define MAX31865_FAULT_HIGHTHRESH 0x80
#define MAX31865_FAULT_LOWTHRESH 0x40
#define MAX31865_FAULT_REFINLOW 0x20
#define MAX31865_FAULT_REFINHIGH 0x10
#define MAX31865_FAULT_RTDINLOW 0x08
#define MAX31865_FAULT_OVUV 0x04

#define RTD_A 3.9083e-3
#define RTD_B -5.775e-7
/* ���Ͷ��� ---------------------------------------------------------------------*/
/* ˽�б��� ---------------------------------------------------------------------*/
/* ��չ���� ---------------------------------------------------------------------*/
/* ˽�к������� -----------------------------------------------------------------*/
/* �������� ---------------------------------------------------------------------*/
/* ����ʵ�� ---------------------------------------------------------------------*/

//max31865.cpp

//������
uint8_t Max31865::readFault(void)
{
    return read8bit(MAX31856_FAULTSTAT_REG);
}

//�������
void Max31865::clearFault(void)
{
    uint8_t reg = read8bit(MAX31856_CONFIG_REG);
    reg &= ~0x2c;
    reg |= MAX31856_CONFIG_FAULTSTAT;
    write8bit(MAX31856_CONFIG_REG, reg);
}

//����ת��F
void Max31865::enableBias(uint8_t b)
{
    uint8_t reg = read8bit(MAX31856_CONFIG_REG);
    if (b)
    {
        reg |= MAX31856_CONFIG_BIAS;
    }
    else
    {
        reg &= ~MAX31856_CONFIG_BIAS;
    }
    write8bit(MAX31856_CONFIG_REG, reg);
}

//�����Զ�ת��
void Max31865::setAutoConvert(uint8_t b)
{
    uint8_t reg = read8bit(MAX31856_CONFIG_REG);
    if (b)
    {
        reg |= MAX31856_CONFIG_MODEAUTO;
    }
    else
    {
        reg &= ~MAX31856_CONFIG_MODEAUTO;
    }
    write8bit(MAX31856_CONFIG_REG, reg);
}

//����RTD��
void Max31865::setWire(numwires_t wires)
{
    uint8_t reg = read8bit(MAX31856_CONFIG_REG);
    if (wires == _3WIRE)
    {
        reg |= MAX31856_CONFIG_3WIRE;
    }
    else
    {
        reg &= ~MAX31856_CONFIG_3WIRE;
    }
    write8bit(MAX31856_CONFIG_REG, reg);
}

//��ȡRTD
uint16_t Max31865::readRTD(void)
{
    uint16_t res =
        {0};
    uint8_t reg = 0;
    clearFault();
    enableBias(1);
    delay_ms(10);
    reg = read8bit(MAX31856_CONFIG_REG);
    reg |= MAX31856_CONFIG_1SHOT;
    write8bit(MAX31856_CONFIG_REG, reg);
    delay_ms(65);
    res = read8bit(MAX31856_RTDMSB_REG);
    res <<= 8;
    res += read8bit(MAX31856_RTDLSB_REG);
    return res >> 1;
}
//��ȡ��������
void Max31865::readRTD_Data(void)
{
    uint8_t read_addr = 0x00;
    uint8_t read_data[8] =
        {0};
    uint8_t i;
    _softspi.CS_Low();
    delay_ms(10);
    _softspi.SPI_ReadWrite(read_addr);
    for (i = 0; i < 8; i++)
    {
        read_data[i] = _softspi.SPI_ReadWrite(i);
    }
    _softspi.CS_High();

    _rtd_data.conf_reg = read_data[0];                                 //Store config reg data
    _rtd_data.rtd_res_raw = ((read_data[1] << 8) | read_data[2]) >> 1; // Store rtd_res_raw
    _rtd_data.HFT_val = ((read_data[3] << 8) | read_data[4]) >> 1;     // Store HFT_val
    _rtd_data.LFT_val = (read_data[5] << 8) | read_data[6];            // Store LFT_val
    _rtd_data.status = read_data[7];                                   //Store fault status reg data
}

//��ȡ�¶�ֵ
float Max31865::getTemperature(float RTDnominal, float refResistor)
{
    float Z1, Z2, Z3, Z4, Rt, temp;
    float rpoly = 0;
    Rt = readRTD();
    Rt /= 32768;
    Rt *= refResistor;

    //Serial.print("Resistance: "); Serial.println(Rt, 8);

    Z1 = -RTD_A;
    Z2 = RTD_A * RTD_A - (4 * RTD_B);
    Z3 = (4 * RTD_B) / RTDnominal;
    Z4 = 2 * RTD_B;

    temp = Z2 + (Z3 * Rt);
    temp = (sqrt(temp) + Z1) / Z4;

    if (temp >= 0)
        return temp;

    // ugh.
    rpoly = Rt;

    temp = -242.02;
    temp += 2.2228 * rpoly;
    rpoly *= Rt; // square
    temp += 2.5859e-3 * rpoly;
    rpoly *= Rt; // ^3
    temp -= 4.8260e-6 * rpoly;
    rpoly *= Rt; // ^4
    temp -= 2.8183e-8 * rpoly;
    rpoly *= Rt; // ^5
    temp += 1.5243e-10 * rpoly;

    return temp;
}

//��8λ�Ĵ���
uint8_t Max31865::read8bit(uint8_t addr)
{
    uint8_t res = 0;

    _softspi.CS_Low();

    _softspi.SPI_ReadWrite(addr);
    ;
    res = SPI_ReceiveData();
    //	delay_ms(10);
    _softspi.CS_High();
    return res;
}

//����һ���ֽ�
uint8_t Max31865::SPI_ReceiveData(void)
{
    //	return SPI2_ReadWriteByte(0xFF);
    return _softspi.SPI_ReadWrite(0xff);
}
//д8λ�Ĵ���
void Max31865::write8bit(uint8_t addr, uint8_t data)
{
    _softspi.CS_Low();

    addr |= 0x80;

    //	SPI2_ReadWriteByte(addr);
    //
    //	SPI2_ReadWriteByte(data);
    _softspi.SPI_ReadWrite(addr);
    _softspi.SPI_ReadWrite(data);

    _softspi.CS_High();
}

void Max31865::begin(uint32_t cs_pin, uint32_t clk_pin, uint32_t miso_pin, uint32_t mosi_pin)
{
    _softspi.begin(cs_pin, clk_pin, miso_pin, mosi_pin);
    write8bit(0x00, 0xc2);
}
