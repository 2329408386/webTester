/**
*文件名: print.c
*作者: 李程鹏 on Mar 16, 2018
*描述: 对打印模块的具体实现
*/

#include "print.h"
#include <stdio.h>

// 打印欢迎信息
void print_welcome(){
	printf("\t\t欢迎来到happyTester\n    这是一款用于测试网站服务器性能的小工具,\n");
	printf("其中有很多设计不好的地方, 或者还没有测试出来的bug,谢谢帮我改进\n");
	printf("    作者:李程鹏, 上次更新于2018年3月19日\n\n");
}

// 当m<n时,打印错误提示消息,并返回-1,否则返回0.
int print_less_than(int m,int n){
	if(m<n){
		printf("参数的数量不能小于%d\n", n);
		return -1;
	}
	return 0;
}

// 打印帮助信息.
void print_help(){	
	print_welcome();
	printf("-n\t请求数(-n 1000)\n");
	printf("-t\t线程数(-t 100)\n");
	printf("-c\t并发数(-c 100)\n");
	printf("-f\t要请求的文件的URL(-f http://localhost:98/index.html)\n");
	printf("-h\t显示帮助信息\n\n");
}

// 打印当命令不存在时的错误信息.
void print_no_command(const char* command){
	printf("我没有设置%s这个参数\n", command);
}

// 打印命令参数长度不符合规范时的错误信息
void print_out_of_range(const char* command){
	printf("%s参数后面需要指定特定的个数\n", command);
}

// 打印当一个参数书写形式不符合规范时的错误信息
void print_nonstandard(const char* command){
	printf("%s这个参数不符合规范,你或许需要输入一个正整数\n", command);
}