/**
* 文件名: http.h
* 作者: 李程鹏 on Mar 17, 2018
* 描述: http模块
*/

#ifndef INCLUDE_HTTP_H
#define INCLUDE_HTTP_H

// 模拟http协议的get请求(我就只实现了get类型请求,post,head,put,delete都没实现)  
char* http_get(const char *url,char* hostname,char* file,int port);  

// 将url分解出主机名(hostname),请求文件名(file)和端口号(port)
// 解析成功返回0,失败返回-1.  
int http_parse_url(const char* url,char* hostname,char* file,int* port);

#endif