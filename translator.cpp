#include "headers/translator.h"

static const size_t CMD_MAX_LENGTH    = 0x100;
static const size_t LABELS_MAX_NUMBER = 0x400;
static const size_t FILE_MAX_SIZE     = 0x1000;

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
                                  n_line, operation, (function_number),     \
                                  labels_array, &n_labels);                 \
    }                                                                       \
    else

FILE* Translator (file_input* const original_file,
                  FILE*       const translated_file)
{
    assert (original_file);
    assert (translated_file);

    char operation[CMD_MAX_LENGTH] = {};

    Label labels_array[LABELS_MAX_NUMBER] = {};
    size_t n_labels = 0;

    size_t operation_addresses[FILE_MAX_SIZE] = {};
    size_t translated_operations_number = 0;

    for (size_t n_line = 0; n_line < original_file->number_of_lines; ++n_line)
    {
        if (original_file->lines_array[n_line].line[0] == '\0')
        {
            continue;
        }

        if (original_file->lines_array[n_line].line[0] == ':')
        {
            SetLabel (original_file, n_line, labels_array, &n_labels);
            continue;
        }

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
                   const size_t  n_line,
                   char* const   operation,
                   const int     function_number,
                         Label * const labels_array,
                         size_t* n_labels)
{
    assert (original_file);
    assert (operation_addresses);
    assert (operation);
    assert (labels_array);
    assert (n_labels);

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

    else if (sscanf (original_file->lines_array[n_line].line,
             "%s :", operation) == 1)
    {
        GetLabel (original_file, n_line, operation,
                  labels_array,  n_labels);
    }
}

#define DEF_CMD(function_name, function_number, n_args, ...)            \
    case ASM_##function_name:                                           \
    {                                                                   \
        TranslateJmpOperation (translated_value, operation_addresses);  \
        break;                                                          \
    }

void TranslateArgument (const PushRegime push_regime,
                        const size_t* operation_addresses,
                        const int     translated_value,
                        const int     function_number)
{
    assert (operation_addresses);

    switch (function_number)
    {
        case ASM_PUSH:
        {
            TranslatePushOperation (push_regime);
            break;
        }

        #include "headers/jumps.h"
        ;
    }

    *(int*)(void*) (translated_string +
                    translated_string_index) = translated_value;
    translated_string_index += sizeof (int);
}

#undef DEF_CMD

void TranslatePushOperation (const PushRegime push_regime)
{
    int translated_push = ASM_PUSH | push_regime;

    translated_string_index -= sizeof (int);
    *(int*)(void*) (translated_string +
                    translated_string_index) = translated_push;
    translated_string_index += sizeof (int);
}

void SetLabel (const file_input* const original_file,
               const size_t            n_line,
                     Label *     const labels_array,
                     size_t*           n_labels)
{
    assert (original_file);
    assert (labels_array);
    assert (n_labels);

    sscanf (original_file->lines_array[n_line].line,
            ":%s", labels_array[*n_labels].label_name);
    labels_array[(*n_labels)++].label_address = translated_string_index;
}

void GetLabel (const file_input* const original_file,
               const size_t            n_line,
                     char*       const operation,
                     Label *     const labels_array,
                     size_t*           n_labels)
{
    assert (original_file);
    assert (operation);
    assert (labels_array);
    assert (n_labels);

    char need_label_name[LABEL_MAX_LENGTH] = {};
    sscanf (original_file->lines_array[n_line].line,
            "%s :%s", operation, need_label_name);

    for (size_t cur_label = 0; cur_label < *n_labels; ++cur_label)
    {
        if (strcmp (labels_array[cur_label].label_name, need_label_name) == 0)
        {
            *(int*)(void*) (translated_string + translated_string_index) =
                      (int) labels_array[cur_label].label_address;
            translated_string_index += sizeof (int);

            return;
        }
    }

    strcpy (labels_array[*n_labels].label_name, need_label_name);
    labels_array[(*n_labels)++].label_address = FILE_MAX_SIZE;
}

void TranslateJmpOperation (const int     translated_value,
                            const size_t* operation_addresses)
{
    assert (operation_addresses);

    *(int*)(void*) (translated_string + translated_string_index) =
              (int) operation_addresses[translated_value];
    translated_string_index += sizeof (int);
}
