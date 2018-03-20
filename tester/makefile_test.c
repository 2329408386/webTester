/**
* 文件名： makefile_test.c
* 作者： 李程鹏 on Mar 16,2018
* 描述： 测试Makefile文件的基本功能
*/

#include "tests.h"
#include <stdio.h>

static void debug_test(){
	printf("Debug info:\nBreakpoint 1 at 0x400556: file entry/entry.c, line 11.\n");
	printf("Breakpoint 1, main (argc=1, argv=0x7fffffffde88) at entry/entry.c:11(gdb) \n\n");
}

static void clean_test(){
	printf("Before clean:\n");
	printf("ls\n");
	printf("entry  gdbBeginner  happyTester  include  Makefile  README  tester\n");
	printf("After clean:\n");
	printf("ls\n");
	printf("entry  gdbBeginner  include  Makefile  README  tester\n\n");
}

// 测试makefile的基本功能
void makefile_test(){
	debug_test();
	clean_test();
}