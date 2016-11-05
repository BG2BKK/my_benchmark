

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
