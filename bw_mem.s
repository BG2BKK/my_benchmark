	.file	"bw_mem.c"
	.globl	TEMP
	.bss
	.align 4
	.type	TEMP, @object
	.size	TEMP, 4
TEMP:
	.zero	4
	.section	.rodata
.LC0:
	.string	"memory malloc error\n"
	.text
	.globl	do_mem_prepare
	.type	do_mem_prepare, @function
do_mem_prepare:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	40(%rax), %rax
	cmpq	$1073741823, %rax
	ja	.L2
	movq	-24(%rbp), %rax
	movq	40(%rax), %rcx
	movl	$1073741824, %eax
	movl	$0, %edx
	divq	%rcx
	movl	%eax, -4(%rbp)
	movl	$1073741824, %eax
	cltd
	idivl	-4(%rbp)
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 40(%rax)
	movq	-24(%rbp), %rax
	movl	-4(%rbp), %edx
	movl	%edx, 48(%rax)
	jmp	.L3
.L2:
	movq	-24(%rbp), %rax
	movl	$1, 48(%rax)
.L3:
	movq	-24(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 32(%rax)
	movq	-24(%rbp), %rax
	movq	32(%rax), %rax
	testq	%rax, %rax
	jne	.L4
	movl	$.LC0, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L4:
	movq	-24(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 24(%rax)
	movq	-24(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	jne	.L5
	movl	$.LC0, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L5:
	movq	-24(%rbp), %rax
	movq	40(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	32(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	bzero
	movq	-24(%rbp), %rax
	movq	40(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	bzero
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	do_mem_prepare, .-do_mem_prepare
	.globl	do_mem_clean
	.type	do_mem_clean, @function
do_mem_clean:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	32(%rax), %rax
	testq	%rax, %rax
	je	.L7
	movq	-8(%rbp), %rax
	movq	32(%rax), %rax
	movq	%rax, %rdi
	call	free
.L7:
	movq	-8(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L8
	movq	-8(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rax, %rdi
	call	free
.L8:
	movq	-8(%rbp), %rax
	movl	$0, 48(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 40(%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	do_mem_clean, .-do_mem_clean
	.globl	do_mem_rd
	.type	do_mem_rd, @function
do_mem_rd:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	.cfi_offset 13, -24
	.cfi_offset 12, -32
	.cfi_offset 3, -40
	movl	%edi, -28(%rbp)
	movq	%rsi, -40(%rbp)
	movl	$0, %r12d
	movq	-40(%rbp), %rax
	movq	32(%rax), %rbx
	movq	-40(%rbp), %rax
	movq	32(%rax), %rdx
	movq	-40(%rbp), %rax
	movq	40(%rax), %rax
	leaq	(%rdx,%rax), %r13
	jmp	.L10
.L12:
	movl	(%rbx), %edx
	leaq	16(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	32(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	48(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	64(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	80(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	96(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	112(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	128(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	144(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	160(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	176(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	192(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	208(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	224(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	240(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	256(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	272(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	288(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	304(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	320(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	336(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	352(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	368(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	384(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	400(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	416(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	432(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	448(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	464(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	480(%rbx), %rax
	movl	(%rax), %eax
	addl	%eax, %edx
	leaq	496(%rbx), %rax
	movl	(%rax), %eax
	addl	%edx, %eax
	addl	%eax, %r12d
	addq	$512, %rbx
.L11:
	cmpq	%r13, %rbx
	jb	.L12
	movl	TEMP(%rip), %eax
	addl	%r12d, %eax
	movl	%eax, TEMP(%rip)
.L10:
	movl	-28(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -28(%rbp)
	testl	%eax, %eax
	jne	.L11
	nop
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	do_mem_rd, .-do_mem_rd
	.globl	get_bw_mem
	.type	get_bw_mem, @function
get_bw_mem:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movq	%rdi, -104(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$40, -84(%rbp)
	movl	-84(%rbp), %eax
	sarl	$2, %eax
	movl	%eax, -80(%rbp)
	movl	-84(%rbp), %eax
	cltq
	salq	$4, %rax
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -72(%rbp)
	movl	-84(%rbp), %eax
	movl	%eax, -88(%rbp)
	movq	-104(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L16
	movq	-104(%rbp), %rax
	movq	(%rax), %rdx
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	*%rdx
	jmp	.L16
.L17:
	movq	-104(%rbp), %rax
	movl	48(%rax), %eax
	movl	%eax, -76(%rbp)
	leaq	-48(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	clock_gettime
	movq	-104(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-104(%rbp), %rcx
	movl	-76(%rbp), %eax
	movq	%rcx, %rsi
	movl	%eax, %edi
	movl	$0, %eax
	call	*%rdx
	leaq	-32(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	clock_gettime
	leaq	-32(%rbp), %rdx
	leaq	-48(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	get_total_us
	movq	%rax, -64(%rbp)
	movq	-104(%rbp), %rax
	movq	40(%rax), %rdx
	movq	-104(%rbp), %rax
	movl	48(%rax), %eax
	cltq
	movq	%rdx, %rcx
	imulq	%rax, %rcx
	movq	-64(%rbp), %rdx
	movl	-88(%rbp), %esi
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	save_bw_result
.L16:
	movl	-88(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -88(%rbp)
	testl	%eax, %eax
	jne	.L17
	movq	-104(%rbp), %rax
	movq	16(%rax), %rax
	testq	%rax, %rax
	je	.L18
	movq	-104(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	*%rdx
.L18:
	movl	-84(%rbp), %eax
	movslq	%eax, %rsi
	movq	-72(%rbp), %rax
	movl	$cmp_bw_result, %ecx
	movl	$16, %edx
	movq	%rax, %rdi
	call	qsort
	movl	-80(%rbp), %eax
	cltq
	salq	$4, %rax
	movq	%rax, %rdx
	movq	-72(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rdx
	movl	-80(%rbp), %eax
	cltq
	salq	$4, %rax
	movq	%rax, %rcx
	movq	-72(%rbp), %rax
	addq	%rcx, %rax
	movq	8(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	get_bandwidth
	movq	%xmm0, %rax
	movq	%rax, -56(%rbp)
	movsd	-56(%rbp), %xmm0
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L20
	call	__stack_chk_fail
.L20:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	get_bw_mem, .-get_bw_mem
	.section	.rodata
	.align 8
.LC1:
	.string	"blocksize: %7ldKB\tbandwidth of mem read: %lf MB/s\n"
	.text
	.globl	bench_bandwidth_mem
	.type	bench_bandwidth_mem, @function
bench_bandwidth_mem:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$do_mem_prepare, -64(%rbp)
	movq	$do_mem_rd, -56(%rbp)
	movq	$do_mem_clean, -48(%rbp)
	movq	$4096, -72(%rbp)
	jmp	.L22
.L23:
	movq	-72(%rbp), %rax
	movq	%rax, -24(%rbp)
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	get_bw_mem
	movq	-72(%rbp), %rax
	shrq	$10, %rax
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$1, %eax
	call	printf
	salq	-72(%rbp)
.L22:
	cmpq	$268435455, -72(%rbp)
	jbe	.L23
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L24
	call	__stack_chk_fail
.L24:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	bench_bandwidth_mem, .-bench_bandwidth_mem
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.2) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
