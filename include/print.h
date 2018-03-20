/**
*文件名:print.h
*作者: 李程鹏 on Mar 16, 2018
*描述: 打印模块
*/

#ifndef INCLUDE_PRINT_H
#define INCLUDE_PRINT_H 

// 打印欢迎信息
void print_welcome();

// 当m<n时,打印错误提示消息,并返回-1,否则返回0.
int print_less_than(int m,int n);

// 打印帮助信息.
void print_help();

// 打印当命令不存在时的错误信息.
void print_no_command(const char* command);

// 打印命令参数不符合规范时的错误信息
void print_out_of_range(const char* command);

// 打印当一个参数不符合规范时的错误信息
void print_nonstandard(const char* command);

#endif