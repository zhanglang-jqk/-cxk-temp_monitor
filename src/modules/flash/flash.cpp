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
/**
 * @brief FLASH 写缓冲区
 * STM32 内部 flash 最小操作单位 4byte
 * @param addr STM32 内部 flash 地址
 * @param pBuf 待写入缓冲区
 * @param len 缓冲区长度，长度为 1 时，FLASH 也会写入 4 byte
 */
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
    int i;
    for (i = 0; i < len / 4; i++)
    {
        u32 data = *(u32 *)(pBuf + i * 4);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i * 4, (uint64_t)data);
    }
    if (len % 4 != 0)
    {
        u32 data = 0;
        memcpy(&data, &pBuf[i * 4], len % 4);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i * 4, (uint64_t)data);
    }
    HAL_FLASH_Lock();
}
/**
 * @brief 读取 FLASH 缓冲区
 * STM32 最小读取长度为 4byte
 * @param addr 读取地址
 * @param pBuf 读取缓冲区
 * @param len 读取长度，长度为 1 ,内部 FLASH 也会读取 4byte
 */
void FLASH_ReadBuffer(u32 addr, u8 *pBuf, u16 len)
{
    int i;
    for (i = 0; i < len / 4; i++)
    {
        u32 tmp = *(u32 *)(addr + (i * 4));
        memcpy(&pBuf[i * 4], &tmp, 4);
    }
    if (len % 4 != 0)
    {
        u32 tmp = *(u32 *)(addr + (i * 4));
        memcpy(&pBuf[i * 4], &tmp, len % 4);
    }
}
//flash.cpp