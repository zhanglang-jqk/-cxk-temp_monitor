/***********************************************************************
 * @file    stm32_flash.h
 * STM32_FLASH
 * @author	:	ch
 * @brief	:
 * @version:	v1.0
 * @Copyright (C)  2021-02-17  .cdWFVCEL. all right reserved
***********************************************************************/

#ifndef __STM32_FLASH_H_
#define __STM32_FLASH_H_
/* ����ͷ�ļ� ------------------------------------------------------------------*/
#include "ch/bsp.h"
#include "modules/flash/flash.h"
/* �궨�� ----------------------------------------------------------------------*/
#define PROGMEM_ADDR FLASH_START_ADDRESS
/* ���Ͷ��� --------------------------------------------------------------------*/
class STM32Flash
{
public:
    void begin(uint32_t startAddr = PROGMEM_ADDR);
    void writeBuffer(uint8_t *pbuf, uint16_t len, uint32_t addr = PROGMEM_ADDR);
    void readBuffer(uint8_t *pbuf, uint16_t len, uint32_t addr = PROGMEM_ADDR);
private:
    uint32_t _startAddr = PROGMEM_ADDR;
};
/* �������� --------------------------------------------------------------------*/
/* �������� --------------------------------------------------------------------*/
#endif // __STM32_FLASH_H_
