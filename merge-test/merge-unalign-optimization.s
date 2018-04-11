	.text
	.file	"merge-unalign-optimization.ll"
	.section	.rodata.cst16,"aM",@progbits,16
	.align	16
.LCPI0_0:
	.short	31                      # 0x1f
	.short	31                      # 0x1f
	.short	31                      # 0x1f
	.short	31                      # 0x1f
	.short	31                      # 0x1f
	.short	31                      # 0x1f
	.short	31                      # 0x1f
	.short	31                      # 0x1f
.LCPI0_1:
	.short	32                      # 0x20
	.short	32                      # 0x20
	.short	32                      # 0x20
	.short	32                      # 0x20
	.short	32                      # 0x20
	.short	1                       # 0x1
	.short	1                       # 0x1
	.short	1                       # 0x1
	.text
	.globl	merge
	.align	16, 0x90
	.type	merge,@function
merge:                                  # @merge
	.cfi_startproc
# BB#0:                                 # %entry
	movd	32(%rsp), %xmm0         # xmm0 = mem[0],zero,zero,zero
	movd	16(%rsp), %xmm1         # xmm1 = mem[0],zero,zero,zero
	punpcklwd	%xmm0, %xmm1    # xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1],xmm1[2],xmm0[2],xmm1[3],xmm0[3]
	movd	40(%rsp), %xmm0         # xmm0 = mem[0],zero,zero,zero
	movd	8(%rsp), %xmm2          # xmm2 = mem[0],zero,zero,zero
	punpcklwd	%xmm0, %xmm2    # xmm2 = xmm2[0],xmm0[0],xmm2[1],xmm0[1],xmm2[2],xmm0[2],xmm2[3],xmm0[3]
	movd	24(%rsp), %xmm0         # xmm0 = mem[0],zero,zero,zero
	punpcklwd	%xmm0, %xmm2    # xmm2 = xmm2[0],xmm0[0],xmm2[1],xmm0[1],xmm2[2],xmm0[2],xmm2[3],xmm0[3]
	punpcklwd	%xmm1, %xmm2    # xmm2 = xmm2[0],xmm1[0],xmm2[1],xmm1[1],xmm2[2],xmm1[2],xmm2[3],xmm1[3]
	movd	%r8d, %xmm0
	movd	%edx, %xmm1
	punpcklwd	%xmm0, %xmm1    # xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1],xmm1[2],xmm0[2],xmm1[3],xmm0[3]
	movd	%r9d, %xmm3
	movd	%esi, %xmm0
	punpcklwd	%xmm3, %xmm0    # xmm0 = xmm0[0],xmm3[0],xmm0[1],xmm3[1],xmm0[2],xmm3[2],xmm0[3],xmm3[3]
	movd	%ecx, %xmm3
	punpcklwd	%xmm3, %xmm0    # xmm0 = xmm0[0],xmm3[0],xmm0[1],xmm3[1],xmm0[2],xmm3[2],xmm0[3],xmm3[3]
	punpcklwd	%xmm1, %xmm0    # xmm0 = xmm0[0],xmm1[0],xmm0[1],xmm1[1],xmm0[2],xmm1[2],xmm0[3],xmm1[3]
	movdqa	.LCPI0_0(%rip), %xmm1   # xmm1 = [31,31,31,31,31,31,31,31]
	pand	%xmm1, %xmm0
	pand	%xmm1, %xmm2
	pmullw	.LCPI0_1(%rip), %xmm0
	por	%xmm2, %xmm0
	pextrw	$4, %xmm0, %eax
	andl	$1023, %eax             # imm = 0x3FF
	movw	%ax, 4(%rdi)
	pextrw	$3, %xmm0, %eax
	andl	$1023, %eax             # imm = 0x3FF
	movw	%ax, 3(%rdi)
	pextrw	$2, %xmm0, %eax
	andl	$1023, %eax             # imm = 0x3FF
	movw	%ax, 2(%rdi)
	pextrw	$1, %xmm0, %eax
	andl	$1023, %eax             # imm = 0x3FF
	movw	%ax, 1(%rdi)
	movd	%xmm0, %eax
	andl	$1023, %eax             # imm = 0x3FF
	movw	%ax, (%rdi)
	movq	%rdi, %rax
	retq
.Lfunc_end0:
	.size	merge, .Lfunc_end0-merge
	.cfi_endproc

	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %entry
	subq	$216, %rsp
