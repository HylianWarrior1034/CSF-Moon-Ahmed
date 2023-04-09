#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Merge the elements in the sorted ranges [begin, mid) and [mid, end),
// copying the result into temparr.
void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  int64_t *endl = arr + mid;
  int64_t *endr = arr + end;
  int64_t *left = arr + begin, *right = arr + mid, *dst = temparr;

  for (;;) {
    int at_end_l = left >= endl;
    int at_end_r = right >= endr;

    if (at_end_l && at_end_r) break;

    if (at_end_l)
      *dst++ = *right++;
    else if (at_end_r)
      *dst++ = *left++;
    else {
      int cmp = compare_i64(left, right);
      if (cmp <= 0)
        *dst++ = *left++;
      else
        *dst++ = *right++;
    }
  }
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  // TODO: Implement
  if (end - begin + 1 <= threshold) {
    qsort(&arr[begin], end - begin + 1, sizeof(int64_t), compare_i64()); // not sure what to put for the comparison function
    return;
  }
  else {
    // create two forks and sort left and right sides
    pid_t pid = fork();

    if (pid == 0) {
      merge_sort(arr, begin, end / 2, threshold); // child
    } else {
      merge_sort(arr, (end / 2) + 1, end, threshold); // parent
    }
  }

  int64_t temparr[end + 1];

  // merge them
  merge(arr, begin, end / 2, end, temparr);

}

int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2]))
    /* TODO: report an error (threshold value is invalid) */;

  // TODO: open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    // report an error cause file couldn't be opened
  }

  // TODO: use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    // report fstat error
  }
  size_t file_size_in_bytes = statbuf.st_size;

  // TODO: map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (data == MAP_FAILED) {
    // report mmap error
  }

  // TODO: sort the data!

  // TODO: unmap and close the file

  // TODO: exit with a 0 exit code if sort was successful
}
