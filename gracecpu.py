# Measure performance break down

def gracecpu(pmu, args):

    inst_spec = pmu.value[27]
    inst_retired = pmu.value[8]
    cpu_cycles = pmu.value[17]
    stall_frontend = pmu.value[35]
    stall_backend = pmu.value[36]
    dtlb_walk = pmu.value[52]
    itlb_walk = pmu.value[53]
    l1i_tlb = pmu.value[38]
    l1i_tlb_refill = pmu.value[2]
    l1d_tlb = pmu.value[37]
    l1d_tlb_refill = pmu.value[5]
    l2d_tlb = pmu.value[47]
    l2d_tlb_refill = pmu.value[45]
    l1i_cache = pmu.value[20]
    l1i_cache_refill = pmu.value[1]
    l1d_cache = pmu.value[4]
    l1d_cache_refill = pmu.value[3]
    l1d_cache_wb = pmu.value[21]
    l2d_cache_refill = pmu.value[23]
    l2d_cache_wb = pmu.value[24]
    l2d_cache = pmu.value[22]
    ll_cache_rd = pmu.value[54]
    ll_cache_miss_rd = pmu.value[55]
    br_immed_spec = pmu.value[113]
    br_indirect_spec = pmu.value[115]
    ld_spec = pmu.value[105]
    st_spec = pmu.value[106]
    dp_spec = pmu.value[108]
    vfp_spec = pmu.value[110]
    ase_inst_spec = pmu.value[284]
    sve_inst_spec = pmu.value[285]
    fp_fixed_ops_spec = pmu.value[449]
    fp_scale_ops_spec = pmu.value[448]

    time   = cpu_cycles/ pmu.CPU_CLOCK/1000000000
    IPC    = inst_retired/cpu_cycles
    GFLOPS = (fp_scale_ops_spec*(pmu.SVE_VEC_LEN/128)+fp_fixed_ops_spec)/(cpu_cycles/pmu.CPU_CLOCK)
    frontend_stalled_cycles = stall_frontend/ cpu_cycles* 100
    backend_stalled_cycles  = stall_backend/ cpu_cycles* 100
    retiring_cycles         = 100 - backend_stalled_cycles - frontend_stalled_cycles
    read_bw  = ll_cache_miss_rd * 64 / (cpu_cycles/pmu.CPU_CLOCK)
    # write_bw = It is impossible to measure write writeback bandwidth since counter is not implemented.

    if args.noheader != True:
        print(f"time(sec), {time:.2f}")
        print(f"IPC(Retired), {IPC:.2f}")
        print(f"GFLOPS, {GFLOPS:.2f}")
        print(f"R_BW(GB/s), {read_bw:.2f}")
        print(f"W_BW(GB/s)")
        print("# Performance break down")
        print(f"  cycle_backend_stalled, {backend_stalled_cycles:.2f}")
        print(f"  cycle_frontend_stalled, {frontend_stalled_cycles:.2f}")
        print(f"  cycle_retiring, {retiring_cycles:.2f}")
    else:
        print(f"{time:.2f}")
        print(f"{IPC:.2f}")
        print(f"{GFLOPS:.2f}")
        print(f"{read_bw:.2f}")
        print()
        print()
        print(f"{backend_stalled_cycles:.2f}")
        print(f"{frontend_stalled_cycles:.2f}")
        print(f"{retiring_cycles:.2f}")

    for denomi in [inst_spec/100, cpu_cycles]:
        branch     =(br_immed_spec+ br_indirect_spec) / denomi
        integer_dp = dp_spec/ denomi
        scalar_fp  = vfp_spec/ denomi
        load       = ld_spec/ denomi
        store      = st_spec/ denomi
        neon_simd  = ase_inst_spec/ denomi
        sve_simd   = sve_inst_spec/ denomi
        others     = inst_spec/denomi - branch - integer_dp - scalar_fp - neon_simd - sve_simd
        if args.noheader != True:
            if denomi == inst_spec/100:
                print(f"# Instruction Mix(%)")
            elif denomi == cpu_cycles:
                print(f"# Instruction per Cycle(Spec)")
            print(f"  branch, {branch:.2f}")
            print(f"  integer_dp, {integer_dp:.2f}")
            print(f"  scalar_fp, {scalar_fp:.2f}")
            print(f"  load, {load:.2f}")
            print(f"  store, {store:.2f}")
            print(f"  asimd_simd, {neon_simd:.2f}")
            print(f"  sve_simd, {sve_simd:.2f}")
            print(f"  others, {others:.2f}")
        else:
            print()
            print(f"{branch:.2f}")
            print(f"{integer_dp:.2f}")
            print(f"{scalar_fp:.2f}")
            print(f"{load:.2f}")
            print(f"{store:.2f}")
            print(f"{neon_simd:.2f}")
            print(f"{sve_simd:.2f}")
            print(f"{others:.2f}")

    l1d_cache_refill_mpki    = l1d_cache_refill/ inst_retired* 1000
    l1d_cache_wb_mpki        = l1d_cache_wb/ inst_retired* 1000
    l2_cache_refill_mpki     = l2d_cache_refill/ inst_retired* 1000
    l2_cache_wb_mpki         = l2d_cache_wb/ inst_retired* 1000
    #ll_cache_read_mpki       = ll_cache_miss_rd/ inst_retired* 1000
    l1d_cache_refill_ratio   = l1d_cache_refill/ l1d_cache* 100
    l1d_cache_wb_ratio       = l1d_cache_wb/ l1d_cache* 100
    l2_cache_refill_ratio    = l2d_cache_refill/ l2d_cache* 100
    l2_cache_wb_ratio        = l2d_cache_wb/ l2d_cache* 100
    #ll_cache_read_miss_ratio = ll_cache_miss_rd/ ll_cache_rd* 100
    if args.noheader != True:
        print("# Cache Effectiveness")
        print(f"  l1d_cache_refill_mpki, {l1d_cache_refill_mpki:.2f}")
        print(f"  l1d_cache_wb_mpki, {l1d_cache_wb_mpki:.2f}")
        print(f"  l2_cache_refill_mpki, {l2_cache_refill_mpki:.2f}")
        print(f"  l2_cache_wb_mpki, {l2_cache_wb_mpki:.2f}")
        print(f"  ll_cache_read_mpki")
        print(f"  l1d_cache_refill_ratio, {l1d_cache_refill_ratio:.2f}")
        print(f"  l1d_cache_wb_ratio, {l1d_cache_wb_ratio:.2f}")
        print(f"  l2_cache_refill_ratio, {l2_cache_refill_ratio:.2f}")
        print(f"  l2_cache_wb_ratio, {l2_cache_wb_ratio:.2f}")
        print(f"  ll_cache_read_miss_ratio")
    else:
        print()
        print(f"{l1d_cache_refill_mpki:.2f}")
        print(f"{l1d_cache_wb_mpki:.2f}")
        print(f"{l2_cache_refill_mpki:.2f}")
        print(f"{l2_cache_wb_mpki:.2f}")
        print()
        print(f"{l1d_cache_refill_ratio:.2f}")
        print(f"{l1d_cache_wb_ratio:.2f}")
        print(f"{l2_cache_refill_ratio:.2f}")
        print(f"{l2_cache_wb_ratio:.2f}")
        print()


    itlb_mpki          = itlb_walk/ inst_retired* 1000
    dtlb_mpki          = dtlb_walk/ inst_retired* 1000
    l1i_tlb_mpki       = l1i_tlb_refill/ inst_retired* 1000
    l1d_tlb_mpki       = l1d_tlb_refill/ inst_retired* 1000
    l2_tlb_mpki        = l2d_tlb_refill/ inst_retired* 1000
    itlb_walk_ratio    = itlb_walk/ l1i_tlb* 100
    dtlb_walk_ratio    = dtlb_walk/ l1d_tlb* 100
    l1i_tlb_miss_ratio = l1i_tlb_refill/ l1i_tlb* 100
    l1d_tlb_miss_ratio = l1d_tlb_refill/ l1d_tlb* 100
    l2_tlb_miss_ratio  = l2d_tlb_refill/ l2d_tlb* 100
    if args.noheader != True:
        print("# TLB Effectiveness")
        print(f"  itlb_mpki, {itlb_mpki:.2f}")
        print(f"  dtlb_mpki, {dtlb_mpki:.2f}")
        print(f"  l1i_tlb_mpki, {l1i_tlb_mpki:.2f}")
        print(f"  l1d_tlb_mpki, {l1d_tlb_mpki:.2f}")
        print(f"  l2_tlb_mpki, {l2_tlb_mpki:.2f}")
        print(f"  itlb_walk_ratio, {itlb_walk_ratio:.2f}")
        print(f"  dtlb_walk_ratio, {dtlb_walk_ratio:.2f}")
        print(f"  l1i_tlb_miss_ratio, {l1i_tlb_miss_ratio:.2f}")
        print(f"  l1d_tlb_miss_ratio, {l1d_tlb_miss_ratio:.2f}")
        print(f"  l2_tlb_miss_ratio, {l2_tlb_miss_ratio:.2f}")
    else:
        print()
        print(f"{itlb_mpki:.2f}")
        print(f"{dtlb_mpki:.2f}")
        print(f"{l1i_tlb_mpki:.2f}")
        print(f"{l1d_tlb_mpki:.2f}")
        print(f"{l2_tlb_mpki:.2f}")
        print(f"{itlb_walk_ratio:.2f}")
        print(f"{dtlb_walk_ratio:.2f}")
        print(f"{l1i_tlb_miss_ratio:.2f}")
        print(f"{l1d_tlb_miss_ratio:.2f}")
        print(f"{l2_tlb_miss_ratio:.2f}")
