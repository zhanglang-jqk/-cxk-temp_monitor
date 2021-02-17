/***********************************************************************
 * @file 	:	flash.cpp
 * @author	:	ch
 * @brief	:	
 * @version:	v1.0
 * @Copyright (C)  2020-12-03  .cdWFVCEL. all right reserved
***********************************************************************/

/* ����ͷ�ļ� -------------------------------------------------------------------*/
#include "flash.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"

/* �궨�� -----------------------------------------------------------------------*/
/* ���Ͷ��� ---------------------------------------------------------------------*/
/* ˽�б��� ---------------------------------------------------------------------*/
/* ��չ���� ---------------------------------------------------------------------*/
/* ˽�к������� -----------------------------------------------------------------*/
/* �������� ---------------------------------------------------------------------*/
/* ����ʵ�� ---------------------------------------------------------------------*/
void FLASH_Init()
{
}
/**
 * @brief FLASH д������
 * STM32 �ڲ� flash ��С������λ 4byte
 * @param addr STM32 �ڲ� flash ��ַ
 * @param pBuf ��д�뻺����
 * @param len ���������ȣ�����Ϊ 1 ʱ��FLASH Ҳ��д�� 4 byte
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
 * @brief ��ȡ FLASH ������
 * STM32 ��С��ȡ����Ϊ 4byte
 * @param addr ��ȡ��ַ
 * @param pBuf ��ȡ������
 * @param len ��ȡ���ȣ�����Ϊ 1 ,�ڲ� FLASH Ҳ���ȡ 4byte
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