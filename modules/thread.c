/**
*文件名: thread.c
*作者: 李程鹏 on Mar 17,2018
*描述: 线程模块的具体实现
*/

#include "http.h"
#include "thread.h"
#include "print.h"

#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

// 定义最大允许的请求数,最大允许的线程数,最大允许的并发数
#define MAX_REQUEST_NUM	100000
#define MAX_THREAD_NUM 100000
#define MAX_CONCURRENCY_NUM 100000

#define SIZE 1024

// 需要呈现给用户的基本信息
static struct message{
	char software[SIZE>>2];		// 服务器软件名
	int valid_length;		// 一个文件,用户关心部分的长度
	int total_length;		// 一个文件的总长度
	float time;		// 完成请求的总耗时(单位ms)
} message;

// 默认情况请求数,线程数和并发数都设置为1就好了
static int request_num=1;
static int thread_num=1;
static int concurrency_num=1;

// 存放参与测试的url
static char url[SIZE];

// 剩余请求的数量
static int remain_request_num;

// 当前并发的tcp连接数量
static int cur_concurrency_num;

// 创建一个大小为MAX_THREAD_NUM的线程id数组
static pthread_t threads[MAX_THREAD_NUM];

// int begin=0,end=0;		// 用于测试并发线程的运行次数
int counter=0;	// 记录发出的http请求个数

// 分别为要请求文件的主机名,文件路径名和端口
static char hostname[SIZE>>2];
static char file[SIZE>>1];
static int port;

// 记录某一段代码开始执行的时间
static clock_t begin;

// tcp请求失败的个数
extern int failed_number;

// 创建互斥量
static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;		// 用于锁定当前并发数
static pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;		// 用于锁定剩余请求数
static pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;		// 用于锁定实际执行的请求数

// 请求http服务的线程
static void* request(void* nothing){

	// 这里提前判断一个线程可以退出的条件,可比傻乎乎的用while循环快很多, 在n=100,c=5,t=20的情况下,都快
	// 20倍不止!!
	loop:if(remain_request_num<=0)
			pthread_exit(0);						
		
	// 每创建一个http请求,相应的并发数加1.
	pthread_mutex_lock(&mutex1);
	// begin++;
	cur_concurrency_num++;				
	// printf("begin:%d\n", cur_concurrency_num);
	pthread_mutex_unlock(&mutex1);		
		
	// 判断当前的tcp连接数是否已达到上限
	if(cur_concurrency_num<=concurrency_num){
			
		// 每发起一次tcp请求, 剩余请求数减少1.
		pthread_mutex_lock(&mutex2);			
		remain_request_num--;
		// printf("%d\n", remain_request_num);
		pthread_mutex_unlock(&mutex2);
		
		if(remain_request_num>=0){
			// 在同一个没有跳转语句的条件语句中, counter每执行一次, http_get()函数就必然
			// 执行一次, 所以可以通过记录counter的数量来判断http请求的次数
			pthread_mutex_lock(&mutex3);
			counter++;
			pthread_mutex_unlock(&mutex3);

			http_get(url,hostname,file,port);
			// printf("%s\n", s);
		}				
		else{
			remain_request_num=0;
			pthread_exit(0);
		}			
	}

	// 每结束一个http请求, 相应的并发数减1
	pthread_mutex_lock(&mutex1);
	// end++;
	cur_concurrency_num--;		
	// printf("end:%d\n", cur_concurrency_num);
	pthread_mutex_unlock(&mutex1);		

	goto loop;
}

// 解析http协议返回的相应信息
static void parse_response_message(const char* response){
	if(!strstr(response,"200")){
		printf("\n你请求的可能是一个空文件,但我还是会为你测试!\n\n");
	}

	char* ptr1;		//临时指针		

	// 获取有效文件长度(http相应头文件里面有)
	ptr1=strstr(response,"Content-Length:");
	if(ptr1){
		ptr1=ptr1+strlen("Content-Length: ");
		message.valid_length=atoi(ptr1);
	}	

	// 获取请求文件总长度
	ptr1=strstr(response,"\r\n\r\n");
	if(ptr1){
		message.total_length=message.valid_length+(ptr1-response);
	}

	// 获取服务器名称
	ptr1=strstr(response,"Server: ");
	if(ptr1){
		ptr1=ptr1+strlen("Server: ");
		char* ptr2=strchr(ptr1,'\n');
		int len=ptr2-ptr1;
		ptr1[len]=0;
		strcpy(message.software,ptr1);
	}
}

