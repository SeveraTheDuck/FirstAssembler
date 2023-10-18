#include "headers/translator.h"

static const size_t CMD_MAX_LENGTH = 256;
static const size_t FILE_MAX_SIZE  = 4096;

static char   translated_string[FILE_MAX_SIZE] = {};
static size_t translated_string_index = 0;

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

#define DEF_CMD(function_name, function_number, n_args, ...)                \
    if (strcasecmp (operation, #function_name) == 0)                        \
    {                                                                       \
        *(int*)(void*) (translated_string +                                 \
                        translated_string_index) = ASM_##function_name;     \
        operation_addresses[translated_operations_number++] =               \
                            translated_string_index;                        \
        translated_string_index += sizeof (int);                            \
                                                                            \
        if (n_args) ReadArgument (original_file, operation_addresses,       \
                                  n_line, operation, (function_number));    \
    }                                                                       \
    else

FILE* Translator (file_input* const original_file,
                  FILE*       const translated_file)
{
    assert (original_file);
    assert (translated_file);

    char operation[CMD_MAX_LENGTH] = {};

    size_t operation_addresses[FILE_MAX_SIZE] = {};
    size_t translated_operations_number = 0;

    for (size_t n_line = 0; n_line < original_file->number_of_lines; ++n_line)
    {
        sscanf (original_file->lines_array[n_line].line, "%255s", operation);
        #include "headers/commands.h"
        ;
    }

    fwrite (translated_string, translated_string_index, sizeof (char),
            translated_file);
    return  translated_file;
}

#undef DEF_CMD

void ReadArgument (const file_input* const original_file,
                   const size_t* operation_addresses,
                   const size_t n_line,
                   char* const operation,
                   const int function_number)
{
    int  n_read_args = 0;
    int  d_arg       = 0;
    char c_arg[2]    = {};

    if (sscanf (original_file->lines_array[n_line].line,
        "%s %d", operation, &d_arg) == 2)
    {
        TranslateArgument (STANDART_REGIME, operation_addresses,
                           d_arg, function_number);
    }

    else if (sscanf (original_file->lines_array[n_line].line,
        "%s r%[abcd]x%n", operation, c_arg, &n_read_args) == 2)
    {
        TranslateArgument (REGISTER_REGIME, operation_addresses,
                           c_arg[0] - 'a', function_number);
    }
}

void TranslateArgument (const PushRegime push_regime,
                        const size_t* operation_addresses,
                        const int  translated_value,
                        const int  function_number)
{
    switch (function_number)
    {
        case ASM_PUSH:
        {
            TranslatePushOperation (push_regime);
            break;
        }

        case ASM_JMP:
        {
            TranslateJmpOperation (translated_value, operation_addresses);
            break;
        }
    }

    *(int*)(void*) (translated_string +
                    translated_string_index) = translated_value;
    translated_string_index += sizeof (int);
}

void TranslatePushOperation (const PushRegime push_regime)
{
    int translated_push = ASM_PUSH | push_regime;

    translated_string_index -= sizeof (int);
    *(int*)(void*) (translated_string +
                    translated_string_index) = translated_push;
    translated_string_index += sizeof (int);
}

void TranslateJmpOperation (const int translated_value,
                            const size_t* operation_addresses)
{
    *(int*)(void*) (translated_string + translated_string_index) =
              (int) operation_addresses[translated_value];
    translated_string_index += sizeof (int);
}
