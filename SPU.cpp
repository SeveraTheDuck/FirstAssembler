#include "headers/SPU.h"

void SPU_CTOR (SPU_struct* const spu)
{
    assert (spu);
    assert (&spu->stk);
    assert (spu->register_vars);

    STACK_CTOR (&spu->stk);

    for (size_t i = 0; i < reg_number; ++i)
    {
        spu->register_vars[i] = 0;
    }
}

void SPU_DTOR (SPU_struct* spu)
{
    assert  (spu);
    assert (&spu->stk);

    STACK_DTOR (&spu->stk);

    for (size_t i = 0; i < reg_number; ++i)
    {
        spu->register_vars[i] = 0;
    }
}

void SPU_process (const char* const spu_file_name)
{
    assert (spu_file_name);

    file_input asm_code = {};
    GetFileInput (spu_file_name, &asm_code, PARTED);

    SPU_struct spu = {};
    SPU_CTOR (&spu);

    Processor     (&asm_code, &spu);

    FreeFileInput (&asm_code);
    SPU_DTOR (&spu);
}

void Processor (file_input* const asm_code,
                SPU_struct* const spu)
{
    assert (asm_code);
    assert (spu);

    int n_operation = 0;

    for (size_t n_line = 0; n_line < asm_code->number_of_lines; ++n_line)
    {
        sscanf (asm_code->lines_array[n_line].line, "%d", &n_operation);
        switch (n_operation)
        {
            case ASM_HLT:
                return;
                break;

            case ASM_PUSH:
                SPU_push (spu, asm_code, n_line, n_operation);
                STACK_DUMP (&spu->stk);

                break;

            case ASM_ADD:
                SPU_add (spu);
                STACK_DUMP (&spu->stk);

                break;

            case ASM_SUB:
                SPU_sub (spu);
                STACK_DUMP (&spu->stk);

                break;

            case ASM_MUL:
                SPU_mul (spu);
                STACK_DUMP (&spu->stk);

                break;

            case ASM_DIV:
                SPU_div (spu);
                STACK_DUMP (&spu->stk);

                break;

            case ASM_OUT:
                SPU_out (spu);
                STACK_DUMP (&spu->stk);

                break;
        }
    }
}

void SPU_push (SPU_struct* const spu,
               file_input* const asm_code,
               const size_t n_line,
               int n_operation)
{
    int push_regime = STANDART_REGIME;
    int push_value = 0;

    sscanf (asm_code->lines_array[n_line].line, "%d %d %d",
            &n_operation, &push_regime, &push_value);

    if (push_regime == STANDART_REGIME)
    {
        StackPush (&spu->stk, push_value);
    }
    else
    {
        StackPush (&spu->stk, spu->register_vars[push_value]);
    }
}

void SPU_add (SPU_struct* const spu)
{
    int l_operator  = 0;
    int r_operator  = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);
    StackPush (&spu->stk, l_operator + r_operator);
}

void SPU_sub (SPU_struct* const spu)
{
    int l_operator  = 0;
    int r_operator  = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);
    StackPush (&spu->stk, l_operator - r_operator);
}

void SPU_mul (SPU_struct* const spu)
{
    int l_operator  = 0;
    int r_operator  = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);
    StackPush (&spu->stk, l_operator * r_operator);
}

void SPU_div (SPU_struct* const spu)
{
    int l_operator  = 0;
    int r_operator  = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);
    StackPush (&spu->stk, l_operator / r_operator);
}

void SPU_out (SPU_struct* const spu)
{
    int output_value = 0;

    StackPop (&spu->stk, &output_value);
    printf ("%d", output_value);
}
