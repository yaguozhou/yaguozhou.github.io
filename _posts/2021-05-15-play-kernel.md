---
layout: post
title:  "配置、编译、定制Linux Kernel"
date:   2021-05-15
categories: Linux
github_comments_issueid: 1
---

# 编译工具要求

- kernel是C写的，有少量的汇编，所以编译kernel只需要3个包：
  - gcc
  - binutils：内含ld，gcc并不能完成所有编译，还需要binutils里的工具来完成汇编和链接
  - make

# 使用kernel所需的工具包

- util-linux
  - 小工具集合，用来控制磁盘分区的创建、挂载等，里面包含很常见的命令，如fdisk、dmesg、mount、lscpu等
- module-init-tools（kmod）
  - 使用kernel module会用到，里面会有lsmod、insmod、modprobe、depmod等
- 文件系统相关
  - ext2/3/4：e2fsprogs包，提供tune2fs之类的命令
  - JFS： IBM的jfsutils包
  - ReiserFS： reiserfsprogs包，在suse采用ext之前，reiserfs是默认的fs
  - XFS： xfsprogs包，Silicon Graphics开发的，是某些linux的默认fs
- 配额
  - quota-tools包，使用kernel提供的配额功能
- NFS
  - nfs-utils包，挂载或运行nfs程序

# 其他工具包

- udev包
  - udev被用来管理/dev目录
- 进程管理包procps
  - 包含ps、top等命令

# 源码下载

从mirror下载会快点，不同版本自己改改url

- `wget https://mirror.bjtu.edu.cn/kernel/linux/kernel/v5.x/linux-5.10.36.tar.xz`

# make

make有很多方便的target，`make help`已经很清晰了

