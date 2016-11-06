

对Linux Server进行性能评估，从带宽和时延两个方面，对Linux Server的CPU、内存、操作系统和IO等指标进行评估，主要借鉴lmbench这个框架。[参考链接](https://bg2bkk.github.io/post/how%20to%20perform%20server%20performance%20evaluation/)

目前主要完成

* 带宽 Bandwidth
	* Memory
		* read
		* write
		* copy
	* Pipe 
		* interact with parent and child process

* 时延 Latency
	* create Process
		* fork
		* fork + execve
	* 调用操作系统接口
		* system call
			* getpid
			* getppid
		* read/write via VFS
			* read /dev/zero
			* write /dev/null
	* 内存子系统时延
		* L1 Cache Latency
		* L2 Cache Latency
		* L3 Cache Latency

## Memory 带宽

> ThinkdPad-X220，搭配intel i5-2520M CPU，操作系统为Ubuntu 16.10 64bit
> L1 Cache 32KB		8路组相连 64B Cache line
> L2 Cache 256KB	8路组相连 64B Cache line
> L3 Cache 3072KB	12路组相连 64B Cache line

### 读内存

测试内存带宽的意义在于衡量计算机系统内，从主存中，经过各级存储和缓存将数据读到CPU的能力。在测试内存带宽的过程中，采用不同大小的数据块，将数据块内每个元素读入CPU，进行测试，可以大致得出各级存储的带宽。

* 读内存带宽
	* 下表是Memory Read的读性能，可以看到当数据块大小为4KB~32KB时，带宽明显高于64KB数据块，因为当前CPU的L1 Cache大小为32KB，使用较小数据块时，尤其在循环读写的过程中，可以充分享受L1 Cache带来的红利；
	* 一旦数据块大小超过L2 Cache，由于L1 Cache发生Conlict，带宽下降；随后在数据块大小达到L2 Cache的256KB和L3 Cache的3072KB时都会有带宽的跳变；
	* 由于L1和L2关系亲密，访问速度和容量的差别在10倍以内，所以L1 miss时L2可以将数据稳定在50000MB/s；
	* 而当数据接近L3 Cache时，由于L3访问速度慢，且容量较大，而且是共享的，因此此时较多的体现的是L3的速度，并且在数据块增大并较多的引起高速缓存失效时，访问带宽接近于从主存中读取数据。

* 目前存在的顾虑
	* 数据块4KB大小时的测试带宽是74036MB/s，~~平均每ns读取78Byte，对于睿频达3GHz的CPU而言，相当于每个cycle读取26B，如何解释呢，~~ CPU每次从L1 Cache读取数据以cache line为单位
	* 做bench过程中，各级存储是怎样起作用的，毕竟系统总线PCIE也才10GB/s的带宽，测试达到这个数据量的话一定是cache起到很大作用

* 写内存带宽

```bash
benchmark on Bandwidth of Memory
-------------------------------------


	Bandwidth of Memory Read

	blocksize:       4KB	bandwidth of mem read: 74036.584484 MB/s
	blocksize:       8KB	bandwidth of mem read: 74990.845844 MB/s
	blocksize:      16KB	bandwidth of mem read: 73919.006713 MB/s
	blocksize:      32KB	bandwidth of mem read: 71869.736103 MB/s
	blocksize:      48KB	bandwidth of mem read: 50845.840161 MB/s
	blocksize:      64KB	bandwidth of mem read: 50965.558431 MB/s
	blocksize:     128KB	bandwidth of mem read: 51501.282503 MB/s
	blocksize:     192KB	bandwidth of mem read: 49226.035775 MB/s
	blocksize:     256KB	bandwidth of mem read: 41491.085900 MB/s
	blocksize:     512KB	bandwidth of mem read: 31507.692308 MB/s
	blocksize:    1024KB	bandwidth of mem read: 31075.503763 MB/s
	blocksize:    2048KB	bandwidth of mem read: 26670.139341 MB/s
	blocksize:    2560KB	bandwidth of mem read: 22759.090370 MB/s
	blocksize:    3072KB	bandwidth of mem read: 19567.377495 MB/s
	blocksize:    4096KB	bandwidth of mem read: 17341.236240 MB/s
	blocksize:    8192KB	bandwidth of mem read: 16377.449020 MB/s
	blocksize:   16384KB	bandwidth of mem read: 16633.637634 MB/s
	blocksize:   32768KB	bandwidth of mem read: 16737.222340 MB/s
	blocksize:   65536KB	bandwidth of mem read: 16818.868668 MB/s
	blocksize:  131072KB	bandwidth of mem read: 16860.962919 MB/s


	Bandwidth of Memory Write

	blocksize:       4KB	bandwidth of mem write: 45464.636150 MB/s
	blocksize:       8KB	bandwidth of mem write: 45636.866031 MB/s
	blocksize:      16KB	bandwidth of mem write: 45096.225833 MB/s
	blocksize:      32KB	bandwidth of mem write: 37205.246521 MB/s
	blocksize:      64KB	bandwidth of mem write: 27972.027972 MB/s
	blocksize:     128KB	bandwidth of mem write: 27495.838032 MB/s
	blocksize:     256KB	bandwidth of mem write: 26442.183546 MB/s
	blocksize:     512KB	bandwidth of mem write: 21045.276116 MB/s
	blocksize:    1024KB	bandwidth of mem write: 20297.726417 MB/s
	blocksize:    2048KB	bandwidth of mem write: 18696.707992 MB/s
	blocksize:    4096KB	bandwidth of mem write: 11184.045260 MB/s
	blocksize:    8192KB	bandwidth of mem write: 9729.493477 MB/s
	blocksize:   16384KB	bandwidth of mem write: 9825.086593 MB/s
	blocksize:   32768KB	bandwidth of mem write: 9860.375542 MB/s
	blocksize:   65536KB	bandwidth of mem write: 9885.314901 MB/s
	blocksize:  131072KB	bandwidth of mem write: 9905.204101 MB/s

	Bandwidth of Memory Copy

	blocksize:       4KB	bandwidth of mem copy: 23679.038039 MB/s
	blocksize:       8KB	bandwidth of mem copy: 23963.306187 MB/s
	blocksize:      16KB	bandwidth of mem copy: 22988.505747 MB/s
	blocksize:      32KB	bandwidth of mem copy: 21537.037816 MB/s
	blocksize:      64KB	bandwidth of mem copy: 21261.575516 MB/s
	blocksize:     128KB	bandwidth of mem copy: 20395.968609 MB/s
	blocksize:     256KB	bandwidth of mem copy: 13415.784509 MB/s
	blocksize:     512KB	bandwidth of mem copy: 13412.094461 MB/s
	blocksize:    1024KB	bandwidth of mem copy: 11705.265083 MB/s
	blocksize:    2048KB	bandwidth of mem copy: 7406.871609 MB/s
	blocksize:    4096KB	bandwidth of mem copy: 6369.426752 MB/s
	blocksize:    8192KB	bandwidth of mem copy: 6320.636508 MB/s
	blocksize:   16384KB	bandwidth of mem copy: 6156.065889 MB/s
	blocksize:   32768KB	bandwidth of mem copy: 6200.686678 MB/s
	blocksize:   65536KB	bandwidth of mem copy: 6317.322048 MB/s
	blocksize:  131072KB	bandwidth of mem copy: 6446.414182 MB/s
```

## Pipe带宽

* pipe带宽的瓶颈点主要在于写操作，在单次写数据超过64KB时，带宽出现拐点；高版本的Linux Kernel中，Pipe的大小可以扩充为最大64KB，所以在单次写入pipe时以64KB为单位可以获得最大性能；read对pipe的性能影响不大；接下来还需要将pipe通信双方进程绑定到不同CPU上，减少内存切换。

```bash
huang@ThinkPad-X220:~/workspace/my_benchmark$ ./bench 
loop cost: 0.001673us	clock func cost: 0.029449us

benchmark on Bandwidth of Pipe
-------------------------------------
blocksize:       4KB	bandwidth of pipe: 3232.976359 MB/s
blocksize:       8KB	bandwidth of pipe: 3585.685272 MB/s
blocksize:      16KB	bandwidth of pipe: 3682.181693 MB/s
blocksize:      32KB	bandwidth of pipe: 3760.447728 MB/s
blocksize:      64KB	bandwidth of pipe: 3963.645935 MB/s
blocksize:     128KB	bandwidth of pipe: 2567.857644 MB/s
blocksize:     256KB	bandwidth of pipe: 2566.518958 MB/s
blocksize:     512KB	bandwidth of pipe: 2719.296382 MB/s
blocksize:    1024KB	bandwidth of pipe: 2647.417734 MB/s
blocksize:    2048KB	bandwidth of pipe: 2407.780140 MB/s
blocksize:    4096KB	bandwidth of pipe: 2384.145433 MB/s
blocksize:    8192KB	bandwidth of pipe: 1943.826453 MB/s
blocksize:   16384KB	bandwidth of pipe: 1906.762303 MB/s
blocksize:   32768KB	bandwidth of pipe: 2377.215872 MB/s
blocksize:   65536KB	bandwidth of pipe: 2375.914170 MB/s
blocksize:  131072KB	bandwidth of pipe: 2321.174369 MB/s

--------------end--------------------
huang@ThinkPad-X220:~/workspace/my_benchmark$ ./bench 
loop cost: 0.001674us	clock func cost: 0.031215us

benchmark on Bandwidth of Pipe
-------------------------------------
blocksize:       4KB	bandwidth of pipe: 3239.563165 MB/s
blocksize:       8KB	bandwidth of pipe: 3456.656765 MB/s
blocksize:      16KB	bandwidth of pipe: 3642.831732 MB/s
blocksize:      32KB	bandwidth of pipe: 3736.844410 MB/s
blocksize:      64KB	bandwidth of pipe: 4000.437548 MB/s
blocksize:     128KB	bandwidth of pipe: 2626.610851 MB/s
blocksize:     256KB	bandwidth of pipe: 2579.241139 MB/s
blocksize:     512KB	bandwidth of pipe: 2800.691421 MB/s
blocksize:    1024KB	bandwidth of pipe: 2736.095079 MB/s
blocksize:    2048KB	bandwidth of pipe: 2448.027234 MB/s
blocksize:    4096KB	bandwidth of pipe: 2414.115029 MB/s
blocksize:    8192KB	bandwidth of pipe: 1961.971475 MB/s
blocksize:   16384KB	bandwidth of pipe: 1939.511486 MB/s
blocksize:   32768KB	bandwidth of pipe: 2402.334769 MB/s
blocksize:   65536KB	bandwidth of pipe: 2401.455883 MB/s
blocksize:  131072KB	bandwidth of pipe: 2350.025244 MB/s

--------------end--------------------

```


## Operating [System Entry]

### system call 系统调用

* getpid
	* 严格说来不算系统调用，Linux对它的实现是缓存起来，不用系统调用即可获得自己的pid
* getppid
	* 获取父进程pid

### write/read 

* read /dev/zero
	* 从设备/dev/zero读一个字节
* write /dev/null
	* 向设备/dev/null写入一个字节
	* 由于大部分操作系统都没有对这两个设备进行优化，所以向/dev/null写入一个字节可以测量纯粹的write调用，从用户进程切入内核态，调用VFS层，无缓冲区的写入设备。

```bash
benchmark on Operating system entry
===================================
getpid() cost: 0.006010us
getppid() cost: 0.049570us
write 1 byte to /dev/null cost: 0.086910us
read 1 byte from /dev/zero cost: 0.123290us

--------------end------------------
```
* [system call 详解](http://www.chongh.wiki/blog/2016/04/08/linux-syscalls/)


## Process Creation

* fork创建进程
	* fork + exit
		* 创建子进程后立刻exit，用以衡量fork()这一过程用时
	* fork + exec
		* 创建子进程后，子进程调用execve执行另外一个程序，该程序打印"hello world"后退出
	* swap_ucontext
		* 采用ucontext保存当前处理器状态，切换到另一个ucontext状态，然后再切换回来

```bash
loop cost: 0.006350us	clock func cost: 0.030847us

benchmark on Creating Process
===================================
fork+exit cost: 72.923700us
fork+exec cost: 306.169600us
swap_ucontext cost: 0.421900us

--------------end------------------
```

## Memory Latency

目前常见的x64 Linux Server都具有 L1/L2/L3/Main Memory四级存储

### L1 Cache Latency

本机具有32KB L1 Cache，采用可以全部填充进L1 Cache的内存块，[可以统计计算出L1 Cache的访问速度；](https://bg2bkk.github.io/post/how%20to%20perform%20server%20performance%20evaluation/)

由下表可以看出，内存块大小在32KB和64KB间延时明显增大，结合本机的32KB L1 Cache可以得出结论 L1 Cache的latency为1.338ns，约合4个cpu cycle（本机主频可达3GHz），这与intel芯片手册相符。

```bash
benchmark on Cache Latency
-------------------------------------
buflen =     1KB	stride =    64	L1 Cache Latency: 1.337800ns
buflen =     4KB	stride =    64	L1 Cache Latency: 1.337700ns
buflen =    16KB	stride =    64	L1 Cache Latency: 1.337700ns
buflen =    32KB	stride =    64	L1 Cache Latency: 1.338300ns
buflen =    48KB	stride =    64	L1 Cache Latency: 4.030900ns
buflen =    64KB	stride =    64	L1 Cache Latency: 4.042000ns
buflen =   192KB	stride =    64	L1 Cache Latency: 4.162100ns
buflen =   320KB	stride =    64	L1 Cache Latency: 4.429900ns
buflen =   448KB	stride =    64	L1 Cache Latency: 4.574200ns
buflen =   576KB	stride =    64	L1 Cache Latency: 4.589900ns
buflen =   832KB	stride =    64	L1 Cache Latency: 4.593400ns
=====================
buflen =     4KB	stride =  1024	L1 Cache Latency: 1.337700ns
buflen =    16KB	stride =  1024	L1 Cache Latency: 1.337700ns
buflen =    32KB	stride =  1024	L1 Cache Latency: 1.337800ns
buflen =    48KB	stride =  1024	L1 Cache Latency: 4.022400ns
buflen =    64KB	stride =  1024	L1 Cache Latency: 4.016500ns
buflen =   192KB	stride =  1024	L1 Cache Latency: 5.033400ns
buflen =   320KB	stride =  1024	L1 Cache Latency: 7.959200ns
buflen =   448KB	stride =  1024	L1 Cache Latency: 9.424500ns
buflen =   576KB	stride =  1024	L1 Cache Latency: 9.396900ns
buflen =   832KB	stride =  1024	L1 Cache Latency: 9.397800ns
=====================
buflen =    16KB	stride =  4096	L1 Cache Latency: 1.337800ns
buflen =    32KB	stride =  4096	L1 Cache Latency: 1.337800ns
buflen =    48KB	stride =  4096	L1 Cache Latency: 4.021300ns
buflen =    64KB	stride =  4096	L1 Cache Latency: 4.008100ns
buflen =   192KB	stride =  4096	L1 Cache Latency: 4.411000ns
buflen =   320KB	stride =  4096	L1 Cache Latency: 8.380600ns
buflen =   448KB	stride =  4096	L1 Cache Latency: 9.362800ns
buflen =   576KB	stride =  4096	L1 Cache Latency: 9.376100ns
buflen =   832KB	stride =  4096	L1 Cache Latency: 9.402000ns
=====================
buflen =    16KB	stride =  8192	L1 Cache Latency: 1.337900ns
buflen =    32KB	stride =  8192	L1 Cache Latency: 1.337700ns
buflen =    48KB	stride =  8192	L1 Cache Latency: 1.337700ns
buflen =    64KB	stride =  8192	L1 Cache Latency: 1.337800ns
buflen =   192KB	stride =  8192	L1 Cache Latency: 4.021700ns
buflen =   320KB	stride =  8192	L1 Cache Latency: 4.025100ns
buflen =   448KB	stride =  8192	L1 Cache Latency: 6.283900ns
buflen =   576KB	stride =  8192	L1 Cache Latency: 8.047600ns
buflen =   832KB	stride =  8192	L1 Cache Latency: 8.372600ns
=====================

--------------end--------------------
```

### L2 Cache Latency

### L3 Cache Latency

TODO
------------------

* measure Cache line
* measure TLB 


