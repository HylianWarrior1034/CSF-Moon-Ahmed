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

	/* create data for offset counter */
	movl $0, 0(%rbp)

	/* create data for char array containing a newline */
	movb $10, 4(%rbp)
	movb $0, 5(%rbp)

	/* create data for char array containing a colon */
	movb $58, 6(%rbp)
	movb $32, 7(%rbp)
	movb $0, 8(%rbp)

	/* create data for char array containing a space */
	movb $32, 9(%rbp)
	movb $0, 10(%rbp)

	/* Pass in the address where the data_buf will be stored. */
	leaq 23(%rbp), %rdi
	call hex_read /* Run hex_read to read input. */
	movl %eax, %r12d /* Save the number of chars read to %r12d. */

.printLoop:
	/* If no characters were read, we exit from the loop. */
	cmpl $0, %r12d
	je .printLoopExit

	/* Pass in offset value and the address containing the offset char array. */
	movl 0(%rbp), %edi
	leaq 14(%rbp), %rsi
	call hex_format_offset /* Run hex_format_offset to set up the offset char array. */

	/* Print the offset char array. */
	leaq 14(%rbp), %rdi
	call hex_write_string

	/* Print a colon and space. */
	leaq 6(%rbp), %rdi
	call hex_write_string

	/* Set %r13d to zero in order to serve as a loop counter and index. */
	movl $0, %r13d

.printHexLineLoop:
	/* After 16 iterations (when %r13d is 16) the loop will terminate. */
	cmpl $16, %r13d
	je .printHexLineLoopExit

	/* Check if the current loop exceeds the number of characters in the current line. */
	cmpl %r12d, %r13d
	jge .indexNotLessThanCharsRead /* Jump if %r13d is greater than or equal to the number of chars in the current line. */

	/* Pass in the current char and the address to store its ascii value (in hex form). */
	movb 23(%rbp,%r13,1), %dil
	leaq 11(%rbp), %rsi
	call hex_format_byte_as_hex /* Run hex_format_byte_as_hex. */

	/* Pas in the address to the ascii value hex digit. */
	leaq 11(%rbp), %rdi
	call hex_write_string /* Print out the ascii value. */

	/* Pass in the address of the space char array. */
	leaq 9(%rbp), %rdi
	call hex_write_string /* Print out a space. */
	jmp .indexNotLessThanCharsReadExit /* Jump to the appropriate line. */

.indexNotLessThanCharsRead:
	/* If we have exceeded the number of characters read on the current line, print three spaces. */
	leaq 9(%rbp), %rdi
	call hex_write_string
	leaq 9(%rbp), %rdi
	call hex_write_string
	leaq 9(%rbp), %rdi
	call hex_write_string

.indexNotLessThanCharsReadExit:
	incl %r13d /* Increment the index. */
	jmp .printHexLineLoop /* Jump to the top of the loop. */

.printHexLineLoopExit:
	/* Print a space. */
	leaq 9(%rbp), %rdi
	call hex_write_string

	/* Set %r13d to 0 again to use it as an index. */
	movl $0, %r13d

.turnToPrintableLoop:
	/* If %r13d is greather than or equal to %r12d (the number of characters read on the current line), then exit the loop. */
	cmpl %r12d, %r13d
	jge .turnToPrintableLoopExit

	/* Copy the data for the current character over to %dil anmd run hex_to_printable. */
	movb 23(%rbp,%r13,1), %dil
	call hex_to_printable

	/* Change the current character to the result of hex_to_printable. */
	movb %al, 23(%rbp,%r13,1)

	/* Increent %r13d and jump back to the top of the loop. */
	incl %r13d
	jmp .turnToPrintableLoop

.turnToPrintableLoopExit:
	/* After exiting the loop, pass in the data for the current line of characters to %rdi. */
	leaq 23(%rbp), %rdi
	call hex_write_string /* Run hex_write_string in order to print the current line of chars. */

	/* Pass in the data for character that is stored on the stack. */
	leaq 4(%rbp), %rdi
	call hex_write_string /* Run hex_write_string to print a newline. */

	/* Increment the offset counter by 16. */
	addl $16, 0(%rbp)

	/* Pass in the location of the data_buf to %rdi. */
	leaq 23(%rbp), %rdi
	call hex_read /* Run hex_read to read the next set of chars. */
	movl %eax, %r12d /* Copy the number of chars read to %r12d. */
	jmp .printLoop /* Jump to the top of the print loop. */
	
.printLoopExit:
	/* Upon exiting the print loop, unallocate all the memory allocated at the beginning and exit the program. */
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
