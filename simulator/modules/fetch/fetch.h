/*
 * fetch.h - simulator of fetch unit
 * Copyright 2015-2018 MIPT-MIPS
 */

#ifndef FETCH_H
#define FETCH_H

#include "bpu/bpu.h"

#include <func_sim/instr_memory.h>
#include <infra/cache/cache_tag_array.h>
#include <modules/core/perf_instr.h>
#include <modules/ports_instance.h>
 
template <typename FuncInstr>
class Fetch : public Log
{
    using Instr = PerfInstr<FuncInstr>;

public:
    explicit Fetch( bool log);
    void clock( Cycle cycle);
    void set_memory( std::unique_ptr<InstrMemoryIface<FuncInstr>> mem)
    {
        memory = std::move( mem);
    }

private:
    std::unique_ptr<InstrMemoryIface<FuncInstr>> memory = nullptr;
    std::unique_ptr<BaseBP> bp = nullptr;
    std::unique_ptr<CacheTagArray> tags = nullptr;
    
    /* Input signals */
    std::unique_ptr<ReadPort<bool>> rp_stall = nullptr;
    std::unique_ptr<ReadPort<bool>> rp_hit_or_miss = nullptr;

    /* Input signals - BP */
    std::unique_ptr<ReadPort<BPInterface>> rp_bp_update = nullptr;
    std::unique_ptr<ReadPort<BPInterface>> rp_bp_update_from_decode = nullptr;
    
    /* Input signals - PC values */
    std::unique_ptr<ReadPort<Target>> rp_flush_target = nullptr;
    std::unique_ptr<ReadPort<Target>> rp_external_target = nullptr;
    std::unique_ptr<ReadPort<Target>> rp_hold_pc = nullptr;
    std::unique_ptr<ReadPort<Target>> rp_target = nullptr;
    std::unique_ptr<ReadPort<Target>> rp_long_latency_pc_holder = nullptr;

    /* Outputs */
    std::unique_ptr<WritePort<Instr>> wp_datapath = nullptr;
    std::unique_ptr<WritePort<Target>> wp_hold_pc = nullptr;
    std::unique_ptr<WritePort<Target>> wp_target = nullptr;
    std::unique_ptr<WritePort<Target>> wp_long_latency_pc_holder = nullptr;
    std::unique_ptr<WritePort<bool>> wp_hit_or_miss = nullptr;

    /* port needed for habdling misprediction at decode stage */
    std::unique_ptr<ReadPort<Target>> rp_flush_target_from_decode = nullptr;

    Target get_target( Cycle cycle);
    Target get_cached_target( Cycle cycle);
    void clock_bp( Cycle cycle);
    void clock_instr_cache( Cycle cycle);
    void save_flush( Cycle cycle);
};

#endif
