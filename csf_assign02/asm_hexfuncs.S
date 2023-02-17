/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */

	.globl hex_read
hex_read:
	/* TODO: implement */
	ret

	.globl hex_write_string
hex_write_string:
	/* TODO: implement */
	ret

	.globl hex_format_offset
hex_format_offset:
	/* TODO: implement */
	ret

	.globl hex_format_byte_as_hex
hex_format_byte_as_hex:
	push %rbp
	movl %rsp, %rbp

	push %rsi
	push %rdi
	push %rbx

	subl 8, %rsp
	movl 8(%rbp), %rbx
	subl 24, %rsp
	movl 16(%rbp), %rdi
	movl '0', (%rdi,0)
	movl '0', (%rdi,1,8)
	movl '\0', (%rdi,2,8)
	push %r8
	movl 1, %r8


	.LoopTop:
		cmpl 0, %rbx
		je .LoopDone
		movl %rbx, %rdx
		movl 0, %rax
		divq 16
		movl %rdx, %rbx

		cmpl 10, %rdx
		jge .GreaterThanTen
		add 48, %rdx
		jmp .EndIf
		.GreaterThanTen:
		add 87, %rdx
		.EndIf:
		movl %rdx, (%rdi,%r8,8)
		decq %r8
		jmp .LoopTop
	.LoopDone:

	subl 32, %rsp
	pop %rbx
	pop %rdi
	pop %rsi
	ret

	.globl hex_to_printable
hex_to_printable:
	/* TODO: implement */
	ret

/* vim:ft=gas:
 */
