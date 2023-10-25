DEF_CMD (HLT, 0x00, 0,
{
    return spu->spu_errors_list;
})

DEF_CMD (PUSH, 0x01, 2,
{
    int push_value = 0;

    if (n_operation & RAM_REGIME)
    {
        push_value = spu->ram_array[dec_arg + reg_arg_value];
    }
    else
    {
        push_value = dec_arg + reg_arg_value;
    }

    StackPush (&spu->stk, push_value);
})

DEF_CMD (POP, 0x02, 2,
{
    if (n_operation & RAM_REGIME)
    {
        StackPop (&spu->stk, &spu->ram_array[dec_arg + reg_arg_value]);
    }
    else
    {
        StackPop (&spu->stk, &spu->register_vars[reg_arg]);
    }
})

DEF_CMD (IN, 0x03, 0,
{
    double float_in_value        = 0;
    Processor_t decimal_in_value = 0;

    printf ("Enter in value:\n");
    scanf  ("%lg", &float_in_value);

    decimal_in_value = (Processor_t) (float_in_value * DOUBLE_PRECISION);

    StackPush (&spu->stk, decimal_in_value);
})

DEF_CMD (OUT, 0x04, 0,
{
    Processor_t decimal_output_value = 0;
    double float_output_value        = 0;

    StackPop (&spu->stk, &decimal_output_value);
    float_output_value = (double) decimal_output_value / DOUBLE_PRECISION;

    printf ("%lg\n", float_output_value);
})

DEF_CMD (ADD, 0x05, 0,
{
    Processor_t l_operator  = 0;
    Processor_t r_operator  = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);

    StackPush (&spu->stk, l_operator + r_operator);
})

DEF_CMD (SUB, 0x06, 0,
{
    Processor_t l_operator  = 0;
    Processor_t r_operator  = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);

    StackPush (&spu->stk, l_operator - r_operator);
})

DEF_CMD (MUL, 0x07, 0,
{
    Processor_t l_operator  = 0;
    Processor_t r_operator  = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);

    Processor_t output_value = l_operator * r_operator;
    output_value /= DOUBLE_PRECISION;

    StackPush (&spu->stk, output_value); //FIXME UB (weak)
})

DEF_CMD (DIV, 0x08, 0,
{
    Processor_t l_operator  = 0;
    Processor_t r_operator  = 0;
    Processor_t result      = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);

    if (r_operator == 0)
    {
        fprintf (stderr, "DIVIDING BY ZERO IS FORBIDDEN IN MATH");
                                                                    // ADD DIVIDING BY ZERO ERROR
        return spu->spu_errors_list;
    }

    result = (int) ((double) (l_operator) / r_operator * DOUBLE_PRECISION);

    StackPush (&spu->stk, result);
})

#include "jumps.h"
// jump.h counter stops at number 0x0F, so next is 0x10.

DEF_CMD (CALL, 0x10, 1,
{
    code_index -= sizeof (int);
    StackPush (&spu->call_stk, (Elem_t) (code_index + sizeof (int)));

    int dest_operation = 0;

    memcpy (&dest_operation, spu_code + code_index, sizeof (int));
    code_index = (size_t) dest_operation;
})

DEF_CMD (RET, 0x11, 0,
{
    int dest_operation = 0;
    StackPop (&spu->call_stk, &dest_operation);
    code_index = (size_t) dest_operation;
})

DEF_CMD (SQRT, 0x12, 0,
{
    Processor_t sqrt_value  = 0;
    double      float_value = 0;

    StackPop (&spu->stk, &sqrt_value);
    float_value = (double) sqrt_value / DOUBLE_PRECISION;
    sqrt_value = (Processor_t) (sqrt (float_value) * DOUBLE_PRECISION);

    StackPush (&spu->stk, sqrt_value);
})

DEF_CMD (SIN, 0x13, 0,
{
    Processor_t sin_value   = 0;
    double      float_value = 0;

    StackPop (&spu->stk, &sin_value);
    float_value = (double) sin_value / DOUBLE_PRECISION;
    sin_value = (Processor_t) (sin (float_value) * DOUBLE_PRECISION);

    StackPush (&spu->stk, sin_value);
})

DEF_CMD (COS, 0x14, 0,
{
    Processor_t cos_value   = 0;
    double      float_value = 0;

    StackPop (&spu->stk, &cos_value);
    float_value = (double) cos_value / DOUBLE_PRECISION;
    cos_value = (Processor_t) (cos (float_value) * DOUBLE_PRECISION);

    StackPush (&spu->stk, cos_value);
})
