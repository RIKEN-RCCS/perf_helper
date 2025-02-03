# Measure performance break down

def fugaku(pmu, args):

    ld_comp_wait_pfp_busy_swpf = pmu.value[162]
    ld_comp_wait_pfp_busy_ex = pmu.value[161]
    ld_comp_wait_l2_miss_ex = pmu.value[155]
    ld_comp_wait_l1_miss_ex = pmu.value[157]
    single_movprfx_commit = pmu.value[175]
    ld_comp_wait_pfp_busy = pmu.value[160]
    ase_sve_ld_multi_spec = pmu.value[427]
    ase_sve_st_multi_spec = pmu.value[428]
    ld_comp_wait_l2_miss = pmu.value[154]
    ld_comp_wait_l1_miss = pmu.value[156]
    sve_st_scatter_spec = pmu.value[434]
    sve_prf_gather_spec = pmu.value[435]
    sve_prf_contig_spec = pmu.value[422]
    rob_empty_stq_busy = pmu.value[167]
    sve_ld_gather_spec = pmu.value[433]
    sve_ldr_preg_spec = pmu.value[414]
    sve_str_preg_spec = pmu.value[415]
    ase_sve_int_spec = pmu.value[344]
    sve_ldr_reg_spec = pmu.value[411]
    sve_str_reg_spec = pmu.value[412]
    base_ld_reg_spec = pmu.value[404]
    base_st_reg_spec = pmu.value[405]
    sve_movprfx_spec = pmu.value[391]
    uop_only_commit = pmu.value[174]
    ld_comp_wait_ex = pmu.value[159]
    ase_sve_ld_spec = pmu.value[400]
    ase_sve_st_spec = pmu.value[401]
    fp_recpe_spec = pmu.value[329]
    wfe_wfi_cycle = pmu.value[168]
    sve_ldff_spec = pmu.value[446]
    _0inst_commit = pmu.value[169]
    ld_comp_wait = pmu.value[158]
    eu_comp_wait = pmu.value[163]
    fl_comp_wait = pmu.value[164]
    br_comp_wait = pmu.value[165]
    _1inst_commit = pmu.value[170]
    _2inst_commit = pmu.value[171]
    _3inst_commit = pmu.value[172]
    _4inst_commit = pmu.value[173]
    fp_fma_spec = pmu.value[317]
    fp_cvt_spec = pmu.value[333]
    cpu_cycles = pmu.value[17]
    fp_mv_spec = pmu.value[143]
    fp_ld_spec = pmu.value[147]
    bc_ld_spec = pmu.value[149]
    fp_st_spec = pmu.value[148]
    rob_empty = pmu.value[166]
    inst_spec = pmu.value[27]
    prd_spec = pmu.value[144]
    prf_spec = pmu.value[402]
    dp_spec = pmu.value[108]
    fp_spec = pmu.value[293]
    ld_spec = pmu.value[105]
    st_spec = pmu.value[106]
    br_pred = pmu.value[18]
    br_immed_spec = pmu.value[113]
    br_indirect_spec = pmu.value[115]
    vfp_spec = pmu.value[110]
    simd_inst_retired = pmu.value[279]
    sve_inst_retired = pmu.value[281]
    stall_frontend = pmu.value[35]
    stall_backend = pmu.value[36]
    fp_scale_ops_spec = pmu.value[448]
    fp_fixed_ops_spec = pmu.value[449]
    l1d_cache = pmu.value[4]
    l1d_cache_refill = pmu.value[3]
    l1d_cache_wb = pmu.value[21]
    l2d_cache = pmu.value[22]
    l2d_cache_refill = pmu.value[23]
    l2d_cache_wb = pmu.value[24]
    inst_retired = pmu.value[8]
    l1i_tlb_refill = pmu.value[2]
    l1d_tlb_refill = pmu.value[5]
    l2d_tlb_refill = pmu.value[45]
    l2d_tlb = pmu.value[47]

    time   = cpu_cycles/ pmu.CPU_CLOCK/1000000000
    IPC    = inst_retired/cpu_cycles
    GFLOPS = (fp_scale_ops_spec*(pmu.SVE_VEC_LEN/128)+fp_fixed_ops_spec)/(cpu_cycles/pmu.CPU_CLOCK)
    frontend_stalled_cycles = stall_frontend/ cpu_cycles* 100
    backend_stalled_cycles  = stall_backend/ cpu_cycles* 100
    retiring_cycles         = 100 - backend_stalled_cycles - frontend_stalled_cycles
    read_bw  = l2d_cache_refill * 256 / (cpu_cycles/pmu.CPU_CLOCK)
    write_bw = l2d_cache_wb * 256 / (cpu_cycles/pmu.CPU_CLOCK)

    if args.noheader != True:
        print(f"time(sec), {time:.2f}")
        print(f"IPC(Retired), {IPC:.2f}")
        print(f"GFLOPS, {GFLOPS:.2f}")
        print(f"R_BW(GB/s), {read_bw:.2f}")
        print(f"W_BW(GB/s), {write_bw:.2f}")
        print("# Performance break down")
        print(f"  cycle_backend_stalled, {backend_stalled_cycles:.2f}")
        print(f"  cycle_frontend_stalled, {frontend_stalled_cycles:.2f}")
        print(f"  cycle_retiring, {retiring_cycles:.2f}")
    else:
        print(f"{time:.2f}")
        print(f"{IPC:.2f}")
        print(f"{GFLOPS:.2f}")
        print(f"{read_bw:.2f}")
        print(f"{write_bw:.2f}")
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
        neon_simd  =(simd_inst_retired- sve_inst_retired)/ denomi
        sve_simd   = sve_inst_retired/ denomi
        others     = inst_spec/denomi - branch - integer_dp - scalar_fp - neon_simd - sve_simd
        if args.noheader != True:
            if denomi == inst_spec/100:
                print("# Instruction Mix(%)")
            elif denomi == cpu_cycles:
                print("# Instruction per Cycle(Spec)")
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
    l1d_cache_refill_ratio   = l1d_cache_refill/ l1d_cache* 100
    l1d_cache_wb_ratio       = l1d_cache_wb/ l1d_cache* 100
    l2_cache_refill_ratio    = l2d_cache_refill/ l2d_cache* 100
    l2_cache_wb_ratio        = l2d_cache_wb/ l2d_cache* 100
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

    l1i_tlb_mpki       = l1i_tlb_refill/ inst_retired* 1000
    l1d_tlb_mpki       = l1d_tlb_refill/ inst_retired* 1000
    l2_tlb_mpki        = l2d_tlb_refill/ inst_retired* 1000
    if args.noheader != True:
        print("# TLB Effectiveness")
        print(f"  itlb_mpki")
        print(f"  dtlb_mpki")
        print(f"  l1i_tlb_mpki, {l1i_tlb_mpki:.2f}")
        print(f"  l1d_tlb_mpki, {l1d_tlb_mpki:.2f}")
        print(f"  l2_tlb_mpki, {l2_tlb_mpki:.2f}")
        print(f"  itlb_walk_ratio")
        print(f"  dtlb_walk_ratio")
        print(f"  l1i_tlb_miss_ratio")
        print(f"  l1d_tlb_miss_ratio")
        print(f"  l2_tlb_miss_ratio")
    else:
        print()
        print()
        print()
        print(f"{l1i_tlb_mpki:.2f}")
        print(f"{l1d_tlb_mpki:.2f}")
        print(f"{l2_tlb_mpki:.2f}")
        print()
        print()
        print()
        print()
        print()

    denomi = 1/cpu_cycles
    if args.noheader != True:
        print(f"# Instruction per Cycle(Spec)")
        print(f",fp_spec,,,,,{fp_spec * denomi:.2f}")
        print(f",,fp_fma_spec,,,,{fp_fma_spec * denomi:.2f}")
        print(f",,fp_recpe_spec,,,,{fp_recpe_spec * denomi:.2f}")
        print(f",,others(Basic FP operations),,,,{(fp_spec-fp_fma_spec-fp_recpe_spec) * denomi:.2f}")
        print(f",fp_cvt_spec,,,,,{fp_cvt_spec * denomi:.2f}")
        print(f",fp_mv_spec,,,,,{fp_mv_spec * denomi:.2f}")
        print(f",ase_sve_int_spec,,,,,{ase_sve_int_spec * denomi:.2f}")
        print(f",prd_spec,,,,,{prd_spec * denomi:.2f}")
        print(f",ld_spec,,,,,{ld_spec * denomi:.2f}")
        print(f",,base_ld_reg_spec,,,,{base_ld_reg_spec * denomi:.2f}")
        print(f",,ase_sve_ld_spec,,,,{ase_sve_ld_spec * denomi:.2f}")
        print(f",,,fp_ld_spec,,,{fp_ld_spec * denomi:.2f}")
        print(f",,,others(All vector loads),,,{(ase_sve_ld_spec-fp_ld_spec) * denomi:.2f}")
        print(f",,,,sve_ldr_reg_spec,,{sve_ldr_reg_spec * denomi:.2f}")
        print(f",,,,,sve_ldr_preg_spec,{sve_ldr_preg_spec * denomi:.2f}")
        print(f",,,,,others(LDR vector),{(sve_ldr_reg_spec-sve_ldr_preg_spec) * denomi:.2f}")
        print(f",,,,bc_ld_spec,,{bc_ld_spec * denomi:.2f}")
        print(f",,,,ase_sve_ld_multi_spec,,{ase_sve_ld_multi_spec * denomi:.2f}")
        print(f",,,,sve_ld_gather_spec,,{sve_ld_gather_spec * denomi:.2f}")
        print(f",,,,sve_ldff_spec,,{sve_ldff_spec * denomi:.2f}")
        print(f",,,,others(Basic vector loads),,{(ase_sve_ld_spec-fp_ld_spec-sve_ldr_reg_spec-bc_ld_spec-ase_sve_ld_multi_spec-sve_ld_gather_spec-sve_ldff_spec) * denomi:.2f}")
        print(f",st_spec,,,,,{st_spec * denomi:.2f}")
        print(f",,base_st_reg_spec,,,,{base_st_reg_spec * denomi:.2f}")
        print(f",,ase_sve_st_spec,,,,{ase_sve_st_spec * denomi:.2f}")
        print(f",,,fp_st_spec,,,{fp_st_spec * denomi:.2f}")
        print(f",,,others(All vector stores),,,{(ase_sve_st_spec-fp_st_spec) * denomi:.2f}")
        print(f",,,,sve_str_reg_spec,,{sve_str_reg_spec * denomi:.2f}")
        print(f",,,,,sve_str_preg_spec,{sve_str_preg_spec * denomi:.2f}")
        print(f",,,,,others(STR vector),{(sve_str_reg_spec-sve_str_preg_spec) * denomi:.2f}")
        print(f",,,,ase_sve_st_multi_spec,,{ase_sve_st_multi_spec * denomi:.2f}")
        print(f",,,,sve_st_scatter_spec,,{sve_st_scatter_spec * denomi:.2f}")
        print(f",,,,others(Basic vector stores),,{(ase_sve_st_spec-fp_st_spec-sve_str_reg_spec-ase_sve_st_multi_spec-sve_st_scatter_spec) * denomi:.2f}")
        print(f",prf_spec,,,,,{prf_spec * denomi:.2f}")
        print(f",,sve_prf_gather_spec,,,,{sve_prf_gather_spec * denomi:.2f}")
        print(f",,sve_prf_contig_spec,,,,{sve_prf_contig_spec * denomi:.2f}")
        print(f",,others(Prefetch in base inst.),,,,{(prf_spec-sve_prf_gather_spec-sve_prf_contig_spec) * denomi:.2f}")
        print(f",dp_spec,,,,,{dp_spec * denomi:.2f}")
        print(f",sve_movprfx_spec,,,,,{sve_movprfx_spec * denomi:.2f}")
        print(f",br_pred,,,,,{br_pred * denomi:.2f}")
        print(f",others(Base insts. excluding load/store),,,,,{(inst_spec-fp_spec-fp_cvt_spec-fp_mv_spec-ase_sve_int_spec-prd_spec-ld_spec-st_spec-prf_spec-dp_spec-sve_movprfx_spec-br_pred) * denomi:.2f}")
    else:
        print()
        print(f"{fp_spec * denomi:.2f}")
        print(f"{fp_fma_spec * denomi:.2f}")
        print(f"{fp_recpe_spec * denomi:.2f}")
        print(f"{(fp_spec-fp_fma_spec-fp_recpe_spec) * denomi:.2f}")
        print(f"{fp_cvt_spec * denomi:.2f}")
        print(f"{fp_mv_spec * denomi:.2f}")
        print(f"{ase_sve_int_spec * denomi:.2f}")
        print(f"{prd_spec * denomi:.2f}")
        print(f"{ld_spec * denomi:.2f}")
        print(f"{base_ld_reg_spec * denomi:.2f}")
        print(f"{ase_sve_ld_spec * denomi:.2f}")
        print(f"{fp_ld_spec * denomi:.2f}")
        print(f"{(ase_sve_ld_spec-fp_ld_spec) * denomi:.2f}")
        print(f"{sve_ldr_reg_spec * denomi:.2f}")
        print(f"{sve_ldr_preg_spec * denomi:.2f}")
        print(f"{(sve_ldr_reg_spec-sve_ldr_preg_spec) * denomi:.2f}")
        print(f"{bc_ld_spec * denomi:.2f}")
        print(f"{ase_sve_ld_multi_spec * denomi:.2f}")
        print(f"{sve_ld_gather_spec * denomi:.2f}")
        print(f"{sve_ldff_spec * denomi:.2f}")
        print(f"{(ase_sve_ld_spec-fp_ld_spec-sve_ldr_reg_spec-bc_ld_spec-ase_sve_ld_multi_spec-sve_ld_gather_spec-sve_ldff_spec) * denomi:.2f}")
        print(f"{st_spec * denomi:.2f}")
        print(f"{base_st_reg_spec * denomi:.2f}")
        print(f"{ase_sve_st_spec * denomi:.2f}")
        print(f"{fp_st_spec * denomi:.2f}")
        print(f"{(ase_sve_st_spec-fp_st_spec) * denomi:.2f}")
        print(f"{sve_str_reg_spec * denomi:.2f}")
        print(f"{sve_str_preg_spec * denomi:.2f}")
        print(f"{(sve_str_reg_spec-sve_str_preg_spec) * denomi:.2f}")
        print(f"{ase_sve_st_multi_spec * denomi:.2f}")
        print(f"{sve_st_scatter_spec * denomi:.2f}")
        print(f"{(ase_sve_st_spec-fp_st_spec-sve_str_reg_spec-ase_sve_st_multi_spec-sve_st_scatter_spec) * denomi:.2f}")
        print(f"{prf_spec * denomi:.2f}")
        print(f"{sve_prf_gather_spec * denomi:.2f}")
        print(f"{sve_prf_contig_spec * denomi:.2f}")
        print(f"{(prf_spec-sve_prf_gather_spec-sve_prf_contig_spec) * denomi:.2f}")
        print(f"{dp_spec * denomi:.2f}")
        print(f"{sve_movprfx_spec * denomi:.2f}")
        print(f"{br_pred * denomi:.2f}")
        print(f"{(inst_spec-fp_spec-fp_cvt_spec-fp_mv_spec-ase_sve_int_spec-prd_spec-ld_spec-st_spec-prf_spec-dp_spec-sve_movprfx_spec-br_pred) * denomi:.2f}")

    if args.noheader != True:
        print(f"# Cycle accounting")
        print(f",0inst_commit,,,,{_0inst_commit* denomi:.2f}")
        print(f",,ld_comp_wait,,,{ld_comp_wait* denomi:.2f}")
        print(f",,,ld_comp_wait_ex,,{ld_comp_wait_ex* denomi:.2f}")
        print(f",,,ld_comp_wait_l2_miss,,{ld_comp_wait_l2_miss* denomi:.2f}")
        print(f",,,,ld_comp_wait_l2_miss_ex,{ld_comp_wait_l2_miss_ex* denomi:.2f}")
        print(f",,,,others,{(ld_comp_wait_l2_miss-ld_comp_wait_l2_miss_ex)* denomi:.2f}")
        print(f",,,ld_comp_wait_l1_miss,,{ld_comp_wait_l1_miss* denomi:.2f}")
        print(f",,,,ld_comp_wait_l1_miss_ex,{ld_comp_wait_l1_miss_ex* denomi:.2f}")
        print(f",,,,others,{(ld_comp_wait_l1_miss-ld_comp_wait_l1_miss_ex)* denomi:.2f}")
        print(f",,,ld_comp_wait_pfp_busy,,{ld_comp_wait_pfp_busy* denomi:.2f}")
        print(f",,,,ld_comp_wait_pfp_busy_ex,{ld_comp_wait_pfp_busy_ex* denomi:.2f}")
        print(f",,,,ld_comp_wait_pfp_busy_swpf,{ld_comp_wait_pfp_busy_swpf* denomi:.2f}")
        print(f",,,,others,{(ld_comp_wait_pfp_busy-ld_comp_wait_pfp_busy_ex-ld_comp_wait_pfp_busy_swpf)* denomi:.2f}")
        print(f",,,others,,{(ld_comp_wait-ld_comp_wait_ex-ld_comp_wait_l2_miss-ld_comp_wait_l1_miss-ld_comp_wait_pfp_busy)* denomi:.2f}")
        print(f",,eu_comp_wait,,,{eu_comp_wait* denomi:.2f}")
        print(f",,,fl_comp_wait,,{fl_comp_wait* denomi:.2f}")
        print(f",,,others,,{(eu_comp_wait-fl_comp_wait)* denomi:.2f}")
        print(f",,br_comp_wait,,,{br_comp_wait* denomi:.2f}")
        print(f",,rob_empty,,,{rob_empty* denomi:.2f}")
        print(f",,,rob_empty_stq_busy,,{rob_empty_stq_busy* denomi:.2f}")
        print(f",,,wfe_wfi_cycle,,{wfe_wfi_cycle* denomi:.2f}")
        print(f",,,others,,{(rob_empty-rob_empty_stq_busy-wfe_wfi_cycle)* denomi:.2f}")
        print(f",,uop_only_commit,,,{uop_only_commit* denomi:.2f}")
        print(f",,single_movprfx_commit,,,{single_movprfx_commit* denomi:.2f}")
        print(f",,others,,,{(_0inst_commit-ld_comp_wait-eu_comp_wait-br_comp_wait-rob_empty-uop_only_commit-single_movprfx_commit)* denomi:.2f}")
        print(f",1inst_commit,,,,{_1inst_commit* denomi:.2f}")
        print(f",2inst_commit,,,,{_2inst_commit* denomi:.2f}")
        print(f",3inst_commit,,,,{_3inst_commit* denomi:.2f}")
        print(f",4inst_commit,,,,{_4inst_commit* denomi:.2f}")
    else:
        print()
        print(f"{_0inst_commit* denomi:.2f}")
        print(f"{ld_comp_wait* denomi:.2f}")
        print(f"{ld_comp_wait_ex* denomi:.2f}")
        print(f"{ld_comp_wait_l2_miss* denomi:.2f}")
        print(f"{ld_comp_wait_l2_miss_ex* denomi:.2f}")
        print(f"{(ld_comp_wait_l2_miss-ld_comp_wait_l2_miss_ex)* denomi:.2f}")
        print(f"{ld_comp_wait_l1_miss* denomi:.2f}")
        print(f"{ld_comp_wait_l1_miss_ex* denomi:.2f}")
        print(f"{(ld_comp_wait_l1_miss-ld_comp_wait_l1_miss_ex)* denomi:.2f}")
        print(f"{ld_comp_wait_pfp_busy* denomi:.2f}")
        print(f"{ld_comp_wait_pfp_busy_ex* denomi:.2f}")
        print(f"{ld_comp_wait_pfp_busy_swpf* denomi:.2f}")
        print(f"{(ld_comp_wait_pfp_busy-ld_comp_wait_pfp_busy_ex-ld_comp_wait_pfp_busy_swpf)* denomi:.2f}")
        print(f"{(ld_comp_wait-ld_comp_wait_ex-ld_comp_wait_l2_miss-ld_comp_wait_l1_miss-ld_comp_wait_pfp_busy)* denomi:.2f}")
        print(f"{eu_comp_wait* denomi:.2f}")
        print(f"{fl_comp_wait* denomi:.2f}")
        print(f"{(eu_comp_wait-fl_comp_wait)* denomi:.2f}")
        print(f"{br_comp_wait* denomi:.2f}")
        print(f"{rob_empty* denomi:.2f}")
        print(f"{rob_empty_stq_busy* denomi:.2f}")
        print(f"{wfe_wfi_cycle* denomi:.2f}")
        print(f"{(rob_empty-rob_empty_stq_busy-wfe_wfi_cycle)* denomi:.2f}")
        print(f"{uop_only_commit* denomi:.2f}")
        print(f"{single_movprfx_commit* denomi:.2f}")
        print(f"{(_0inst_commit-ld_comp_wait-eu_comp_wait-br_comp_wait-rob_empty-uop_only_commit-single_movprfx_commit)* denomi:.2f}")
        print(f"{_1inst_commit* denomi:.2f}")
        print(f"{_2inst_commit* denomi:.2f}")
        print(f"{_3inst_commit* denomi:.2f}")
        print(f"{_4inst_commit* denomi:.2f}")
