
# Performance Evaluation Library Using Linux perf for ARM Processors

## Introduction
The `perf_helper` library is designed for performance measurement using `perf`. 

- **PMU driver:**  armv8_pmuv3_0
- **Compilers:**  
  - GCC
  - GFORTRAN

---

## Adding Section Measurements
To measure performance within specific sections of your code:
1. Use `perf_initialize` and `perf_finalize` outside parallel regions.
2. Use `perf_start_section` and `perf_stop_section` within parallel regions.
3. Specify the events to measure using the `PERF_EVENTS` environment variable.

**Note:** Section IDs range from `0` to `99`, and nested sections are supported.

---

## Code Examples

### C
```c
#include "perf_helper.h"
void compute(int n, double x);

int main() {
    int n = 1000000;
    double x;

    perf_initialize();
    #pragma omp parallel
    {
        perf_start_section(0);
        perf_start_section(1);
        compute(n, x);
        perf_stop_section(1);
        perf_start_section(2);
        compute(n, x);
        perf_stop_section(2);
        perf_stop_section(0);
    }
    perf_finalize();
}
```

### Fortran90
```fortran
program main
  use perf_helper_mod
  implicit none
  integer, parameter :: n = 1000000
  double precision :: x
  integer :: i

  call perf_initialize()
  !$omp parallel
  call perf_start_section(0)
  do i = 1, 10
    ! Section 1
    call perf_start_section(1)
    call sample(n, x)
    call perf_stop_section(1)
    ! Section 2
    call perf_start_section(2)
    call sample(n, x)
    call perf_stop_section(2)
  end do
  call perf_stop_section(0)
  !$omp end parallel
  call perf_finalize()
end program main
```

---

## Compilation

### For GCC
```bash
#!/bin/sh
gcc -fopenmp -c main.c -o main.o
gcc -fopenmp -c test.c -o test.o
gcc -fopenmp main.o test.o -lperf_helper
```

### For Fortran90
```bash
#!/bin/sh
gfortran -fopenmp -c main.f90 -o main.o
gfortran -fopenmp -c test.f90 -o test.o
gfortran -fopenmp main.o test.o -lperf_helper
```

---

## Execution

1. Define performance events:
    ```bash
    export PERF_EVENTS="INST_SPEC,CPU_CYCLES,STALL_FRONTEND,STALL_BACKEND"
    ```
2. Run the executable:
    ```bash
    ./a.out
    ```
