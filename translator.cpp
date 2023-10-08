#include "headers/translator.h"

FILE* TranslateFile (file_input* const original_file)
{
    assert (original_file);

    FILE*   translated_file = fopen ("translated.txt", "wb");
    assert (translated_file);

    char operation[100] = {}; // is it worth it?
    int  push_value = 0;

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
                    sscanf (original_file->lines_array[n_line].line,
                            "%s %d", operation, &push_value); // skip push operation
                    fprintf (translated_file, " %d", push_value);
                }

                fprintf (translated_file, "\n");
            }
        }
    }

    return translated_file;
}
