#ifndef SPU_H
#define SPU_H

#include "../FileOpenLib/filestruct.h"
#include "../stack/headers/stack.h"
#include "translator.h"

#define SPU_VERIFY(spu)                         \
    SPU_verify (spu);                           \
    if (spu)                                    \
    {                                           \
        if (*(int*) &spu->spu_errors_list != 0) \
        {                                       \
            return spu->spu_errors_list;        \
        }                                       \
    }                                           \
    else                                        \
    {                                           \
        SPU_error spu_nullptr = { 1 };          \
        return spu_nullptr;                     \
    }

const size_t REG_NUMBER = 4;

struct SPU_error
{
    int SPU_NULLPTR                : 1;
    int SPU_FILE_WRONG_DECODE_WORD : 1;
    int SPU_STACK_ERR              : 11;
    int SPU_WRONG_INPUT_COMMAND    : 1;
};

struct SPU_struct
{
    Stack stk;
    int register_vars[REG_NUMBER];
    SPU_error spu_errors_list;
};

SPU_error SPU_СTOR (SPU_struct* spu);

SPU_error SPU_DTOR (SPU_struct* spu);

SPU_error SPU_verify (SPU_struct* spu);

void SPU_process (const char* const spu_file_name);

SPU_error Processor (file_input* const asm_code,
                SPU_struct* const spu);

SPU_error SPU_push (SPU_struct* const spu,
                    file_input* const asm_code,
                    const size_t n_line,
                    int n_operation);

SPU_error SPU_pop (SPU_struct* const spu,
                   file_input* const asm_code,
                   const size_t n_line,
                   int n_operation);

SPU_error SPU_in   (SPU_struct* const spu);

SPU_error SPU_add  (SPU_struct* const spu);

SPU_error SPU_sub  (SPU_struct* const spu);

SPU_error SPU_mul  (SPU_struct* const spu);

SPU_error SPU_div  (SPU_struct* const spu);

SPU_error SPU_out  (SPU_struct* const spu);

#endif
