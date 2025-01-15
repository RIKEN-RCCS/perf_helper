#include "perf_helper.h"
#include <stdio.h>

// Wrapper for Fortran
void f_perf_initialize() {
    perf_initialize();
}

void f_perf_start_section(int *section) {
    perf_start_section(*section);
}

void f_perf_stop_section(int *section) {
    perf_stop_section(*section);
}

void f_perf_finalize() {
    perf_finalize();
}

// Wrapper for C
int initialize_perf() {
    return(perf_initialize());
}

int start_section_perf(int section) {
    return(perf_start_section(section));
}

int stop_section_perf(int section) {
    return(perf_stop_section(section));
}

void finalize_perf() {
    perf_finalize();
}
