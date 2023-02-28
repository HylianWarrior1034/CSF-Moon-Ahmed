/* Hexdump main function, assembly language version */

	.section .text

	.globl main
main:
	/* TODO: add your code here */
	/* Allocating memory for six different char arrays */
	subq $17, %rsp
	subq $9, %rsp
	subq $3, %rsp
	subq $2, %rsp
	subq $3, %rsp
	subq $2, %rsp
	/* Allocating memory for an unsigned integer */
	subq $4, %rsp
	/* Moving %rbp to the current location of %rsp */
	movq %rsp, %rbp
	/* Allocating extra padding memory to make %rsp's address a multiple of 16. */
	subq $8, %rsp

	/* offset counter */
	movl $0, 0(%rbp)

	/* char array containing a newline */
	movb $10, 4(%rbp)
	movb $0, 5(%rbp)

	/* char array containing a colon */
	movb $58, 6(%rbp)
	movb $32, 7(%rbp)
	movb $0, 8(%rbp)

	/* char array containing a space */
	movb $32, 9(%rbp)
	movb $0, 10(%rbp)

	leaq 23(%rbp), %rdi
	call hex_read
	movl %eax, %r12d

.printLoop:
	cmpl $0, %r12d
	je .printLoopExit

	movl 0(%rbp), %edi
	leaq 14(%rbp), %rsi
	call hex_format_offset

	leaq 14(%rbp), %rdi
	call hex_write_string

	leaq 6(%rbp), %rdi
	call hex_write_string

	movl $0, %r13d

.printHexLineLoop:
	cmpl $16, %r13d
	je .printHexLineLoopExit

	cmpl %r12d, %r13d
	jge .indexNotLessThanCharsRead

	movb 23(%rbp,%r13,1), %dil
	leaq 11(%rbp), %rsi
	call hex_format_byte_as_hex

	leaq 11(%rbp), %rdi
	call hex_write_string

	leaq 9(%rbp), %rdi
	call hex_write_string
	jmp .indexNotLessThanCharsReadExit

.indexNotLessThanCharsRead:
	leaq 9(%rbp), %rdi
	call hex_write_string
	leaq 9(%rbp), %rdi
	call hex_write_string
	leaq 9(%rbp), %rdi
	call hex_write_string

.indexNotLessThanCharsReadExit:
	incl %r13d
	jmp .printHexLineLoop

.printHexLineLoopExit:
	leaq 9(%rbp), %rdi
	call hex_write_string

	movl $0, %r13d

.turnToPrintableLoop:
	cmpl %r12d, %r13d
	jge .turnToPrintableLoopExit

	movb 23(%rbp,%r13,1), %dil
	call hex_to_printable

	movb %al, 23(%rbp,%r13,1)

	incl %r13d
	jmp .turnToPrintableLoop

.turnToPrintableLoopExit:
	leaq 23(%rbp), %rdi
	call hex_write_string

	leaq 4(%rbp), %rdi
	call hex_write_string

	addl $16, 0(%rbp)

	leaq 23(%rbp), %rdi
	call hex_read
	movl %eax, %r12d
	jmp .printLoop
	
.printLoopExit:
	addq $10, %rsp
	addq $4, %rsp
	addq $3, %rsp
	addq $2, %rsp
	addq $3, %rsp
	addq $9, %rsp
	addq $17, %rsp
	ret

/* vim:ft=gas:
 */
