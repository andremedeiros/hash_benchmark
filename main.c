#include <stdio.h>
#include <string.h>

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
    st_hash_t entry = {.id = 1};
    st_insert(hash_table, (st_data_t)strdup(keys[i]), (st_data_t)&entry);
  }

  return hash_table;
}

ut_hash_t *
setup_ut_hash_table() {
  ut_hash_t *hash_table = NULL;
  for(int i = 0; i < CYCLES; i++) {
    ut_hash_t *entry = (ut_hash_t *)malloc(sizeof(ut_hash_t));
    entry->id = 1;

    HASH_ADD_KEYPTR(hh, hash_table, keys[i], strlen(keys[i]), entry);
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

int
main(int argc, char **argv) {
  setup_keys();

  bench_st_writes();
  bench_st_reads();
  bench_st_nonexistant_reads();

  bench_ut_writes();
  bench_ut_reads();
  bench_ut_nonexistant_reads();
  return 0;
}
