#include <stdio.h>
#include <string.h>

#include "tommy.h"
#include "bench.h"
#include "st.h"
#include "uthash.h"
#include "structs.h"

#define RUNS   2
#define CYCLES 100

char *keys[CYCLES],
     *other_keys[CYCLES];

st_table  *st_hash_table;
ut_hash_t *ut_hash_table = NULL;
tommy_hashlin *tommy_lin_hash_table;
tommy_hashdyn *tommy_dyn_hash_table;

void
setup_keys() {
  for(int i = 0; i < CYCLES; i++) {
    keys[i] = (char *)malloc(1024);
    sprintf(keys[i], "key_%d", i);

    other_keys[i] = (char *)malloc(1024);
    sprintf(other_keys[i], "other_key_%d", i);
  }
}

st_table *
setup_st_hash_table() {
  st_table *hash_table = st_init_strtable();
  for(int i = 0; i < CYCLES; i++) {
    st_hash_t *entry = (st_hash_t *)malloc(sizeof(st_hash_t));
    entry->id = i;

    st_insert(hash_table, (st_data_t)strdup(keys[i]), (st_data_t)&entry);
  }

  return hash_table;
}

ut_hash_t *
setup_ut_hash_table() {
  ut_hash_t *hash_table = NULL;
  for(int i = 0; i < CYCLES; i++) {
    ut_hash_t *entry = (ut_hash_t *)malloc(sizeof(ut_hash_t));
    entry->id = i;

    HASH_ADD_KEYPTR(hh, hash_table, keys[i], strlen(keys[i]), entry);
  }

  return hash_table;
}

tommy_hashlin *
setup_tommy_lin_hash_table() {
  tommy_hashlin *hash_table = malloc(sizeof(tommy_hashlin));
  tommy_hashlin_init(hash_table);

  for(int i = 0; i < CYCLES; i++) {
    tommy_elm_t *entry = (tommy_elm_t *)malloc(sizeof(tommy_elm_t));
    entry->id = i;

    tommy_hashlin_insert(hash_table, &entry->node, entry, tommy_hash_u64(0, keys[i], strlen(keys[i])));
  }

  return hash_table;
}

tommy_hashdyn *
setup_tommy_dyn_hash_table() {
  tommy_hashdyn *hash_table = malloc(sizeof(tommy_hashdyn));
  tommy_hashdyn_init(hash_table);

  for(int i = 0; i < CYCLES; i++) {
    tommy_elm_t *entry = (tommy_elm_t *)malloc(sizeof(tommy_elm_t));
    entry->id = i;

    tommy_hashdyn_insert(hash_table, &entry->node, entry, tommy_hash_u32(0, keys[i], strlen(keys[i])));
  }

  return hash_table;
}

void
bench_st_writes() {
  BENCHMARK(bench_st_writes, RUNS);

  st_hash_table = setup_st_hash_table();

  END_BENCHMARK(bench_st_writes);
  MEASURE_SUMMARY(bench_st_writes);
}

void
bench_st_reads() {
  BENCHMARK(bench_st_reads, RUNS);

  for(int i = 0; i < CYCLES; i++) {
    st_hash_t *entry = NULL;
    st_lookup(st_hash_table, (st_data_t)keys[i], (st_data_t *)&entry);
  }

  END_BENCHMARK(bench_st_reads);
  MEASURE_SUMMARY(bench_st_reads);
}

void
bench_st_nonexistant_reads() {
  BENCHMARK(bench_st_nonexistant_reads, RUNS);

  for(int i = 0; i < CYCLES; i++) {
    st_hash_t *entry = NULL;
    st_lookup(st_hash_table, (st_data_t)other_keys[i], (st_data_t *)&entry);
  }

  END_BENCHMARK(bench_st_nonexistant_reads);
  MEASURE_SUMMARY(bench_st_nonexistant_reads);
}

