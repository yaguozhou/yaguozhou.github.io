---
layout: post
title:  "info和man的区别"
date:   2021-05-03
categories: Linux
---

# 概述

大多数*nix的用户都使用过man page，但很少人使用和系统地翻阅info，其实info才是gnu projects的文档第一手资料，浏览info文档可以系统性了解丰富的gnu projects。

# 历史

man page是unix传统的分发文档的方式，不过在90年代早期，gnu project认为man文档已经过时了，gnu认为复杂的软件应该有完备的文档，而不是一些短的manual page，于是写了info命令要取代man命令，你会发现只要是gnu的projects，都会以info文档为主，info是gnu projects默认的文档方式。

# info简介

![info](/img/info.png)

- info文档的源码格式称为[texinfo](https://www.gnu.org/software/texinfo/)，texinfo是gnu project的官方默认文档格式，我们看到的文档是用`makeinfo`命令处理texinfo格式的文本而成
- 文档默认位于`/usr/share/info`目录

- man像小册子，而info更像是书

# info命令

格式是： `info [OPTION...] [MANUAL] [MENU-OR-INDEX-ITEM...]`， 举几个例子：
- `info coreutils` ： 找到名字叫coreutils的entry并显示
- `info coreu` ： 和上面一样
- `info ls` ： 虽然ls是位于coreutils内部的，但依然能直接找到这个entry
- `info -k gethostname` ： 搜索

# info常用快捷键

![info](/img/info-info.png)

info默认的key-binding和emacs类似，显示的界面也和emacs非常相似，如果你熟悉emacs的话，会感到很亲切，而man默认用less，key-binding是vi。

执行`info info`可以查看完整的info教程，一般记住常用的几个就可以了：

- 全局内容移动： `<SPC>` scroll-forward ， `<DEL>` scroll-backward ，一页一页地翻
- 全局选node，包括子node： `[` `]` ，前，后
- 快速选node： `<` `>` `d`： 第一node，最后node，info tree的最开始
- 选择node： `n` `p` `u` `l` `t` ： 后，前，上，回，顶
- cross-references简称xrefs
  - `* Foo` 开头的是个menu reference
  - `*Note Foo` 开头的是note reference
  - `m` 选menu， `f` 选xref

# 总结

只要你安装好一台gnu/linux操作系统，就可以畅游info命令带给你的gnu projects丰富多彩的文档了！