```
➜  linux-5.10.36 make help 
Cleaning targets:
  clean		  - Remove most generated files but keep the config and
                    enough build support to build external modules
  mrproper	  - Remove all generated files + config + various backup files
  distclean	  - mrproper + remove editor backup and patch files

Configuration targets:
  config	  - Update current config utilising a line-oriented program
  nconfig         - Update current config utilising a ncurses menu based program
  menuconfig	  - Update current config utilising a menu based program
  xconfig	  - Update current config utilising a Qt based front-end
  gconfig	  - Update current config utilising a GTK+ based front-end
  oldconfig	  - Update current config utilising a provided .config as base
  localmodconfig  - Update current config disabling modules not loaded
                    except those preserved by LMC_KEEP environment variable
  localyesconfig  - Update current config converting local mods to core
                    except those preserved by LMC_KEEP environment variable
  defconfig	  - New config with default from ARCH supplied defconfig
  savedefconfig   - Save current config as ./defconfig (minimal config)
  allnoconfig	  - New config where all options are answered with no
  allyesconfig	  - New config where all options are accepted with yes
  allmodconfig	  - New config selecting modules when possible
  alldefconfig    - New config with all symbols set to default
  randconfig	  - New config with random answer to all options
  yes2modconfig	  - Change answers from yes to mod if possible
  mod2yesconfig	  - Change answers from mod to yes if possible
  listnewconfig   - List new options
  helpnewconfig   - List new options and help text
  olddefconfig	  - Same as oldconfig but sets new symbols to their
                    default value without prompting
  tinyconfig	  - Configure the tiniest possible kernel
  testconfig	  - Run Kconfig unit tests (requires python3 and pytest)

Other generic targets:
  all		  - Build all targets marked with [*]
* vmlinux	  - Build the bare kernel
* modules	  - Build all modules
  modules_install - Install all modules to INSTALL_MOD_PATH (default: /)
  dir/            - Build all files in dir and below
  dir/file.[ois]  - Build specified target only
  dir/file.ll     - Build the LLVM assembly file
                    (requires compiler support for LLVM assembly generation)
  dir/file.lst    - Build specified mixed source/assembly target only
                    (requires a recent binutils and recent build (System.map))
  dir/file.ko     - Build module including final link
  modules_prepare - Set up for building external modules
  tags/TAGS	  - Generate tags file for editors
  cscope	  - Generate cscope index
  gtags           - Generate GNU GLOBAL index
  kernelrelease	  - Output the release version string (use with make -s)
  kernelversion	  - Output the version stored in Makefile (use with make -s)
  image_name	  - Output the image name (use with make -s)
  headers_install - Install sanitised kernel headers to INSTALL_HDR_PATH
                    (default: ./usr)

Static analysers:
  checkstack      - Generate a list of stack hogs
  versioncheck    - Sanity check on version.h usage
  includecheck    - Check for duplicate included header files
  export_report   - List the usages of all exported symbols
  headerdep       - Detect inclusion cycles in headers
  coccicheck      - Check with Coccinelle
  clang-analyzer  - Check with clang static analyzer
  clang-tidy      - Check with clang-tidy

Tools:
  nsdeps          - Generate missing symbol namespace dependencies

Kernel selftest:
  kselftest         - Build and run kernel selftest
                      Build, install, and boot kernel before
                      running kselftest on it
                      Run as root for full coverage
  kselftest-all     - Build kernel selftest
  kselftest-install - Build and install kernel selftest
  kselftest-clean   - Remove all generated kselftest files
  kselftest-merge   - Merge all the config dependencies of
		      kselftest to existing .config.

Userspace tools targets:
  use "make tools/help"
  or  "cd tools; make help"

Kernel packaging:
  rpm-pkg             - Build both source and binary RPM kernel packages
  binrpm-pkg          - Build only the binary kernel RPM package
  deb-pkg             - Build both source and binary deb kernel packages
  bindeb-pkg          - Build only the binary kernel deb package
  snap-pkg            - Build only the binary kernel snap package
                        (will connect to external hosts)
  dir-pkg             - Build the kernel as a plain directory structure
  tar-pkg             - Build the kernel as an uncompressed tarball
  targz-pkg           - Build the kernel as a gzip compressed tarball
  tarbz2-pkg          - Build the kernel as a bzip2 compressed tarball
  tarxz-pkg           - Build the kernel as a xz compressed tarball
  perf-tar-src-pkg    - Build perf-5.10.36.tar source tarball
  perf-targz-src-pkg  - Build perf-5.10.36.tar.gz source tarball
  perf-tarbz2-src-pkg - Build perf-5.10.36.tar.bz2 source tarball
  perf-tarxz-src-pkg  - Build perf-5.10.36.tar.xz source tarball

Documentation targets:
 Linux kernel internal documentation in different formats from ReST:
  htmldocs        - HTML
  latexdocs       - LaTeX
  pdfdocs         - PDF
  epubdocs        - EPUB
  xmldocs         - XML
  linkcheckdocs   - check for broken external links
                    (will connect to external hosts)
  refcheckdocs    - check for references to non-existing files under
                    Documentation
  cleandocs       - clean all generated files

  make SPHINXDIRS="s1 s2" [target] Generate only docs of folder s1, s2
  valid values for SPHINXDIRS are: PCI RCU accounting admin-guide arm arm64 block bpf cdrom core-api cpu-freq crypto dev-tools devicetree doc-guide driver-api fault-injection fb filesystems firmware-guide fpga gpu hid hwmon i2c ia64 ide iio infiniband input isdn kbuild kernel-hacking leds livepatch locking m68k maintainer mhi mips misc-devices netlabel networking openrisc parisc pcmcia power powerpc process riscv s390 scheduler scsi security sh sound sparc spi staging target timers trace translations usb userspace-api virt vm w1 watchdog x86 xtensa

  make SPHINX_CONF={conf-file} [target] use *additional* sphinx-build
  configuration. This is e.g. useful to build with nit-picking config.

  Default location for the generated documents is Documentation/output

Architecture specific targets (x86):
* bzImage      - Compressed kernel image (arch/x86/boot/bzImage)
  install      - Install kernel using
                  (your) ~/bin/installkernel or
                  (distribution) /sbin/installkernel or
                  install to $(INSTALL_PATH) and run lilo
  fdimage      - Create 1.4MB boot floppy image (arch/x86/boot/fdimage)
  fdimage144   - Create 1.4MB boot floppy image (arch/x86/boot/fdimage)
  fdimage288   - Create 2.8MB boot floppy image (arch/x86/boot/fdimage)
  isoimage     - Create a boot CD-ROM image (arch/x86/boot/image.iso)
                  bzdisk/fdimage*/isoimage also accept:
                  FDARGS="..."  arguments for the booted kernel
                  FDINITRD=file initrd for the booted kernel

  i386_defconfig              - Build for i386
  x86_64_defconfig            - Build for x86_64

  make V=0|1 [targets] 0 => quiet build (default), 1 => verbose build
  make V=2   [targets] 2 => give reason for rebuild of target
  make O=dir [targets] Locate all output files in "dir", including .config
  make C=1   [targets] Check re-compiled c source with $CHECK
                       (sparse by default)
  make C=2   [targets] Force check of all c source with $CHECK
  make RECORDMCOUNT_WARN=1 [targets] Warn about ignored mcount sections
  make W=n   [targets] Enable extra build checks, n=1,2,3 where
		1: warnings which may be relevant and do not occur too often
		2: warnings which occur quite often but may still be relevant
		3: more obscure warnings, can most likely be ignored
		Multiple levels can be combined with W=12 or W=123

Execute "make" or "make all" to build all targets marked with [*] 
For further info see the ./README file        
```

