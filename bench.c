/*
 * bench.c
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
#include "bench.h"

unsigned long unixtime() {
    struct timeval tp;
    if (gettimeofday((struct timeval *) &tp, (NUL)) == 0) {
        return tp.tv_sec;
    }
    return 0;
}

double microtime() {
    struct timeval tp;
    long sec = 0L;
    double msec = 0.0;
    if (gettimeofday((struct timeval *) &tp, (NUL)) == 0) {
        msec = (double) (tp.tv_usec / MICRO_IN_SEC);
        sec = tp.tv_sec;
        if (msec >= 1.0)
            msec -= (long) msec;
        return sec + msec;
    }
    return 0;
}

void bench_start(bench *b) {
    b->start = microtime();
}

void bench_stop(bench *b) {
    b->end = microtime();
}

double bench_iteration_speed(bench *b) {
    return (b->N * b->R) / (b->end - b->start);
}

double bench_duration(bench *b) {
    return (b->end - b->start);
}

void bench_print_summary(bench *b) {
    printf("%s -> %s%ld%s runs, %s%ld%s iterations each run, finished in %s%lf%s seconds\n", BENCH_PRIMARY_COLOR, BENCH_SECONDARY_COLOR, b->R, BENCH_PRIMARY_COLOR, BENCH_SECONDARY_COLOR, b->N, BENCH_PRIMARY_COLOR, BENCH_SECONDARY_COLOR, bench_duration(b), BENCH_PRIMARY_COLOR );
    printf("%s -> %s%.2f%s i/sec\n", BENCH_PRIMARY_COLOR, BENCH_SECONDARY_COLOR, bench_iteration_speed(b), BENCH_PRIMARY_COLOR );
    printf("%s\n", BENCH_RESET_COLOR);
}

void bench_csv_write(char *filename, int countOfB, ...) {
    FILE *fp = fopen(filename, "w");
    if(!fp) {
        return;
    }

    unsigned long ts = unixtime();
    fprintf(fp, "%ld", ts);

    int i;
    bench * b;
    va_list vl;
    va_start(vl,countOfB);
    for (i=0 ; i < countOfB ; i++) {
        b = va_arg(vl, bench*);
        fprintf(fp, ",%.2f", bench_iteration_speed(b) );
    }
    va_end(vl);

    fprintf(fp, "\n");
    fclose(fp);
}

/**
 * Combine multiple benchmark result into one measure entry.
 *
 * bench_csv_append("benchmark.csv", 3, &b1, &b2)
 */
void bench_csv_append(char *filename, int countOfB, ...) {
    FILE *fp = fopen(filename, "a+");
    if(!fp) {
        return;
    }

    unsigned long ts = unixtime();
    fprintf(fp, "%ld", ts);


    int i;
    bench * b;
    va_list vl;
    va_start(vl,countOfB);
    for (i=0 ; i < countOfB ; i++) {
        b = va_arg(vl, bench*);
        fprintf(fp, ",%.2f", bench_iteration_speed(b) );
    }
    va_end(vl);

    fprintf(fp, "\n");
    fclose(fp);
}
