// C implementation of hexdump main function

#include "hexfuncs.h" // this is the only header file which may be included!

int main(void)
{
  unsigned offset = 0;
  char data_buf[17];
  char offset2[16];
  char asciival[3];
  char space[2] = {' ', '\0'};
  char newline[3] = {' ', '\n', '\0'};
  char colon[3] = {':', ' ', '\0'};
  unsigned nread = hex_read(data_buf);
  data_buf[17] = '\0';
  while (nread > 0)
  {
    hex_format_offset(offset, offset2);
    hex_write_string(offset2);
    hex_write_string(colon);

    for (int i = 0; i < 16; i++)
    {
      if (i < nread)
      {
        hex_format_byte_as_hex(data_buf[i], asciival);
        hex_write_string(asciival);
        hex_write_string(space);
      }
      else
      {
        hex_write_string(space);
        hex_write_string(space);
        hex_write_string(space);
      }
    }
    hex_write_string(space);
    hex_write_string(data_buf);
    hex_write_string(newline);
    nread = hex_read(data_buf);
    data_buf[17] = '\0';
    offset += 16;
  }
}
