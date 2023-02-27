// C implementation of hexdump main function

#include "hexfuncs.h" // this is the only header file which may be included!

int main(void)
{

  // number of read bytes
  unsigned offset = 0;

  // char array to store the input stream
  char data_buf[17];

  // char array to store the offset hex value
  char offset2[16];

  // char array to store the unsigned char value of characters
  char asciival[3];

  // char arrays for formatting
  char space[2] = {' ', '\0'};
  char colon[3] = {':', ' ', '\0'};

  // read from stdin until nothing is being read (reached the end)
  while (1)
  {
    // read the data stream, nread is the number of bytes read
    unsigned nread = hex_read(data_buf);

    if (nread == 0)
    {
      break;
    }

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

    for (int i = 0; i < nread; i++) {
      data_buf[i] = hex_to_printable(data_buf[i]);
    }

    // null terminator and newline for data_buf
    data_buf[nread] = '\n';
    data_buf[nread + 1] = '\0';
    hex_write_string(data_buf);
    offset += 16;
  }
}
