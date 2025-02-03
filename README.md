
# Performance Evaluation Library Using Linux perf for ARM Processors

## Introduction
The `perf_helper` library is designed for performance measurement using `perf`. 

- **PMU driver:**  armv8_pmuv3_0
- **Compilers:**  
  - GCC (gcc, gfortran)
  - ARM Compiler (armclang, armflang)
  - Fujitsu Compiler (fccpx, frtpx)

---

## Build perf_helper library
To build perf_helper library, simply execute make.
- for GCC : `make COMPILER=gcc`
- for ARM Compiler : `make COMPILER=acfl`
- for Fujitsu Compiler : `make COMPILER=fj`

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

### Sample script:

This script execute load module 9times and measure each counter set defined by environment variables COUNTER[0-9].
Since the measurement results are output to standard output, it is recommended to redirect them to an appropriate file.

```bash
#!/bin/sh
# 1.Performance
COUNTER1="INST_SPEC,CPU_CYCLES,STALL_FRONTEND,STALL_BACKEND,FP_SCALE_OPS_SPEC,FP_FIXED_OPS_SPEC"
# 2.Instruction Mix
COUNTER2="CPU_CYCLES,BR_IMMED_SPEC,BR_INDIRECT_SPEC,LD_SPEC,ST_SPEC"
COUNTER3="DP_SPEC,VFP_SPEC,ASE_INST_SPEC,SVE_INST_SPEC"
# 3.Cache Effectiveness
COUNTER4="INST_RETIRED,L1D_CACHE,L1D_CACHE_REFILL,L1D_CACHE_WB"
COUNTER5="L2D_CACHE,L2D_CACHE_REFILL,L2D_CACHE_WB,LL_CACHE_RD,LL_CACHE_MISS_RD"
# 4.TLB Effectivenes
COUNTER6="INST_RETIRED,DTLB_WALK,ITLB_WALK,L1I_TLB_REFILL,L1D_TLB_REFILL"
COUNTER7="L1D_TLB,L1I_TLB,L2D_TLB_REFILL,L2D_TLB"
# Performance Debug with implimention defined counter
COUNTER8="0x0E1,0x0E2,0x15B,0x158,0x159,0x15A"
COUNTER9="0x15C,0x15D,0x15E,0x15F,0x160"

LD=${1-"./a.out"}
export OMP_NUM_THREADS=${2-"2"}
export THREAD_STACK_SIZE=8192
export OMP_PROC_BIND=spread

STA=0
END=`expr ${STA} + ${OMP_NUM_THREADS} - 1`

for i in `seq 1 9`;do
  C=COUNTER${i}
  export PAPI_EVENTS=`eval echo '$'$C`
  taskset -c ${STA}-${END} ${LD}
done
```

### Analyze performance counter output

The counter values output to output.txt can be analyzed using the following command.

```bash
python3 anal.py --cpu graviton4 output.txt
```
