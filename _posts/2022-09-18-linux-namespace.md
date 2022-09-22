---
layout: post
title:  "Linux Namespace"
date:   2022-09-17
categories: Linux
---

# What

是linux kernel的其中一种能力，就是某一些进程只能看见某一些资源

# 历史

- 受Plan 9启发
- 2002开始干，最初只有mount namespace，其他类型的namespace慢慢出来

# 种类

共有8种namespace：

- mount namespace
- pid namespace： 在这个namespace里被创建的第一个进程就是1了
- network namespace： 独立网络栈（独立的ip地址、路由表、socket、conn tracking table、fw）。一个新的namespace里只有lo口。interface可以在namespace间挪动。删除一个namespace时会销毁掉里面的virtual interface，而physical interface会被移回默认namespace
- ipc namespace： 不同的namespace里的进程就不能使用共享内存互相通信了，就被隔离了
- uts namespace： UNIX Time-Sharing，可以让不同进程有不同的hostname和domain name
- user namespace： 例如在namespace里是root，但对应到真实系统里某个一般用户。在namespace里有映射表
- cgroup namespace： linux 4.6开始才有
- time namespace： 允许进程看到不同的系统时间，在linux 5.6里才有

# 实现

kernel给每个进程都指定了链接，在`/proc/<pid>/ns/`

```
[laptop ns]# pwd
/proc/1183/ns
[laptop ns]# l
total 0
lrwxrwxrwx 1 zyg docker 0  9月22日 11:50 uts -> 'uts:[4026531838]'
lrwxrwxrwx 1 zyg docker 0  9月22日 11:50 user -> 'user:[4026531837]'
lrwxrwxrwx 1 zyg docker 0  9月22日 11:50 time_for_children -> 'time:[4026531834]'
lrwxrwxrwx 1 zyg docker 0  9月22日 11:50 time -> 'time:[4026531834]'
lrwxrwxrwx 1 zyg docker 0  9月22日 11:50 pid_for_children -> 'pid:[4026531836]'
lrwxrwxrwx 1 zyg docker 0  9月22日 11:50 pid -> 'pid:[4026531836]'
lrwxrwxrwx 1 zyg docker 0  9月22日 11:50 net -> 'net:[4026531840]'
lrwxrwxrwx 1 zyg docker 0  9月22日 11:50 mnt -> 'mnt:[4026531841]'
lrwxrwxrwx 1 zyg docker 0  9月22日 11:50 ipc -> 'ipc:[4026531839]'
lrwxrwxrwx 1 zyg docker 0  9月22日 11:50 cgroup -> 'cgroup:[4026531835]'
```

# 操纵namespace的syscall

- clone，用参数
- unshare
- setns

