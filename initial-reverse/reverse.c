#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void print_line(FILE *fp_in, FILE *fp_out) {
  char *line = NULL;
  size_t size = 0;
  size_t nread;
  getline(&line, &size, fp_in);
  fprintf(fp_out, "%s", line);
  free(line);
}

int main(int argc, char *argv[]) {
  FILE *fp_in = stdin;
  FILE *fp_out = stdout;

  if (argc > 3) {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
  }

  if (argc > 1) {
    fp_in = fopen(argv[1], "r");
    if (fp_in == NULL) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
      exit(1);
    }
  }

  if (argc == 3) {
    struct stat in_stat;
    struct stat out_stat;

    if (stat(argv[1], &in_stat) != 0) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
      exit(1);
    }

    if (stat(argv[2], &out_stat) == 0 && in_stat.st_dev == out_stat.st_dev &&
        in_stat.st_ino == out_stat.st_ino) {
      fprintf(stderr, "reverse: input and output file must differ\n");
      exit(1);
    }
    fp_out = fopen(argv[2], "w");
    if (fp_out == NULL) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
      exit(1);
    }
  }

  long start = ftell(fp_in);
  fseek(fp_in, 0, SEEK_END);
  long end = ftell(fp_in);
  long cur = end - 1;

  for (long cur = end - 1, offset = cur - end; offset * -1 <= end;
       cur--, offset = cur - end) {
    fseek(fp_in, offset, SEEK_END);
    char c = getc(fp_in);
    if (c == '\n' && offset != -1) {
        print_line(fp_in, fp_out);
    }
  }
  fseek(fp_in, 0, SEEK_SET);
  print_line(fp_in, fp_out);
  fclose(fp_in);
  fclose(fp_out);
}
