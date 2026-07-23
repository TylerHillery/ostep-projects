#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct encoding {
  uint32_t repeat;
  char value;
};

int main(int argc, char *argv[]) {
  struct encoding encoding;

  if (argc < 2) {
    fprintf(stdout, "wunzip: file1 [file2 ...]\n");
    exit(EXIT_FAILURE);
  }

  for (int argi = 1; argi < argc; argi++) {
    FILE *fp = fopen(argv[argi], "rb");

    if (fp == NULL) {
      perror("fopen");
      exit(EXIT_FAILURE);
    }

    while (fread(&encoding.repeat, sizeof(encoding.repeat), 1, fp) == 1) {
      if (fread(&encoding.value, sizeof(encoding.value), 1, fp) != 1) {
        break;
      }
      for (uint32_t i = 0; i < encoding.repeat; i++) {
        putchar(encoding.value);
      }
    }

    fclose(fp);
  }
}
