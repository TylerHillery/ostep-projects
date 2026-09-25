#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  long value;
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

struct node {
  struct node *left;
  struct node *right;
  char *key;
  long value;
};

struct node *root = NULL;

struct node *cmd_put(struct node *root, char *key, long value);
struct node *cmd_get(struct node *root, char *key);
struct node *cmd_delete(struct node *root, char *key);

struct operation parse_args(char *s) {
  char *tok = NULL;
  char *delimeter = ",";
  long i = 0;

  struct operation op;

  tok = strtok(s, delimeter);

  while (tok != NULL) {
    if (i == 0) {
      op.command = command_from_char(tok[0]);
    } else {
      switch (op.command) {
      case CMD_PUT:
        if (i == 1) {
          char *end;
          long parsed;

          errno = 0;
          parsed = strtol(tok, &end, 10);

          if (errno == ERANGE) {
            fprintf(stderr, "error: '%s' is out of range\n", tok);
            exit(EXIT_FAILURE);
          }

          if (end == tok) {
            fprintf(stderr, "error: value '%s' is not a number\n", tok);
            exit(EXIT_FAILURE);
          }

          if (*end != '\0') {
            fprintf(stderr, "error: invalid number '%s'\n", tok);
            exit(EXIT_FAILURE);
          }

          op.args.put.value = parsed;
        } else if (i == 2) {
          op.args.put.key = tok;
        } else if (i > 2) {
          fprintf(stderr, "error: too many arguments supplied put"
                          "only takes a value and a key\n");
          exit(EXIT_FAILURE);
        }
        break;
      case CMD_GET:
      case CMD_DELETE:
        if (i == 1) {
          op.args.key.key = tok;
        } else if (i > 1) {
          fprintf(stderr, "error: too many arguments supplied get and delete "
                          "only take a key\n");
          exit(EXIT_FAILURE);
        }
        break;
      case CMD_CLEAR:
      case CMD_ALL:
        fprintf(stderr,
                "error: command clear and all don't have any arguments\n");
        exit(EXIT_FAILURE);
        break;
      }
    }

    i++;
    tok = strtok(NULL, delimeter);
  }

  // handle if to few arguments are supplied
  switch (op.command) {
  case CMD_PUT:
    if (i != 3) {
      fprintf(stderr, "error: put requires a value and key\n");
      exit(EXIT_FAILURE);
    }
    break;

  case CMD_GET:
  case CMD_DELETE:
    if (i != 2) {
      fprintf(stderr, "error: command requires a key\n");
      exit(EXIT_FAILURE);
    }
    break;

  case CMD_CLEAR:
  case CMD_ALL:
    if (i != 1) {
      fprintf(stderr, "error: command takes no arguments\n");
      exit(EXIT_FAILURE);
    }
    break;
  }

  return op;
}


/*
 * This is stupid first attemp of in mem storage for the kv a shared struct, 
 * that no matter the key it will set the value and get this value
 */

struct kv {
  char key;
  long value;
};

struct kv execute(struct operation op, struct kv db) {
  switch (op.command) {
  case CMD_PUT:
    printf("This is the PUT command - key: %s, value: %ld\n", op.args.put.key,
           op.args.put.value);
    db.value = op.args.put.value;
    return db;
  case CMD_GET:
    printf("This is the GET command - key: %s\n", op.args.key.key);
    printf("This is current value key: %ld\n", db.value);
    return db;
  case CMD_DELETE:
    printf("This is the DELETE command - key: %s\n", op.args.key.key);
    printf("NOT IMPLEMENTED YET\n");
    return db;
  case CMD_CLEAR:
    printf("This is the CLEAR command\n");
    printf("NOT IMPLEMENTED YET\n");
    return db;
  case CMD_ALL:
    printf("This is the ALL command\n");
    printf("NOT IMPLEMENTED YET\n");
    return db;
  }
}

int main(int argc, char *argv[]) {
  // (p = PUT) p,value,key space p,value,key
  // (g = GET) g,key return value
  // (d = DELETE), d,key
  // (c = CLEAR), c (removes all kvs from db)
  // (a = ALL), a (prints all in any order)

  struct kv db;

  if (argc == 1) {
    fprintf(
        stderr,
        "You did not enter is a command. Usage: kv <%s>,<key|value>,<key>\n",
        COMMANDS_STR);
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < argc; i++) {
    struct operation op = parse_args(argv[i]);
    db = execute(op, db);
  }
}