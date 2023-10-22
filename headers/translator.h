#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <stdlib.h>
#include "../FileOpenLib/filestruct.h"

const size_t LABEL_MAX_LENGTH  = 0x100;

#define DEF_CMD(function_name, function_number, ...) \
    ASM_##function_name = function_number,

enum OperationsDecode
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

struct Label
{
    char   label_name[LABEL_MAX_LENGTH];
    size_t label_address;
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
                   const size_t  n_line,
                   char* const   operation,
                   const int     function_number,
                         Label * const labels_array,
                         size_t* n_labels);

void TranslateArgument (const PushRegime push_regime,
                        const size_t* operation_addresses,
                        const int     translated_value,
                        const int     function_number);

void TranslatePushOperation (const PushRegime push_regime);

void SetLabel (const file_input* const original_file,
               const size_t            n_line,
                     Label *     const labels_array,
                     size_t*           n_labels);

void GetLabel (const file_input* const original_file,
               const size_t            n_line,
                     char*       const operation,
                     Label *     const labels_array,
                     size_t*           n_labels);

void TranslateJmpOperation (const int     translated_value,
                            const size_t* operation_addresses);

#endif
