DEF_CMD (HLT, 0x00, 0,
{
    return spu->spu_errors_list;
})

DEF_CMD (PUSH, 0x01, 2,
{
    Processor_t push_value = 0;

    if (n_operation & RAM_MODE)
    {
        push_value = spu->ram_array[(int64_t) float_arg +
                                    (int64_t) reg_arg_value];
    }

    else
    {
        push_value = float_arg + reg_arg_value;
    }

    StackPush (&spu->stk, push_value);
})

DEF_CMD (POP, 0x02, 2,
{
    if (n_operation & RAM_MODE)
    {
        StackPop (&spu->stk, &spu->ram_array[(int64_t) float_arg +
                                             (int64_t) reg_arg_value]);
    }

    else
    {
        StackPop (&spu->stk, &spu->register_vars[reg_arg]);
    }
})

DEF_CMD (IN, 0x03, 0,
{
    double float_in_value = 0;

    printf ("Enter in value:\n");
    scanf  ("%lg", &float_in_value);

    StackPush (&spu->stk, float_in_value);
})

DEF_CMD (OUT, 0x04, 0,
{
    double float_output_value = 0;

    StackPop (&spu->stk, &float_output_value);

    fprintf (stderr, "%lg\n", float_output_value);
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

    StackPush (&spu->stk, output_value);
})

DEF_CMD (DIV, 0x08, 0,
{
    Processor_t l_operator  = 0;
    Processor_t r_operator  = 0;
    Processor_t result      = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);

    if (DOUBLE_CMP (r_operator, 0))
    {
        fprintf (stderr, "DIVIDING BY ZERO IS FORBIDDEN IN MATH");
        return spu->spu_errors_list;
    }

    result = l_operator / r_operator;

    StackPush (&spu->stk, result);
})

#include "jumps.h"
// jump.h counter stops at number 0x0F, so next is 0x10.

//???
DEF_CMD (CALL, 0x0F, 1,
{
    code_index -= sizeof (double);
    StackPush (&spu->call_stk, (Elem_t) (code_index + sizeof (int)));

    int dest_operation = 0;

    memcpy (&dest_operation, spu_code + code_index, sizeof (int));
    code_index = (size_t) dest_operation;
})

DEF_CMD (RET, 0x10, 0,
{
    double dest_operation = 0;

    StackPop (&spu->call_stk, &dest_operation);

    if (dest_operation < 0)
    {
        fprintf (stderr, "Wrong return address: " OUTPUT_F "\n", dest_operation);
        abort (); // do not abort?
    }
    code_index = (size_t) dest_operation;
})

DEF_CMD (SQRT, 0x11, 0,
{
    Processor_t sqrt_value  = 0;
    double      float_value = 0;

    StackPop (&spu->stk, &float_value);
    sqrt_value = sqrt (float_value); // i

    StackPush (&spu->stk, sqrt_value);
})

DEF_CMD (SIN, 0x12, 0,
{
    Processor_t sin_value   = 0;
    double      float_value = 0;

    StackPop (&spu->stk, &float_value);
    sin_value = sin (float_value);

    StackPush (&spu->stk, sin_value);
})

DEF_CMD (COS, 0x13, 0,
{
    Processor_t cos_value   = 0;
    double      float_value = 0;

    StackPop (&spu->stk, &float_value);
    cos_value = cos (float_value);

    StackPush (&spu->stk, cos_value);
})

DEF_CMD (DIV_MODULE, 0x14, 0,
{
    Processor_t l_operator  = 0;
    Processor_t r_operator  = 0;
    Processor_t result      = 0;

    StackPop  (&spu->stk, &r_operator);
    StackPop  (&spu->stk, &l_operator);

    if (DOUBLE_CMP (r_operator, 0))
    {
        fprintf (stderr, "DIVIDING BY ZERO IS FORBIDDEN IN MATH");
        return spu->spu_errors_list;
    }

    result = (int64_t) l_operator % (int64_t) r_operator;

    StackPush (&spu->stk, result);
})

DEF_CMD (POW, 0x15, 0,
{
    double float_value = 0;
    double power       = 0;

    StackPop (&spu->stk, &power);
    StackPop (&spu->stk, &float_value);

    float_value = pow (float_value, power);

    StackPush (&spu->stk, float_value);
})

DEF_CMD (IS_EQUAL, 0x16, 0,
{
    double left_value  = 0;
    double right_value = 0;

    StackPop (&spu->stk, &right_value);
    StackPop (&spu->stk, &left_value);

    // bool answer = DOUBLE_CMP (right_value - left_value, 0);

    StackPush (&spu->stk, left_value);
})

DEF_CMD (GREATER, 0x17, 0,
{
    double left_value  = 0;
    double right_value = 0;

    StackPop (&spu->stk, &right_value);
    StackPop (&spu->stk, &left_value);

    StackPush (&spu->stk, right_value - left_value > __FLT_EPSILON__);
})

DEF_CMD (LESS, 0x18, 0,
{
    double left_value  = 0;
    double right_value = 0;

    StackPop (&spu->stk, &right_value);
    StackPop (&spu->stk, &left_value);

    StackPush (&spu->stk, right_value - left_value < -__FLT_EPSILON__);
})

DEF_CMD (GOE, 0x19, 0,
{
    double left_value  = 0;
    double right_value = 0;

    StackPop (&spu->stk, &right_value);
    StackPop (&spu->stk, &left_value);

    StackPush (&spu->stk, right_value - left_value > -__FLT_EPSILON__);
})

DEF_CMD (LOE, 0x1A, 0,
{
    double left_value  = 0;
    double right_value = 0;

    StackPop (&spu->stk, &right_value);
    StackPop (&spu->stk, &left_value);

    StackPush (&spu->stk, right_value - left_value < __FLT_EPSILON__);
})

DEF_CMD (NOT_EQUAL, 0x1B, 0,
{
    double left_value  = 0;
    double right_value = 0;

    StackPop (&spu->stk, &right_value);
    StackPop (&spu->stk, &left_value);

    StackPush (&spu->stk, !(DOUBLE_CMP (right_value - left_value, 0)));
})

DEF_CMD (NOT, 0x1C, 0,
{
    double float_value = 0;

    StackPop (&spu->stk, &float_value);

    StackPush (&spu->stk, !(DOUBLE_CMP (float_value, 0)));
})

DEF_CMD (LN, 0x1D, 0,
{
    double float_value = 0;

    StackPop (&spu->stk, &float_value);
    float_value = log (float_value);

    StackPush (&spu->stk, float_value);
})

DEF_CMD (MEM_PRINT, 0x1E, 0,
{
    PrintMemory (spu->ram_array);
})
