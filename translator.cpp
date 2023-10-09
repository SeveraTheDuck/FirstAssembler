#include "headers/translator.h"

void TranslateFile (const char* const original_file_name,
                    const char* const translated_file_name)
{
    assert (original_file_name);
    assert (translated_file_name);

    file_input original_file = {};
    GetFileInput (original_file_name, &original_file, PARTED);

    FILE* translated_file = fopen (translated_file_name, "wb");
    assert     (translated_file);
    Translator (&original_file, translated_file);

    FreeFileInput (&original_file);
    fclose (translated_file);
}

FILE* Translator (file_input* const original_file,
                  FILE*       const translated_file)
{
    assert (original_file);
    assert (translated_file);

    char operation[100] = {}; // is it worth it?

    for (size_t n_line = 0; n_line < original_file->number_of_lines; ++n_line)
    {
        sscanf (original_file->lines_array[n_line].line, "%s", operation);

        for (size_t n_operation = 0;
                    n_operation < OPERATIONS_NUMBER;
                  ++n_operation)
        {
            if (!strcmp (operation, operations_array[n_operation]))
            {
                fprintf (translated_file, "%zd", n_operation);

                if (n_operation == ASM_PUSH)
                {
                    TranslatorPush (original_file, translated_file,
                                    n_line, operation);
                }

                else if (n_operation == ASM_POP)
                {
                    TranslatorPop (original_file, translated_file,
                                   n_line, operation);
                }

                fprintf (translated_file, "\n");
            }
        }
    }

    return translated_file;
}

void TranslatorPush (file_input* const original_file,
                     FILE* const translated_file,
                     const size_t n_line,
                     char* const operation)
{
    assert (original_file);
    assert (translated_file);
    assert (operation);

    int  push_value = 0;
    int  push_check = 0;
    char push_reg   = 0;

    if (sscanf (original_file->lines_array[n_line].line,
                "%s %d", operation, &push_value) == 2)
    {
        fprintf (translated_file, " %d %d", STANDART_REGIME, push_value);
    }
    else
    {
        sscanf (original_file->lines_array[n_line].line,
                "%s r%[abcd]%n", operation,
                &push_reg, &push_check);
        fprintf (translated_file, " %d %d", REGISTER_REGIME, push_reg - 'a');
    }
}

void TranslatorPop (file_input* const original_file,
                    FILE* const translated_file,
                    const size_t n_line,
                    char* const operation)
{
    assert (original_file);
    assert (translated_file);
    assert (operation);

    char pop_reg = 0;

    sscanf (original_file->lines_array[n_line].line,
            "%s r%[abcd]x", operation, &pop_reg);
    fprintf (translated_file, " %d", pop_reg - 'a');
}
