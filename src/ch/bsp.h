/***********************************************************************
 * @file        bsp.h
 * @author	:	ch
 * @brief	:
 * @version:	v1.0
 * @Copyright (C)  2021-02-13  .cdWFVCEL. all right reserved
***********************************************************************/

#ifndef __BSP_H_
#define __BSP_H_
/* 包含头文件 ------------------------------------------------------------------*/
#ifdef ARDUINO
#include <Arduino.h>
#include <Wire.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/* 宏定义 ----------------------------------------------------------------------*/
#define CUBEMX

#ifdef CUBEMX
#include "main.h"
#endif

#ifdef ARDUINO
#define delay_ms delay
#define delay_us delayMicroseconds
#endif

    /* 类型定义 --------------------------------------------------------------------*/
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef int8_t s8;
    typedef int16_t s16;
    typedef int32_t s32;
/* 变量声明 --------------------------------------------------------------------*/
/* 函数声明 --------------------------------------------------------------------*/
#ifdef CUBEMX
    void MX_GPIO_Init(void);
#endif

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
#endif // __BSP_H_