# 内核配置

生成配置的方式有很多种，结果存放在`.config`中（在已安装好的机器上通常可以通过`/proc/config.gz`来查看.config信息）

- `make oldconfig` 会认为.config是老kernel的，如果新kernel里的option没出现在.config里，就会提示，适合把老kernel的配置移植到新kernel
- `make defconfig` 创建默认配置
- `make menuconfig` 界面配置

    ![menuconfig](/img/menuconfig.png)

- `make gconfig`  gtk界面配置

    ![gconfig](/img/gconfig.png)

- `make xconfig`  Qt界面配置

    ![xconfig](/img/xconfig.png)


# 编译

- `make -j16`，把数字写成具体core数量的2倍
- `make -j16 O=~/linux/tmp` 编译到指定目录

我的笔记本cpu跑满，编译一把耗时 **40分钟**  `make -j16  17275.96s user 1426.30s system 768% cpu 40:32.52 total` 

- cpu： Intel(R) Core(TM) i7-6700HQ CPU @ 2.60GHz， HT后8核
- 内存： 16G

    ![make-j](/img/make-j-2.png)

    ![make-j](/img/make-j.png)

# 编译文档

`make htmldocs` 8分钟

# 查找已安装模块的硬件对应的编译配置

1. 以我的笔记本上的无线网卡为例

	```
	➜  ~ ip a 
	2: wlp2s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
	    link/ether 9c:b6:d0:d3:c5:d5 brd ff:ff:ff:ff:ff:ff
	    inet 172.25.6.9/20 brd 172.25.15.255 scope global dynamic noprefixroute wlp2s0
	       valid_lft 28448sec preferred_lft 28448sec
	    inet6 fe80::c6ea:94a1:5e04:2fd4/64 scope link noprefixroute 
	       valid_lft forever preferred_lft forever
	```

1. 在/sys下找到设备对应的module

	```
	➜  ~ readlink /sys/class/net/wlp2s0/device/driver/module 
	../../../../module/ath10k_pci 
	```

