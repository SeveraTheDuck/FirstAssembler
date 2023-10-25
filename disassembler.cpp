#include "headers/disassembler.h"

static const size_t MAX_DECODED_FILE_SIZE =
                    MAX_TRANSLATED_FILE_SIZE * sizeof (int);
static const unsigned char OPERATION_NUMBER_MASK = 0x1F;

void DecodeFile (const char* const translated_file_name,
                 const char* const decoded_file_name)
{
    assert (translated_file_name);
    assert (decoded_file_name);

    FILE*   translated_file = fopen (translated_file_name, "rb");
    assert (translated_file);

    unsigned char spu_code[MAX_TRANSLATED_FILE_SIZE] = {};

    const size_t spu_code_length = fread (spu_code, sizeof (char),
                                          MAX_TRANSLATED_FILE_SIZE,
                                          translated_file);

    fclose (translated_file);

    Decoder (spu_code, spu_code_length, decoded_file_name);
}

#define DEF_CMD(function_name, function_number, n_args, ...)            \
    case (function_number):                                             \
    {                                                                   \
                                                                        \
        decoded_file_index +=                                           \
            snprintf (decoded_file_array + decoded_file_index,          \
                      MAX_WORD_LENGTH, "%0X\t%s ",                      \
                      (int) spu_code_index, #function_name);            \
        spu_code_index++;                                               \
                                                                        \
        if (n_args)                                                     \
        {                                                               \
            PrintArgs(command, spu_code, &spu_code_index,               \
                      decoded_file_array, &decoded_file_index);         \
        }                                                               \
                                                                        \
        break;                                                          \
    }

void Decoder (const unsigned char* const spu_code,
              const size_t spu_code_length,
              const char* const decoded_file_name)
{
    assert (decoded_file_name);

    unsigned char command = 0;
    size_t spu_code_index = 0;
    char   decoded_file_array[MAX_DECODED_FILE_SIZE] = {};
    int decoded_file_index = 0;

    while (spu_code_index < spu_code_length)
    {
        command = spu_code[spu_code_index];

        switch (command & OPERATION_NUMBER_MASK)
        {
            #include "headers/commands.h"

            default:
            {
                decoded_file_index +=
                    snprintf (decoded_file_array + decoded_file_index,
                              MAX_WORD_LENGTH, "Command not found.");

                WriteToFile (decoded_file_name,
                             decoded_file_array,
                             decoded_file_index);

                return;
            }
        }

        decoded_file_array[decoded_file_index++] = '\n';
    }

    WriteToFile (decoded_file_name,
                             decoded_file_array,
                             decoded_file_index);
}

#undef DEF_CMD

void WriteToFile (const char* const decoded_file_name,
                  const char* const decoded_file_array,
                  const int         decoded_file_index)
{
    assert (decoded_file_name);
    assert (decoded_file_array);

    FILE*   decoded_file = fopen (decoded_file_name, "wb");
    assert (decoded_file);

    fwrite (decoded_file_array, sizeof (char),
            (size_t) decoded_file_index, decoded_file);

    fclose (decoded_file);
}

void PrintArgs (const unsigned char command,
                const unsigned char* const spu_code,
                size_t* const spu_code_index,
                char* const decoded_file_array,
                int* const decoded_file_index)
{
    assert (spu_code);
    assert (spu_code_index);
    assert (decoded_file_array);
    assert (decoded_file_index);

    unsigned char reg_name = 0;
    int dec_argument       = 0;

    if (command & RAM_REGIME)
    {
        decoded_file_array[(*decoded_file_index)++] = '[';
    }

    if (command & REGISTER_REGIME)
    {
        reg_name = spu_code[(*spu_code_index)++] + 'a';

        *decoded_file_index +=
            snprintf (decoded_file_array + *decoded_file_index,
                      sizeof ("rax "), "r%cx ", reg_name);
    }

    if (command & STANDART_REGIME)
    {
        memcpy ((void*) &dec_argument, spu_code + *spu_code_index,
                sizeof (int));
        *spu_code_index += sizeof (int);

        if ((ASM_JMP <= (command & OPERATION_NUMBER_MASK) &&
                        (command & OPERATION_NUMBER_MASK) <= ASM_JNE) ||
                        (command & OPERATION_NUMBER_MASK) == ASM_CALL) // HARDCODING!!!
        {
            SetDisasmLabel (dec_argument,
                            decoded_file_array,
                            decoded_file_index);
        }

        else
        {
            *decoded_file_index +=
                snprintf (decoded_file_array + *decoded_file_index,
                        sizeof (int), "%d", dec_argument);
        }
    }

    if (command & RAM_REGIME)
    {
        decoded_file_array[(*decoded_file_index)++] = ']';
    }
}

void SetDisasmLabel (const int   dec_argument,
                     char* const decoded_file_array,
                     int * const decoded_file_index)
{
    assert (decoded_file_array);
    assert (decoded_file_index);

    static size_t number_of_labels = 0;

    *decoded_file_index +=
        snprintf (decoded_file_array + *decoded_file_index,
                 MAX_WORD_LENGTH, "label_%zd %0X",
                 number_of_labels++, dec_argument);
}
