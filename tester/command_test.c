/**
*文件名:command_test.c
*作者: 李程鹏 on Mar 17,2018
*描述: 对命令行模块的测试
*/

#include "tests.h"
#include "command.h"
#include "thread.h"
#include <stdio.h>

/**
测试指令:
1. ./happyTester	--当参数数量过少时
2. ./happyTester -h 	--打印帮助信息
3. ./happyTester -f		--参数后面确实文件名或数量信息时
4. ./happyTester -c fd09	--当参数后面跟的数字不符合规范时
5. ./happyTester -n 98		--当参数正确,但确实url或url错误时.
6. ./happyTester -n 98 -m http://localhost:98/		--当参数不存在时
7. ./happyTester -n 4000 -c 300 -t 500 -f http://localhost:98/		--一个正确的请求
*/

// 测试command模块(这里测试主要是在运行时测试)
void command_test(int argc, char const *argv[]){
	command(argc,argv);
	print_results();
}