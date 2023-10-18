#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <stdlib.h>
#include "../FileOpenLib/filestruct.h"

/// @brief This const tells you the number of operations.
const size_t OPERATIONS_NUMBER = 9; //FIXME: fucking hardcore

/// @brief This array consists of lines of operations.
/// It is used to make ordered list of operations without explicit number list,
/// just using indexes.
const char* const operations_array[OPERATIONS_NUMBER] = {"hlt",
                                                         "push",
                                                         "pop",
                                                         "in",
                                                         "add",
                                                         "sub",
                                                         "mul",
                                                         "div",
                                                         "out"};

#define DEF_CMD(function_name, function_number, ...) \
    ASM_##function_name = function_number,

enum operations_decode
{
    #include "commands.h"
    ASM_VERSION = 2
};

#undef DEF_CMD

enum PushRegime
{
    STANDART_REGIME = 0,
    REGISTER_REGIME = 0x40
};

void TranslateFile (const char* const original_file_name,
                    const char* const translated_file_name);

/// @brief This function translates original file with operations into asm language.
/// @param original_file Pointer to struct with information about original file.
/// @return Pointer to opened translated file on asm language.
FILE* Translator (file_input* const original_file,
                  FILE*       const translated_file);

void ReadArgument (const file_input* const original_file,
                   const size_t* operation_addresses,
                   const size_t n_line,
                   char* const operation,
                   const int function_number);

void TranslateArgument (const PushRegime push_regime,
                        const size_t* operation_addresses,
                        const int  translated_value,
                        const int  function_number);

void TranslatePushOperation (const PushRegime push_regime);

void TranslateJmpOperation (const int translated_value,
                            const size_t* operation_addresses);

#endif
