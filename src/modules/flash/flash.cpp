/***********************************************************************
 * @file 	:	flash.cpp
 * @author	:	ch
 * @brief	:	
 * @version:	v1.0
 * @Copyright (C)  2020-12-03  .cdWFVCEL. all right reserved
***********************************************************************/

/* 包含头文件 -------------------------------------------------------------------*/
#include "flash.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"

/* 宏定义 -----------------------------------------------------------------------*/
/* 类型定义 ---------------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------------------*/
/* 扩展变量 ---------------------------------------------------------------------*/
/* 私有函数声明 -----------------------------------------------------------------*/
/* 函数声明 ---------------------------------------------------------------------*/
/* 函数实现 ---------------------------------------------------------------------*/
void FLASH_Init()
{
}
// void    FLASH_PageErase(uint32_t PageAddress);
void FLASH_WriteBuffer(u32 addr, u8 *pBuf, u16 len)
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef eraseInit;
    eraseInit.NbPages = 1;
    eraseInit.PageAddress = addr;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.Banks = FLASH_BANK_1;
    u32 err = 0;
    HAL_FLASHEx_Erase(&eraseInit, &err);

    for (int i = 0; i < len / 4; i++)
    {
        u32 data = *(u32 *)(pBuf + i * 4);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i * 4, (uint64_t)data);
    }
    HAL_FLASH_Lock();
}
void FLASH_ReadBuffer(u32 addr, u8 *pBuf, u16 len)
{
    for (int i = 0; i < len / 4; i++)
    {
        u32 tmp = *(u32 *)(addr + (i * 4));
        memcpy(&pBuf[i * 4], &tmp, 4);
    }
}
//flash.cpp