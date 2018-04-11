	.text
	.file	"merge.ll"
	.globl	merge
	.align	16, 0x90
	.type	merge,@function
merge:                                  # @merge
	.cfi_startproc
# BB#0:                                 # %entry
	movdqa	%xmm0, %xmm2
	punpckldq	%xmm1, %xmm0    # xmm0 = xmm0[0],xmm1[0],xmm0[1],xmm1[1]
	punpckhdq	%xmm1, %xmm2    # xmm2 = xmm2[2],xmm1[2],xmm2[3],xmm1[3]
	movdqa	%xmm2, %xmm1
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
	pushq	%rbp
.Ltmp0:
	.cfi_def_cfa_offset 16
	pushq	%r15
.Ltmp1:
	.cfi_def_cfa_offset 24
	pushq	%r14
.Ltmp2:
	.cfi_def_cfa_offset 32
	pushq	%r13
.Ltmp3:
	.cfi_def_cfa_offset 40
	pushq	%r12
.Ltmp4:
	.cfi_def_cfa_offset 48
	pushq	%rbx
.Ltmp5:
	.cfi_def_cfa_offset 56
	subq	$40, %rsp
.Ltmp6:
	.cfi_def_cfa_offset 96
.Ltmp7:
	.cfi_offset %rbx, -56
.Ltmp8:
	.cfi_offset %r12, -48
.Ltmp9:
	.cfi_offset %r13, -40
.Ltmp10:
	.cfi_offset %r14, -32
.Ltmp11:
	.cfi_offset %r15, -24
.Ltmp12:
	.cfi_offset %rbp, -16
	movaps	16(%rsp), %xmm1
	xorps	%xmm0, %xmm0
	movss	%xmm0, %xmm1            # xmm1 = xmm0[0],xmm1[1,2,3]
	movl	$1, %eax
	movd	%eax, %xmm0
	shufps	$0, %xmm1, %xmm0        # xmm0 = xmm0[0,0],xmm1[0,0]
	shufps	$226, %xmm1, %xmm0      # xmm0 = xmm0[2,0],xmm1[2,3]
	movl	$2, %eax
	movd	%eax, %xmm1
	shufps	$48, %xmm0, %xmm1       # xmm1 = xmm1[0,0],xmm0[3,0]
	shufps	$132, %xmm1, %xmm0      # xmm0 = xmm0[0,1],xmm1[0,2]
	movl	$3, %eax
	movd	%eax, %xmm1
	shufps	$32, %xmm0, %xmm1       # xmm1 = xmm1[0,0],xmm0[2,0]
	shufps	$36, %xmm1, %xmm0       # xmm0 = xmm0[0,1],xmm1[2,0]
	movaps	%xmm0, %xmm1
	callq	merge
	movd	%xmm0, %esi
	pshufd	$229, %xmm0, %xmm2      # xmm2 = xmm0[1,1,2,3]
	movd	%xmm2, %ebp
	pshufd	$78, %xmm0, %xmm2       # xmm2 = xmm0[2,3,0,1]
	movd	%xmm2, %r14d
	pshufd	$231, %xmm0, %xmm0      # xmm0 = xmm0[3,1,2,3]
	movd	%xmm0, %ebx
	movd	%xmm1, %r13d
	pshufd	$229, %xmm1, %xmm0      # xmm0 = xmm1[1,1,2,3]
	movd	%xmm0, %r12d
	pshufd	$78, %xmm1, %xmm0       # xmm0 = xmm1[2,3,0,1]
	movd	%xmm0, %r15d
	pshufd	$231, %xmm1, %xmm0      # xmm0 = xmm1[3,1,2,3]
	movd	%xmm0, 12(%rsp)         # 4-byte Folded Spill
	movl	$.Lstr, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$.Lstr, %edi
	xorl	%eax, %eax
	movl	%ebp, %esi
	callq	printf
	movl	$.Lstr, %edi
	xorl	%eax, %eax
	movl	%r14d, %esi
	callq	printf
	movl	$.Lstr, %edi
	xorl	%eax, %eax
	movl	%ebx, %esi
	callq	printf
	movl	$.Lstr, %edi
	xorl	%eax, %eax
	movl	%r13d, %esi
	callq	printf
	movl	$.Lstr, %edi
	xorl	%eax, %eax
	movl	%r12d, %esi
	callq	printf
	movl	$.Lstr, %edi
	xorl	%eax, %eax
	movl	%r15d, %esi
	callq	printf
	movl	$.Lstr, %edi
	xorl	%eax, %eax
	movl	12(%rsp), %esi          # 4-byte Reload
	callq	printf
	xorl	%eax, %eax
	addq	$40, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
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
