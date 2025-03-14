#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <omp.h>

#define MAX_SECTIONS 100
#define MAX_EVENTS 10
#define MAX_THREADS 256

typedef struct {
    int fd[MAX_EVENTS];
    uint64_t event_codes[MAX_EVENTS];
    char *event_name[MAX_EVENTS];
    uint64_t total_values[MAX_SECTIONS][MAX_EVENTS];
    uint64_t values[MAX_SECTIONS][MAX_EVENTS];
    int num_events;
    int on_off_flag[MAX_SECTIONS][2];
} PerfContext;

extern PerfContext thread_context[MAX_THREADS];
int perf_initialize();
int perf_start_section(int section);
int perf_stop_section(int section);
void perf_finalize();

// Wrapper for Fortran
void f_perf_initialize();
void f_perf_start_section(int *section);
void f_perf_stop_section(int *section);
void f_perf_finalize();

typedef struct {
    char *name;
    int hex_code;
} EventMapping;

static const EventMapping event_list[] = {
    {"SW_INCR", 0x0000},
    {"L1I_CACHE_REFILL", 0x0001},
    {"L1I_TLB_REFILL", 0x0002},
    {"L1D_CACHE_REFILL", 0x0003},
    {"L1D_CACHE", 0x0004},
    {"L1D_TLB_REFILL", 0x0005},
    {"LD_RETIRED", 0x0006},
    {"ST_RETIRED", 0x0007},
    {"INST_RETIRED", 0x0008},
    {"EXC_TAKEN", 0x0009},
    {"EXC_RETURN", 0x000A},
    {"CID_WRITE_RETIRED", 0x000B},
    {"PC_WRITE_RETIRED", 0x000C},
    {"BR_IMMED_RETIRED", 0x000D},
    {"BR_RETURN_RETIRED", 0x000E},
    {"UNALIGNED_LDST_RETIRED", 0x000F},
    {"BR_MIS_PRED", 0x0010},
    {"CPU_CYCLES", 0x0011},
    {"BR_PRED", 0x0012},
    {"MEM_ACCESS", 0x0013},
    {"L1I_CACHE", 0x0014},
    {"L1D_CACHE_WB", 0x0015},
    {"L2D_CACHE", 0x0016},
    {"L2D_CACHE_REFILL", 0x0017},
    {"L2D_CACHE_WB", 0x0018},
    {"BUS_ACCESS", 0x0019},
    {"MEMORY_ERROR", 0x001A},
    {"INST_SPEC", 0x001B},
    {"TTBR_WRITE_RETIRED", 0x001C},
    {"BUS_CYCLES", 0x001D},
    {"CHAIN", 0x001E},
    {"L1D_CACHE_ALLOCATE", 0x001F},
    {"L2D_CACHE_ALLOCATE", 0x0020},
    {"BR_RETIRED", 0x0021},
    {"BR_MIS_PRED_RETIRED", 0x0022},
    {"STALL_FRONTEND", 0x0023},
    {"STALL_BACKEND", 0x0024},
    {"L1D_TLB", 0x0025},
    {"L1I_TLB", 0x0026},
    {"L2I_CACHE", 0x0027},
    {"L2I_CACHE_REFILL", 0x0028},
    {"L3D_CACHE_ALLOCATE", 0x0029},
    {"L3D_CACHE_REFILL", 0x002A},
    {"L3D_CACHE", 0x002B},
    {"L3D_CACHE_WB", 0x002C},
    {"L2D_TLB_REFILL", 0x002D},
    {"L2I_TLB_REFILL", 0x002E},
    {"L2D_TLB", 0x002F},
    {"L2I_TLB", 0x0030},
    {"REMOTE_ACCESS", 0x0031},
    {"LL_CACHE", 0x0032},
    {"LL_CACHE_MISS", 0x0033},
    {"DTLB_WALK", 0x0034},
    {"ITLB_WALK", 0x0035},
    {"LL_CACHE_RD", 0x0036},
    {"LL_CACHE_MISS_RD", 0x0037},
    {"REMOTE_ACCESS_RD", 0x0038},
    {"L1D_CACHE_LMISS_RD", 0x0039},
    {"OP_RETIRED", 0x003A},
    {"OP_SPEC", 0x003B},
    {"STALL", 0x003C},
    {"STALL_SLOT_BACKEND", 0x003D},
    {"STALL_SLOT_FRONTEND", 0x003E},
    {"STALL_SLOT", 0x003F},
    {"L1D_CACHE_RD", 0x0040},
    {"L1D_CACHE_WR", 0x0041},
    {"L1D_CACHE_REFILL_RD", 0x0042},
    {"L1D_CACHE_REFILL_WR", 0x0043},
    {"L1D_CACHE_REFILL_INNER", 0x0044},
    {"L1D_CACHE_REFILL_OUTER", 0x0045},
    {"L1D_CACHE_WB_VICTIM", 0x0046},
    {"L1D_CACHE_WB_CLEAN", 0x0047},
    {"L1D_CACHE_INVAL", 0x0048},
    {"L1D_TLB_REFILL_RD", 0x004C},
    {"L1D_TLB_REFILL_WR", 0x004D},
    {"L1D_TLB_RD", 0x004E},
    {"L1D_TLB_WR", 0x004F},
    {"L2D_CACHE_RD", 0x0050},
    {"L2D_CACHE_WR", 0x0051},
    {"L2D_CACHE_REFILL_RD", 0x0052},
    {"L2D_CACHE_REFILL_WR", 0x0053},
    {"L2D_CACHE_WB_VICTIM", 0x0056},
    {"L2D_CACHE_WB_CLEAN", 0x0057},
    {"L2D_CACHE_INVAL", 0x0058},
    {"L2D_TLB_REFILL_RD", 0x005C},
    {"L2D_TLB_REFILL_WR", 0x005D},
    {"L2D_TLB_RD", 0x005E},
    {"L2D_TLB_WR", 0x005F},
    {"BUS_ACCESS_RD", 0x0060},
    {"BUS_ACCESS_WR", 0x0061},
    {"BUS_ACCESS_SHARED", 0x0062},
    {"BUS_ACCESS_NOT_SHARED", 0x0063},
    {"BUS_ACCESS_NORMAL", 0x0064},
    {"BUS_ACCESS_PERIPH", 0x0065},
    {"MEM_ACCESS_RD", 0x0066},
    {"MEM_ACCESS_WR", 0x0067},
    {"UNALIGNED_LD_SPEC", 0x0068},
    {"UNALIGNED_ST_SPEC", 0x0069},
    {"UNALIGNED_LDST_SPEC", 0x006A},
    {"LDREX_SPEC", 0x006C},
    {"STREX_PASS_SPEC", 0x006D},
    {"STREX_FAIL_SPEC", 0x006E},
    {"STREX_SPEC", 0x006F},
    {"LD_SPEC", 0x0070},
    {"ST_SPEC", 0x0071},
    {"LDST_SPEC", 0x0072},
    {"DP_SPEC", 0x0073},
    {"ASE_SPEC", 0x0074},
    {"VFP_SPEC", 0x0075},
    {"PC_WRITE_SPEC", 0x0076},
    {"CRYPTO_SPEC", 0x0077},
    {"BR_IMMED_SPEC", 0x0078},
    {"BR_RETURN_SPEC", 0x0079},
    {"BR_INDIRECT_SPEC", 0x007A},
    {"ISB_SPEC", 0x007C},
    {"DSB_SPEC", 0x007D},
    {"DMB_SPEC", 0x007E},
    {"CSDB_SPEC", 0x007F},
    {"EXC_UNDEF", 0x0081},
    {"EXC_SVC", 0x0082},
    {"EXC_PABORT", 0x0083},
    {"EXC_DABORT", 0x0084},
    {"EXC_IRQ", 0x0086},
    {"EXC_FIQ", 0x0087},
    {"EXC_SMC", 0x0088},
    {"EXC_HVC", 0x008A},
    {"EXC_TRAP_PABORT", 0x008B},
    {"EXC_TRAP_DABORT", 0x008C},
    {"EXC_TRAP_OTHER", 0x008D},
    {"EXC_TRAP_IRQ", 0x008E},
    {"EXC_TRAP_FIQ", 0x008F},
    {"RC_LD_SPEC", 0x0090},
    {"RC_ST_SPEC", 0x0091},
    {"DCZVA_SPEC", 0x009F},
    {"L3D_CACHE_RD", 0x00A0},
    {"L3D_CACHE_WR", 0x00A1},
    {"L3D_CACHE_REFILL_RD", 0x00A2},
    {"L3D_CACHE_REFILL_WR", 0x00A3},
    {"L3D_CACHE_WB_VICTIM", 0x00A6},
    {"L3D_CACHE_WB_CLEAN", 0x00A7},
    {"L3D_CACHE_INVAL", 0x00A8},
    {"FP_MV_SPEC", 0x105},
    {"PRD_SPEC", 0x108},
    {"IEL_SPEC", 0x109},
    {"IREG_SPEC", 0x10A},
    {"FP_LD_SPEC", 0x112},
    {"FP_ST_SPEC", 0x113},
    {"BC_LD_SPEC", 0x11A},
    {"EFFECTIVE_INST_SPEC", 0X121},
    {"PRE_INDEX_SPEC", 0X123},
    {"POST_INDEX_SPEC", 0X124},
    {"UOP_SPLIT", 0X139},
    {"LD_COMP_WAIT_L2_MISS", 0x180},
    {"LD_COMP_WAIT_L2_MISS_EX", 0x181},
    {"LD_COMP_WAIT_L1_MISS", 0x182},
    {"LD_COMP_WAIT_L1_MISS_EX", 0x183},
    {"LD_COMP_WAIT", 0x184},
    {"LD_COMP_WAIT_EX", 0x185},
    {"LD_COMP_WAIT_PFP_BUSY", 0x186},
    {"LD_COMP_WAIT_PFP_BUSY_EX", 0x187},
    {"LD_COMP_WAIT_PFP_BUSY_SWPF", 0x188},
    {"EU_COMP_WAIT", 0x189},
    {"FL_COMP_WAIT", 0x18A},
    {"BR_COMP_WAIT", 0x18B},
    {"ROB_EMPTY", 0x18C},
    {"ROB_EMPTY_STQ_BUSY", 0x18D},
    {"WFE_WFI_CYCLE", 0x18E},
    {"0INST_COMMIT", 0x190},
    {"1INST_COMMIT", 0x191},
    {"2INST_COMMIT", 0x192},
    {"3INST_COMMIT", 0x193},
    {"4INST_COMMIT", 0x194},
    {"UOP_ONLY_COMMIT", 0x198},
    {"SINGLE_MOVPRFX_COMMIT", 0x199},
    {"EAGA_VAL", 0x1A0},
    {"EAGB_VAL", 0x1A1},
    {"EXA_VAL", 0x1A2},
    {"EXB_VAL", 0x1A3},
    {"FLA_VAL", 0x1A4},
    {"FLB_VAL", 0x1A5},
    {"PRX_VAL", 0x1A6},
    {"FLA_VAL_PRD_CNT", 0x1B4},
    {"FLB_VAL_PRD_CNT", 0x1B5},
    {"EA_CORE", 0x1E0},
    {"L1D_CACHE_REFILL_DM", 0x200},
    {"L1D_CACHE_REFILL_HWPRF", 0x202},
    {"L1_MISS_WAIT", 0x208},
    {"L1I_MISS_WAIT", 0x209},
    {"L1HWPF_STREAM_PF", 0x230},
    {"L1HWPF_INJ_ALLOC_PF", 0x231},
    {"L1HWPF_INJ_NOALLOC_PF", 0x232},
    {"L2HWPF_STREAM_PF", 0x233},
    {"L2HWPF_INJ_ALLOC_PF", 0x234},
    {"L2HWPF_INJ_NOALLOC_PF", 0x235},
    {"L2HWPF_OTHER", 0x236},
    {"L1_PIPE0_VAL", 0x240},
    {"L1_PIPE1_VAL", 0x241},
    {"L1_PIPE0_VAL_IU_TAG_ADRS_SCE", 0x250},
    {"L1_PIPE0_VAL_IU_TAG_ADRS_PFE", 0x251},
    {"L1_PIPE1_VAL_IU_TAG_ADRS_SCE", 0x252},
    {"L1_PIPE1_VAL_IU_TAG_ADRS_PFE", 0x253},
    {"L1_PIPE0_COMP", 0x260},
    {"L1_PIPE1_COMP", 0x261},
    {"L1I_PIPE_COMP", 0x268},
    {"L1I_PIPE_VAL", 0x269},
    {"L1_PIPE_ABORT_STLD_INTLK", 0x274},
    {"L1_PIPE0_VAL_IU_NOT_SEC0", 0x2A0},
    {"L1_PIPE1_VAL_IU_NOT_SEC0", 0x2A1},
    {"L1_PIPE_COMP_GATHER_2FLOW", 0x2B0},
    {"L1_PIPE_COMP_GATHER_1FLOW", 0x2B1},
    {"L1_PIPE_COMP_GATHER_0FLOW", 0x2B2},
    {"L1_PIPE_COMP_SCATTER_1FLOW", 0x2B3},
    {"L1_PIPE0_COMP_PRD_CNT", 0x2B8},
    {"L1_PIPE1_COMP_PRD_CNT", 0x2B9},
    {"L2D_CACHE_REFILL_DM", 0x300},
    {"L2D_CACHE_REFILL_HWPRF", 0x302},
    {"L2_MISS_WAIT", 0x308},
    {"L2_MISS_COUNT", 0x309},
    {"BUS_READ_TOTAL_TOFU", 0x314},
    {"BUS_READ_TOTAL_PCI", 0x315},
    {"BUS_READ_TOTAL_MEM", 0x316},
    {"BUS_WRITE_TOTAL_CMG0", 0x318},
    {"BUS_WRITE_TOTAL_CMG1", 0x319},
    {"BUS_WRITE_TOTAL_CMG2", 0x31A},
    {"BUS_WRITE_TOTAL_CMG3", 0x31B},
    {"BUS_WRITE_TOTAL_TOFU", 0x31C},
    {"BUS_WRITE_TOTAL_PCI", 0x31D},
    {"BUS_WRITE_TOTAL_MEM", 0x31E},
    {"L2D_SWAP_DM", 0x325},
    {"L2D_CACHE_MIBMCH_PRF", 0x326},
    {"L2_PIPE_VAL", 0x330},
    {"L2_PIPE_COMP_ALL", 0x350},
    {"L2_PIPE_COMP_PF_L2MIB_MCH", 0x370},
    {"L2D_CACHE_SWAP_LOCAL", 0x396},
    {"EA_L2", 0x3E0},
    {"EA_MEMORY", 0x3E8},
    {"SAMPLE_POP", 0x4000},
    {"SAMPLE_FEED", 0x4001},
    {"SAMPLE_FILTRATE", 0x4002},
    {"SAMPLE_COLLISION", 0x4003},
    {"CNT_CYCLES", 0x4004},
    {"STALL_BACKEND_MEM", 0x4005},
    {"L1I_CACHE_LMISS", 0x4006},
    {"L2D_CACHE_LMISS_RD", 0x4009},
    {"L2I_CACHE_LMISS", 0x400A},
    {"L3D_CACHE_LMISS_RD", 0x400B},
    {"TRB_WRAP", 0x400C},
    {"PMU_OVFS", 0x400D},
    {"TRB_TRIG", 0x400E},
    {"PMU_HOVFS", 0x400F},
    {"TRCEXTOUT0", 0x4010},
    {"TRCEXTOUT1", 0x4011},
    {"TRCEXTOUT2", 0x4012},
    {"TRCEXTOUT3", 0x4013},
    {"CTI_TRIGOUT4", 0x4018},
    {"CTI_TRIGOUT5", 0x4019},
    {"CTI_TRIGOUT6", 0x401A},
    {"CTI_TRIGOUT7", 0x401B},
    {"LDST_ALIGN_LAT", 0x4020},
    {"LD_ALIGN_LAT", 0x4021},
    {"ST_ALIGN_LAT", 0x4022},
    {"MEM_ACCESS_CHECKED", 0x4024},
    {"MEM_ACCESS_CHECKED_RD", 0x4025},
    {"MEM_ACCESS_CHECKED_WR", 0x4026},
    {"TSTART_RETIRED", 0x4030},
    {"TCOMMIT_RETIRED", 0x4031},
    {"TME_TRANSACTION_FAILED", 0x4032},
    {"TME_INST_RETIRED_COMMITTED", 0x4034},
    {"TME_CPU_CYCLES_COMMITTED", 0x4035},
    {"TME_FAILURE_CNCL", 0x4038},
    {"TME_FAILURE_NEST", 0x4039},
    {"TME_FAILURE_ERR", 0x403A},
    {"TME_FAILURE_IMP", 0x403B},
    {"TME_FAILURE_MEM", 0x403C},
    {"TME_FAILURE_SIZE", 0x403D},
    {"TME_FAILURE_TLBI", 0x403E},
    {"TME_FAILURE_WSET", 0x403F},
    {"SIMD_INST_RETIRED", 0x8000},
    {"ASE_INST_RETIRED", 0x8001},
    {"SVE_INST_RETIRED", 0x8002},
    {"ASE_SVE_INST_RETIRED", 0x8003},
    {"SIMD_INST_SPEC", 0x8004},
    {"ASE_INST_SPEC", 0x8005},
    {"SVE_INST_SPEC", 0x8006},
    {"ASE_SVE_INST_SPEC", 0x8007},
    {"UOP_SPEC", 0x8008},
    {"ASE_UOP_SPEC", 0x8009},
    {"SVE_UOP_SPEC", 0x800A},
    {"ASE_SVE_UOP_SPEC", 0x800B},
    {"SIMD_UOP_SPEC", 0x800C},
    {"SVE_MATH_SPEC", 0x800E},
    {"FP_SPEC", 0x8010},
    {"ASE_FP_SPEC", 0x8011},
    {"SVE_FP_SPEC", 0x8012},
    {"ASE_SVE_FP_SPEC", 0x8013},
    {"FP_HP_SPEC", 0x8014},
    {"ASE_FP_HP_SPEC", 0x8015},
    {"SVE_FP_HP_SPEC", 0x8016},
    {"ASE_SVE_FP_HP_SPEC", 0x8017},
    {"FP_SP_SPEC", 0x8018},
    {"ASE_FP_SP_SPEC", 0x8019},
    {"SVE_FP_SP_SPEC", 0x801A},
    {"ASE_SVE_FP_SP_SPEC", 0x801B},
    {"FP_DP_SPEC", 0x801C},
    {"ASE_FP_DP_SPEC", 0x801D},
    {"SVE_FP_DP_SPEC", 0x801E},
    {"ASE_SVE_FP_DP_SPEC", 0x801F},
    {"FP_DIV_SPEC", 0x8020},
    {"ASE_FP_DIV_SPEC", 0x8021},
    {"SVE_FP_DIV_SPEC", 0x8022},
    {"ASE_SVE_FP_DIV_SPEC", 0x8023},
    {"FP_SQRT_SPEC", 0x8024},
    {"ASE_FP_SQRT_SPEC", 0x8025},
    {"SVE_FP_SQRT_SPEC", 0x8026},
    {"ASE_SVE_FP_SQRT_SPEC", 0x8027},
    {"FP_FMA_SPEC", 0x8028},
    {"ASE_FP_FMA_SPEC", 0x8029},
    {"SVE_FP_FMA_SPEC", 0x802A},
    {"ASE_SVE_FP_FMA_SPEC", 0x802B},
    {"FP_MUL_SPEC", 0x802C},
    {"ASE_FP_MUL_SPEC", 0x802D},
    {"SVE_FP_MUL_SPEC", 0x802E},
    {"ASE_SVE_FP_MUL_SPEC", 0x802F},
    {"FP_ADDSUB_SPEC", 0x8030},
    {"ASE_FP_ADDSUB_SPEC", 0x8031},
    {"SVE_FP_ADDSUB_SPEC", 0x8032},
    {"ASE_SVE_FP_ADDSUB_SPEC", 0x8033},
    {"FP_RECPE_SPEC", 0x8034},
    {"ASE_FP_RECPE_SPEC", 0x8035},
    {"SVE_FP_RECPE_SPEC", 0x8036},
    {"ASE_SVE_FP_RECPE_SPEC", 0x8037},
    {"FP_CVT_SPEC", 0x8038},
    {"ASE_FP_CVT_SPEC", 0x8039},
    {"SVE_FP_CVT_SPEC", 0x803A},
    {"ASE_SVE_FP_CVT_SPEC", 0x803B},
    {"SVE_FP_AREDUCE_SPEC", 0x803C},
    {"ASE_FP_PREDUCE_SPEC", 0x803D},
    {"SVE_FP_VREDUCE_SPEC", 0x803E},
    {"ASE_SVE_FP_VREDUCE_SPEC", 0x803F},
    {"INT_SPEC", 0x8040},
    {"ASE_INT_SPEC", 0x8041},
    {"SVE_INT_SPEC", 0x8042},
    {"ASE_SVE_INT_SPEC", 0x8043},
    {"INT_DIV_SPEC", 0x8044},
    {"INT_DIV64_SPEC", 0x8045},
    {"SVE_INT_DIV_SPEC", 0x8046},
    {"SVE_INT_DIV64_SPEC", 0x8047},
    {"INT_MUL_SPEC", 0x8048},
    {"ASE_INT_MUL_SPEC", 0x8049},
    {"SVE_INT_MUL_SPEC", 0x804A},
    {"ASE_SVE_INT_MUL_SPEC", 0x804B},
    {"INT_MUL64_SPEC", 0x804C},
    {"SVE_INT_MUL64_SPEC", 0x804D},
    {"INT_MULH64_SPEC", 0x804E},
    {"SVE_INT_MULH64_SPEC", 0x804F},
    {"SVE_SPEC", 0x8056},
    {"ASE_SVE_SPEC", 0x8057},
    {"NONFP_SPEC", 0x8058},
    {"ASE_NONFP_SPEC", 0x8059},
    {"SVE_NONFP_SPEC", 0x805A},
    {"ASE_SVE_NONFP_SPEC", 0x805B},
    {"ASE_INT_VREDUCE_SPEC", 0x805D},
    {"SVE_INT_VREDUCE_SPEC", 0x805E},
    {"ASE_SVE_INT_VREDUCE_SPEC", 0x805F},
    {"SVE_PERM_SPEC", 0x8060},
    {"SVE_PERM_IGRANULE_SPEC", 0x8061},
    {"SVE_PERM_XGRANULE_SPEC", 0x8062},
    {"SVE_PERM_VARIABLE_SPEC", 0x8063},
    {"SVE_XPIPE_SPEC", 0x8064},
    {"SVE_XPIPE_Z2R_SPEC", 0x8065},
    {"SVE_XPIPE_R2Z_SPEC", 0x8066},
    {"SVE_PGEN_NVEC_SPEC", 0x8067},
    {"SVE_PGEN_FLG_SPEC", 0x8069},
    {"SVE_PGEN_CMP_SPEC", 0x806A},
    {"SVE_PGEN_FCM_SPEC", 0x806B},
    {"SVE_PGEN_LOGIC_SPEC", 0x806C},
    {"SVE_PPERM_SPEC", 0x806D},
    {"SVE_PSCAN_SPEC", 0x806E},
    {"SVE_PCNT_SPEC", 0x806F},
    {"SVE_PLOOP_WHILE_SPEC", 0x8070},
    {"SVE_PLOOP_TEST_SPEC", 0x8071},
    {"SVE_PLOOP_ELTS_SPEC", 0x8072},
    {"SVE_PLOOP_TERM_SPEC", 0x8073},
    {"SVE_PRED_SPEC", 0x8074},
    {"SVE_PRED_EMPTY_SPEC", 0x8075},
    {"SVE_PRED_FULL_SPEC", 0x8076},
    {"SVE_PRED_PARTIAL_SPEC", 0x8077},
    {"SVE_UNPRED_SPEC", 0x8078},
    {"SVE_PRED_NOT_FULL_SPEC", 0x8079},
    {"SVE_MOVPRFX_SPEC", 0x807C},
    {"SVE_MOVPRFX_Z_SPEC", 0x807D},
    {"SVE_MOVPRFX_M_SPEC", 0x807E},
    {"SVE_MOVPRFX_U_SPEC", 0x807F},
    {"SVE_LDST_SPEC", 0x8080},
    {"SVE_LD_SPEC", 0x8081},
    {"SVE_ST_SPEC", 0x8082},
    {"SVE_PRF_SPEC", 0x8083},
    {"ASE_SVE_LDST_SPEC", 0x8084},
    {"ASE_SVE_LD_SPEC", 0x8085},
    {"ASE_SVE_ST_SPEC", 0x8086},
    {"PRF_SPEC", 0x8087},
    {"BASE_LDST_REG_SPEC", 0x8088},
    {"BASE_LD_REG_SPEC", 0x8089},
    {"BASE_ST_REG_SPEC", 0x808A},
    {"BASE_PRF_SPEC", 0x808B},
    {"FPASE_LDST_REG_SPEC", 0x808C},
    {"FPASE_LD_REG_SPEC", 0x808D},
    {"FPASE_ST_REG_SPEC", 0x808E},
    {"SVE_LDST_REG_SPEC", 0x8090},
    {"SVE_LDR_REG_SPEC", 0x8091},
    {"SVE_STR_REG_SPEC", 0x8092},
    {"SVE_LDST_PREG_SPEC", 0x8094},
    {"SVE_LDR_PREG_SPEC", 0x8095},
    {"SVE_STR_PREG_SPEC", 0x8096},
    {"SVE_LDST_ZREG_SPEC", 0x8098},
    {"SVE_LDR_ZREG_SPEC", 0x8099},
    {"SVE_STR_ZREG_SPEC", 0x809A},
    {"SVE_LDST_CONTIG_SPEC", 0x809C},
    {"SVE_LD_CONTIG_SPEC", 0x809D},
    {"SVE_ST_CONTIG_SPEC", 0x809E},
    {"SVE_PRF_CONTIG_SPEC", 0x809F},
    {"SVE_LDSTNT_CONTIG_SPEC", 0x80A0},
    {"SVE_LDNT_CONTIG_SPEC", 0x80A1},
    {"SVE_STNT_CONTIG_SPEC", 0x80A2},
    {"ASE_SVE_LDST_MULTI_SPEC", 0x80A4},
    {"ASE_SVE_LD_MULTI_SPEC", 0x80A5},
    {"ASE_SVE_ST_MULTI_SPEC", 0x80A6},
    {"SVE_LDST_MULTI_SPEC", 0x80A8},
    {"SVE_LD_MULTI_SPEC", 0x80A9},
    {"SVE_ST_MULTI_SPEC", 0x80AA},
    {"SVE_LDST_NONCONTIG_SPEC", 0x80AC},
    {"SVE_LD_GATHER_SPEC", 0x80AD},
    {"SVE_ST_SCATTER_SPEC", 0x80AE},
    {"SVE_PRF_GATHER_SPEC", 0x80AF},
    {"SVE_LDST64_NONCONTIG_SPEC", 0x80B0},
    {"SVE_LD64_GATHER_SPEC", 0x80B1},
    {"SVE_ST64_SCATTER_SPEC", 0x80B2},
    {"SVE_PRF64_GATHER_SPEC", 0x80B3},
    {"ASE_SVE_UNALIGNED_LDST_SPEC", 0x80B4},
    {"ASE_SVE_UNALIGNED_LD_SPEC", 0x80B5},
    {"ASE_SVE_UNALIGNED_ST_SPEC", 0x80B6},
    {"ASE_SVE_UNALIGNED_CONTIG_LDST_SPEC", 0x80B8},
    {"ASE_SVE_UNALIGNED_CONTIG_LD_SPEC", 0x80B9},
    {"ASE_SVE_UNALIGNED_CONTIG_ST_SPEC", 0x80BA},
    {"SVE_LDFF_SPEC", 0x80BC},
    {"SVE_LDFF_FAULT_SPEC", 0x80BD},
    {"FP_SCALE_OPS_SPEC", 0x80C0},
    {"FP_FIXED_OPS_SPEC", 0x80C1},
    {"FP_HP_SCALE_OPS_SPEC", 0x80C2},
    {"FP_HP_FIXED_OPS_SPEC", 0x80C3},
    {"FP_SP_SCALE_OPS_SPEC", 0x80C4},
    {"FP_SP_FIXED_OPS_SPEC", 0x80C5},
    {"FP_DP_SCALE_OPS_SPEC", 0x80C6},
    {"FP_DP_FIXED_OPS_SPEC", 0x80C7},
    {"INT_SCALE_OPS_SPEC", 0x80C8},
    {"INT_FIXED_OPS_SPEC", 0x80C9},
    {"LDST_SCALE_OPS_SPEC", 0x80CA},
    {"LDST_FIXED_OPS_SPEC", 0x80CB},
    {"LD_SCALE_OPS_SPEC", 0x80CC},
    {"LD_FIXED_OPS_SPEC", 0x80CD},
    {"ST_SCALE_OPS_SPEC", 0x80CE},
    {"ST_FIXED_OPS_SPEC", 0x80CF},
    {"LDST_SCALE_BYTES_SPEC", 0x80DA},
    {"LDST_FIXED_BYTES_SPEC", 0x80DB},
    {"LD_SCALE_BYTES_SPEC", 0x80DC},
    {"LD_FIXED_BYTES_SPEC", 0x80DD},
    {"ST_SCALE_BYTES_SPEC", 0x80DE},
    {"ST_FIXED_BYTES_SPEC", 0x80DF},
    {"ASE_INT8_SPEC", 0x80E1},
    {"SVE_INT8_SPEC", 0x80E2},
    {"ASE_SVE_INT8_SPEC", 0x80E3},
    {"ASE_INT16_SPEC", 0x80E5},
    {"SVE_INT16_SPEC", 0x80E6},
    {"ASE_SVE_INT16_SPEC", 0x80E7},
    {"ASE_INT32_SPEC", 0x80E9},
    {"SVE_INT32_SPEC", 0x80EA},
    {"ASE_SVE_INT32_SPEC", 0x80EB},
    {"ASE_INT64_SPEC", 0x80ED},
    {"SVE_INT64_SPEC", 0x80EE},
    {"ASE_SVE_INT64_SPEC", 0x80EF},
    {"ASE_FP_DOT_SPEC", 0x80F1},
    {"SVE_FP_DOT_SPEC", 0x80F2},
    {"ASE_SVE_FP_DOT_SPEC", 0x80F3},
    {"ASE_FP_MMLA_SPEC", 0x80F5},
    {"SVE_FP_MMLA_SPEC", 0x80F6},
    {"ASE_SVE_FP_MMLA_SPEC", 0x80F7},
    {"ASE_INT_DOT_SPEC", 0x80F9},
    {"SVE_INT_DOT_SPEC", 0x80FA},
    {"ASE_SVE_INT_DOT_SPEC", 0x80FB},
    {"ASE_INT_MMLA_SPEC", 0x80FD},
    {"SVE_INT_MMLA_SPEC", 0x80FE},
    {"ASE_SVE_INT_MMLA_SPEC", 0x80FF},
    {"BR_SKIP_RETIRED", 0x8107},
    {"BR_IMMED_TAKEN_RETIRED", 0x8108},
    {"BR_IMMED_SKIP_RETIRED", 0x8109},
    {"BR_IND_TAKEN_RETIRED", 0x810A},
    {"BR_IND_SKIP_RETIRED", 0x810B},
    {"BR_INDNR_TAKEN_RETIRED", 0x810C},
    {"BR_INDNR_SKIP_RETIRED", 0x810D},
    {"BR_RETURN_ANY_RETIRED", 0x810E},
    {"BR_RETURN_SKIP_RETIRED", 0x810F},
    {"BR_IMMED_PRED_RETIRED", 0x8110},
    {"BR_IMMED_MIS_PRED_RETIRED", 0x8111},
    {"BR_IND_PRED_RETIRED", 0x8112},
    {"BR_IND_MIS_PRED_RETIRED", 0x8113},
    {"BR_RETURN_PRED_RETIRED", 0x8114},
    {"BR_RETURN_MIS_PRED_RETIRED", 0x8115},
    {"BR_INDNR_PRED_RETIRED", 0x8116},
    {"BR_INDNR_MIS_PRED_RETIRED", 0x8117},
    {"BR_TAKEN_PRED_RETIRED", 0x8118},
    {"BR_TAKEN_MIS_PRED_RETIRED", 0x8119},
    {"BR_SKIP_PRED_RETIRED", 0x811A},
    {"BR_SKIP_MIS_PRED_RETIRED", 0x811B},
    {"BR_PRED_RETIRED", 0x811C},
    {"BR_IND_RETIRED", 0x811D},
    {"BR_INDNR_RETIRED", 0x811E},
    {"BRB_FILTRATE", 0x811F},
    {"INST_FETCH_PERCYC", 0x8120},
    {"MEM_ACCESS_RD_PERCYC", 0x8121},
    {"SAMPLE_FEED_DS", 0x8122},
    {"SAMPLE_BUFFER_FULL", 0x8123},
    {"INST_FETCH", 0x8124},
    {"BUS_REQ_RD_PERCYC", 0x8125},
    {"BUS_REQ_WR_PERCYC", 0x8126},
    {"PMU_SNAPSHOT", 0x8127},
    {"DTLB_WALK_PERCYC", 0x8128},
    {"ITLB_WALK_PERCYC", 0x8129},
    {"SAMPLE_FEED_BR", 0x812A},
    {"SAMPLE_FEED_LD", 0x812B},
    {"SAMPLE_FEED_ST", 0x812C},
    {"SAMPLE_FEED_OP", 0x812D},
    {"SAMPLE_FEED_EVENT", 0x812E},
    {"SAMPLE_FEED_LAT", 0x812F},
    {"L1D_TLB_RW", 0x8130},
    {"L1I_TLB_RD", 0x8131},
    {"L1D_TLB_PRFM", 0x8132},
    {"L1I_TLB_PRFM", 0x8133},
    {"DTLB_HWUPD", 0x8134},
    {"ITLB_HWUPD", 0x8135},
    {"DTLB_STEP", 0x8136},
    {"ITLB_STEP", 0x8137},
    {"DTLB_WALK_LARGE", 0x8138},
    {"ITLB_WALK_LARGE", 0x8139},
    {"DTLB_WALK_SMALL", 0x813A},
    {"ITLB_WALK_SMALL", 0x813B},
    {"DTLB_WALK_RW", 0x813C},
    {"ITLB_WALK_RD", 0x813D},
    {"DTLB_WALK_PRFM", 0x813E},
    {"ITLB_WALK_PRFM", 0x813F},
    {"L1D_CACHE_RW", 0x8140},
    {"L1I_CACHE_RD", 0x8141},
    {"L1D_CACHE_PRFM", 0x8142},
    {"L1I_CACHE_PRFM", 0x8143},
    {"L1D_CACHE_MISS", 0x8144},
    {"L1I_CACHE_HWPRF", 0x8145},
    {"L1D_CACHE_REFILL_PRFM", 0x8146},
    {"L1I_CACHE_REFILL_PRFM", 0x8147},
    {"L2D_CACHE_RW", 0x8148},
    {"L2I_CACHE_RD", 0x8149},
    {"L2D_CACHE_PRFM", 0x814A},
    {"L2I_CACHE_PRFM", 0x814B},
    {"L2D_CACHE_MISS", 0x814C},
    {"L2I_CACHE_HWPRF", 0x814D},
    {"L2D_CACHE_REFILL_PRFM", 0x814E},
    {"L2I_CACHE_REFILL_PRFM", 0x814F},
    {"L3D_CACHE_RW", 0x8150},
    {"L3D_CACHE_PRFM", 0x8151},
    {"L3D_CACHE_MISS", 0x8152},
    {"L3D_CACHE_REFILL_PRFM", 0x8153},
    {"L1D_CACHE_HWPRF", 0x8154},
    {"L2D_CACHE_HWPRF", 0x8155},
    {"L3D_CACHE_HWPRF", 0x8156},
    {"LL_CACHE_HWPRF", 0x8157},
    {"STALL_FRONTEND_MEMBOUND", 0x8158},
    {"STALL_FRONTEND_L1I", 0x8159},
    {"STALL_FRONTEND_L2I", 0x815A},
    {"STALL_FRONTEND_MEM", 0x815B},
    {"STALL_FRONTEND_TLB", 0x815C},
    {"STALL_FRONTEND_CPUBOUND", 0x8160},
    {"STALL_FRONTEND_FLOW", 0x8161},
    {"STALL_FRONTEND_FLUSH", 0x8162},
    {"STALL_FRONTEND_RENAME", 0x8163},
    {"STALL_BACKEND_MEMBOUND", 0x8164},
    {"STALL_BACKEND_L1D", 0x8165},
    {"STALL_BACKEND_L2D", 0x8166},
    {"STALL_BACKEND_TLB", 0x8167},
    {"STALL_BACKEND_ST", 0x8168},
    {"STALL_BACKEND_CPUBOUND", 0x816A},
    {"STALL_BACKEND_BUSY", 0x816B},
    {"STALL_BACKEND_ILOCK", 0x816C},
    {"STALL_BACKEND_RENAME", 0x816D},
    {"STALL_BACKEND_ATOMIC", 0x816E},
    {"STALL_BACKEND_MEMCPYSET", 0x816F},
    {"CAS_NEAR_FAIL", 0x8170},
    {"CAS_NEAR_PASS", 0x8171},
    {"CAS_NEAR_SPEC", 0x8172},
    {"CAS_FAR_SPEC", 0x8173},
    {"CAS_SPEC", 0x8174},
    {"LSE_LD_SPEC", 0x8175},
    {"LSE_ST_SPEC", 0x8176},
    {"LSE_LDST_SPEC", 0x8177},
    {"REMOTE_ACCESS_WR", 0x8178},
    {"BRNL_INDNR_TAKEN_RETIRED", 0x8179},
    {"BL_TAKEN_RETIRED", 0x817A},
    {"BRNL_TAKEN_RETIRED", 0x817B},
    {"BL_IND_TAKEN_RETIRED", 0x817C},
    {"BRNL_IND_TAKEN_RETIRED", 0x817D},
    {"BL_IMMED_TAKEN_RETIRED", 0x817E},
    {"BRNL_IMMED_TAKEN_RETIRED", 0x817F},
    {"BR_UNCOND_RETIRED", 0x8180},
    {"BR_COND_RETIRED", 0x8181},
    {"BR_COND_TAKEN_RETIRED", 0x8182},
    {"BR_HINT_COND_RETIRED", 0x8183},
    {"BR_HINT_COND_PRED_RETIRED", 0x8184},
    {"BR_HINT_COND_MIS_PRED_RETIRED", 0x8185},
    {"UOP_RETIRED", 0x8186},
    {"DTLB_WALK_BLOCK", 0x8188},
    {"ITLB_WALK_BLOCK", 0x8189},
    {"DTLB_WALK_PAGE", 0x818A},
    {"ITLB_WALK_PAGE", 0x818B},
    {"BUS_REQ_RD", 0x818D},
    {"BUS_REQ_WR", 0x818E},
    {"BUS_REQ", 0x818F},
    {"ISNP_HIT_RD", 0x8190},
    {"ISNP_HIT_NEAR_RD", 0x8191},
    {"ISNP_HIT_FAR_RD", 0x8192},
    {"ISNP_HIT_REMOTE_RD", 0x8193},
    {"DSNP_HIT_RD", 0x8194},
    {"DSNP_HIT_NEAR_RD", 0x8195},
    {"DSNP_HIT_FAR_RD", 0x8196},
    {"DSNP_HIT_REMOTE_RD", 0x8197},
    {"DSNP_HIT_WR", 0x8198},
    {"DSNP_HIT_NEAR_WR", 0x8199},
    {"DSNP_HIT_FAR_WR", 0x819A},
    {"DSNP_HIT_REMOTE_WR", 0x819B},
    {"DSNP_HIT_RW", 0x819C},
    {"DSNP_HIT_NEAR_RW", 0x819D},
    {"DSNP_HIT_FAR_RW", 0x819E},
    {"DSNP_HIT_REMOTE_RW", 0x819F},
    {"DSNP_HIT_PRFM", 0x81A0},
    {"DSNP_HIT_NEAR_PRFM", 0x81A1},
    {"DSNP_HIT_FAR_PRFM", 0x81A2},
    {"DSNP_HIT_REMOTE_PRFM", 0x81A3},
    {"DSNP_HIT_HWPRF", 0x81A4},
    {"DSNP_HIT_NEAR_HWPRF", 0x81A5},
    {"DSNP_HIT_FAR_HWPRF", 0x81A6},
    {"DSNP_HIT_REMOTE_HWPRF", 0x81A7},
    {"ISNP_HIT_PRFM", 0x81A8},
    {"ISNP_HIT_NEAR_PRFM", 0x81A9},
    {"ISNP_HIT_FAR_PRFM", 0x81AA},
    {"ISNP_HIT_REMOTE_PRFM", 0x81AB},
    {"ISNP_HIT_HWPRF", 0x81AC},
    {"ISNP_HIT_NEAR_HWPRF", 0x81AD},
    {"ISNP_HIT_FAR_HWPRF", 0x81AE},
    {"ISNP_HIT_REMOTE_HWPRF", 0x81AF},
    {"ISNP_HIT", 0x81B0},
    {"ISNP_HIT_NEAR", 0x81B1},
    {"ISNP_HIT_FAR", 0x81B2},
    {"ISNP_HIT_REMOTE", 0x81B3},
    {"DSNP_HIT", 0x81B4},
    {"DSNP_HIT_NEAR", 0x81B5},
    {"DSNP_HIT_FAR", 0x81B6},
    {"DSNP_HIT_REMOTE", 0x81B7},
    {"L1I_CACHE_REFILL_HWPRF", 0x81B8},
    {"L2I_CACHE_REFILL_HWPRF", 0x81B9},
    {"L1D_CACHE_REFILL_HWPRF", 0x81BC},
    {"L2D_CACHE_REFILL_HWPRF", 0x81BD},
    {"L3D_CACHE_REFILL_HWPRF", 0x81BE},
    {"LL_CACHE_REFILL_HWPRF", 0x81BF},
    {"L1I_CACHE_HIT_RD", 0x81C0},
    {"L2I_CACHE_HIT_RD", 0x81C1},
    {"L1D_CACHE_HIT_RD", 0x81C4},
    {"L2D_CACHE_HIT_RD", 0x81C5},
    {"L3D_CACHE_HIT_RD", 0x81C6},
    {"LL_CACHE_HIT_RD", 0x81C7},
    {"L1D_CACHE_HIT_WR", 0x81C8},
    {"L2D_CACHE_HIT_WR", 0x81C9},
    {"L3D_CACHE_HIT_WR", 0x81CA},
    {"LL_CACHE_HIT_WR", 0x81CB},
    {"L1D_CACHE_HIT_RW", 0x81CC},
    {"L2D_CACHE_HIT_RW", 0x81CD},
    {"L3D_CACHE_HIT_RW", 0x81CE},
    {"LL_CACHE_HIT_RW", 0x81CF},
    {"L1I_CACHE_HIT_RD_FPRFM", 0x81D0},
    {"L2I_CACHE_HIT_RD_FPRFM", 0x81D1},
    {"L1D_CACHE_HIT_RD_FPRFM", 0x81D4},
    {"L2D_CACHE_HIT_RD_FPRFM", 0x81D5},
    {"L3D_CACHE_HIT_RD_FPRFM", 0x81D6},
    {"LL_CACHE_HIT_RD_FPRFM", 0x81D7},
    {"L1D_CACHE_HIT_WR_FPRFM", 0x81D8},
    {"L2D_CACHE_HIT_WR_FPRFM", 0x81D9},
    {"L3D_CACHE_HIT_WR_FPRFM", 0x81DA},
    {"LL_CACHE_HIT_WR_FPRFM", 0x81DB},
    {"L1D_CACHE_HIT_RW_FPRFM", 0x81DC},
    {"L2D_CACHE_HIT_RW_FPRFM", 0x81DD},
    {"L3D_CACHE_HIT_RW_FPRFM", 0x81DE},
    {"LL_CACHE_HIT_RW_FPRFM", 0x81DF},
    {"L1I_CACHE_HIT_RD_FHWPRF", 0x81E0},
    {"L2I_CACHE_HIT_RD_FHWPRF", 0x81E1},
    {"L1D_CACHE_HIT_RD_FHWPRF", 0x81E4},
    {"L2D_CACHE_HIT_RD_FHWPRF", 0x81E5},
    {"L3D_CACHE_HIT_RD_FHWPRF", 0x81E6},
    {"LL_CACHE_HIT_RD_FHWPRF", 0x81E7},
    {"L1D_CACHE_HIT_WR_FHWPRF", 0x81E8},
    {"L2D_CACHE_HIT_WR_FHWPRF", 0x81E9},
    {"L3D_CACHE_HIT_WR_FHWPRF", 0x81EA},
    {"LL_CACHE_HIT_WR_FHWPRF", 0x81EB},
    {"L1D_CACHE_HIT_RW_FHWPRF", 0x81EC},
    {"L2D_CACHE_HIT_RW_FHWPRF", 0x81ED},
    {"L3D_CACHE_HIT_RW_FHWPRF", 0x81EE},
    {"LL_CACHE_HIT_RW_FHWPRF", 0x81EF},
    {"L1I_CACHE_HIT_RD_FPRF", 0x81F0},
    {"L2I_CACHE_HIT_RD_FPRF", 0x81F1},
    {"L1D_CACHE_HIT_RD_FPRF", 0x81F4},
    {"L2D_CACHE_HIT_RD_FPRF", 0x81F5},
    {"L3D_CACHE_HIT_RD_FPRF", 0x81F6},
    {"LL_CACHE_HIT_RD_FPRF", 0x81F7},
    {"L1D_CACHE_HIT_WR_FPRF", 0x81F8},
    {"L2D_CACHE_HIT_WR_FPRF", 0x81F9},
    {"L3D_CACHE_HIT_WR_FPRF", 0x81FA},
    {"LL_CACHE_HIT_WR_FPRF", 0x81FB},
    {"L1D_CACHE_HIT_RW_FPRF", 0x81FC},
    {"L2D_CACHE_HIT_RW_FPRF", 0x81FD},
    {"L3D_CACHE_HIT_RW_FPRF", 0x81FE},
    {"LL_CACHE_HIT_RW_FPRF", 0x81FF},
    {"L1I_CACHE_HIT", 0x8200},
    {"L2I_CACHE_HIT", 0x8201},
    {"L1D_CACHE_HIT", 0x8204},
    {"L2D_CACHE_HIT", 0x8205},
    {"L3D_CACHE_HIT", 0x8206},
    {"LL_CACHE_HIT", 0x8207},
    {"L1I_CACHE_HIT_PRFM", 0x8208},
    {"L2I_CACHE_HIT_PRFM", 0x8209},
    {"L1D_CACHE_HIT_PRFM", 0x820C},
    {"L2D_CACHE_HIT_PRFM", 0x820D},
    {"L3D_CACHE_HIT_PRFM", 0x820E},
    {"LL_CACHE_HIT_PRFM", 0x820F},
    {"L1D_CACHE_HITM_RD", 0x8214},
    {"L2D_CACHE_HITM_RD", 0x8215},
    {"L3D_CACHE_HITM_RD", 0x8216},
    {"LL_CACHE_HITM_RD", 0x8217},
    {"L1D_CACHE_HITM_WR", 0x8218},
    {"L2D_CACHE_HITM_WR", 0x8219},
    {"L3D_CACHE_HITM_WR", 0x821A},
    {"LL_CACHE_HITM_WR", 0x821B},
    {"L1D_CACHE_HITM_RW", 0x821C},
    {"L2D_CACHE_HITM_RW", 0x821D},
    {"L3D_CACHE_HITM_RW", 0x821E},
    {"LL_CACHE_HITM_RW", 0x821F},
    {"DSNP_HITM_RD", 0x8224},
    {"DSNP_HITM_NEAR_RD", 0x8225},
    {"DSNP_HITM_FAR_RD", 0x8226},
    {"DSNP_HITM_REMOTE_RD", 0x8227},
    {"DSNP_HITM_WR", 0x8228},
    {"DSNP_HITM_NEAR_WR", 0x8229},
    {"DSNP_HITM_FAR_WR", 0x822A},
    {"DSNP_HITM_REMOTE_WR", 0x822B},
    {"DSNP_HITM_RW", 0x822C},
    {"DSNP_HITM_NEAR_RW", 0x822D},
    {"DSNP_HITM_FAR_RW", 0x822E},
    {"DSNP_HITM_REMOTE_RW", 0x822F},
    {"LOCAL_MEM", 0x8230},
    {"LOCAL_MEM_RD", 0x8231},
    {"LOCAL_MEM_WR", 0x8232},
    {"LOCAL_MEM_RW", 0x8233},
    {"LOCAL_MEM_PRFM", 0x8234},
    {"LOCAL_MEM_LD_RETIRED", 0x8235},
    {"LOCAL_MEM_ST_RETIRED", 0x8236},
    {"LOCAL_MEM_LDST_RETIRED", 0x8237},
    {"REMOTE_MEM", 0x8238},
    {"REMOTE_MEM_RD", 0x8239},
    {"REMOTE_MEM_WR", 0x823A},
    {"REMOTE_MEM_RW", 0x823B},
    {"REMOTE_MEM_PRFM", 0x823C},
    {"REMOTE_MEM_LD_RETIRED", 0x823D},
    {"REMOTE_MEM_ST_RETIRED", 0x823E},
    {"REMOTE_MEM_LDST_RETIRED", 0x823F},
    {"L1I_LFB_HIT_RD", 0x8240},
    {"L2I_LFB_HIT_RD", 0x8241},
    {"L1D_LFB_HIT_RD", 0x8244},
    {"L2D_LFB_HIT_RD", 0x8245},
    {"L3D_LFB_HIT_RD", 0x8246},
    {"LL_LFB_HIT_RD", 0x8247},
    {"L1D_LFB_HIT_WR", 0x8248},
    {"L2D_LFB_HIT_WR", 0x8249},
    {"L3D_LFB_HIT_WR", 0x824A},
    {"LL_LFB_HIT_WR", 0x824B},
    {"L1D_LFB_HIT_RW", 0x824C},
    {"L2D_LFB_HIT_RW", 0x824D},
    {"L3D_LFB_HIT_RW", 0x824E},
    {"LL_LFB_HIT_RW", 0x824F},
    {"L1I_LFB_HIT_RD_FPRFM", 0x8250},
    {"L2I_LFB_HIT_RD_FPRFM", 0x8251},
    {"L1D_LFB_HIT_RD_FPRFM", 0x8254},
    {"L2D_LFB_HIT_RD_FPRFM", 0x8255},
    {"L3D_LFB_HIT_RD_FPRFM", 0x8256},
    {"LL_LFB_HIT_RD_FPRFM", 0x8257},
    {"L1D_LFB_HIT_WR_FPRFM", 0x8258},
    {"L2D_LFB_HIT_WR_FPRFM", 0x8259},
    {"L3D_LFB_HIT_WR_FPRFM", 0x825A},
    {"LL_LFB_HIT_WR_FPRFM", 0x825B},
    {"L1D_LFB_HIT_RW_FPRFM", 0x825C},
    {"L2D_LFB_HIT_RW_FPRFM", 0x825D},
    {"L3D_LFB_HIT_RW_FPRFM", 0x825E},
    {"LL_LFB_HIT_RW_FPRFM", 0x825F},
    {"L1I_LFB_HIT_RD_FHWPRF", 0x8260},
    {"L2I_LFB_HIT_RD_FHWPRF", 0x8261},
    {"L1D_LFB_HIT_RD_FHWPRF", 0x8264},
    {"L2D_LFB_HIT_RD_FHWPRF", 0x8265},
    {"L3D_LFB_HIT_RD_FHWPRF", 0x8266},
    {"LL_LFB_HIT_RD_FHWPRF", 0x8267},
    {"L1D_LFB_HIT_WR_FHWPRF", 0x8268},
    {"L2D_LFB_HIT_WR_FHWPRF", 0x8269},
    {"L3D_LFB_HIT_WR_FHWPRF", 0x826A},
    {"LL_LFB_HIT_WR_FHWPRF", 0x826B},
    {"L1D_LFB_HIT_RW_FHWPRF", 0x826C},
    {"L2D_LFB_HIT_RW_FHWPRF", 0x826D},
    {"L3D_LFB_HIT_RW_FHWPRF", 0x826E},
    {"LL_LFB_HIT_RW_FHWPRF", 0x826F},
    {"L1I_LFB_HIT_RD_FPRF", 0x8270},
    {"L2I_LFB_HIT_RD_FPRF", 0x8271},
    {"L1D_LFB_HIT_RD_FPRF", 0x8274},
    {"L2D_LFB_HIT_RD_FPRF", 0x8275},
    {"L3D_LFB_HIT_RD_FPRF", 0x8276},
    {"LL_LFB_HIT_RD_FPRF", 0x8277},
    {"L1D_LFB_HIT_WR_FPRF", 0x8278},
    {"L2D_LFB_HIT_WR_FPRF", 0x8279},
    {"L3D_LFB_HIT_WR_FPRF", 0x827A},
    {"LL_LFB_HIT_WR_FPRF", 0x827B},
    {"L1D_LFB_HIT_RW_FPRF", 0x827C},
    {"L2D_LFB_HIT_RW_FPRF", 0x827D},
    {"L3D_LFB_HIT_RW_FPRF", 0x827E},
    {"LL_LFB_HIT_RW_FPRF", 0x827F},
    {"L1I_CACHE_PRF", 0x8280},
    {"L2I_CACHE_PRF", 0x8281},
    {"L1D_CACHE_PRF", 0x8284},
    {"L2D_CACHE_PRF", 0x8285},
    {"L3D_CACHE_PRF", 0x8286},
    {"LL_CACHE_PRF", 0x8287},
    {"L1I_CACHE_REFILL_PRF", 0x8288},
    {"L2I_CACHE_REFILL_PRF", 0x8289},
    {"L1D_CACHE_REFILL_PRF", 0x828C},
    {"L2D_CACHE_REFILL_PRF", 0x828D},
    {"L3D_CACHE_REFILL_PRF", 0x828E},
    {"LL_CACHE_REFILL_PRF", 0x828F},
    {"ISNP_HIT_PRF", 0x8290},
    {"ISNP_HIT_NEAR_PRF", 0x8291},
    {"ISNP_HIT_FAR_PRF", 0x8292},
    {"ISNP_HIT_REMOTE_PRF", 0x8293},
    {"DSNP_HIT_PRF", 0x8294},
    {"DSNP_HIT_NEAR_PRF", 0x8295},
    {"DSNP_HIT_FAR_PRF", 0x8296},
    {"DSNP_HIT_REMOTE_PRF", 0x8297},
    {"LL_CACHE_RW", 0x8298},
    {"LL_CACHE_PRFM", 0x8299},
    {"LL_CACHE_REFILL", 0x829A},
    {"LL_CACHE_REFILL_PRFM", 0x829B},
    {"LL_CACHE_WB", 0x829C},
    {"LL_CACHE_WR", 0x829D},
    {"LL_CACHE_REFILL_WR", 0x829F},
    {"MEM_ACCESS_RW", 0x82A0},
    {"INST_FETCH_RD", 0x82A1},
    {"MEM_ACCESS_PRFM", 0x82A2},
    {"INST_FETCH_PRFM", 0x82A3},
    {"ASE_SVE_RETIRED", 0x82A4},
    {"LD_ANY_RETIRED", 0x82A8},
    {"ST_ANY_RETIRED", 0x82A9},
    {"LDST_ANY_RETIRED", 0x82AA},
    {"DP_RETIRED", 0x82AB},
    {"ASE_RETIRED", 0x82AC},
    {"VFP_RETIRED", 0x82AD},
    {"SVE_RETIRED", 0x82AE},
    {"CRYPTO_RETIRED", 0x82AF},
    {"L1I_CACHE_MISS_RETIRED", 0x82B0},
    {"L2I_CACHE_MISS_RETIRED", 0x82B1},
    {"L1D_CACHE_MISS_LD_RETIRED", 0x82B4},
    {"L2D_CACHE_MISS_LD_RETIRED", 0x82B5},
    {"L3D_CACHE_MISS_LD_RETIRED", 0x82B6},
    {"LL_CACHE_MISS_LD_RETIRED", 0x82B7},
    {"L1D_CACHE_MISS_ST_RETIRED", 0x82B8},
    {"L2D_CACHE_MISS_ST_RETIRED", 0x82B9},
    {"L3D_CACHE_MISS_ST_RETIRED", 0x82BA},
    {"LL_CACHE_MISS_ST_RETIRED", 0x82BB},
    {"L1D_CACHE_MISS_LDST_RETIRED", 0x82BC},
    {"L2D_CACHE_MISS_LDST_RETIRED", 0x82BD},
    {"L3D_CACHE_MISS_LDST_RETIRED", 0x82BE},
    {"LL_CACHE_MISS_LDST_RETIRED", 0x82BF},
    {"L1D_CACHE_HITM_LD_RETIRED", 0x82C4},
    {"L2D_CACHE_HITM_LD_RETIRED", 0x82C5},
    {"L3D_CACHE_HITM_LD_RETIRED", 0x82C6},
    {"LL_CACHE_HITM_LD_RETIRED", 0x82C7},
    {"L1D_CACHE_HITM_ST_RETIRED", 0x82C8},
    {"L2D_CACHE_HITM_ST_RETIRED", 0x82C9},
    {"L3D_CACHE_HITM_ST_RETIRED", 0x82CA},
    {"LL_CACHE_HITM_ST_RETIRED", 0x82CB},
    {"L1D_CACHE_HITM_LDST_RETIRED", 0x82CC},
    {"L2D_CACHE_HITM_LDST_RETIRED", 0x82CD},
    {"L3D_CACHE_HITM_LDST_RETIRED", 0x82CE},
    {"LL_CACHE_HITM_LDST_RETIRED", 0x82CF},
    {"L1I_LFB_HIT_RETIRED", 0x82D0},
    {"L2I_LFB_HIT_RETIRED", 0x82D1},
    {"L1D_LFB_HIT_LD_RETIRED", 0x82D4},
    {"L2D_LFB_HIT_LD_RETIRED", 0x82D5},
    {"L3D_LFB_HIT_LD_RETIRED", 0x82D6},
    {"LL_LFB_HIT_LD_RETIRED", 0x82D7},
    {"L1D_LFB_HIT_ST_RETIRED", 0x82D8},
    {"L2D_LFB_HIT_ST_RETIRED", 0x82D9},
    {"L3D_LFB_HIT_ST_RETIRED", 0x82DA},
    {"LL_LFB_HIT_ST_RETIRED", 0x82DB},
    {"L1D_LFB_HIT_LDST_RETIRED", 0x82DC},
    {"L2D_LFB_HIT_LDST_RETIRED", 0x82DD},
    {"L3D_LFB_HIT_LDST_RETIRED", 0x82DE},
    {"LL_LFB_HIT_LDST_RETIRED", 0x82DF},
    {"L1I_CACHE_HIT_RETIRED", 0x82E0},
    {"L2I_CACHE_HIT_RETIRED", 0x82E1},
    {"L1D_CACHE_HIT_LD_RETIRED", 0x82E4},
    {"L2D_CACHE_HIT_LD_RETIRED", 0x82E5},
    {"L3D_CACHE_HIT_LD_RETIRED", 0x82E6},
    {"LL_CACHE_HIT_LD_RETIRED", 0x82E7},
    {"L1D_CACHE_HIT_ST_RETIRED", 0x82E8},
    {"L2D_CACHE_HIT_ST_RETIRED", 0x82E9},
    {"L3D_CACHE_HIT_ST_RETIRED", 0x82EA},
    {"LL_CACHE_HIT_ST_RETIRED", 0x82EB},
    {"L1D_CACHE_HIT_LDST_RETIRED", 0x82EC},
    {"L2D_CACHE_HIT_LDST_RETIRED", 0x82ED},
    {"L3D_CACHE_HIT_LDST_RETIRED", 0x82EE},
    {"LL_CACHE_HIT_LDST_RETIRED", 0x82EF},
    {"ITLB_HIT_RETIRED", 0x82F0},
    {"DTLB_HIT_LD_RETIRED", 0x82F1},
    {"DTLB_HIT_ST_RETIRED", 0x82F2},
    {"DTLB_HIT_LDST_RETIRED", 0x82F3},
    {"ITLB_WALK_RETIRED", 0x82F4},
    {"DTLB_WALK_LD_RETIRED", 0x82F5},
    {"DTLB_WALK_ST_RETIRED", 0x82F6},
    {"DTLB_WALK_LDST_RETIRED", 0x82F7},
    {"DTLB_WALK_PRF", 0x82F8},
    {"ITLB_WALK_PRF", 0x82F9},
    {"DTLB_WALK_HWPRF", 0x82FA},
    {"ITLB_WALK_HWPRF", 0x82FB},
    {"L1D_TLB_PRF", 0x82FC},
    {"L1I_TLB_PRF", 0x82FD},
    {"L1D_TLB_HWPRF", 0x82FE},
    {"L1I_TLB_HWPRF", 0x82FF},
    {"DSNP_HITM_LD_RETIRED", 0x8304},
    {"DSNP_HITM_NEAR_LD_RETIRED", 0x8305},
    {"DSNP_HITM_FAR_LD_RETIRED", 0x8306},
    {"DSNP_HITM_REMOTE_LD_RETIRED", 0x8307},
    {"DSNP_HITM_ST_RETIRED", 0x8308},
    {"DSNP_HITM_NEAR_ST_RETIRED", 0x8309},
    {"DSNP_HITM_FAR_ST_RETIRED", 0x830A},
    {"DSNP_HITM_REMOTE_ST_RETIRED", 0x830B},
    {"DSNP_HITM_LDST_RETIRED", 0x830C},
    {"DSNP_HITM_NEAR_LDST_RETIRED", 0x830D},
    {"DSNP_HITM_FAR_LDST_RETIRED", 0x830E},
    {"DSNP_HITM_REMOTE_LDST_RETIRED", 0x830F},
    {"ISNP_HIT_RETIRED", 0x8310},
    {"ISNP_HIT_NEAR_RETIRED", 0x8311},
    {"ISNP_HIT_FAR_RETIRED", 0x8312},
    {"ISNP_HIT_REMOTE_RETIRED", 0x8313},
    {"DSNP_HIT_LD_RETIRED", 0x8314},
    {"DSNP_HIT_NEAR_LD_RETIRED", 0x8315},
    {"DSNP_HIT_FAR_LD_RETIRED", 0x8316},
    {"DSNP_HIT_REMOTE_LD_RETIRED", 0x8317},
    {"DSNP_HIT_ST_RETIRED", 0x8318},
    {"DSNP_HIT_NEAR_ST_RETIRED", 0x8319},
    {"DSNP_HIT_FAR_ST_RETIRED", 0x831A},
    {"DSNP_HIT_REMOTE_ST_RETIRED", 0x831B},
    {"DSNP_HIT_LDST_RETIRED", 0x831C},
    {"DSNP_HIT_NEAR_LDST_RETIRED", 0x831D},
    {"DSNP_HIT_FAR_LDST_RETIRED", 0x831E},
    {"DSNP_HIT_REMOTE_LDST_RETIRED", 0x831F},
    {"L1D_CACHE_REFILL_PERCYC", 0x8320},
    {"L2D_CACHE_REFILL_PERCYC", 0x8321},
    {"L3D_CACHE_REFILL_PERCYC", 0x8322},
    {"L1I_CACHE_REFILL_PERCYC", 0x8324},
    {"L2I_CACHE_REFILL_PERCYC", 0x8325},
    {"L1GCS_CACHE", 0x8330},
    {"L1GCS_CACHE_RW", 0x8331},
    {"L1GCS_CACHE_HWPRF", 0x8332},
    {"L1GCS_CACHE_MISS", 0x8334},
    {"L1GCS_CACHE_MISS_RD", 0x8335},
    {"L1GCS_CACHE_HIT_RW", 0x8336},
    {"L1GCS_CACHE_HIT_RW_FHWPRF", 0x8337},
    {"L1GCS_CACHE_REFILL", 0x8338},
    {"L1GCS_CACHE_REFILL_HWPRF", 0x8339},
    {"L1GCS_CACHE_REFILL_PERCYC", 0x833A},
    {"L1GCS_LFB_HIT_RW", 0x833C},
    {"L1GCS_LFB_HIT_RW_FHWPRF", 0x833D},
    {"L1GCS_CACHE_INVAL", 0x833E},
    {"L1GCS_TLB", 0x8340},
    {"L1GCS_TLB_RW", 0x8341},
    {"L1GCS_TLB_HWPRF", 0x8342},
    {"GCSTLB_WALK", 0x8344},
    {"GCSTLB_WALK_RW", 0x8345},
    {"GCSTLB_WALK_PERCYC", 0x8346},
    {"GCSTLB_WALK_STEP", 0x8347},
    {"STALL_BACKEND_L1GCS", 0x8350},
    {"STALL_BACKEND_GCSTLB", 0x8351},
    {"IMP_STALL_FRONTEND_MEM", 0x0E1},
    {"IMP_STALL_FRONTEND_TLB", 0x0E2},
    {"IMP_L2_CACHE_IF_REFILL", 0x108},
    {"IMP_L2_CACHE_PF_LATE_REFILL", 0x10B},
    {"IMP_CT_FLUSH", 0x120},
    {"IMP_CT_FLUSH_MEM", 0x121},
    {"IMP_CT_FLUSH_BAD_BRANCH", 0x122},
    {"IMP_CT_FLUSH_PREDECODE_ERR", 0x123},
    {"IMP_CT_FLUSH_ISB", 0x124},
    {"IMP_CT_FLUSH_OTHER", 0x125},
    {"IMP_LS_RAR", 0x127},
    {"IMP_LS_RAW", 0x128},
    {"IMP_STALL_FRONTED_FLUSH", 0x15B},
    {"IMP_STALL_BACKEND_RENAME_FRF", 0x158},
    {"IMP_STALL_BACKEND_RENAME_GRF", 0x159},
    {"IMP_STALL_BACKEND_RENAME_VRF", 0x15A},
    {"IMP_STALL_BACKEND_IQ_SX", 0x15C},
    {"IMP_STALL_BACKEND_IQ_MX", 0x15D},
    {"IMP_STALL_BACKEND_IQ_LS", 0x15E},
    {"IMP_STALL_BACKEND_IQ_VX", 0x15F},
    {"IMP_STALL_BACKEND_MCQ", 0x160},
    {"IMP_NEAR_CAS", 0x17B},
    {"IMP_NEAR_CAS_PAS", 0x17C},
};

static const int event_list_size = sizeof(event_list) / sizeof(EventMapping);

#ifdef __cplusplus
}
#endif
