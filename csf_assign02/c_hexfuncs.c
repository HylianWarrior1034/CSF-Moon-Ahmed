// C implementation of hexdump functions

#include <unistd.h> // this is the only system header file you may include!
#include "hexfuncs.h"

// TODO: add function implementations here
// Read up to 16 bytes from standard input into data_buf.
// Returns the number of characters read.
unsigned hex_read(char data_buf[])
{
    unsigned nread;
    nread = read(0, data_buf, 16);
    return nread;
}

// Write given nul-terminated string to standard output.
void hex_write_string(const char s[])
{
    write(1, s, 16);
}

// Format an unsigned value as an offset string consisting of exactly 8
// hex digits.  The formatted offset is stored in sbuf, which must
// have enough room for a string of length 8.
void hex_format_offset(unsigned offset, char sbuf[])
{
}

// Format a byte value (in the range 0-255) as string consisting
// of two hex digits.  The string is stored in sbuf.
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[])
{
}
// Convert a byte value (in the range 0-255) to a printable character
// value.  If byteval is already a printable character, it is returned
// unmodified.  If byteval is not a printable character, then the
// ASCII code for '.' should be returned.
char hex_to_printable(unsigned char byteval)
{
    if (byteval < 32 || byteval > 126)
    {
        return 46;
    }

    return byteval;
}