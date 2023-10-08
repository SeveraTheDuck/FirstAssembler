#include "headers/emulator.h"

void ProcessSPU (const char* const spu_file_name)
{
    assert (spu_file_name);

    file_input asm_code = {};
    GetFileInput (spu_file_name, &asm_code, PARTED);
    Processor    (&asm_code);

    FreeFileInput (&asm_code);
}

void Processor (file_input* const asm_code)
{
    assert (asm_code);

    int n_operation = 0;
    int l_operator  = 0;
    int r_operator  = 0;

    Stack stk = {};
    STACK_CTOR (&stk);

    for (size_t n_line = 0; n_line < asm_code->number_of_lines; ++n_line)
    {
        n_operation = atoi (asm_code->lines_array[n_line].line);
        switch (n_operation)
        {
            case ASM_HLT:
                STACK_DTOR (&stk);
                return;

                break;

            case ASM_PUSH:
                r_operator = atoi (asm_code->lines_array[n_line].line +
                                   strlen ("1 "));
                StackPush  (&stk, r_operator);
                STACK_DUMP (&stk);

                break;

            case ASM_ADD:
                StackPop   (&stk, &r_operator);
                StackPop   (&stk, &l_operator);
                StackPush  (&stk, l_operator + r_operator);
                STACK_DUMP (&stk);

                break;

            case ASM_SUB:
                StackPop   (&stk, &r_operator);
                StackPop   (&stk, &l_operator);
                StackPush  (&stk, l_operator - r_operator);
                STACK_DUMP (&stk);

                break;

            case ASM_MUL:
                StackPop   (&stk, &r_operator);
                StackPop   (&stk, &l_operator);
                StackPush  (&stk, l_operator * r_operator);
                STACK_DUMP (&stk);

                break;

            case ASM_DIV:
                StackPop   (&stk, &r_operator);
                StackPop   (&stk, &l_operator);
                StackPush  (&stk, l_operator / r_operator);
                STACK_DUMP (&stk);

                break;

            case ASM_OUT:
                StackPop   (&stk, &r_operator);
                STACK_DUMP (&stk);
                printf ("\n%d\n", r_operator);

                break;
        }
    }

    STACK_DTOR (&stk);
}
