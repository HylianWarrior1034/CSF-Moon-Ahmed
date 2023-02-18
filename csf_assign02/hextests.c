// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct
{
  char test_data_1[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void)
{
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs)
{
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatOffsetSpecial(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testFormatByteAsHexSpecial(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);
void testHexToPrintableSpecial(TestObjs *objs);

int main(int argc, char **argv)
{
  if (argc > 1)
  {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatOffsetSpecial);
  TEST(testFormatByteAsHex);
  TEST(testFormatByteAsHexSpecial);
  TEST(testHexToPrintable);
  TEST(testHexToPrintableSpecial);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs)
{
  (void)objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(0x00000001u, buf);
  ASSERT(0 == strcmp(buf, "00000001"));

  hex_format_offset(0xabcd1234u, buf);
  ASSERT(0 == strcmp(buf, "abcd1234"));

  hex_format_offset(16u, buf);
  ASSERT(0 == strcmp(buf, "00000010"));
}

void testFormatOffsetSpecial(TestObjs *objs)
{
  (void)objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(0x00000000u, buf);
  ASSERT(0 == strcmp(buf, "00000000"));

  hex_format_offset(0u, buf);
  ASSERT(0 == strcmp(buf, "00000000"));

  hex_format_offset(0xffffffffu, buf);
  ASSERT(0 == strcmp(buf, "ffffffff"));
}

void testFormatByteAsHex(TestObjs *objs)
{
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));

  hex_format_byte_as_hex('e', buf);
  ASSERT(0 == strcmp(buf, "65"));
}

void testFormatByteAsHexSpecial(TestObjs *objs)
{
  char buf[16];
  hex_format_byte_as_hex(' ', buf);
  ASSERT(0 == strcmp(buf, "20"));

  hex_format_byte_as_hex('!', buf);
  ASSERT(0 == strcmp(buf, "21"));

  hex_format_byte_as_hex(']', buf);
  ASSERT(0 == strcmp(buf, "5D"));
}

void testHexToPrintable(TestObjs *objs)
{
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('e' == hex_to_printable(objs->test_data_1[1]));
}

void testHexToPrintableSpecial(TestObjs *objs)
{
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
  ASSERT('.' == hex_to_printable('/n'));
}
