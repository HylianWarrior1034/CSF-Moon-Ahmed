/* Hexdump main function, assembly language version */

	.section .text

	.globl main
main:
	/* TODO: add your code here */
	/* Allocating memory for three different char arrays */
	subq $17, %rsp
	mov %rsp, %r12
	subq $9, %rsp
	mov %rsp, %r13
	subq $3, %rsp
	mov %rsp, %r14
	
	/* put other stuff here */

	addq $3, %rsp
	addq $9, %rsp
	addq $17, %rsp
	ret

/* vim:ft=gas:
 */
