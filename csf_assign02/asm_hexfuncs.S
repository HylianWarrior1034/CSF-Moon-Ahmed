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
	pushq %r12
	pushq %r13
	pushq %r14
	
	movq %rdi, %r12
	movq $2, %r13
	
	movb $0, (%rsi,%r13,1)
	decq %r13
	jmp .FormatByteLoop

.FormatByteLoop:
	cmp $0, %r12 
	je .FormatByteFinish
	
	movq $0, %rdx
	movq %r12, %rax
	movq $16, %r14
	divq %r14

	cmp $10, %rdx
	jge .RemainderNotLessThanTen
	addq $48, %rdx
	jmp .RemainderIfEnd

.RemainderNotLessThanTen:
	addq $87, %rdx

.RemainderIfEnd:
	movb %dl, (%rsi,%r13,1)
	decq %r13
	shr $4, %r12
	jmp .FormatByteLoop

.FormatByteFinish:
	popq %r14
	popq %r13
	popq %r12
	ret

	.globl hex_to_printable
hex_to_printable:
	/* TODO: implement */
	ret

/* vim:ft=gas:
 */
