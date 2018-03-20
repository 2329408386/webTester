/**
* 文件名:http_test.c
* 作者: 李程鹏 on Mar 19, 2018
* 描述: 对http模块的测试
*/

#include "tests.h"

// 测试http模块(主要是命令行输入, 我只测试了3种情况,访问的文件不存在,是文本文件,是图像文件)
void http_test(){
	/**
	1. ./happyTester -n 400 -c 40 -t 50 -f http://localhost:98/resources/monkey.jpeg		--当测试的文件不存在时
	2. ./happyTester -n 400 -c 40 -t 50 -f http://localhost:98/		--当测试的文件是一个普通html文件时
	3. ./happyTester -n 400 -c 40 -t 50 -f http://localhost:98/resources/test1.jpeg		--当测试的文件是一个图片文件时
	*/	

	;
}