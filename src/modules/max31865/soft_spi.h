/***********************************************************************
 * @file    soft_spi.h
 * SOFT_SPI
 * @author	:	ch
 * @brief	:
 * @version:	v1.0
 * @Copyright (C)  2021-02-13  .cdWFVCEL. all right reserved
***********************************************************************/

#ifndef __SOFT_SPI_H_
#define __SOFT_SPI_H_
/* 包含头文件 ------------------------------------------------------------------*/
#include "ch/bsp.h"
/* 宏定义 ----------------------------------------------------------------------*/
/* 类型定义 --------------------------------------------------------------------*/
class SoftSPI
{
public:
    uint8_t SPI_ReadWrite(uint8_t byte);

    void begin(uint32_t cs_pin = PIN_SPI_SS, uint32_t clk_pin = PIN_SPI_SCK, uint32_t miso_pin = PIN_SPI_MISO, uint32_t mosi_pin = PIN_SPI_MOSI);
    inline void setCLK_Pin(uint32_t clk_pin) { _clk_pin = clk_pin; };
    inline void setCS_Pin(uint32_t cs_pin) { _cs_pin = cs_pin; }
    inline void setMISO_Pin(uint32_t miso_pin) { _miso_pin = miso_pin; };
    inline void setMOSI_Pin(uint32_t mosi_pin) { _mosi_pin = mosi_pin; };

    inline void CS_High() { digitalWrite(_cs_pin, HIGH); };
    inline void CS_Low() { digitalWrite(_cs_pin, LOW); };
    inline void CLK_High() { digitalWrite(_clk_pin, HIGH); };
    inline void CLK_Low() { digitalWrite(_clk_pin, LOW); };
    inline void MOSI_High() { digitalWrite(_mosi_pin, HIGH); };
    inline void MOSI_Low() { digitalWrite(_mosi_pin, LOW); };
    inline uint8_t MISO_Read() { return digitalRead(_miso_pin); };

private:
    uint32_t _clk_pin;
    uint32_t _miso_pin;
    uint32_t _mosi_pin;
    uint32_t _cs_pin;
};
/* 变量声明 --------------------------------------------------------------------*/
/* 函数声明 --------------------------------------------------------------------*/
#endif // __SOFT_SPI_H_
