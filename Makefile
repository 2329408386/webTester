# !Makefile
#--------
# 文件名： Makefile
# 作者 ：李程鹏 on Mar 16, 2018
# 描述 ：Makefile 文件
#---------

# 获取源文件和目标文件的列表#
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))

# 设置默认编译器#
CC = gcc

# 设置编译器参数 #
# -c:只负责编译汇编文件，不自动链接; -Wall:显示所以warning; -ggdb:生成调试信息; -I:默认包含路径为include文件夹
C_FLAGS = -c -Wall -ggdb  -I include

#为Makefile文件设置生成文件超集#
all:$(C_OBJECTS) link

#将源文件编译成目标文件#
# "$<"表示第一个依赖文件的名称,"$@"表示目标文件的名称.
%.o: %.c
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

#将目标文件链接成可执行文件#
link:
	@echo 链接内核文件...
	$(CC) $(C_OBJECTS) -o happyTester -lpthread

#设置清理文件选项#
.PHONY:clean
clean:
	$(RM) $(C_OBJECTS) happyTester

#设置调试选项#
.PHONY:debug
debug:
	cgdb -x ./entry/gdbBeginner
