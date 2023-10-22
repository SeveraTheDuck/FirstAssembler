DEF_CMD (HLT, 0x00, 0,
{
    return spu->spu_errors_list;
})

DEF_CMD (PUSH, 0x01, 1,
{
    code_index -= sizeof (int);
    int push_regime = (*(int*)(void*) (spu_code + code_index)) & REGISTER_REGIME;
    code_index += sizeof (int);

    Processor_t push_value = *(Processor_t*)(void*) (spu_code + code_index);
    code_index += sizeof (Processor_t);

    if (push_regime == STANDART_REGIME)
    {
        StackPush (&spu->stk, push_value);
    }
    else if (push_regime == REGISTER_REGIME)
    {
        StackPush (&spu->stk, spu->register_vars[push_value]);
    }
})

DEF_CMD (POP, 0x02, 1,
{
    Processor_t reg_number = *(Processor_t*)(void*) (spu_code + code_index);
    code_index += sizeof (Processor_t);

    StackPop (&spu->stk, &spu->register_vars[reg_number]);
})

DEF_CMD (IN, 0x03, 0,
{
    Processor_t in_value = 0;
    printf ("Enter in value:\n");
    scanf ("%d", &in_value);
    StackPush (&spu->stk, in_value);
})

DEF_CMD (ADD, 0x04, 0,
{
    Processor_t l_operator  = 0;
    Processor_t r_operator  = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);
    StackPush (&spu->stk, l_operator + r_operator);
})

DEF_CMD (SUB, 0x05, 0,
{
    Processor_t l_operator  = 0;
    Processor_t r_operator  = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);
    StackPush (&spu->stk, l_operator - r_operator);
})

DEF_CMD (MUL, 0x06, 0,
{
    Processor_t l_operator  = 0;
    Processor_t r_operator  = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);
    StackPush (&spu->stk, l_operator * r_operator); //FIXME UB (weak)
})

DEF_CMD (DIV, 0x07, 0,
{
    Processor_t l_operator  = 0;
    Processor_t r_operator  = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);

    if (r_operator == 0)
    {
        fprintf (stderr, "DIVIDING BY ZERO IS FORBIDDEN IN MATH");
                                                                    // ADD DIVIDING BY ZERO ERROR
        return spu->spu_errors_list;
    }
    StackPush (&spu->stk, l_operator / r_operator);
})

DEF_CMD (OUT, 0x08, 0,
{
    Processor_t output_value = 0;

    StackPop (&spu->stk, &output_value);
    printf ("%d\n", output_value);
})

#include "jumps.h"
