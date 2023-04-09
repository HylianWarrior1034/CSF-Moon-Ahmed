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

int compare_i64(const void *left, const void *right) {
  if (*(int64_t *) left > *(int64_t *) right) {
    return 1;
  } else {
    return -1;
  }
}

/*
int compare_i64(const void *a, const void *b)
{
    const int64_t *ia = (const int64_t *)a; // casting pointer types 
    const int64_t *ib = (const int64_t *)b;
    return *ia  - *ib; 
}
*/


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
    qsort(&arr[begin], end - begin + 1, sizeof(int64_t), compare_i64); // not sure what to put for the comparison function
    return;
  }
  else {
    // create two forks and sort left and right sides
    //pid_t pid = fork();
    size_t mid = (end + begin) / 2;

    merge_sort(arr, begin, mid, threshold); // child
    merge_sort(arr, mid + 1, end, threshold); // parent
    /*
    if (pid == -1) {
      fprintf(stderr, "Error: new fork could not open.\n");
      exit(2);
    }else if (pid == 0) {
      merge_sort(arr, begin, mid, threshold); // child
      exit(0);
    } else {
      merge_sort(arr, mid + 1, end, threshold); // parent
    }
    */
  }

  /*
  int wstatus;
  // blocks until the process indentified by pid_to_wait_for completes
  pid_t actual_pid = waitpid(0, &wstatus, 0);
  if (actual_pid == -1) {
    fprintf(stderr, "Error: waitpid failure.\n");
    exit(3);
  }

  if (!WIFEXITED(wstatus)) {
    // subprocess crashed, was interrupted, or did not exit normally
    // handle as error
    fprintf(stderr, "Error: subprocess crashed, was interrupted, or did not exit normally.\n");
    exit(4);
  }

  if (WEXITSTATUS(wstatus) != 0) {
    // subprocess returned a non-zero exit code
    // if following standard UNIX conventions, this is also an error
    fprintf(stderr, "Error: subprocess returned error.\n");
    exit(5);
  }
  */

  int64_t temparr[(end - begin) + 1];
  
  // merge them
  merge(arr, begin, ((end + begin) / 2) + 1, end, temparr);

  int j = 0;
  for (size_t i = begin; i <= end; i++) {
    arr[i] = temparr[j++];
  }

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
    fprintf(stderr, "Error: file could not be opened.\n");
    return -1;
  }

  // TODO: use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    fprintf(stderr, "Error: fstat.\n");
    close(fd);
    return 1;
  }
  size_t file_size_in_bytes = statbuf.st_size;

  // TODO: map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (data == MAP_FAILED) {
    fprintf(stderr, "Error: mmap error.\n");
    close(fd);
    return 1;
  }

  // TODO: sort the data!
  merge_sort(data, 0, (file_size_in_bytes / 8) - 1, threshold);

  // TODO: unmap and close the file
  munmap(NULL, file_size_in_bytes);
  close(fd);

  // TODO: exit with a 0 exit code if sort was successful
  return 0;
}
