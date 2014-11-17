/**
 * bench.h
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */
#ifndef BENCH_H
#define BENCH_H

#define BENCH_RESET_COLOR "\e[m"
#define BENCH_PRIMARY_COLOR "\e[36m"
#define BENCH_SECONDARY_COLOR "\e[33m"

#include <stdlib.h>

#define MICRO_IN_SEC 1000000.00
#define SEC_IN_MIN 60
#define NUL  '\0'

typedef struct {
    long N; // N for each run
    long R; // runs
    double secs;
    double start;
    double end;
} bench;

unsigned long unixtime();

double microtime();

void bench_start(bench *b);

void bench_stop(bench *b);

double bench_iteration_speed(bench *b);

void bench_print_summary(bench *b);

double bench_duration(bench *b);

void bench_csv_append(char *filename, int countOfB, ...);

void bench_csv_write(char *filename, int countOfB, ...);

#define MEASURE(B) \
        bench B; B.N = 1; B.R = 1; \
        printf("\n%s -> Measuring %s" #B "%s...\n", BENCH_PRIMARY_COLOR, BENCH_SECONDARY_COLOR, BENCH_PRIMARY_COLOR); \
        bench_start(&B);

#define END_MEASURE(B) \
        bench_stop(&B);

#define MEASURE_SUMMARY(B) bench_print_summary(&B);

#define BENCHMARK(B, RUN) \
        bench B; \
        B.N = 100000; \
        B.R = RUN; \
        printf("\n%s -> Benchmarking %s" #B "%s...\n", BENCH_PRIMARY_COLOR, BENCH_SECONDARY_COLOR, BENCH_PRIMARY_COLOR); \
        bench_start(&B); \
        for (int r_##B = 0; r_##B < B.R ; r_##B++ ) { \
            for (int i_##B = 0; i_##B < B.N ; i_##B++ ) {

#define END_BENCHMARK(B) \
            } \
        } \
        bench_stop(&B);

#define BENCHMARK_SUMMARY(B) bench_print_summary(&B);

#define BENCHMARK_CSV_APPEND(filename, countOfB, ...) \
    bench_csv_append(filename, countOfB, __VA_ARGS__)

#define BENCHMARK_CSV_WRITE(filename, countOfB, ...) \
    bench_csv_write(filename, countOfB, __VA_ARGS__)

#define BR(b) &b

#endif /* !BENCH_H */
