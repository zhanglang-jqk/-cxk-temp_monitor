/***********************************************************************
 * @file    host_server.h
 * HOST_SERVER
 * @author	:	ch
 * @brief	:
 * @version:	v1.0
 * @Copyright (C)  2021-02-18  .cdWFVCEL. all right reserved
***********************************************************************/

#ifndef __HOST_SERVER_H_
#define __HOST_SERVER_H_
/* 包含头文件 ------------------------------------------------------------------*/
#include "ch/bsp.h"
#include <ArduinoJson.h>
/* 宏定义 ----------------------------------------------------------------------*/
/* 类型定义 --------------------------------------------------------------------*/
class HostServerBean
{
public:
    String product_key;
    String device_name;
    String device_secret;
    /*  ---------------------------------------------------------------------------------------------------------------*/
    String alink_body_format;
    String alink_topic_prop_post;
    String alink_topic_prop_postrsp;
    String alink_topic_prop_set;
    String alink_method_prop_post;
    /*  ---------------------------------------------------------------------------------------------------------------*/
    int collect_interval = 0;
    int total_report_count = 0;
    String ip_address;
    int port = 1883;
    int standby_time = 5;
    String tostring();
};

class HostServer
{
public:
    HostServer();
    HostServerBean bean;
    void parseBean(const char *);

private:
    // DynamicJsonBuffer jsonBuffer;
    StaticJsonBuffer<1024> _jsonBuffer;
    // JsonObject _josnObj;
};
/* 变量声明 --------------------------------------------------------------------*/
/* 函数声明 --------------------------------------------------------------------*/
#endif // __HOST_SERVER_H_