void
bench_st_writes_and_deletes() {
  st_table *hash_table = st_init_strtable();

  st_hash_t entry = {.id = 1};

  char *key = malloc(7);
  strcpy(key, "object");

  BENCHMARK(bench_st_writes_and_deletes, RUNS);

  st_insert(hash_table, (st_data_t)strdup(key), (st_data_t)&entry);
  st_delete(hash_table, (st_data_t *)&key, 0);

  END_BENCHMARK(bench_st_writes_and_deletes);
  MEASURE_SUMMARY(bench_st_writes_and_deletes);
}

void
bench_ut_writes() {
  BENCHMARK(bench_ut_writes, RUNS);

  ut_hash_table = setup_ut_hash_table();

  END_BENCHMARK(bench_ut_writes);
  MEASURE_SUMMARY(bench_ut_writes);
}

void
bench_ut_reads() {
  BENCHMARK(bench_ut_reads, RUNS);

  for(int i = 0; i < CYCLES; i++) {
    ut_hash_t *entry = NULL;
    HASH_FIND_STR(ut_hash_table, keys[i], entry);
  }

  END_BENCHMARK(bench_ut_reads);
  MEASURE_SUMMARY(bench_ut_reads);
}


void
bench_ut_nonexistant_reads() {
  BENCHMARK(bench_ut_nonexistant_reads, RUNS);

  for(int i = 0; i < CYCLES; i++) {
    ut_hash_t *entry = NULL;
    HASH_FIND_STR(ut_hash_table, other_keys[i], entry);
  }

  END_BENCHMARK(bench_ut_nonexistant_reads);
  MEASURE_SUMMARY(bench_ut_nonexistant_reads);
}

void
bench_ut_writes_and_deletes() {
  ut_hash_t *hash_table = NULL;
  ut_hash_t entry = {.id = 1, .hh = NULL};
  ut_hash_t *lookup;

  char *key = malloc(7);
  strcpy(key, "object");

  BENCHMARK(bench_ut_writes_and_deletes, RUNS);

  HASH_ADD_KEYPTR(hh, hash_table, key, strlen(key), &entry);
  HASH_FIND_STR(hash_table, key, lookup);
  HASH_DEL(hash_table, lookup);

  END_BENCHMARK(bench_ut_writes_and_deletes);
  MEASURE_SUMMARY(bench_ut_writes_and_deletes);
}

void
bench_tommy_lin_writes() {
  BENCHMARK(bench_tommy_lin_writes, RUNS);

  tommy_lin_hash_table = setup_tommy_lin_hash_table();

  END_BENCHMARK(bench_tommy_lin_writes);
  MEASURE_SUMMARY(bench_tommy_lin_writes);
}

// No need to compare objects as we just want whatever is set for the specific key.
static int dumb_cmp(const void *arg, const void *obj) { return 0; }

void
bench_tommy_lin_reads() {
  BENCHMARK(bench_tommy_lin_reads, RUNS);

  for(int i = 0; i < CYCLES; i++) {
    tommy_elm_t *entry = tommy_hashlin_search(tommy_lin_hash_table, &dumb_cmp, keys[i], tommy_hash_u64(0, keys[i], strlen(keys[i])));
  }

  END_BENCHMARK(bench_tommy_lin_reads);
  MEASURE_SUMMARY(bench_tommy_lin_reads);
}

void
bench_tommy_lin_nonexistant_reads() {
  BENCHMARK(bench_tommy_lin_nonexistant_reads, RUNS);

  for(int i = 0; i < CYCLES; i++) {
    tommy_elm_t *entry = tommy_hashlin_search(tommy_lin_hash_table, &dumb_cmp, other_keys[i], tommy_hash_u64(0, other_keys[i], strlen(other_keys[i])));
  }

  END_BENCHMARK(bench_tommy_lin_nonexistant_reads);
  MEASURE_SUMMARY(bench_tommy_lin_nonexistant_reads);
}

