#include "headers/emulator.h"

void EmulateCPU (file_input* const asm_code)
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
            case 0:
                STACK_DTOR (&stk);
                return;

                break;

            case 1:
                r_operator = atoi (asm_code-> lines_array[n_line].line +
                                   strlen ("1 "));
                StackPush (&stk, r_operator);
                STACK_DUMP (&stk);

                break;

            case 2:
                StackPop  (&stk, &r_operator);
                StackPop  (&stk, &l_operator);
                StackPush (&stk, l_operator + r_operator);
                STACK_DUMP (&stk);

                break;

            case 3:
                StackPop  (&stk, &r_operator);
                StackPop  (&stk, &l_operator);
                StackPush (&stk, l_operator - r_operator);
                STACK_DUMP (&stk);

                break;

            case 4:
                StackPop  (&stk, &r_operator);
                StackPop  (&stk, &l_operator);
                StackPush (&stk, l_operator * r_operator);
                STACK_DUMP (&stk);

                break;

            case 5:
                StackPop  (&stk, &r_operator);
                StackPop  (&stk, &l_operator);
                StackPush (&stk, l_operator / r_operator);
                STACK_DUMP (&stk);

                break;

            case 6:
                StackPop  (&stk, &r_operator);
                STACK_DUMP (&stk);
                printf ("%d", r_operator);

                break;
        }
    }

    STACK_DTOR (&stk);
}
