# 定义变量
srcs = ppipe.c
objs = ppipe.o
opts = -g -c

# 默认目标
all: ppipe

# 链接目标
ppipe: $(objs)
	gcc $(objs) -o ppipe

# 编译目标
ppipe.o: $(srcs)
	gcc $(opts) $(srcs)

# 清理
clean:
	rm -f ppipe *.o
