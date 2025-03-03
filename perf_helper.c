#define _GNU_SOURCE
#include "perf_helper.h"
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

// Use extern "C" to prevent name mangling when linking with Fortran
#ifdef __cplusplus
extern "C" {
#endif

PerfContext thread_context[MAX_THREADS];

// syscall wrapper for perf_event_open
static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

static int get_event_code_from_name(const char *event_name, uint64_t *event_code) {
    for (int i = 0; i < event_list_size; i++) {
        if (strcasecmp(event_list[i].name, event_name) == 0) {
            *event_code = event_list[i].hex_code;
            return i;
        }
    }
    fprintf(stderr, "Error: Event name '%s' not found.\n", event_name);
    return -1;
}

int perf_initialize() {
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        PerfContext *ctx = &thread_context[thread_id];
        memset(ctx, 0, sizeof(PerfContext));

        char *env_events = getenv("PERF_EVENTS");
        if (!env_events) {
            fprintf(stderr, "Environment variable PERF_EVENTS is not set (thread %d).\n", thread_id);
            exit(EXIT_FAILURE);
        }

	char *temp_env = strdup(env_events);
	char *saveptr;
        char *event = strtok_r(temp_env, ",", &saveptr);
        while (event && ctx->num_events < MAX_EVENTS) {
            struct perf_event_attr pe;
            memset(&pe, 0, sizeof(struct perf_event_attr));
            pe.size = sizeof(struct perf_event_attr);
            pe.disabled = 1;
            pe.exclude_kernel = 1;
            pe.exclude_hv = 1;

            uint64_t event_code;
	    int event_index;

	    if (strncmp(event, "0x", 2) == 0) {
                event_code = strtoull(event, NULL, 16);
                ctx->event_name[ctx->num_events] = strdup(event);
            } else {
                event_index = get_event_code_from_name(event, &event_code);
                if (event_index < 0) {
                    free(temp_env);
                    exit(EXIT_FAILURE);
                }
                ctx->event_name[ctx->num_events] = strdup(event_list[event_index].name);
            }

	    pe.type = PERF_TYPE_RAW;
            pe.config = event_code;

            int fd = perf_event_open(&pe, 0, -1, -1, 0);
            if (fd == -1) {
                perror("perf_event_open");
                free(temp_env);
                exit(EXIT_FAILURE);
            }

            ctx->fd[ctx->num_events] = fd;
            ctx->event_codes[ctx->num_events] = event_code;
            ctx->num_events++;
            event = strtok_r(NULL, ",", &saveptr);
        }
        free(temp_env);

        for (int i = 0; i < ctx->num_events; i++) {
            ioctl(ctx->fd[i], PERF_EVENT_IOC_RESET, 0);
            ioctl(ctx->fd[i], PERF_EVENT_IOC_ENABLE, 0);
        }
        for (int j = 0; j < MAX_SECTIONS; j++) {
            for (int i = 0; i < MAX_EVENTS; i++) {
                ctx->total_values[j][i] = 0;
            }
            ctx->on_off_flag[j][0] = 0;
            ctx->on_off_flag[j][1] = 0;
        }

    }

    return 0;
}

int perf_start_section(int section) {
    int thread_id = omp_get_thread_num();
    PerfContext *ctx = &thread_context[thread_id];

    for (int i = 0; i < ctx->num_events; i++) {
        if (read(ctx->fd[i], &ctx->values[section][i], sizeof(uint64_t)) == -1) {
            perror("read");
            return -1;
        }
    }
    ctx->on_off_flag[section][0]++;
    return 0;
}

int perf_stop_section(int section) {
    int thread_id = omp_get_thread_num();
    PerfContext *ctx = &thread_context[thread_id];

    for (int i = 0; i < ctx->num_events; i++) {
        uint64_t value;
        if (read(ctx->fd[i], &value, sizeof(uint64_t)) == -1) {
            perror("read");
            return -1;
        }
        ctx->total_values[section][i] += (value - ctx->values[section][i]);
    }
    ctx->on_off_flag[section][1]++;
    return 0;
}

void perf_finalize() {
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        PerfContext *ctx = &thread_context[thread_id];

        for (int j = 0; j < MAX_SECTIONS; j++) {
            if (ctx->on_off_flag[j][0] == ctx->on_off_flag[j][1] && ctx->on_off_flag[j][0] != 0) {
                for (int i = 0; i < ctx->num_events; i++) {
                    printf("Measured counts for %s %lu section %d thread %d\n",
                        ctx->event_name[i], ctx->total_values[j][i], j, thread_id);
                }
            } else if (ctx->on_off_flag[j][0] != ctx->on_off_flag[j][1]) {
                fprintf(stderr, "Mismatched start/stop in section %d (thread %d).\n", j, thread_id);
            }
        }

        for (int i = 0; i < ctx->num_events; i++) {
            ioctl(ctx->fd[i], PERF_EVENT_IOC_DISABLE, 0);
            close(ctx->fd[i]);
        }
    }
}

#ifdef __cplusplus
}
#endif

