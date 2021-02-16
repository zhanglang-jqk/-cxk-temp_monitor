/***********************************************************************
 * @file    quectel_client.h
 * QUECTEL_CLIENT
 * @author	:	ch
 * @brief	:
 * @version:	v1.0
 * @Copyright (C)  2021-02-15  .cdWFVCEL. all right reserved
***********************************************************************/

#ifndef __QUECTEL_CLIENT_H_
#define __QUECTEL_CLIENT_H_
/* 包含头文件 ------------------------------------------------------------------*/
#include "ch/bsp.h"
#include "Client.h"
/* 宏定义 ----------------------------------------------------------------------*/
#define PWR4G_PIN PB0
#define VDD_PIN PA11
#define PWRKEY_PIN PA15

#define SHUTDOWN_TIME (10 * 1000)

#define OPEN_4GPWR() digitalWrite(PWR4G_PIN, HIGH)
#define CLOSE_4GPWR() digitalWrite(PWR4G_PIN, LOW)

#define OPEN_VDD() digitalWrite(VDD_PIN, LOW)
#define CLOSE_VDD() digitalWrite(VDD_PIN, HIGH)

//LTE module's pwrkey pin low > 500ms so start
#define PWRKEY_HIGH() digitalWrite(PWRKEY_PIN, LOW)
#define PWRKEY_LOW() digitalWrite(PWRKEY_PIN, HIGH)
/* 类型定义 --------------------------------------------------------------------*/
class QuectelClient : public Client
{
public:
    int connect(IPAddress ip, uint16_t port);
    int connect(const char *host, uint16_t port);
    size_t write(uint8_t);
    size_t write(const uint8_t *buf, size_t size);
    int avfailable();
    int read();
    int read(uint8_t *buf, size_t size);
    int peek();
    void flush();
    void stop();
    uint8_t connected();
    int available();
    // virtual operator bool() = 0;
private:
    bool _isConnectd = false;
    uint8_t recvBuf[1024] = {0};
    uint16_t recvBufLen = 0;
};
/* 变量声明 --------------------------------------------------------------------*/
/* 函数声明 --------------------------------------------------------------------*/
#endif // __QUECTEL_CLIENT_H_
