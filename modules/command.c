/**
*文件名:command.c
*作者: 李程鹏 on Mar 16,2018
*描述: 对命令行解析模块的具体实现
*/

#include "thread.h"
#include "command.h"
#include "print.h"
#include <string.h>
#include <stdlib.h>

// 检查一个字符串是否全部由0~9这10个数字组成, 是返回1, 否返回0.
static int is_number(const char* s){
	int i=0,len=strlen(s);
	for(;i<len&&s[i]!=0;i++){
		if(s[i]<'0'||s[i]>'9')
			return 0;
	}
	return 1;
}

// 检查命令行参数中某个位置的输入是否满足条件, 当不满足条件时, 返回-1, 满足条件时, 返回解析出来的整数
static int check_is_standard(int* i,int argc,char const *argv[]){
	(*i)++;
	if(*i>=argc){		// 当一个参数后面没有跟任何参数时, 比如: ./happyTester -t, 后面没了!
		print_out_of_range(argv[--(*i)]);
		return -1;
	}else if(is_number(argv[*i])==0){		// 当一个参数后面跟的不是"纯数字"时, 比如: ./happyTester -t 23f5 
		print_nonstandard(argv[--(*i)]);
		return -1;
	}else{
		int toReturn=atoi(argv[*i]);		
		(*i)++;
		return toReturn;
	}
}

// 解析命令行参数, 以完成对线程池的初始化
void command(int argc, char const *argv[]){
	// 检查参数的数量是否满足条件
	if(print_less_than(argc,2)==-1)
		return ;

	// 检查是否是查看帮助命令.
	if(argc==2&&strcmp(argv[1],"-h")==0){
		print_help();
		exit(0);
	}

	// 解析每一个参数, 并根据参数对线程池进行初始化.
	int i=1;
	while(i<argc){	
		if(strcmp(argv[i],"-n")==0){		// 当参数为"-n"时,说明其后跟的是总请求数
			int number=check_is_standard(&i,argc,argv);
			if(number==-1)
				return ;
			set_request_num(number);
		}else if(strcmp(argv[i],"-t")==0){	// 当参数为"-t"时,说明其后跟的是最多可以创建线程的数量
			int number=check_is_standard(&i,argc,argv);
			if(number==-1)
				return ;
			set_thread_num(number);
		}else if(strcmp(argv[i],"-c")==0){		// 当参数为"-c"时,说明其后跟的是最多可允许的tcp并发数
			int number=check_is_standard(&i,argc,argv);
			if(number==-1)
				return ;
			set_concurrency_num(number);
		}else if(strcmp(argv[i],"-f")==0){		// 当参数为"-f"时,说明其后跟的是要参与测试的URL
			i++;
			if(i>=argc){
				print_out_of_range(argv[--i]);
				return ;
			}
			set_url(argv[i]);
			i++;
		}else if(strcmp(argv[i],"-h")==0){		// 因为前面已经检查过参数为-h的情况了, 这里自动忽略.
			i++;
		}else {
			print_no_command(argv[i]);
			return ;
		}	
	}		
}