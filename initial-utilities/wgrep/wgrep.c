#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void grep_stream(char *search, FILE *fp) {
  char *line = NULL;
  size_t size = 0;
  ssize_t nread;
  if (fp == NULL) {
    fprintf(stdout, "wgrep: cannot open file\n");
    free(line);
    exit(EXIT_FAILURE);
  }

  while ((nread = getline(&line, &size, fp)) != EOF) {
    if (strstr(line, search) != NULL) {
      fputs(line, stdout);
    }
  }
  free(line);
}

int main(int argc, char *argv[]) {

  FILE *fp = NULL;

  if (argc < 2) {
    fprintf(stdout, "wgrep: searchterm [file ...]\n");
    exit(EXIT_FAILURE);
  }

  char *search_term = argv[1];

  if (argc > 2) {
    for (int i = 2; i < argc; i++) {
      fp = fopen(argv[i], "r");
      grep_stream(search_term, fp);
      fclose(fp);
    } 
  } else {
    grep_stream(search_term, stdin);
  }
  return EXIT_SUCCESS;
}