#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "getpid_demo: pid = %d\n", getpid());
  exit();
}
