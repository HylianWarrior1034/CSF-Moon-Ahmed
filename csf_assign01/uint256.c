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

  // dynamically allocating memory for reading the string
  char* new_hex = (char*) malloc(sizeof(char) * 65);

  // initializing all characters in the string to '0'
  for (int i = 0; i < (int) 64; i++) {
    new_hex[i] = '0';
  }

  // assigning each char in the string to its corresponding value from hex
  for (int i = 0; i < (int)strlen(hex); i++) {
    new_hex[64-strlen(hex)+i] = hex[i];
  }
  
  // parsing the data that was moved to new_hex in order to create the UInt256 value
  for (int i = 0; i < 4; i++) {
    char* temp = malloc(16);
    char* cpy = new_hex;
    strncpy(temp, cpy+(i*16), 16);
    char* end;
    result.data[3-i] = strtoul(temp, &end, 16);
    free(temp);
  }
  
  // freeing the dynamically allocated memory
  free(new_hex);
  
  return result;
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex(UInt256 val) {
  char *hex = malloc(sizeof(char) * 65); // allocating memory for the hex string
  
  // copying the data from val into our hex string
  sprintf(hex, "%016lx%016lx%016lx%016lx", val.data[3], val.data[2], val.data[1], val.data[0]);

  // loop until all of the leading zeros in the hex string are gone
  while (strlen(hex) != 1) {
    if (*hex == '0') {
      char *temp = malloc(sizeof(char) * (strlen(hex) + 1));
      sprintf(temp, "%s", &hex[1]);
      free(hex);
      hex = temp;
    } else {
      break;
    }
  }
  
  return hex;
}

// Get 64 bits of data from a UInt256 value.
// Index 0 is the least significant 64 bits, index 3 is the most
// significant 64 bits.
uint64_t uint256_get_bits(UInt256 val, unsigned index) {
  uint64_t bits;

  bits = val.data[index]; // retrieving the specified 64 bits from the given uint256 input
  
  return bits;
}

// Compute the sum of two UInt256 values.
UInt256 uint256_add(UInt256 left, UInt256 right) {
  UInt256 sum;

  // set the sum equal to zero
  sum.data[3] = 0UL;
  sum.data[2] = 0UL;
  sum.data[1] = 0UL;
  sum.data[0] = 0UL;

  // initialize a variable to be used to keep track of overflows
  int carry = 0;

  for (int i = 0; i < 4; i++) {
    uint64_t temp_sum; // create empty unint64 to store the temporary sum
    uint64_t one = uint256_get_bits(left, i);
    uint64_t two = uint256_get_bits(right, i);

    // adding the appropriate bits from the left and right values (as well as any bits overflowing from a previous addition)
    temp_sum = one + two + carry;
    // checking if an overflow occured
    if (temp_sum < one || temp_sum < two) {
      carry = 1;
    } else {
      carry = 0;
    }

    // setting corresponding bits of the overall sum to the appropiate values
    sum.data[i] = temp_sum;
  }

  return sum;
}

// Compute the negation of a UInt256 value.
UInt256 uint256_negate(UInt256 val) {
  UInt256 result;
  UInt256 one; // create UInt256 value equivalent to 1 (used for the last part of the negation)
  one.data[0] = 1UL;
  one.data[1] = 0UL;
  one.data[2] = 0UL;
  one.data[3] = 0UL;

  // invert all the bits of the input and stor it into the result
  for (int i = 0; i < 4; i++) {
    result.data[i] = ~(val.data[i]);
  }
  result = uint256_add(result, one); // add one to the result to complete the negation process
  
  return result;
}

// Compute the difference of two UInt256 values.
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  // add the left operand and the negation of the right operand
  return uint256_add(left, uint256_negate(right));
}

// Compute the product of two UInt256 values.
UInt256 uint256_mul(UInt256 left, UInt256 right) {
  UInt256 product; // set product equal to zero
  product.data[0] = 0UL;
  product.data[1] = 0UL;
  product.data[2] = 0UL;
  product.data[3] = 0UL;

  // create a 64-bit integer equivalent to 1 (for bit-wise operations)
  uint64_t one = 1UL;

  // For every set bit in the left operand that is set, the right operand 
  // will be shifted left the number of times equal to the position of that 
  // bit and added to the overall product.
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 64; j++) {
      // check bits of left operand
      if (left.data[i] & (one << j)) {
        // add the value of the right operand (shifted the appropriate number of times) to the total product
        product = uint256_add(product, uint256_leftShift(right, (j + (i * 64))));
      }
    }
  }
  return product;
}

// Compute the state of a UInt256 value that has been shifted left a specific number of times.
UInt256 uint256_leftShift(UInt256 val, unsigned int shift) {
  UInt256 result; // copy the data of val over to the result
  result.data[0] = val.data[0];
  result.data[1] = val.data[1];
  result.data[2] = val.data[2];
  result.data[3] = val.data[3];

  // create a 64-bit integer equivalent to 1 (for bit-wise operations)
  uint64_t one = 1UL;

  // For a single left shift, every bit stored in result will be set equal to 
  // the bit on its right. This will be repeated for however many left shifts 
  // must be performed.
  for (int i = 0; i < (int) shift; i++) {
    // double for-loop to iterate through every bit
    for (int j = 3; j >= 0; j--) {
      for (int k = 63; k >= 0; k--) {
        if (j == 0 && k == 0) { // check if we have reached the far-right bit
          // current bit is set to 0 if it is the far-right bit
          result.data[j] &= ~(one << 0);
        } else if (k == 0) { // check if we've reached the last bit of one of the 64-bit chunks

          // reads the first bit from the next 64-bit chunk and copies it to current bit
          if (result.data[j - 1] & (one << 63)) {
            result.data[j] |= (one << k);
          } else {
            result.data[j] &= ~(one << k);
          }

        } else {

          // check value of the bit to the right
          if (result.data[j] & (one << (k - 1))) {
            result.data[j] |= (one << k); // set current bit to 1 if the bit to the right is 1
          } else {
            result.data[j] &= ~(one << k); // set current bit to 0 if the bit to the right is 0
          }

        }
      }
    }
  }

  return result;
}