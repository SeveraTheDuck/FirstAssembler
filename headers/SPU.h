#ifndef SPU_H
#define SPU_H

#include <math.h>
#include "../stack/headers/stack.h"
#include "constants.h"

const size_t RAM_AMOUNT = 100;
const int    DOUBLE_PRECISION = 10000; ///< Four digits after the point.

#define SPU_GET_ELEM_INFO __LINE__, __FILE__, __func__

#define SPU_CTOR_RECIVE_INFO const size_t       init_line,      \
                             const char*  const init_file,      \
                             const char*  const init_func

#define SPU_DUMP_RECIEVE_INFO const int   line,                 \
                              const char* file_name,            \
                              const char* func_name

#define SPU_VERIFY(spu)                                         \
    spu->spu_errors_list.SPU_ERROR_OCCURED = SPU_verify (spu);  \
    if (spu)                                                    \
    {                                                           \
        if (spu->spu_errors_list.SPU_ERROR_OCCURED)             \
        {                                                       \
            SPU_dump (spu, SPU_GET_ELEM_INFO);                  \
            return spu->spu_errors_list;                        \
        }                                                       \
    }                                                           \
    else                                                        \
    {                                                           \
        SPU_error spu_nullptr = { 1 };                          \
        return spu_nullptr;                                     \
    }

typedef unsigned int SPU_error_t;
struct SPU_error
{
    SPU_error_t SPU_NULLPTR                : 1;
    SPU_error_t SPU_FILE_WRONG_DECODE_WORD : 1;
    SPU_error_t SPU_STACK_ERR              : 11;
    SPU_error_t SPU_WRONG_INPUT_COMMAND    : 1;
    SPU_error_t SPU_ERROR_OCCURED          : 1;
};

struct SPU_struct
{
    Stack stk;
    Stack call_stk;

    Processor_t ram_array[RAM_AMOUNT];

    int register_vars[REG_NUMBER];

    SPU_error spu_errors_list;

    size_t      init_line;
    const char* init_file;
    const char* init_func;
};

SPU_error   SPU_СTOR    (SPU_struct* const spu,
                         SPU_CTOR_RECIVE_INFO);

SPU_error   SPU_DTOR    (SPU_struct* spu);

SPU_error_t SPU_verify  (SPU_struct* spu);

void        SPU_dump    (SPU_struct* spu, SPU_DUMP_RECIEVE_INFO);

void        SPU_process (const char* const spu_file_name);

SPU_error   Processor (const unsigned char spu_code[],
                       const size_t        code_length,
                       SPU_struct* const   spu);

#endif
