#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <stdlib.h>
#include "../FileOpenLib/filestruct.h"

/// @brief This const tells you the number of operations.
const size_t OPERATIONS_NUMBER = 7;

/// @brief This array consists of lines of operations.
/// It is used to make ordered list of operations without explicit number list,
/// just using indexes.
const char* const operations_array[OPERATIONS_NUMBER] = {"hlt",
                                                         "push",
                                                         "add",
                                                         "sub",
                                                         "mul",
                                                         "div",
                                                         "out"};

enum operations_decode
{
    ASM_HLT = 0,
    ASM_PUSH,
    ASM_ADD,
    ASM_SUB,
    ASM_MUL,
    ASM_DIV,
    ASM_OUT
};

/// @brief This function translates original file with operations into asm language.
/// @param original_file Pointer to struct with information about original file.
/// @return Pointer to opened translated file on asm language.
FILE* TranslateFile (file_input* const original_file);

#endif
