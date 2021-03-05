/***********************************************************************
 * @file 	:	host_server.cpp
 * @author	:	ch
 * @brief	:
 * @version:	v1.0
 * @Copyright (C)  2021-02-18  .cdWFVCEL. all right reserved
***********************************************************************/

/* 包含头文件 -------------------------------------------------------------------*/
#include "host_server.h"
/* 宏定义 -----------------------------------------------------------------------*/
/* 类型定义 ---------------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------------------*/
/* 扩展变量 ---------------------------------------------------------------------*/
/* 私有函数声明 -----------------------------------------------------------------*/
/* 函数声明 ---------------------------------------------------------------------*/
/* 函数实现 ---------------------------------------------------------------------*/
// JsonObject &_josnObj;
//host_server.cpp
void HostServer::parseBean(const char *json_str)
{
    JsonObject &obj = _jsonBuffer.parseObject(json_str);
    bean.product_key = obj["product_key"].as<String>();
    bean.device_name = obj["device_name"].as<String>();
    bean.device_secret = obj["device_secret"].as<String>();
    bean.collect_interval = obj["collect_interval"];
    bean.total_report_count = obj["total_report_count"];
    bean.ip_address = obj["ip_address"].as<String>();
    bean.port = obj["port"];
    bean.standby_time = obj["standby_time"];

    bean.alink_body_format = "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}";
    bean.alink_topic_prop_post = "/sys/" + bean.product_key + "/" + bean.device_name + "/thing/event/property/post";
    bean.alink_topic_prop_postrsp = "/sys/" + bean.product_key + "/" + bean.device_name + "/thing/event/property/post_reply";
    bean.alink_topic_prop_set = "/sys/" + bean.product_key + "/" + bean.device_name + "/thing/service/property/set";
    bean.alink_method_prop_post = "thing.event.property.post";
}

String HostServerBean::tostring()
{
    String out;
    out += "/* HostServerBean ---------------------------------------------------------------------------------------------------------------*/\r\n";
    out += "product_key:" + product_key + "\r\n";
    out += "device_name:" + device_name + "\r\n";
    out += "device_secret:" + device_secret + "\r\n";
    out += "collect_interval:" + String(collect_interval) + "\r\n";
    out += "total_report_count:" + String(total_report_count) + "\r\n";
    out += "ip_address:" + ip_address + "\r\n";
    out += "port:" + String(port) + "\r\n";
    out += "standby_time:" + String(standby_time) + "\r\n";

    out += "alink_body_format:" + alink_body_format + "\r\n";
    out += "alink_topic_prop_post:" + alink_topic_prop_post + "\r\n";
    out += "alink_topic_prop_postrsp:" + alink_topic_prop_postrsp + "\r\n";
    out += "alink_topic_prop_set:" + alink_topic_prop_set + "\r\n";
    out += "alink_method_prop_post:" + alink_method_prop_post + "\r\n";

    out += "/* HostServerBean ---------------------------------------------------------------------------------------------------------------*/\r\n";
    return out;
}

HostServer::HostServer()
{
}