void
bench_tommy_lin_writes_and_deletes() {
  tommy_hashlin *hash_table = malloc(sizeof(tommy_hashlin));
  tommy_hashlin_init(hash_table);

  char *key = malloc(7);
  strcpy(key, "object");

  tommy_elm_t *entry = (tommy_elm_t *)malloc(sizeof(tommy_elm_t));
  entry->id = 1;

  BENCHMARK(bench_tommy_lin_writes_and_deletes, RUNS);

  tommy_hashlin_insert(hash_table, &entry->node, entry, tommy_hash_u64(0, key, strlen(key)));
  tommy_hashlin_remove(hash_table, &dumb_cmp, key, tommy_hash_u64(0, key, strlen(key)));

  END_BENCHMARK(bench_tommy_lin_writes_and_deletes);
  MEASURE_SUMMARY(bench_tommy_lin_writes_and_deletes);
}

void
bench_tommy_dyn_writes() {
  BENCHMARK(bench_tommy_dyn_writes, RUNS);

  tommy_dyn_hash_table = setup_tommy_dyn_hash_table();

  END_BENCHMARK(bench_tommy_dyn_writes);
  MEASURE_SUMMARY(bench_tommy_dyn_writes);
}

void
bench_tommy_dyn_reads() {
  BENCHMARK(bench_tommy_dyn_reads, RUNS);

  for(int i = 0; i < CYCLES; i++) {
    tommy_elm_t *entry = tommy_hashdyn_search(tommy_dyn_hash_table, &dumb_cmp, keys[i], tommy_hash_u64(0, keys[i], strlen(keys[i])));
  }

  END_BENCHMARK(bench_tommy_dyn_reads);
  MEASURE_SUMMARY(bench_tommy_dyn_reads);
}

void
bench_tommy_dyn_nonexistant_reads() {
  BENCHMARK(bench_tommy_dyn_nonexistant_reads, RUNS);

  for(int i = 0; i < CYCLES; i++) {
    tommy_elm_t *entry = tommy_hashdyn_search(tommy_dyn_hash_table, &dumb_cmp, other_keys[i], tommy_hash_u64(0, other_keys[i], strlen(other_keys[i])));
  }

  END_BENCHMARK(bench_tommy_dyn_nonexistant_reads);
  MEASURE_SUMMARY(bench_tommy_dyn_nonexistant_reads);
}

void
bench_tommy_dyn_writes_and_deletes() {
  tommy_hashdyn *hash_table = malloc(sizeof(tommy_hashdyn));
  tommy_hashdyn_init(hash_table);

  char *key = malloc(7);
  strcpy(key, "object");

  tommy_elm_t *entry = (tommy_elm_t *)malloc(sizeof(tommy_elm_t));
  entry->id = 1;

  BENCHMARK(bench_tommy_dyn_writes_and_deletes, RUNS);

  tommy_hashdyn_insert(hash_table, &entry->node, entry, tommy_hash_u32(0, key, strlen(key)));
  tommy_hashdyn_remove(hash_table, &dumb_cmp, key, tommy_hash_u32(0, key, strlen(key)));

  END_BENCHMARK(bench_tommy_dyn_writes_and_deletes);
  MEASURE_SUMMARY(bench_tommy_dyn_writes_and_deletes);
}

int
main(int argc, char **argv) {
  setup_keys();

  bench_st_writes();
  bench_st_reads();
  bench_st_nonexistant_reads();
  bench_st_writes_and_deletes();

  bench_ut_writes();
  bench_ut_reads();
  bench_ut_nonexistant_reads();
  bench_ut_writes_and_deletes();

  bench_tommy_lin_writes();
  bench_tommy_lin_reads();
  bench_tommy_lin_nonexistant_reads();
  bench_tommy_lin_writes_and_deletes();

  bench_tommy_dyn_writes();
  bench_tommy_dyn_reads();
  bench_tommy_dyn_nonexistant_reads();
  bench_tommy_dyn_writes_and_deletes();

  return 0;
}
