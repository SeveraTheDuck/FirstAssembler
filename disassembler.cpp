#include "headers/disassembler.h"

void DecodeFile (const char* const translated_file_name,
                 const char* const decoded_file_name)
{
    assert (translated_file_name);
    assert (decoded_file_name);

    file_input translated_file = {};
    GetFileInput (translated_file_name, &translated_file, PARTED);

    FILE* decoded_file = fopen (decoded_file_name, "wb");
    assert  (decoded_file);
    Decoder (&translated_file, decoded_file);

    FreeFileInput (&translated_file);
    fclose (decoded_file);
}

FILE* Decoder (file_input* const translated_file,
               FILE*       const decoded_file)
{
    assert (translated_file);
    assert (decoded_file);

    int n_operation = 0;
    int push_value  = 0;

    for (size_t n_line = 0;
                n_line < translated_file->number_of_lines;
              ++n_line)
    {
        sscanf  (translated_file->lines_array[n_line].line,
                 "%d", &n_operation);
        fprintf (decoded_file, "%s", operations_array[n_operation]);

        if (n_operation == ASM_PUSH)
        {
            sscanf  (translated_file->lines_array[n_line].line,
                     "%d %d", &n_operation, &push_value);
            fprintf (decoded_file, " %d", push_value);
        }

        fprintf (decoded_file, "\n");
    }

    return decoded_file;
}
