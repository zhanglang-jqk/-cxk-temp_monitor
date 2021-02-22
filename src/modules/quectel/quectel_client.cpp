/***********************************************************************
 * @file 	:	quectel_client.cpp
 * @author	:	ch
 * @brief	:
 * @version:	v1.0
 * @Copyright (C)  2021-02-15  .cdWFVCEL. all right reserved
***********************************************************************/

/* 包含头文件 -------------------------------------------------------------------*/
#include "quectel_client.h"
#include "ethernet.h"
/* 宏定义 -----------------------------------------------------------------------*/
/* 类型定义 ---------------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------------------*/
Ethernet eth = Ethernet();

const char apn[10] = "CMNET";
// const char apn[10] = "UNINET";
// const char URL[100] = "116.5.192.226";
// char http_cmd[100] = "GET /media/uploads/mbed_official/hello.txt HTTP/1.0\n\r\n\r";
// int port = 8081;
/* 扩展变量 ---------------------------------------------------------------------*/
/* 私有函数声明 -----------------------------------------------------------------*/
/* 函数声明 ---------------------------------------------------------------------*/
/* 函数实现 ---------------------------------------------------------------------*/

//quectel_client.cpp
int QuectelClient::connect(IPAddress ip, uint16_t port)
{
}

int QuectelClient::connect(const char *host, uint16_t port)
{
    // wio.Power_On();
    pinMode(PWR4G_PIN, OUTPUT), OPEN_4GPWR();
    pinMode(VDD_PIN, OUTPUT), CLOSE_VDD();

    pinMode(PWRKEY_PIN, OUTPUT);
    PWRKEY_LOW(), delay(800), PWRKEY_HIGH();
    // eth.Power_On();
    while (false == eth.Check_If_Power_On())
    {
        Serial.println("Waitting for module to alvie...");
        delay(1000);
    }

    while (!eth.init())
    {
        delay(1000);
        Serial.println("Accessing network...");
    }
    Serial.println("Initialize done...");

    eth.join(apn);
    Serial.print("\n\rIP: ");
    Serial.print(eth.ip_string);

    // if (eth.connect(host, port, TCP))
    if (eth.connect("101.133.196.109", port, TCP))
    // if (eth.connect("116.5.194.71", 8081, TCP))
    {
        Serial.println("Connect OK!");
        Serial.flush();
        _isConnectd = true;
    }
    else
    {
        Serial.println("Connect Error!");
        _isConnectd = false;
    }
    return _isConnectd;
}

size_t QuectelClient::write(uint8_t)
{
    return 0;
}

size_t QuectelClient::write(const uint8_t *buf, size_t size)
{
    char cmd[32];
    // int len = strlen(data);
    snprintf(cmd, sizeof(cmd), "AT+QISEND=0,%d\r\n", size);
    if (!check_with_cmd(cmd, ">", DATA, 2 * DEFAULT_TIMEOUT))
    {
        ERROR("ERROR:QISEND\r\n"
              "Data length: ");
        ERROR(size);
        return false;
    }

    for (int i = 0; i < size; i++)
        send_byte(buf[i]);
    return 0;
}

int QuectelClient::avfailable()
{
    return 0;
}

int QuectelClient::read()
{
    uint8_t ret = 0;
    if (recvBufLen > 0)
    {
        ret = recvBuf[0];
        for (int i = 0; i < recvBufLen - 1; i++)
        {
            recvBuf[i] = recvBuf[i + 1];
        }
        recvBufLen--;
    }
    return ret;
}

int QuectelClient::read(uint8_t *buf, size_t size)
{
    // if (recvBufLen < size)
    // {
    //     return 0;
    // }
    // else
    // {
    //     memcpy(buf, recvBuf, size);
    //     for (int i = 0, j = size; i < recvBufLen - size; i++, j++)
    //     {
    //         recvBuf[i] = recvBuf[j];
    //     }
    //     memcpy((char *)recvBuf[recvBufLen - size], 0, recvBufLen - size);
    //     recvBufLen = recvBufLen - size;
    // }
    return 0;
}

int QuectelClient::peek()
{
    return 0;
}

void QuectelClient::flush()
{
}

void QuectelClient::stop()
{
}

uint8_t QuectelClient::connected()
{
    return _isConnectd;
}

int QuectelClient::available()
{
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+QIRD=0,1500\r\n");
    send_cmd(cmd);
    char tmpBuf[256] = {0};
    if (read_string_until((char *)tmpBuf, 256, "OK"))
    {
        uint16_t recvLen = 0;
        sscanf(tmpBuf, "%*[^A]AT+QIRD=0,1500\r\r\n+QIRD: %d\r\n\r\nOK", &recvLen);
        if (recvLen > 0)
        {
            uint16_t pos = 0;
            for (int i = 0;; i++)
            {
                if (tmpBuf[i] == 'O' && tmpBuf[i + 1] == 'K')
                {
                    pos = i - 4 - recvLen;
                    memcpy(&recvBuf[recvBufLen], &tmpBuf[pos], recvLen);
                    recvBufLen += recvLen;
                    break;
                }
            }
        }
    }
    else if (strcmp((char *)tmpBuf, "+QIURC: \"closed\"\r\n") == 0)
    {
        ERROR("ERROR:QIRD");
    }
    return recvBufLen;
}
//