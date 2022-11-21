---
layout: post
title:  "kernel module"
date:   2021-09-25
categories: kernel
---

## kernel threads

OS的tasks都要靠kernel threads来执行，ps命令的输出中很容易能辨认出来哪些是kernel threads（带中括号的便是）

![threads](/img/kernelthreads.png)

## kernel tainted

kernel一个重要的任务就是要硬件初始化，该工作主要靠driver，driver通常都是以module的形式被load进kernel。有一些硬件厂商不愿意开源它的driver。如果内核里揉入了闭源的driver，那么称kernel是tainted

## kernel的行为

kernel通过几种方式来告诉我们它正在做什么，例如kernel写的log、/proc

### kernel log

kernel把最近的log保存在被称为kernel ring buffer的内存里，`dmesg`或`journalctl -k`可以查看这段内存的日志内容

![klog](/img/klog.png)

### /proc

kernel通过`/proc`告诉你它的全部状态是什么，例如`cat /proc/meminfo`直接查看kernel告诉你的memory状态

### uname， hostnamectl

查看kernel版本

## kernel module

kernel module就是一段代码，它可以被kernel按需加载和卸载。引入module的好处是kernel本身可以变的更小一些

kernel在1990年就实现了module化的编译，模块化driver可谓是linux的一个特色

### 硬件初始化的简要过程

- 启动期间，kernel开始探测硬件
- 一旦探测到新硬件，systemd-udevd进程就负责load合适的driver
  - systemd-udevd读`/usr/lib/udev/rules.d`
  - systemd-udevd再读用户自定义的`/etc/udev/rules.d`目录
  - 然后合适的modules就被自动载入，硬件信息也被写入`/sys`目录暴露出去
- systemd-udevd不仅负责启动时硬件探测，而且持续地探测热插拔情况，通过`udevadm monitor`能查看到硬件变化的日志

### 管理kernel modules



#### 自动载入

除了udev能载入module，你可以通过在目录`/etc/modules-load.d/`下创建文件来自己定义要自动载入的module。

#### 操作modules

| 命令        | 描述                    |
|-------------|-------------------------|
| lsmod       | list所有module          |
| modinfo     | 看看module信息          |
| modprobe    | load module（包含依赖） |
| modprobe -r | unload module           |

    像有些老的命令，比如insmod、 rmmod，由于没考虑依赖问题，被废弃了

#### 查看哪些硬件用了哪些module

`lspci -k`

![lspci](/img/lspci.png)

#### 给module加参数

可以通过在目录`/etc/modprobe.d/`下新建文件，给module自动加载时自动带上参数，例如：

```
cat /etc/modprobe.d/alsa-base.conf
options snd-hda-intel probe_mask=1
```

#### 让module开机启动

示例：新建`/etc/sysconfig/modules/br_netfilter.modules`，内容：

```
#!/bin/bash
modprobe br_netfilter
```

### 动手写一个简单的module

hello.c

```c
#include <linux/kernel.h> /* for KERN_DEBUG */
#include <linux/module.h> /* for all kernel modules */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaguo Zhou <yaguozhou@126.com>");
MODULE_DESCRIPTION("my test driver");

static int __init  my_init_module(void)
{
  printk(KERN_DEBUG "hello\n"); /* linux/kern_levels.h定义了日志8个级别 */
  return 0; /* 初始化成功 */
}

static void __exit my_cleanup_module(void)
{
  printk(KERN_DEBUG "goodbey\n");
}

module_init(my_init_module);
module_exit(my_cleanup_module);
```

Makefile

```makefile
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

```

编译

![build](/img/build-module.png)

查看modinfo

![info](/img/modinfo.png)

加载

![insmod](/img/insmod.png)

如果想用modprobe加载module的话，会报：
```
> modprobe hello
modprobe: FATAL: Module hello not found in directory /lib/modules/5.15.78-1-MANJARO
```
做法应该是：
1. 将.ko文件拷贝到```/lib/module/`uname -r`(内核版本号)/kernel/driver/xxx```目录下
   根据具体用途的区别分为net、ide、scsi、usb、video、parport、md、block、ata等，或者新建自己的目录
2. `depmod -a`，更新模块依赖新，主要是更新modules.dep文件
3. `modprobe hello`


