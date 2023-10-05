#include "headers/disassembler.h"

FILE* DecodeFile (file_input* const translated_file)
{
    assert (translated_file);
    FILE* decoded_file = fopen ("decoded.txt", "wb");
    assert(decoded_file);

    int n_operation = 0;

    for (size_t n_line = 0; n_line < translated_file->number_of_lines; ++n_line)
    {
        n_operation = atoi (translated_file->lines_array[n_line].line);
        fprintf (decoded_file, "%s", operations_array[n_operation]);

        if (n_operation == 1)
        {
            fprintf (decoded_file, " %s",
                     translated_file->lines_array[n_line].line + strlen ("1 "));
        }

        fprintf (decoded_file, "\n");
    }

    return decoded_file;
}
