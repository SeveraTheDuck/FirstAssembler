#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <stdlib.h>
#include "../FileOpenLib/filestruct.h"

const size_t LABEL_MAX_LENGTH  = 0x100;

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
    STANDART_REGIME = 0x20,
    REGISTER_REGIME = 0x40,
    RAM_REGIME      = 0x80
};

struct Label
{
    char   label_name[LABEL_MAX_LENGTH];
    size_t label_address;
};

struct Fixup
{
    size_t label_address;
    size_t label_id;
};

void TranslateFile (const char* const original_file_name,
                    const char* const translated_file_name);

/// @brief This function translates original file with commands into asm language.
/// @param original_file Pointer to struct with information about original file.
/// @return Pointer to opened translated file on asm language.
FILE* Translator (file_input* const original_file,
                  FILE*       const translated_file);

void ReadArguments (const char  *       original_line,
                    const int           function_number,
                          Label * const labels_array,
                          size_t*       n_labels);

void TranslateArguments (const int  function_number,
                         const int  decimal_argument,
                         const char register_argument);

void SetLabel (const char  * const original_line,
                     Label * const labels_array,
                     size_t*       n_labels);

void GetLabel (const int           function_number,
               const char  * const need_label_name,
                     Label * const labels_array,
                     size_t*       n_labels);

#endif
