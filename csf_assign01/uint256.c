#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uint256.h"

// Create a UInt256 value from a single uint64_t value.
// Only the least-significant 64 bits are initialized directly,
// all other bits are set to 0.
UInt256 uint256_create_from_u64(uint64_t val) {
  UInt256 result;

  result.data[0] = val; // setting the least significant 64 bits to the given input
  result.data[1] = (uint64_t) 0; // setting the rest of the bits to 0
  result.data[2] = (uint64_t) 0;
  result.data[3] = (uint64_t) 0;
  
  return result;
}

// Create a UInt256 value from an array of 4 uint64_t values.
// The element at index 0 is the least significant, and the element
// at index 3 is the most significant.
UInt256 uint256_create(const uint64_t data[4]) {
  UInt256 result;

  result.data[0] = data[0]; // setting every bit to its respective value from the given input
  result.data[1] = data[1];
  result.data[2] = data[2];
  result.data[3] = data[3];
  
  return result;
}

// Create a UInt256 value from a string of hexadecimal digits.
UInt256 uint256_create_from_hex(const char *hex) {
  UInt256 result;

  char* new_hex = (char*) malloc(sizeof(char) * 65);

  for (int i = 0; i < (int) 64; i++) {
    new_hex[i] = '0';
  }

  for (int i = 0; i < (int)strlen(hex); i++) {
    new_hex[64-strlen(hex)+i] = hex[i];
  }
  
  for (int i = 0; i < 4; i++) {
    char* temp = malloc(16);
    char* cpy = new_hex;
    strncpy(temp, cpy+(i*16), 16);
    char* end;
    result.data[3-i] = strtoul(temp, &end, 16);
    free(temp);
  }
  
  free(new_hex);
  
  return result;
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex(UInt256 val) {
  char* hex = malloc(65);
  hex += 2;
  // TODO: implement
  for (int i = 0; i < 4; i++) {
    char temp[17];
    sprintf(temp, "%016lx", val.data[i]);
    strcat(hex, temp);
  }
  
  while (strlen(hex) != 1) {
    if (*hex == '0') {
      hex++;
    } else {
      break;
    }
  }

  printf("%s\n", hex);
  return hex;
}

// Get 64 bits of data from a UInt256 value.
// Index 0 is the least significant 64 bits, index 3 is the most
// significant 64 bits.
uint64_t uint256_get_bits(UInt256 val, unsigned index) {
  uint64_t bits;

  bits = val.data[index]; // retrieving the least significant 64 bits from the given uint256 input
  
  return bits;
}

// Compute the sum of two UInt256 values.
UInt256 uint256_add(UInt256 left, UInt256 right) {
  UInt256 sum;

  sum.data[3] = 0;
  sum.data[2] = 0;
  sum.data[1] = 0;
  sum.data[0] = 0;

  int carry = 0;

  for (int i = 0; i < 4; i++) {
    uint64_t temp_sum; // create empty unint64 to store the temporary sum
    uint64_t one = uint256_get_bits(left, i);
    uint64_t two = uint256_get_bits(right, i);

    // adding the appropriate bits from the left and right values (as well as any bits overflowing from a previous addition)
    temp_sum = one + two + carry;
    // checking if an overflow occured
    if (temp_sum < one) {
      carry = 1;
    } else {
      carry = 0;
    }

    // setting corresponding bits of the overall sum to the appropiate values
    sum.data[i] = temp_sum;

    /*
    for (int j = 0; j < 64; j++) {
      
      int temp_int1 = one & 1;
      int temp_int2 = two & 1;
      //printf("%d %d %d\n", temp_int1, temp_int2, carry);
      temp_sum = temp_int1 + temp_int2 + carry;

      if (temp_sum == 3) {
        carry = 1;
        sum.data[i] |= 1 << j;
      } else if (temp_sum == 2) {
        carry = 1;
      } else if (temp_sum == 1) {
        carry = 0;
        sum.data[i] |= 1 << j;
      }
      
      one >>= 1;
      two >>= 1;
    }
    */
  }

  //printf("%lx %lx %lx %lx \n", sum.data[3], sum.data[2], sum.data[1], sum.data[0]);
  return sum;
}

// Compute the difference of two UInt256 values.
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  UInt256 result;
  // TODO: implement
  return result;
}

// Compute the product of two UInt256 values.
UInt256 uint256_mul(UInt256 left, UInt256 right) {
  UInt256 product;
  // TODO: implement
  return product;
}
