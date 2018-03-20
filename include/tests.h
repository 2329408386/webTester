/**
*　文件名：　tests.h
*　作者：　李程鹏　on Mar 16, 2018
* 描述: 测试文件, 用于测试各个模块的功能
*/

#ifndef INCLUDE_TESTS_H
#define INCLUDE_TESTS_H 

// 测试makefile的基本功能
void makefile_test();

// 测试command模块
void command_test(int argc, char const *argv[]);

// 测试http模块
void http_test();

// 测试thread模块
void thread_test();

#endif