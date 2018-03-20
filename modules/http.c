/**
* 文件名: http.c
* 作者: 李程鹏 on Mar 17, 2018
* 描述; http模块的实现
*/

#include "http.h" 

#include <stdio.h>  
#include <stdlib.h>  
#include <arpa/inet.h>  
#include <netdb.h>  
#include <string.h>  
#include <unistd.h> 
#include <pthread.h>

// 定义默认的buffer size  
#define BUFFER_SIZE 1024  

// 定义http get请求,从上到下分别是请求行,请求头,空行和消息体,消息体可以为空
// 该get请求定义为可以接受任意类型的文件.    
#define HTTP_GET "GET %s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n\r\n"  
  
#define HTTP_PORT 80    
#define HTTPS_PORT 443
 
// tcp请求失败的个数
int failed_number=0;

// 创建互斥量
static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;      // 用于锁定失败的tcp请求个数

// 创建一个http链接  
static int http_create(const char* hostname, int port){  
    // 记录主机信息(包括主机名、别名、地址类型、地址长度和地址列表)
    struct hostent* host_entry;  
    
    // 用于处理网络地址的结构体
    struct sockaddr_in server_addr;   
    
    // socket文件描述符
    int socket_fd;  
    

    if((host_entry = gethostbyname(hostname))==NULL){  
        return -1;  
    }   
    
    // sin=socket in?
    
    server_addr.sin_family = AF_INET;    // ipv4协议
    server_addr.sin_port = htons(port);     // 设置连接到服务器的端口号
    server_addr.sin_addr = *((struct in_addr *)host_entry->h_addr);     // 设置服务器的ip地址
  
    // 创建socket AF_INET:ipv4, SOCK_STREAM: tcp, 0:protocol
    if((socket_fd = socket(AF_INET,SOCK_STREAM,0))==-1){  
        return -1;  
    }  
    
    // 建立和服务器的http连接
    if(connect(socket_fd, (struct sockaddr* )&server_addr,sizeof(struct sockaddr)) == -1){  
        return -1;  
    }  
    
    // 返回socket文件描述符
    return socket_fd;  
}   
  
// 将url分解出主机名(hostname),请求文件路径名(file)和端口号(port)
// 解析成功返回0,失败返回-1.  
int http_parse_url(const char* url,char* hostname,char* file,int* port)  
{  
    char *ptr1,*ptr2;  
  
    // 将ptr1指向url的开始地址
    ptr1 = (char *)url;  
  
    // 判断url是否以"http://"开头, 不满足则说明该url不满足http协议要求,https协议也类似  
    if(!strncmp(ptr1,"http://",strlen("http://"))){  
        ptr1 += strlen("http://");  
    }else if(!strncmp(ptr1,"https://",strlen("https://"))){
        ptr1 += strlen("https://"); 
    }else{  
        return -1;  
    }  
  
    // 寻找主机与请求文件的分隔符
    ptr2 = strchr(ptr1,'/');  
    
    // 当找到这个分隔符的时候
    if(ptr2){  
        // 分解出主机名, 放入hostname变量中(我也不知道为什么, 这里就是需要把strlen(ptr1)-strlen(ptr2)的结果先保存起来).
        int len=strlen(ptr1) - strlen(ptr2);
        memcpy(hostname,ptr1,len);         
        hostname[len] = '\0';  
        
        // 判断'/'后面是否跟了文件名, 如果跟了,则把文件名放入到file变量中.
        if(*ptr2){  
            memcpy(file,ptr2,strlen(ptr2) );  
            file[strlen(ptr2)] = '\0';  
        }
    }else{  
        memcpy(hostname,ptr1,strlen(ptr1));  
        hostname[strlen(ptr1)] = '\0';  
    }  
    
    // 获取端口号 
    ptr1 = strchr(hostname,':');  
    if(ptr1){  
        *ptr1++='\0';  
        *port=atoi(ptr1);  
    }else if(!strncmp(url,"http://",strlen("http://"))){  
        *port=HTTP_PORT;  
    }else
        *port=HTTPS_PORT; 
    
    // 分解成功返回0.
    return 0;  
}         
  
// 模拟http协议的get请求   
char* http_get(const char* url,char* hostname,char* file,int port)  
{   
    // 创建socket时,需要返回的文件描述符(-1表示创建socket失败)
    int socket_fd = -1;  
    
    // 存放http请求信息
    char http_request_buffer[BUFFER_SIZE<<2] = {'\0'};  

    // 待返回字符串指针
    char* toReturn;  
    
    // 检查url是否为空
    if(!url){  
        printf("url不能为空\n");  
        return NULL;  
    }  
  
    // 创建用户端socket
    socket_fd =  http_create(hostname,port);  
    if(socket_fd < 0){  
        // 记录失败的tcp请求个数
        pthread_mutex_lock(&mutex1);
        failed_number++;              
        pthread_mutex_unlock(&mutex1);  
        
        printf("创建http连接失败\n");  
        return NULL;  
    }  
    
    // 将文件,主机名,端口号按照定义的HTTP_GET(http请求的格式)的形式写入http_request_buffer
    sprintf(http_request_buffer,HTTP_GET,file,hostname,port);  
  
    // 向服务器端发送http请求
    if(send(socket_fd,http_request_buffer,strlen(http_request_buffer),0)<0){ 
        // 记录失败的tcp请求个数
        pthread_mutex_lock(&mutex1);
        failed_number++;              
        pthread_mutex_unlock(&mutex1);

        printf("http请求发送失败\n");  
        return NULL;  
    }   
    
    // 接受服务器端返回的信息.
    if(recv(socket_fd, http_request_buffer,BUFFER_SIZE<<2,0)<= 0){ 
        // 记录失败的tcp请求个数
        pthread_mutex_lock(&mutex1);
        failed_number++;              
        pthread_mutex_unlock(&mutex1);

        printf("信息接受失败,检查一下路径吧,有可能末尾少加了一个斜杠\n");  
        return NULL;  
    }  

    // 关闭连接
    close(socket_fd);
  
    // 返回从服务器请求到的数据.
    toReturn=http_request_buffer;
    return toReturn;  
}  
