---
layout: post
title:  "keepalived"
date:   2022-04-07
categories: Linux
---

keepalived实现了VRRP v2和v3

工作过程：

1. 选master，靠priority，比大
2. master选定后，其他角色靠定期接收master的message，收到说明master还活着
3. 如果设置了preemption，那么后面加入的更高priority的就会变成master
4. master一旦就绪，就会发一个免费arp，刷新mac地址表
5. vrrp是发给组播地址
6. vrrp既不是tcp，也不是udp，而是基于ip的

/etc/keepalived/keepalived.conf

```
vrrp_instance VI_1 {
        state MASTER       // 启动时的初始状态
        interface eth0     // vrrp跑的所在接口
        virtual_router_id 51
        priority 255
        advert_int 1      // 单位秒
        authentication {
              auth_type PASS
              auth_pass 12345
        }
        virtual_ipaddress {
              192.168.122.200/24   // vip
        }
}
```

当收到master的message时就会触发切vip（例如mater宕机了，网络不通了），有时还需要靠细粒度的触发机制再切，例如查看指定的进程、文件、网卡、脚本等

检查进程：

```
vrrp_track_process track_apache { 
      process httpd
      weight 10    // 如果httpd进程在的话，优先级就是244+10，如果httpd进程死了，则触发vip切换
}

vrrp_instance VI_1 {
      state MASTER
      interface eth0
      virtual_router_id 51
      priority 244
      advert_int 1
      authentication {
         auth_type PASS
         auth_pass 12345
      }
      virtual_ipaddress {
         192.168.122.200/24
      }
      track_process {
         track_apache
      }
}
```

检查网卡：

```
vrrp_instance VI_1 {
      state MASTER
      interface eth0
      virtual_router_id 51
      priority 244
      advert_int 1
      authentication {
         auth_type PASS
         auth_pass 12345
      }
      virtual_ipaddress {
         192.168.122.200/24
      }
      track_interface {
         ens9 weight 5   //只要ens9是up的，那么priority就等于 244+5=249
      }
}
```

检查脚本：

```
vrrp_script keepalived_check {
      script "/usr/local/bin/keepalived_check.sh"
      interval 1   // 1s跑一次
      timeout 5    // 最多等脚本执行5秒
      rise 3       // 返回3次成功才算真成功
      fall 3       // 3次失败才算失败
}

vrrp_instance VI_1 {
      state MASTER
      interface eth0
      virtual_router_id 51
      priority 244
      advert_int 1
      authentication {
         auth_type PASS
         auth_pass 12345
      }
      virtual_ipaddress {
         192.168.122.200/24
      }
      track_script {
         keepalived_check
      }
}
```

Notify：

```
server1# cat /usr/local/bin/keepalived_notify.sh
#!/bin/bash

echo "$1 $2 has transitioned to the $3 state with a priority of $4" > /var/run/keepalived_status

server1# cat keepalived.conf
vrrp_script keepalived_check {
      script "/usr/local/bin/keepalived_check.sh"
      interval 1
      timeout 5
      rise 3
      fall 3
}

vrrp_instance VI_1 {
      state MASTER
      interface eth0
      virtual_router_id 51
      priority 244
      advert_int 1
      authentication {
         auth_type PASS
         auth_pass 12345
      }
      virtual_ipaddress {
         192.168.122.200/24
      }
      track_script {
         keepalived_check
      }
      notify "/usr/local/bin/keepalived_notify.sh"   //每次state切换都会调用脚本进行通知，类似一个hook，可以做一些事情
}
```


Reference：

- https://www.redhat.com/sysadmin/advanced-keepalived
