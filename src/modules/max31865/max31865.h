/***********************************************************************
 * @file    max31865.h
 * MAX31865
 * @author	:	ch
 * @brief	:
 * @version:	v1.0
 * @Copyright (C)  2021-02-13  .cdWFVCEL. all right reserved
***********************************************************************/

#ifndef __MAX31865_H_
#define __MAX31865_H_
/* 包含头文件 ------------------------------------------------------------------*/
#include "ch/bsp.h"
#include "modules/max31865/soft_spi.h"
// class SoftSPI;
/* 宏定义 ----------------------------------------------------------------------*/
#define RTD 100.0
#define REF_RESISTOR 430.0
/* 类型定义 --------------------------------------------------------------------*/
class Max31865
{
public:
    typedef enum max31865_numwires
    {
        _2WIRE = 0,
        _3WIRE = 1,
        _4WIRE = 2
    } numwires_t;

    typedef struct
    {
        uint16_t rtd_res_raw; // RTD IC raw resistance register
        uint8_t status;       // RTD status - full status code
        uint8_t conf_reg;     // Configuration register readout
        uint16_t HFT_val;     // High fault threshold register readout
        uint16_t LFT_val;     // Low fault threshold register readout
    } data;

    void begin(uint32_t cs_pin = PIN_SPI_SS, uint32_t clk_pin = PIN_SPI_SCK, uint32_t miso_pin = PIN_SPI_MISO, uint32_t mosi_pin = PIN_SPI_MOSI);
    uint16_t readRTD(void);
    void readRTD_Data(void);
    float getTemperature(float RTDnominal = RTD, float refResistor = REF_RESISTOR);

private:
    uint8_t readFault(void);
    void clearFault(void);
    void enableBias(uint8_t b);
    void setAutoConvert(uint8_t b);
    void setWire(numwires_t wires);
    uint8_t read8bit(uint8_t addr);
    void write8bit(uint8_t addr, uint8_t data);
    uint8_t SPI_ReceiveData(void);
    /*  ---------------------------------------------------------------------------------------------------------------*/
    data _rtd_data;
    SoftSPI _softspi;
};
/* 变量声明 --------------------------------------------------------------------*/
/* 函数声明 --------------------------------------------------------------------*/
#endif // __MAX31865_H_
