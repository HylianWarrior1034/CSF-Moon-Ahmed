/* Assembly language implementation of hexdump functions */

	.section .rodata

	.section .text

	.globl hex_read
hex_read:
	pushq %r12 /* Store previous values of any callee registers going to be used. */
	movq %rdi, %r12 /* Move address of the data_buf to %r12 */
	movl $0, %edi /* Copy the value 0 to %rdi (setting up to call read function). */
	movq %r12, %rsi /* Pass in address of data_buf to %rsi. */
	movq $16, %rdx /* Pass in maximum number of chars to read (16) to %rdx. */
	call read /* Call read in order to read from the IO input. */
	movb $0, (%r12,%rax,1) /* Put a null terminator at the end of the obtained chars. */
	popq %r12 /* Restore any used callee registers and return. */
	ret

	.globl hex_write_string
hex_write_string:
	pushq %r12 /* Store previous values of any callee registers going to be used. */
	movq $0, %r12 /* Assign 0 to %r12 to us it as a char counter. */
	
.lengthLoop:
	/* When the %r12th char in the char array is 0, exit the loop. */
	cmpb $0, (%rdi, %r12, 1)
	jle .writeString
	addq $1, %r12 /* If we have not yet reached the end of the char array, increment %r12 and jump to the top of the loop. */
	jmp .lengthLoop

.writeString:
	movq %rdi, %rsi /* Copy address of the char array to %rsi. */
	movl $1, %edi /* Copy 1 to %rdi to indicate standard output. */
	movq %r12, %rdx /* Copy the char array length to %rdx. */
	call write /* Call write to print out the char array. */
	popq %r12 /* Restore used callee registers and return. */
	ret

	.globl hex_format_offset
hex_format_offset:
	pushq %r14 	/* Store previous values of any callee registers going to be used. */
	movq $0, %r10   /* starting counter to fill sbuf with 0's */
	movq $7, %r11	/* counter counting down to fill sbuf */

.fillZero:
	cmpq $8, %r10 /* If loop has executed 8 times, exit loop. */
	jge .fillChar
	movb $48, (%rsi, %r10, 1) /* Copy the ascii value for 0 to the current index. */
	addq $1, %r10 /* Increment index and jump to the top of the loop. */
	jmp .fillZero

.fillChar:
	cmpq $0, %rdi /* If the inputted offset value is zero, exit loop. */
	jbe .finishFormat
	movq %rdi, %rax /* Copy offset value to %rax. */
	movq $0, %rdx /* Copy zero to %rdx. */
	movq $16, %r14 /* Copy 16 to %r14. */
	divq %r14 /* Divide value in %rdx:%rax by value in %r14 (offset divided by 16). */

	cmpq $10, %rdx /* Compare the remainder of the division to 10. */
	jb   .remLessThanTen
	/* If the remainder is greater than or equal to 10, increment the remainder by 87. */
	addq $87, %rdx
	movb %dl, (%rsi, %r11, 1) /* Put value of the remainder as the current char of the offset char array. */
	subq $1, %r11 /* Decrement char. */
	shr $4, %rdi /* Shift the bits in the offset value to the right by 4. */
	jmp .fillChar /* Jump to top of loop. */
	
.remLessThanTen:
	/* If remainder is less than 10, increment it by 48. */
	addq $48, %rdx
	movb %dl, (%rsi, %r11, 1) /* Put value of the remainder as the current char of the offset char array. */
	subq $1, %r11 /* Decrement char. */
	shr $4, %rdi /* Shift the bits in the offset value to the right by 4. */
	jmp .fillChar /* Jump to top of loop. */
	
.finishFormat:
	/* Setr the last character in the offset char array to a null terminator. */
	movq $8, %r11
	movb $0, (%rsi, %r11, 1)
	popq %r14 /* Resotre callee registers and return. */
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
	
	/* Move appropriate values to %rdx, %rax, and %r14 in order to set up for a divq call. */
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
