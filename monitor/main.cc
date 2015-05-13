#include <stdio.h>

int main(int argc, char *arg[])
{
  monitor_config_t config;
  if (!read_config(config)) {
    return -1;
  }

  for (;;) {
    check_processes(config);
    sleep(5);
  }

  return 0;
}

