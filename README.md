

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



