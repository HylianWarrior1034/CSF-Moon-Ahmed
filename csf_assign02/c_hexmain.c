// C implementation of hexdump main function

#include "hexfuncs.h" // this is the only header file which may be included!

int main(void)
{
  unsigned offset = 0;
  char data_buf[17];
  char offset2[16];
  char asciival[3];
  unsigned nread = hex_read(data_buf);
  while (nread > 0)
  {
    hex_format_offset(offset, offset2);
    hex_write_string(offset2);
    hex_write_string(': ');

    for (int i = 0; i < nread; i++)
    {
      hex_format_byte_as_hex(data_buf[i], asciival);
      hex_write_string(asciival);
      hex_write_string(' ');
    }
    hex_write_string(' ');
    hex_write_string(data_buf);
    hex_write_string('\n');
    nread = hex_read(data_buf);
    offset += 16;
  }
}