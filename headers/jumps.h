DEF_CMD (JMP, 0x09, 1,
{
    int dest_operation = *(int*)(void*) (spu_code + code_index);
    code_index = (size_t) dest_operation;
})

DEF_CMD (JA, 0x0A, 1,
{
    Processor_t l_operator = 0;
    Processor_t r_operator = 0;
    StackPop (&spu->stk, &r_operator);
    StackPop (&spu->stk, &l_operator);

    if (r_operator > l_operator)
    {
        int dest_operation = *(int*)(void*) (spu_code + code_index);
        code_index = (size_t) dest_operation;
    }
    else
    {
        code_index += sizeof (int);
    }
})

DEF_CMD (JAE, 0x0B, 1,
{
    Processor_t l_operator = 0;
    Processor_t r_operator = 0;
    StackPop (&spu->stk, &r_operator);
    StackPop (&spu->stk, &l_operator);

    if (r_operator >= l_operator)
    {
        int dest_operation = *(int*)(void*) (spu_code + code_index);
        code_index = (size_t) dest_operation;
    }
    else
    {
        code_index += sizeof (int);
    }
})

DEF_CMD (JB, 0x0C, 1,
{
    Processor_t l_operator = 0;
    Processor_t r_operator = 0;
    StackPop (&spu->stk, &r_operator);
    StackPop (&spu->stk, &l_operator);

    if (r_operator < l_operator)
    {
        int dest_operation = *(int*)(void*) (spu_code + code_index);
        code_index = (size_t) dest_operation;
    }
    else
    {
        code_index += sizeof (int);
    }
})

DEF_CMD (JBE, 0x0D, 1,
{
    Processor_t l_operator = 0;
    Processor_t r_operator = 0;
    StackPop (&spu->stk, &r_operator);
    StackPop (&spu->stk, &l_operator);

    if (r_operator <= l_operator)
    {
        int dest_operation = *(int*)(void*) (spu_code + code_index);
        code_index = (size_t) dest_operation;
    }
    else
    {
        code_index += sizeof (int);
    }
})

DEF_CMD (JE, 0x0E, 1,
{
    Processor_t l_operator = 0;
    Processor_t r_operator = 0;
    StackPop (&spu->stk, &r_operator);
    StackPop (&spu->stk, &l_operator);

    if (r_operator == l_operator)
    {
        int dest_operation = *(int*)(void*) (spu_code + code_index);
        code_index = (size_t) dest_operation;
    }
    else
    {
        code_index += sizeof (int);
    }
})

DEF_CMD (JNE, 0x0F, 1,
{
    Processor_t l_operator = 0;
    Processor_t r_operator = 0;
    StackPop (&spu->stk, &r_operator);
    StackPop (&spu->stk, &l_operator);

    if (r_operator != l_operator)
    {
        int dest_operation = *(int*)(void*) (spu_code + code_index);
        code_index = (size_t) dest_operation;
    }
    else
    {
        code_index += sizeof (int);
    }
})
