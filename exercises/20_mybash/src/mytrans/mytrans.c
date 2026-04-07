// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *resolve_input_path(const char *filename, char *fallback,
                                      size_t fallback_size) {
  const char *prefix = "/workspace/";
  if (!filename) {
    return filename;
  }

  FILE *fp = fopen(filename, "r");
  if (fp) {
    fclose(fp);
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
      fp = fopen(fallback, "r");
      if (fp) {
        fclose(fp);
        return fallback;
      }
    }
  }

  return filename;
}

static const char *resolve_dict_path(char *buf, size_t buf_size) {
  const char *candidates[] = {
      "src/mytrans/dict.txt",
      "../src/mytrans/dict.txt",
      "../../src/mytrans/dict.txt",
      "../../../src/mytrans/dict.txt",
  };

  for (size_t i = 0; i < sizeof(candidates) / sizeof(candidates[0]); i++) {
    FILE *fp = fopen(candidates[i], "r");
    if (fp) {
      fclose(fp);
      return candidates[i];
    }
  }

  snprintf(buf, buf_size, "%s", "src/mytrans/dict.txt");
  return buf;
}

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

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int __cmd_mytrans(const char* filename) {
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  char dict_fallback[256];
  const char *dict_path = resolve_dict_path(dict_fallback, sizeof(dict_fallback));
  if (load_dictionary(dict_path, table, &dict_count) != 0) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%ld词条。\n", dict_count);

  char fallback[512];
  const char *resolved = resolve_input_path(filename, fallback, sizeof(fallback));
  FILE* file = fopen(resolved, "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件 %s。\n", resolved);
    free_hash_table(table);
    return 1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
        continue;
    }

    // 使用 strtok 按空格分割单词
    char *word = strtok(line, " ");
    while (word != NULL) {
      char normalized[256] = {0};
      int pos = 0;

      for (int i = 0; word[i] != '\0' && pos < (int)sizeof(normalized) - 1; i++) {
        unsigned char ch = (unsigned char)word[i];
        if (isalpha(ch) || ch == '\'' || ch == '.' || ch == '/') {
          normalized[pos++] = (char)tolower(ch);
        }
      }
      normalized[pos] = '\0';

      if (normalized[0] == '\0') {
        word = strtok(NULL, " ");
        continue;
      }

      const char *translation = hash_table_lookup(table, normalized);
      printf("原文: %s\t", normalized);
      if (translation) {
          printf("翻译: %s\n", translation);
      } else {
          printf("未找到该单词的翻译。\n");
      }

      word = strtok(NULL, " ");
    }
  }

  fclose(file);
  free_hash_table(table);
  return 0;
}