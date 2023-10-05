#include "headers/translator.h"

FILE* TranslateFile (file_input* const original_file)
{
    assert (original_file);
    FILE*   translated_file = fopen ("translated.txt", "wb");
    assert (translated_file);

    for (size_t n_line = 0; n_line < original_file->number_of_lines; ++n_line)
    {
        for (size_t n_operator = 0;
                    n_operator < OPERATIONS_NUMBER;
                  ++n_operator)
        {
            // "push" defined as first operation, it has different syntax
            if (n_operator == 1)
            {
                if (strstr (original_file->lines_array[n_line].line, "push ") ==
                            original_file->lines_array[n_line].line)
                {
                    fprintf (translated_file, "%zd ", n_operator);
                    PrintPushValue (translated_file,
                                    original_file->lines_array[n_line].line);

                    break;
                }
            }
            if (!strcmp (original_file->lines_array[n_line].line,
                         operations_array[n_operator]))
            {
                fprintf (translated_file, "%zd\n", n_operator);

                break;
            }
        }
    }

    return translated_file;
}

void PrintPushValue (FILE* const translated_file,
                     const char* const line)
{
    int dec = atoi (line + strlen ("push "));
    fprintf (translated_file, "%d\n", dec);
}
