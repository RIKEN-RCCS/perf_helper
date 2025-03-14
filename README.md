
# Performance Evaluation Library Using Linux perf for ARM Processors

## Introduction
The `perf_helper` library is designed for performance measurement using `perf`. 
Performance measurement is conducted by collecting values from PMU counters built into ARM processors.
It supports interval-based measurement by inserting functions into the source code to mark the start and end of measurement sections.
The number of PMU counters that can be measured simultaneously is limited by the CPU architecture.
Although mechanisms like multiplexing exist to increase the apparent number of simultaneously measurable counters, this library prioritizes measurement accuracy and does not incorporate such mechanisms.

- **PMU driver:**  armv8_pmuv3_0
- **Compilers:**  
  - GCC (gcc, gfortran)
  - ARM Compiler (armclang, armflang)
  - Fujitsu Compiler (fccpx, frtpx)
  - LLVM (clang, flang)

---

## Build perf_helper library
To build perf_helper library, simply execute make.
- for GCC : `make COMPILER=gcc`
- for ARM Compiler : `make COMPILER=acfl`
- for Fujitsu Compiler : `make COMPILER=fj`
- for LLVM : `make COMPILER=llvm`

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

### Experimental : Python

> note : perf_start_section and perf_stop_section can be executed inside an OpenMP parallel region to obtain information for each thread. However, since Python does not have a built-in feature to directly execute OpenMP, in the example below, these functions are executed outside the parallel region, resulting in collecting information only for the master thread.

```python
import torch
import time
import ctypes

perf = ctypes.CDLL('./libperf_helper.so')

perf.perf_start_section.argtypes = [ctypes.c_int]
perf.perf_stop_section.argtypes = [ctypes.c_int]

print("oneDNN (MKL-DNN) enabled:", torch.backends.mkldnn.is_available())
print("ACL enabled:", torch.backends.quantized.engine)

def benchmark_matmul():
    x = torch.randn(1000, 1000)
    y = torch.randn(1000, 1000)
    start = time.time()
    perf.perf_start_section(0)
    for _ in range(100):
        z = torch.matmul(x, y)
    perf.perf_stop_section(0)
    end = time.time()
    print(f"Execution time: {end - start:.5f} seconds")

perf.perf_initialize()
benchmark_matmul()
perf.perf_finalize()
```

---

## Compilation

### For GCC
```bash
#!/bin/sh
gcc -fopenmp -c main.c -o main.o -J/usr/local/lib
gcc -fopenmp -c test.c -o test.o
gcc -fopenmp main.o test.o -lperf_helper
```

### For Fortran90
```bash
#!/bin/sh
gfortran -fopenmp -c main.f90 -o main.o -J/usr/local/lib
gfortran -fopenmp -c test.f90 -o test.o
gfortran -fopenmp main.o test.o -lperf_helper
```

---

## Execution

### Sample script for Neoverse:

This script execute load module 9 times and measure each counter set defined by environment variables COUNTER[1-9].
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
# Performance Debug with implementation defined counters
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
  export PERF_EVENTS=`eval echo '$'$C`
  taskset -c ${STA}-${END} ${LD}
done
```

### Sample script for A64FX:

This script execute load module 10 times and measure each counter set defined by environment variables COUNTER[1-10].
Since the measurement results are output to standard output, it is recommended to redirect them to an appropriate file.

```bash
#!/bin/sh
# 1.Cycle Accounting
COUNTER1="cpu_cycles,0inst_commit,ld_comp_wait,ld_comp_wait_ex,ld_comp_wait_l2_miss,ld_comp_wait_l2_miss_ex,ld_comp_wait_l1_miss,ld_comp_wait_l1_miss_ex,ld_comp_wait_pfp_busy"
COUNTER2="ld_comp_wait_pfp_busy_ex,ld_comp_wait_pfp_busy_swpf,eu_comp_wait,fl_comp_wait,br_comp_wait,rob_empty,rob_empty_stq_busy,wfe_wfi_cycle"
COUNTER3="uop_only_commit,single_movprfx_commit,1inst_commit,2inst_commit,3inst_commit,4inst_commit"
# 2.Instruction Mix
COUNTER4="cpu_cycles,inst_spec,fp_spec,fp_fma_spec,fp_recpe_spec,fp_cvt_spec,fp_mv_spec,ase_sve_int_spec,prd_spec"
COUNTER5="ld_spec,base_ld_reg_spec,ase_sve_ld_spec,fp_ld_spec,sve_ldr_reg_spec,sve_ldr_preg_spec,bc_ld_spec,ase_sve_ld_multi_spec"
COUNTER6="sve_ld_gather_spec,sve_ldff_spec,st_spec,base_st_reg_spec,ase_sve_st_spec,fp_st_spec,sve_str_reg_spec,sve_str_preg_spec"
COUNTER7="ase_sve_st_multi_spec,sve_st_scatter_spec,prf_spec,sve_prf_gather_spec,sve_prf_contig_spec,br_pred,sve_movprfx_spec,dp_spec"
# 3.Graviton3E like analysis
COUNTER8="cpu_cycles,BR_IMMED_SPEC,BR_INDIRECT_SPEC,VFP_SPEC,SIMD_INST_RETIRED,SVE_INST_RETIRED,inst_spec"
COUNTER9="inst_retired,stall_frontend,stall_backend,fp_scale_ops_spec,fp_fixed_ops_spec,l1i_tlb_refill,l1d_tlb_refill"
COUNTER10="l1d_cache,l1d_cache_refill,l1d_cache_wb,l2d_cache,l2d_cache_refill,l2d_cache_wb,l2d_tlb_refill,l2d_tlb"

export OMP_NUM_THREADS=${1-"48"}
export THREAD_STACK_SIZE=8192
export OMP_PROC_BIND=spread

STA=12
END=`expr 12 + ${OMP_NUM_THREADS} - 1`

xospastop

for i in `seq 1 10`;do
 C=COUNTER${i}
 export PERF_EVENTS=`eval echo '$'$C`
 taskset -c ${STA}-${END} ./a.out
done
```

### Analyze performance counter output

The counter values output to output.txt can be analyzed using the following command.
Specify the `--cpu` option appropriately according to the measured environment.

```bash
python3 anal.py --cpu {fugaku|graviton3e|graviton4|gracecpu} output.txt
```
