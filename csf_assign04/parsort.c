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
  } else if (*(int64_t *) left < *(int64_t *) right) {
    return -1;
  } else {
    return 0;
  }
}

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
  size_t mid = (end + begin) / 2;
  if (end - begin <= threshold) {
    qsort(arr + begin, end - begin, sizeof(int64_t), compare_i64); // not sure what to put for the comparison function
    return;
  }
  else {
    // create two forks and sort left and right sides
    pid_t pid = fork();

    if (pid == -1) {
      fprintf(stderr, "Error: new fork could not open.\n");
      exit(8);
    }else if (pid == 0) {
      merge_sort(arr, begin, mid, threshold); // child
      exit(0);
    } else {
      merge_sort(arr, mid, end, threshold); // parent
    }

    int wstatus;
    // blocks until the process indentified by pid_to_wait_for completes
    pid_t actual_pid = waitpid(pid, &wstatus, 0);

    if (actual_pid == -1) {
      fprintf(stderr, "Error: waitpid failure.\n");
      exit(9);
    }

    if (!WIFEXITED(wstatus)) {
      // subprocess crashed, was interrupted, or did not exit normally
      // handle as error
      fprintf(stderr, "Error: subprocess crashed, was interrupted, or did not exit normally.\n");
      exit(10);
    }

    if (WEXITSTATUS(wstatus) != 0) {
      // subprocess returned a non-zero exit code
      // if following standard UNIX conventions, this is also an error
      fprintf(stderr, "Error: subprocess returned error.\n");
      exit(11);
    }
  }
  
  int64_t *temparr = malloc(sizeof(uint64_t) * (end - begin));

  // merge them
  merge(arr, begin, mid, end, temparr);

  for (size_t i = begin; i < end; i++) {
    arr[i] = temparr[i - begin];
  }

  free(temparr);

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
  if (end != argv[2] + strlen(argv[2])) {
    fprintf(stderr, "Error: invalid threshold.\n");
    return 2;
  }

  // TODO: open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    fprintf(stderr, "Error: file could not be opened.\n");
    close(fd);
    return 3;
  }

  // TODO: use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    fprintf(stderr, "Error: fstat error.\n");
    close(fd);
    return 4;
  }
  size_t file_size_in_bytes = statbuf.st_size;

  // TODO: map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  int64_t closestat = close(fd);
  
  if (data == MAP_FAILED) {
    fprintf(stderr, "Error: mmap error.\n");
    return 5;
  }

  if (closestat != 0) {
    fprintf(stderr, "Error: failed to close the file\n");
    return 6;
  }

  // TODO: sort the data!
  merge_sort(data, 0, (file_size_in_bytes / sizeof(int64_t)), threshold);

  // TODO: unmap and close the file
  int64_t unmap = munmap(data, file_size_in_bytes);
  if (unmap != 0) {
    fprintf(stderr, "Error: faliture to unmap the data\n");
    return 7;
  }

  // TODO: exit with a 0 exit code if sort was successful
  return 0;
}
