#include "headers/SPU.h"
// ГДЕ-ТО UB BLYAT
static const size_t MAX_FILE_SIZE = 4096;

SPU_error SPU_CTOR (SPU_struct* const spu,
                    SPU_CTOR_RECIVE_INFO)
{
    assert (spu);
    assert (spu->register_vars);

    STACK_CTOR (&spu->stk);

    spu->init_line = init_line;
    spu->init_file = init_file;
    spu->init_func = init_func;

    for (size_t i = 0; i < REG_NUMBER; ++i)
    {
        spu->register_vars[i] = 0;
    }

    memset (&spu->spu_errors_list, 0, sizeof (SPU_error));

    SPU_VERIFY (spu);
    return spu->spu_errors_list;
}


SPU_error SPU_DTOR (SPU_struct* spu)
{
    SPU_VERIFY (spu);

    STACK_DTOR (&spu->stk);

    for (size_t i = 0; i < REG_NUMBER; ++i)
    {
        spu->register_vars[i] = 0;
    }

    spu->spu_errors_list = {};

    return spu->spu_errors_list;
}

SPU_error_t SPU_verify (SPU_struct* spu)
{
    SPU_error_t spu_error_occured = 0;
    if (!spu)
    {
        SPU_error_t spu_nullptr = 1;
        return spu_nullptr;
    }

    if (StackVerify (&(spu->stk)))
    {
        spu_error_occured = 1;
    }

    return spu_error_occured;
}

void SPU_dump (SPU_struct* spu, SPU_DUMP_RECIEVE_INFO)
{
    fprintf (stderr, "\nSPU WAS DUMPED\n");
    fprintf (stderr, "SPU [%p]\nfrom %s(%zd) %s()\n",
                                                  spu,
                                                  spu->init_file,
                                                  spu->init_line,
                                                  spu->init_func);
    fprintf (stderr, "called from %s(%d) %s()\n", file_name, line, func_name);
    STACK_DUMP (&spu->stk);
}

void SPU_process (const char* const spu_file_name)
{
    assert (spu_file_name);

    FILE* spu_file = fopen (spu_file_name, "rb");
    assert (spu_file);

    char spu_code[MAX_FILE_SIZE] = {};
    size_t code_length = 0;
    code_length = fread (spu_code, sizeof (char), MAX_FILE_SIZE,
                         spu_file);

    fclose(spu_file);

    SPU_struct spu = {};
    SPU_CTOR (&spu, SPU_GET_ELEM_INFO);

    Processor (spu_code, code_length, &spu);

    SPU_DTOR (&spu);
}

#define DEF_CMD(function_name, function_number, n_args, action) \
    case ASM_##function_name:                                   \
        if (cmd_array[current_operation] <= code_index - sizeof (int))\
        {\
            cmd_array[current_operation++] = code_index - sizeof (int);\
            fprintf (stderr, "I am new cmd_array unit %zd\nI am function %s\n\n", cmd_array[current_operation - 1], #function_name);\
        }\
        SPU_VERIFY (spu);                                       \
        action                                                  \
        SPU_VERIFY (spu);                                       \
        break;

SPU_error Processor (const char spu_code[MAX_FILE_SIZE],
                     const size_t code_length,
                     SPU_struct* const spu)
{
    assert (spu_code);
    SPU_VERIFY (spu);

    int n_operation = 0;
    size_t code_index = 0;

    size_t* cmd_array = (size_t*) calloc (code_length / 2, sizeof (size_t*));
    size_t current_operation = 0;

    while (code_index < code_length)
    {
        n_operation = *(int*)(void*) (spu_code + code_index);
        code_index += sizeof (int);

        switch (n_operation & 0x3F)
        {
            #include "headers/commands.h"
        }
    }

    SPU_VERIFY (spu);
    free (cmd_array);
    return spu->spu_errors_list;
}

#undef DEF_CMD
