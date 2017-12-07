#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

unsigned char buffer[2048];
int main(int argc, const char *argv[])
{
  int fd = open("/dev/mem", O_SYNC);
  FILE *file_fd = fopen(argv[1], "w");
  int size;
  int page_size = sysconf(_SC_PAGESIZE);
  unsigned char *address;

  if (fd < 0)
  {
    fprintf(stderr, "I/O Error /DEV/MEM cannot be open\n");
    exit(1);
  }

  if (file_fd == NULL)
  {
    fprintf(stderr, "File %s cannot be open\n", argv[1]);
    exit(-1);
  }

  address = (unsigned char *)mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_LOCKED, fd, 1 * page_size);

  if (address != MAP_FAILED)
  {
    for (size = 0; size < page_size; size++)
    {
      fprintf(file_fd, "%d ", *address++);
    }
  }

  read(fd, buffer, 1024);

  for (size = 0; size < 1024; size++)
  {
    fprintf(file_fd, "%d ", buffer[size]);
  }

  close(fd);
  fclose(file_fd);

  exit(EXIT_SUCCESS);
}