1. 搜索包含ath10k_pci的Makefile，找出`CONFIG_`开头的内核参数

	```
	➜  linux-5.10.36 find . -type f -name Makefile | xargs grep ath10k_pci
	./drivers/net/wireless/ath/ath10k/Makefile:obj-$(CONFIG_ATH10K_PCI) += ath10k_pci.o
	./drivers/net/wireless/ath/ath10k/Makefile:ath10k_pci-y += pci.o
	./drivers/net/wireless/ath/ath10k/Makefile:ath10k_pci-$(CONFIG_ATH10K_AHB) += ahb.o
	```
1. `make menuconfig`中搜索

    ![](/img/ath10k.png)

    ![](/img/ath10k-search.png)


# 查找未安装模块的PCI设备的编译配置

1. 通过`lspci`找到设备

	```
    ➜  lspci
    00:17.0 SATA controller: Intel Corporation HM170/QM170 Chipset SATA Controller [AHCI Mode] (rev 31)
	```

    `00:17.0`是设备的PCI总线ID

1. 到/sys目录下找到vendor id和device id（内核给PCI设备的编号以0000:开头，lspci省略了这块）

	```
	➜  ~ cat /sys/bus/pci/devices/0000:00:17.0/vendor
	0x8086
	➜  ~ cat /sys/bus/pci/devices/0000:00:17.0/device
	0xa103
	```

1. 到源码中搜索vendor id和device id（`include/linux/pci_ids.h`是PCI ID经常存放的地方）

	```
	➜  linux-5.10.36 rg 0x8086 include
    include/linux/pci_ids.h
    2653:#define PCI_VENDOR_ID_INTEL		0x8086
	```

	```
	➜  linux-5.10.36 rg 0xa103
	drivers/ata/ahci.c
	392:	{ PCI_VDEVICE(INTEL, 0xa103), board_ahci_mobile }, /* Sunrise M AHCI */
	```

    ```
	➜  linux-5.10.36 find . -type f -name Makefile | xargs grep -i ahci
	./arch/mips/netlogic/xlp/Makefile:ifdef CONFIG_SATA_AHCI
	./arch/mips/netlogic/xlp/Makefile:obj-y				+= ahci-init.o
	./arch/mips/netlogic/xlp/Makefile:obj-y				+= ahci-init-xlp2.o
	./drivers/ata/Makefile:obj-$(CONFIG_SATA_AHCI)		+= ahci.o libahci.o
	./drivers/ata/Makefile:obj-$(CONFIG_SATA_ACARD_AHCI)	+= acard-ahci.o libahci.o
	./drivers/ata/Makefile:obj-$(CONFIG_SATA_AHCI_SEATTLE)	+= ahci_seattle.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_SATA_AHCI_PLATFORM) += ahci_platform.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_SATA_HIGHBANK)	+= sata_highbank.o libahci.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_BRCM)		+= ahci_brcm.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_CEVA)		+= ahci_ceva.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_DA850)	+= ahci_da850.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_DM816)	+= ahci_dm816.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_IMX)		+= ahci_imx.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_MTK)		+= ahci_mtk.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_MVEBU)	+= ahci_mvebu.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_OCTEON)	+= ahci_octeon.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_SUNXI)	+= ahci_sunxi.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_ST)		+= ahci_st.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_TEGRA)	+= ahci_tegra.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_XGENE)	+= ahci_xgene.o libahci.o libahci_platform.o
	./drivers/ata/Makefile:obj-$(CONFIG_AHCI_QORIQ)	+= ahci_qoriq.o libahci.o libahci_platform.o
	./drivers/phy/socionext/Makefile:obj-$(CONFIG_PHY_UNIPHIER_AHCI)	+= phy-uniphier-ahci.o
	```

# 参数传递给kernel的时机

- 构建时（.config）
- 启动时（grub配置文件）
- 运行时（/proc，/sys）

全部参数见（v5.10.36）： [Documentation/admin-guide/kernel-parameters.txt](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/Documentation/admin-guide/kernel-parameters.txt?h=v5.10.36) 

