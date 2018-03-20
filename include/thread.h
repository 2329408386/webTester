/**
*文件名:thread.h
*作者: 李程鹏 on Mar 17, 2018
*描述: 线程模块
*/

#ifndef INCLUDE_THREAD_H
#define INCLUDE_THREAD_H 

// 初始化线程池
void init_threads();

// 合并所有进程
void join_threads();

// 设置请求数量
void set_request_num(int number);

// 设置线程数量
void set_thread_num(int number);

// 设置tcp连接数量
void set_concurrency_num(int number);

// 设置测试的url地址
void set_url(const char* url);

// 打印最终结果
void print_results();

#endif
