#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "../FileOpenLib/filestruct.h"
#include "constants.h"

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

void DoFixups (const Label* const labels_array);

void ReadArguments (const char  *       original_line,
                    const int           function_number,
                          Label * const labels_array,
                          size_t*       n_labels);

void TranslateArguments (const int     function_number,
                         const double  float_argument,
                         const char    register_argument);

void SetLabel (const char  * const original_line,
                     Label * const labels_array,
                     size_t*       n_labels);

void GetLabel (const int           function_number,
               const char  * const need_label_name,
                     Label * const labels_array,
                     size_t*       n_labels);

void SetUnknownLabel (const char  * const need_label_name,
                            Label * const labels_array,
                            size_t*       n_labels);

#endif