// 初始化线程池
void init_threads(){

	if(thread_num/concurrency_num>2||(thread_num-concurrency_num>=200))
		printf("当线程数比允许的并发数,相对多很多的时候(比如大于2倍关系或者线程大于并发数超过200),可能会很慢哦......\n\n");

	// 简析url,并检查是否简析成功(这里最好是先简析吧,线程一旦创建就是会马上开始运行的).
    if(http_parse_url(url,hostname,file,&port)){  
        printf("url链接解析失败,请检查一下你的输入是否符合条件\n");  
        exit(0); 
    }
   	
   	char* response=http_get(url,hostname,file,port);
   	parse_response_message(response);

	remain_request_num=request_num;
	cur_concurrency_num=0;
	int i=0;

	begin=clock();
	for(;i<thread_num;i++){
		int isSuccess = pthread_create( &threads[i], NULL, request, NULL);
		if(isSuccess!=0)
			printf("创建线程时失败了\n");
	}

}

// 合并所有进程
void join_threads(){
	int i=0;
	for(;i<thread_num;i++)
		pthread_join(threads[i],NULL);
	message.time=(float)(clock()-begin)/CLOCKS_PER_SEC*1000;
}

// 设置请求数量
void set_request_num(int number){
	if(number>MAX_REQUEST_NUM){
		number=MAX_REQUEST_NUM;
		printf("我设计的最大允许请求数是%d,已将你的请求自动改成%d\n", MAX_REQUEST_NUM,MAX_REQUEST_NUM);
	}
	request_num=number;
}

// 设置线程数量
void set_thread_num(int number){
	if(number>MAX_THREAD_NUM){
		number=MAX_THREAD_NUM;
		printf("我设计的最大允许线程数是%d,已将你的请求自动改成%d\n", MAX_THREAD_NUM,MAX_THREAD_NUM);
	}
	thread_num=number;
}

// 设置tcp连接数量
void set_concurrency_num(int number){
	if(number>MAX_CONCURRENCY_NUM){
		number=MAX_CONCURRENCY_NUM;
		printf("我设计的最大允许并发数是%d,已将你的请求自动改成%d\n", MAX_CONCURRENCY_NUM,MAX_CONCURRENCY_NUM);
	}
	concurrency_num=number;
}

// 设置测试的url地址
void set_url(const char* outer_url){
	if(strlen(outer_url)>=SIZE){
		printf("不好意思, 这个url太长了\n");
		return ;
	}
	strcpy(url,outer_url);
}

// 打印最终结果
void print_results(){
	print_welcome();

	//效率统计信息
    printf("总耗时:%.4fs\n", message.time/1000);
    printf("每秒完成请求数:%.2f请求/s\n", (request_num-failed_number)/(message.time/1000));
    printf("每个请求耗时:%.2fms/请求\n", message.time/(request_num-failed_number));
	printf("每个请求耗时(考虑并发):%.6fms/请求\n", message.time/(request_num-failed_number)/100);
	printf("平均传输速率: %.2fbytes/s\n\n", (request_num-failed_number)*message.total_length/(message.time/1000));
	
	// 传输情况基本信息
	printf("总传输长度:%dbytes\n", (request_num-failed_number)*message.total_length);
	printf("有效传输长度:%dbytes\n\n", (request_num-failed_number)*message.valid_length);

	// 请求完成情况基本信息
	printf("请求数:%-d\n", request_num);
	printf("成功数:%d\n", request_num-failed_number);
	printf("失败数:%d\n", failed_number);
	printf("线程数:%d\n", thread_num);
	printf("并发数:%d\n\n", concurrency_num);

	// 请求文件基本信息
	printf("文件路径名:%s\n", file);
	printf("文件有效长度:%dbytes\n\n", message.valid_length);

	// 服务器基本信息
	printf("服务器软件:%s\n",message.software);
	printf("服务器名称:%s\n", hostname);
	printf("服务器端口:%d\n\n",port);

	// printf("%.4f\n", message.time);
}