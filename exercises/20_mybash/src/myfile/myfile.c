#include "myfile.h"

static const char *resolve_input_path(const char *filename, char *fallback,
                                      size_t fallback_size) {
  const char *prefix = "/workspace/";
  if (!filename) {
    return filename;
  }

  if (access(filename, R_OK) == 0) {
    return filename;
  }

  if (strncmp(filename, prefix, strlen(prefix)) == 0) {
    const char *suffix = filename + strlen(prefix);
    const char *patterns[] = {
        "%s",
        "../%s",
        "../../%s",
        "../../../%s",
    };

    for (size_t i = 0; i < sizeof(patterns) / sizeof(patterns[0]); i++) {
      snprintf(fallback, fallback_size, patterns[i], suffix);
      if (access(fallback, R_OK) == 0) {
        return fallback;
      }
    }
  }

  return filename;
}

void print_elf_type(uint16_t e_type) {
    const char *type_str;
    switch (e_type) {
    case ET_NONE:
      type_str = "Unknown (ET_NONE)";
      break;
    case ET_REL:
      type_str = "Relocatable (ET_REL)";
      break;
    case ET_EXEC:
      type_str = "Executable (ET_EXEC)";
      break;
    case ET_DYN:
      type_str = "Shared Object/PIE (ET_DYN)";
      break;
    case ET_CORE:
      type_str = "Core Dump (ET_CORE)";
      break;
    default:
      if (e_type >= ET_LOOS && e_type <= ET_HIOS)
        type_str = "OS-Specific";
      else if (e_type >= ET_LOPROC && e_type <= ET_HIPROC)
        type_str = "Processor-Specific";
      else
        type_str = "Invalid";
    }
    printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int __cmd_myfile(const char* filename) {
  char filepath[512];
  char fallback[512];
    int fd;
    Elf64_Ehdr ehdr;

  const char *resolved = resolve_input_path(filename, fallback, sizeof(fallback));
  strncpy(filepath, resolved, sizeof(filepath) - 1);
  filepath[sizeof(filepath) - 1] = '\0';
    fflush(stdout);
    printf("filepath: %s\n", filepath);

    fd = open(filepath, O_RDONLY);
    if (fd < 0) {
      perror("open");
      return 1;
    }

    if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
      perror("read");
      close(fd);
      return 1;
    }

    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
      fprintf(stderr, "%s is not an ELF file\n", filepath);
      close(fd);
      return 1;
    }

    print_elf_type(ehdr.e_type);
    close(fd);
    return 0;
}