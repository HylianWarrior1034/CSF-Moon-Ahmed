// C implementation of hexdump main function

#include "hexfuncs.h" // this is the only header file which may be included!

int main(void)
{
  // TODO: implement the main function
  unsigned offset = 0;
  char data_buf[17];
  while (hex_read(data_buf) > 0)
  {
  }
}
