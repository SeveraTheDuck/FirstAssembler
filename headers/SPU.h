#ifndef SPU_H
#define SPU_H

#include "../FileOpenLib/filestruct.h"
#include "../stack/headers/stack.h"
#include "translator.h"

const size_t reg_number = 4;

struct SPU_struct
{
    Stack stk;
    int register_vars[reg_number];
};

void SPU_СTOR (SPU_struct* spu);

void SPU_DTOR (SPU_struct* spu);

void SPU_process (const char* const spu_file_name);

void Processor (file_input* const asm_code,
                SPU_struct* const spu);

void SPU_push (SPU_struct* const spu,
               file_input* const asm_code,
               const size_t n_line,
               int n_operation);

void SPU_add  (SPU_struct* const spu);

void SPU_sub  (SPU_struct* const spu);

void SPU_mul  (SPU_struct* const spu);

void SPU_div  (SPU_struct* const spu);

void SPU_out  (SPU_struct* const spu);

#endif
