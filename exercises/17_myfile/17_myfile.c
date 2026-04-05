#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_elf_type(uint16_t e_type) {
  const char *type_str;
  switch (e_type) {
    case ET_REL:
      type_str = "Relocatable";
      break;
    case ET_EXEC:
      type_str = "Executable";
      break;
    case ET_DYN:
      type_str = "Shared Object/PIE";
      break;
    default:
      type_str = "Unknown";
      break;
  }
  printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int main(int argc, char *argv[]) {
  char filepath[2][256] = {
    "./17_myfile.o",
    "./17_myfile",
  };

  int fd;
  Elf64_Ehdr ehdr;

  for (int i = 0; i < 2; i++) {
    const char *target = filepath[i];

    fd = open(target, O_RDONLY);
    if (fd < 0) {
      perror("open");
      continue;
    }

    ssize_t nread = read(fd, &ehdr, sizeof(ehdr));
    close(fd);
    if (nread != sizeof(ehdr)) {
      fprintf(stderr, "failed to read ELF header: %s\n", target);
      continue;
    }

    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
      fprintf(stderr, "not an ELF file: %s\n", target);
      continue;
    }

    printf("File: %s\n", target);
    print_elf_type(ehdr.e_type);
  }
  
  return 0;
}
