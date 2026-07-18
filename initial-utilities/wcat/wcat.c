#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void print_file(char *file_path) {
  FILE *fp;
  char buffer[BUFSIZ];

  if ((fp = fopen(file_path, "r")) == NULL) {
    fprintf(stdout, "wcat: cannot open file\n");
    exit(EXIT_FAILURE);
  }

  while ((fgets(buffer, BUFSIZ, fp)) != NULL) {
    printf("%s", buffer);
  }
  fclose(fp);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    exit(0);
  }

  for (int i = 1; i < argc; i++) {
    print_file(argv[i]);
  }

  return EXIT_SUCCESS;
}