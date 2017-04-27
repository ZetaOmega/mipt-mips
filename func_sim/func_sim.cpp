#include <iostream>

#include "func_sim.h"

std::string MIPS::step()
{
    // fetch
    uint32 instr_bytes = fetch();

    // decode
    FuncInstr instr(instr_bytes, PC);

    // read sources
    read_src(instr);

    // execute
    instr.execute();

    // load/store
    load_store(instr);

    // writeback
    wb(instr);

    // PC update
    PC = instr.get_new_PC();

    // dump
    return instr.Dump();
}

void MIPS::init( const std::string& tr)
{
    assert( mem == nullptr);
    mem = new FuncMemory(tr.c_str());
    PC = mem->startPC();
}

void MIPS::run(const std::string& tr, uint32 instrs_to_run)
{
    init( tr);
    for (uint32 i = 0; i < instrs_to_run; ++i)
        std::cout << step() << std::endl;
}

