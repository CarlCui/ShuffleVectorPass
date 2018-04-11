	.text
	.file	"merge-unalign.ll"
	.globl	merge
	.align	16, 0x90
	.type	merge,@function
merge:                                  # @merge
	.cfi_startproc
# BB#0:                                 # %entry
	pushq	%rbp
.Ltmp0:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp1:
	.cfi_def_cfa_offset 24
.Ltmp2:
	.cfi_offset %rbx, -24
.Ltmp3:
	.cfi_offset %rbp, -16
	movb	24(%rsp), %r10b
	movb	32(%rsp), %r11b
	movb	40(%rsp), %bpl
	movb	48(%rsp), %bl
	movb	56(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	andb	$31, %r9b
	movb	%r9b, (%rdi)
	andb	$31, %bl
	movb	%bl, (%rdi)
	andb	$31, %r8b
	movb	%r8b, (%rdi)
	andb	$31, %bpl
	movb	%bpl, (%rdi)
	andb	$31, %cl
	movb	%cl, (%rdi)
	andb	$31, %r11b
	movb	%r11b, (%rdi)
	andb	$31, %dl
	movb	%dl, (%rdi)
	andb	$31, %r10b
	movb	%r10b, (%rdi)
	andb	$31, %sil
	movb	%sil, (%rdi)
	movq	%rdi, %rax
	popq	%rbx
	popq	%rbp
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
.Ltmp4:
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
