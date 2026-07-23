#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int current;
  char previous = 0;
  uint32_t running_count = 0;
  int have_previous = 0;

  if (argc < 2) {
    fprintf(stdout, "wzip: file1 [file2 ...]\n");
    exit(EXIT_FAILURE);
  }

  for (int argi = 1; argi < argc; argi++) {
    FILE *fp = fopen(argv[argi], "r");

    if (fp == NULL) {
      perror("fopen");
      exit(EXIT_FAILURE);
    }

    while ((current = fgetc(fp)) != EOF) {
      if (!have_previous) {
        previous = (char)current;
        running_count = 1;
        have_previous = 1;
        continue;
      }

      if (current == previous) {
        running_count++;
      } else {
        fwrite(&running_count, sizeof(running_count), 1, stdout);
        fwrite(&previous, sizeof(previous), 1, stdout);

        previous = (char)current;
        running_count = 1;
      }
    }

    fclose(fp);
  }

  if (have_previous) {
    fwrite(&running_count, sizeof(running_count), 1, stdout);
    fwrite(&previous, sizeof(previous), 1, stdout);
  }
}
