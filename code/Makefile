# 随着linux代码的增长，kbuild system被引入来组织简化编译的过程
# Kbuild Makefile和userspace的Makefile有些区别

# 告诉kbuild hello.o是个loadable kernel module (LKM)
# 告诉kbuild去找hello.c，然后编译成hello.o
obj-m += hello.o

# -C意思是离开当前目录，前往kernel的source目录，make会用那里kbuild Makefile，通过M参数定位到我们当前目录
all:
	make -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) clean
