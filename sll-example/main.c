#include "../libsll/sll.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

void print_item(void* data, void* in, void* out, void* err) { printf("%d\n", *(int*)data); }

int main(int argc, char *argv[])
{
  srand(time(NULL));
 
  assert(argc == 4);

  int n = atoi(argv[1]);
  int from = atoi(argv[2]);
  int to = atoi(argv[3]);

  sll* sll;
  sll_init(sll, sizeof(int));
  for (int i = 0; i < n; i++) {
    int number = rand() % (to - from + 1) + from;
    void* n = &number;
    sll_add(sll, &number);
  }

  sll_for_each(sll, print_item, NULL, NULL, NULL);
  sll_clear(sll);

  return 0;
}
