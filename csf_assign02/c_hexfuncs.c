// C implementation of hexdump functions

#include <unistd.h> // this is the only system header file you may include!
#include "hexfuncs.h"

// TODO: add function implementations here
// Read up to 16 bytes from standard input into data_buf.
// Returns the number of characters read.
unsigned hex_read(char data_buf[])
{
    unsigned nread;
    nread = (unsigned)read(0, data_buf, 16); // read and copy up to 16 bytes from stdin
    return nread;                            // return number of bytes read
}

// Write given nul-terminated string to standard output.
void hex_write_string(const char s[])
{
    int length = 0;
    while (s[length] != '\0') // find total length of s
    {
        length++;
    }
    write(1, s, length); // write s to stdout
}

// Format an unsigned value as an offset string consisting of exactly 8
// hex digits.  The formatted offset is stored in sbuf, which must
// have enough room for a string of length 8.
void hex_format_offset(unsigned offset, char sbuf[])
{
    for (int i = 0; i < 8; i++)
    {
        sbuf[i] = '0'; // fill sbuf with '0'
    }
    unsigned temp;
    int j = 7;
    while (offset != 0)
    {
        temp = offset % 16;
        if (temp < 10)
        {
            temp += 48;
        }
        else
        {
            temp += 87;
        }
        sbuf[j] = temp; // fill in the string starting from the rightmost value
        j--;
        offset >>= 4; // divide by 16
    }
    sbuf[8] = '\0'; // null terminate
}

// Format a byte value (in the range 0-255) as string consisting
// of two hex digits.  The string is stored in sbuf.
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[])
{
    unsigned byteval_temp = byteval;
    unsigned temp;
    sbuf[0] = '0';
    sbuf[1] = '0';
    sbuf[2] = '\0';
    int j = 1;
    while (byteval_temp != 0)
    {
        temp = byteval_temp % 16;
        if (temp < 10)
        {
            temp += 48;
        }
        else
        {
            temp += 87;
        }
        sbuf[j] = temp;
        j--;
        byteval_temp >>= 4;
    }
}
// Convert a byte value (in the range 0-255) to a printable character
// value.  If byteval is already a printable character, it is returned
// unmodified.  If byteval is not a printable character, then the
// ASCII code for '.' should be returned.
char hex_to_printable(unsigned char byteval)
{
    if (byteval < 32 || byteval > 126)
    {
        return 46; // return unsigned char value for '.'
    }

    return byteval; // return bytevalue
}
