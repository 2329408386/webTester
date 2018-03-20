/**
* 文件名: entry.c
* 作者: 李程鹏 on Mar 16,2018
* 描述: 项目的入口文件
*/

#include "tests.h"
#include "command.h"
#include "thread.h"
#include <stdio.h>

int main(int argc, char const *argv[]){
	// 解析输入的指令
	command(argc,argv);
	
	// 初始化线程池(每个线程中包含http get请求)
	init_threads();
	
	// 让主线程等待所有请求线程结束
	join_threads();

	// 打印最终的测试结果
	print_results();
	return 0;
}