.Ltmp0:
	.cfi_def_cfa_offset 224
	movl	$4, 32(%rsp)
	movl	$3, 24(%rsp)
	movl	$2, 16(%rsp)
	movl	$1, 8(%rsp)
	movl	$0, (%rsp)
	leaq	192(%rsp), %rdi
	movl	$0, %esi
	movl	$1, %edx
	movl	$2, %ecx
	movl	$3, %r8d
	movl	$4, %r9d
	callq	merge
	movl	$4, 32(%rsp)
	movl	$3, 24(%rsp)
	movl	$2, 16(%rsp)
	movl	$1, 8(%rsp)
	movl	$0, (%rsp)
	leaq	176(%rsp), %rdi
	movl	$0, %esi
	movl	$1, %edx
	movl	$2, %ecx
	movl	$3, %r8d
	movl	$4, %r9d
	callq	merge
	movl	$4, 32(%rsp)
	movl	$3, 24(%rsp)
	movl	$2, 16(%rsp)
	movl	$1, 8(%rsp)
	movl	$0, (%rsp)
	leaq	160(%rsp), %rdi
	movl	$0, %esi
	movl	$1, %edx
	movl	$2, %ecx
	movl	$3, %r8d
	movl	$4, %r9d
	callq	merge
	movl	$4, 32(%rsp)
	movl	$3, 24(%rsp)
	movl	$2, 16(%rsp)
	movl	$1, 8(%rsp)
	movl	$0, (%rsp)
	leaq	144(%rsp), %rdi
	movl	$0, %esi
	movl	$1, %edx
	movl	$2, %ecx
	movl	$3, %r8d
	movl	$4, %r9d
	callq	merge
	movl	$4, 32(%rsp)
	movl	$3, 24(%rsp)
	movl	$2, 16(%rsp)
	movl	$1, 8(%rsp)
	movl	$0, (%rsp)
	leaq	128(%rsp), %rdi
	movl	$0, %esi
	movl	$1, %edx
	movl	$2, %ecx
	movl	$3, %r8d
	movl	$4, %r9d
	callq	merge
	movl	$4, 32(%rsp)
	movl	$3, 24(%rsp)
	movl	$2, 16(%rsp)
	movl	$1, 8(%rsp)
	movl	$0, (%rsp)
	leaq	112(%rsp), %rdi
	movl	$0, %esi
	movl	$1, %edx
	movl	$2, %ecx
	movl	$3, %r8d
	movl	$4, %r9d
	callq	merge
	movl	$4, 32(%rsp)
	movl	$3, 24(%rsp)
	movl	$2, 16(%rsp)
	movl	$1, 8(%rsp)
	movl	$0, (%rsp)
	leaq	96(%rsp), %rdi
	movl	$0, %esi
	movl	$1, %edx
	movl	$2, %ecx
	movl	$3, %r8d
	movl	$4, %r9d
	callq	merge
	movl	$4, 32(%rsp)
	movl	$3, 24(%rsp)
	movl	$2, 16(%rsp)
	movl	$1, 8(%rsp)
	movl	$0, (%rsp)
	leaq	80(%rsp), %rdi
	movl	$0, %esi
	movl	$1, %edx
	movl	$2, %ecx
	movl	$3, %r8d
	movl	$4, %r9d
	callq	merge
	movl	$4, 32(%rsp)
	movl	$3, 24(%rsp)
	movl	$2, 16(%rsp)
	movl	$1, 8(%rsp)
	movl	$0, (%rsp)
	leaq	64(%rsp), %rdi
	movl	$0, %esi
	movl	$1, %edx
	movl	$2, %ecx
	movl	$3, %r8d
	movl	$4, %r9d
	callq	merge
	movl	$4, 32(%rsp)
	movl	$3, 24(%rsp)
	movl	$2, 16(%rsp)
	movl	$1, 8(%rsp)
	movl	$0, (%rsp)
	leaq	48(%rsp), %rdi
	movl	$0, %esi
	movl	$1, %edx
	movl	$2, %ecx
	movl	$3, %r8d
	movl	$4, %r9d
	callq	merge
	xorl	%eax, %eax
	addq	$216, %rsp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc

	.type	.Lstr,@object           # @str
	.section	.rodata,"a",@progbits
.Lstr:
	.ascii	"%d "
	.size	.Lstr, 3


	.section	".note.GNU-stack","",@progbits
