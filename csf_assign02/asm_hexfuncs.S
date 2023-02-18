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
	/* Saving the contents of the callee registers that we'll be using in this function. */
	pushq %r12
	pushq %r13
	pushq %r14
	
	/* Move the first parameter into %r12. */
	movq %rdi, %r12
	/* Assign a value to %r13 to use it as an index for the char array. */
	movq $0, %r13
	
	/* Put null terminator at the end of the char array and decrement our index. */
	movb $48, (%rsi,%r13,1)
	movq $1, %r13
	movb $48, (%rsi,%r13,1)
	movq $2, %r13
	movb $0, (%rsi,%r13,1)
	decq %r13
	/* Start a while loop. */
	jmp .FormatByteLoop

.FormatByteLoop:
	/* Loop will exit when %r12, the register containing the value of the first parameter, becomes zero. */
	cmp $0, %r12 
	je .FormatByteFinish
	
	/* Move appropraite values to %rdx, %rax, and %r14 in order to set up for a divq call. */
	movq $0, %rdx
	movq %r12, %rax
	movq $16, %r14
	/* Divide The value in %rax (which was moved from %r12) by 16. */
	divq %r14

	/* Get the remainder of the previous division and compare it to 10. */
	cmp $10, %rdx
	jge .RemainderNotLessThanTen
	addq $48, %rdx /* If remainder is less than ten, add 48 to it. */
	jmp .RemainderIfEnd

.RemainderNotLessThanTen:
	addq $87, %rdx /* If remainder greater than or equal to ten, add 87 to it. */

.RemainderIfEnd:
	/* Move the (last 8 bits) of the remainder into the current index of the char array. */
	movb %dl, (%rsi,%r13,1)
	decq %r13 /* Decrement the index. */
	shr $4, %r12 /* Shift the bits of %r12 to the right by 4 places. */
	jmp .FormatByteLoop /* Go back to the top of the loop. */

.FormatByteFinish:
	/* Once while loop exits, re-assign the previous contents of the callee registers we used and return from the program. */
	popq %r14
	popq %r13
	popq %r12
	ret

	.globl hex_to_printable
hex_to_printable:
	/* Saving the contents of the callee registers that we'll be using in this function. */
	pushq %r12

	/* Move the value of the char parameter into %r12. */
	movq %rdi, %r12

	/* If the value of the inputed char is less than 32 or greater than 126, then jump to .ConvertToPeriod. */
	cmp $32, %r12
	jl .ConvertToPeriod
	cmp $126, %r12
	jg .ConvertToPeriod
	
	/* The char is within the specified range, so we can return it as is and return from the function. */
	movq %r12, %rax
	popq %r12 /* Re-assign %r12 to its previous value. */
	ret
	
.ConvertToPeriod:
	/* The char is outside the specified range, so convert it to the value of a '.' character, return the value, and return from the function. */
	movq $46, %rax
	popq %r12 /* Re-assign %r12 to its previous value. */
	ret

/* vim:ft=gas:
 */
