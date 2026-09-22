#include <stdio.h>
#include <stdlib.h>

enum COMMANDS { CMD_PUT, CMD_GET, CMD_DELETE, CMD_CLEAR, CMD_ALL };

char *COMMANDS_STR = "p, g, d, c, a";

enum COMMANDS command_from_char(char c) {
  switch (c) {
  case 'p':
    return CMD_PUT;
  case 'g':
    return CMD_GET;
  case 'd':
    return CMD_DELETE;
  case 'c':
    return CMD_CLEAR;
  case 'a':
    return CMD_ALL;
  default:
    fprintf(stderr,
            "error: '%c' is not a supported command; must be one of %s\n", c,
            COMMANDS_STR);
    exit(EXIT_FAILURE);
  }
}

struct put_args {
  int value;
  char *key;
};

struct key_args {
  char *key;
};

union operation_args {
  struct put_args put;
  struct key_args key;
};

struct operation {
  enum COMMANDS command;
  union operation_args args;
};

void execute(enum COMMANDS command) {
  switch (command) {
  case CMD_PUT:
    printf("This is the PUT command\n");
    break;
  case CMD_GET:
    printf("This is the GET command\n");
    break;
  case CMD_DELETE:
    printf("This is the DELETE command\n");
    break;
  case CMD_CLEAR:
    printf("This is the CLEAR command\n");
    break;
  case CMD_ALL:
    printf("This is the ALL command\n");
    break;
  }
}

int main(int argc, char *argv[]) {
  // (p = PUT) p,value,key space p,value,key
  // (g = GET) g,key return value
  // (d = DELETE), d,key
  // (c = CLEAR), c (removes all kvs from db)
  // (a = ALL), a (prints all in any order)

  if (argc == 1) {
    fprintf(
        stderr,
        "You did not enter is a command. Usage: kv <%s>,<key|value>,<key>\n",
        COMMANDS_STR);
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < argc; i++) {
    char c = argv[i][0];
    enum COMMANDS command = command_from_char(c);
    execute(command);
  }
}