// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
  if (!str)
    return;

  char *start = str;
  while (*start && isspace((unsigned char)*start)) {
    start++;
  }

  char *end = start + strlen(start);
  while (end > start && isspace((unsigned char)*(end - 1))) {
    end--;
  }

  *end = '\0';

  if (start != str) {
    memmove(str, start, (size_t)(end - start) + 1);
  }
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;

  if (dict_count) {
    *dict_count = 0;
  }

  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\r\n")] = '\0';
    trim(line);

    if (line[0] == '\0') {
      continue;
    }

    if (line[0] == '#') {
      // 遇到新单词前，先提交上一条记录
      if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
        if (!hash_table_insert(table, current_word, current_translation)) {
          fclose(file);
          return -1;
        }
        if (dict_count) {
          (*dict_count)++;
        }
      }

      strncpy(current_word, line + 1, sizeof(current_word) - 1);
      current_word[sizeof(current_word) - 1] = '\0';
      trim(current_word);
      current_translation[0] = '\0';
      in_entry = 1;
      continue;
    }

    if (in_entry && strncmp(line, "Trans:", 6) == 0) {
      char *trans = line + 6;
      trim(trans);

      if (current_translation[0] != '\0') {
        strncat(current_translation, "@",
                sizeof(current_translation) - strlen(current_translation) - 1);
      }
      strncat(current_translation, trans,
              sizeof(current_translation) - strlen(current_translation) - 1);
    }
  }

  // 提交文件末尾最后一个词条
  if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
    if (!hash_table_insert(table, current_word, current_translation)) {
      fclose(file);
      return -1;
    }
    if (dict_count) {
      (*dict_count)++;
    }
  }

  fclose(file);
  return 0;
}
