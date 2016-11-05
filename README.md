

对Linux Server进行性能评估，从带宽和时延两个方面，对Linux Server的CPU、内存、操作系统和IO等指标进行评估，主要借鉴lmbench这个框架。[参考链接](https://bg2bkk.github.io/post/how%20to%20perform%20server%20performance%20evaluation/)

目前主要完成

* 带宽 Bandwidth
	* Pipe 

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


# benchmark 结果

### 在ThinkdPad-X220，搭配intel i5-2520M CPU，操作系统为Ubuntu 16.10 64bit

```bash
loop cost: 0.006350us	clock func cost: 0.030847us

benchmark on Operating system entry
===================================
getpid() cost: 0.006010us
getppid() cost: 0.049570us
write 1 byte to /dev/null cost: 0.086910us
read 1 byte from /dev/zero cost: 0.123290us

--------------end------------------

benchmark on Creating Process
===================================
fork+exit cost: 72.923700us
fork+exec cost: 306.169600us
swap_ucontext cost: 0.421900us

--------------end------------------

benchmark on Bandwidth of Pipe
===================================
bandwidth of pipe: 1956.343357 MB/s

--------------end------------------
```
