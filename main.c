#include <stdio.h>
#include <string.h>

#include "bench.h"
#include "st.h"
#include "uthash.h"

typedef struct my_struct {
    int id;
} my_struct_t;

typedef struct my_struct1 {
    int id;
    UT_hash_handle hh;
} my_struct1_t;

float t1_wall, t1_cpu, t2_wall, t2_cpu;

int main(int argc, char **argv) {
  char *keys[1000000];

  for(unsigned long i = 0; i < 1000000; i++) {
    keys[i] = (char *)malloc(1024);
    sprintf(keys[i], "key_%lu", i);
  }

  // ST: Writes ---------------------------------------------------------------

  t1_wall = wall();
  t1_cpu = cpu();

  st_table *foo = st_init_strtable_with_size(64);

  for(unsigned long i = 0; i < 1000000; i++) {
    my_struct_t *entry = (my_struct_t *)malloc(sizeof(my_struct_t));
    entry->id = 1;
    st_insert(foo, (st_data_t)strdup(keys[i]), (st_data_t)entry);
  }

  struct my_struct1 *entries = NULL;

  t2_wall = wall();
  t2_cpu = cpu();

  printf("st time (wall):     %2.2f\n", t2_wall - t1_wall);
  printf("st time (cpu):      %2.2f\n", t2_cpu - t1_cpu);

  // -------------------------------------------------------------------------

  t1_wall = wall();
  t1_cpu = cpu();

  for(unsigned long i = 0; i < 1000000; i++) {
    my_struct1_t *entry = (my_struct1_t *)malloc(sizeof(my_struct1_t));
    entry->id = 1;
    HASH_ADD_KEYPTR(hh, entries, keys[i], strlen(keys[i]), entry);
  }

  t2_wall = wall();
  t2_cpu = cpu();

  printf("uthash time (wall): %2.2f\n", t2_wall - t1_wall);
  printf("uthash time (cpu):  %2.2f\n", t2_cpu - t1_cpu);

  return 0;
}
