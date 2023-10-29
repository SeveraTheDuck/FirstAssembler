#ifndef SPU_CONSTANTS_H
#define SPU_CONSTANTS_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../config.h"

const size_t LABEL_MAX_LENGTH = 0x100;
const size_t RAM_AMOUNT       = 0x400;
const int    DOUBLE_PRECISION = 10000; ///< Four digits after the point.
const size_t REG_NUMBER       = 4;

#define DEF_CMD(function_name, function_number, ...)  \
    ASM_##function_name = function_number,

enum OperationsDecode
{
    #include "commands.h"
    ASM_VERSION = 2
};

#undef DEF_CMD

enum PushRegime
{
    STANDART_MODE = 0x20,
    REGISTER_MODE = 0x40,
    RAM_MODE      = 0x80
};

#endif
