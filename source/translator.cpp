#include "../headers/translator.h"

/// @brief Max length of a command word while reading from asm file.
static const size_t CMD_MAX_LENGTH    = 0x100;

/// @brief Max number of labels in asm file.
static const size_t LABELS_MAX_NUMBER = 0x400;

/// @brief Max size of the asm file.
static const size_t FILE_MAX_SIZE     = 0x1000;

/// @brief Array of translated commands and numbers.
/// Will be pushed in translated file via fwrite().
static char translated_string[FILE_MAX_SIZE] = {};

/// @brief Index of translated_string[] array.
static size_t translated_string_index = 0;

/// @brief Array of labels fixups. Includes address to rewrite and number of label.
static Fixup label_fixups_array[LABELS_MAX_NUMBER] = {};

/// @brief Number of fixups.
static size_t n_label_fixups = 0;

void TranslateFile (const char* const original_file_name,
                    const char* const translated_file_name)
{
    assert (original_file_name);
    assert (translated_file_name);

    file_input original_file = {};
    GetFileInput (original_file_name, &original_file, PARTED);

    FILE*   translated_file = fopen (translated_file_name, "wb");
    assert (translated_file);
    Translator (&original_file, translated_file);

    FreeFileInput (&original_file);
    fclose (translated_file);
}

/// @brief Code generator for Translator().
#define DEF_CMD(function_name, function_number, n_args, ...)                \
    if (strcasecmp (command_name, #function_name) == 0)                     \
    {                                                                       \
        if (n_args)                                                         \
        {                                                                   \
            ReadArguments (original_file->lines_array[n_line].line +        \
                           command_len + 1,                                 \
                           (function_number),                               \
                           labels_array, &n_labels);                        \
        }                                                                   \
                                                                            \
        else                                                                \
        {                                                                   \
            *(translated_string +                                           \
              translated_string_index) = ASM_##function_name;               \
            translated_string_index += sizeof (char);                       \
        }                                                                   \
    }                                                                       \
    else

FILE* Translator (file_input* const original_file,
                  FILE      * const translated_file)
{
    assert (original_file);
    assert (translated_file);

    /// Array where the scaned command word is located.
    char command_name[CMD_MAX_LENGTH] = {};

    Label  labels_array[LABELS_MAX_NUMBER] = {};
    size_t n_labels = 0;

    for (size_t n_line = 0; n_line < original_file->number_of_lines; ++n_line)
    {
        // ';' is used to write comments
        if (original_file->lines_array[n_line].line[0] == '\0' ||
            original_file->lines_array[n_line].line[0] == ';')
        {
            continue;
        }

        if (original_file->lines_array[n_line].line[0] == ':')
        {
            SetLabel (original_file->lines_array[n_line].line,
                      labels_array, &n_labels);
            continue;
        }

        int command_len = 0;
        sscanf (original_file->lines_array[n_line].line,
                "%255s%n", command_name, &command_len);

        #include "../headers/commands.h"
        /* else */
        {
            fprintf (stderr, "UNKNOWN COMMAND %s\n", command_name);
            return nullptr;
        }
    }

    DoFixups (labels_array);

    fwrite (translated_string, translated_string_index, sizeof (char),
            translated_file);
    return  translated_file;
}

#undef DEF_CMD

void DoFixups (const Label* const labels_array)
{
    assert (labels_array);

    for (size_t i = 0; i < n_label_fixups; ++i)
    {
        memcpy (translated_string + label_fixups_array[i].label_address,
              &(labels_array[label_fixups_array[i].label_id]
               .label_address), sizeof (int));
    }
}

void ReadArguments (const char  *       original_line,
                          int           function_number,
                          Label * const labels_array,
                          size_t*       n_labels)
{
    assert (original_line);
    assert (labels_array);
    assert (n_labels);

    int  n_read_args    = 0;
    int  d_arg          = 0;
    char r_arg[2]       = {};
    char label_name[LABEL_MAX_LENGTH] = {};

    size_t original_line_length = strlen (original_line);

    if (original_line[0] == '[' &&
        original_line[original_line_length - 1] == ']')
    {
        function_number |= RAM_MODE;
        original_line += sizeof (char);
    }

    if (sscanf (original_line, ":%s", label_name) == 1)
    {
        function_number |= STANDART_MODE;
        GetLabel (function_number, label_name, labels_array,  n_labels);
        return;
    }

    if (sscanf (original_line,
                "r%[abcd]x %n", r_arg, &n_read_args) == 1)
    {
        function_number |= REGISTER_MODE;
    }

    if (sscanf (original_line + n_read_args,
                "%d", &d_arg) == 1)
    {
        function_number |= STANDART_MODE;
    }

    TranslateArguments (function_number, d_arg, r_arg[0] - 'a');
}

void TranslateArguments (const int  function_number,
                         const int  decimal_argument,
                         const char register_argument)
{
    *(translated_string + translated_string_index) = (char) function_number;
    translated_string_index += sizeof (char);

    if (function_number & REGISTER_MODE)
    {
        memcpy (translated_string + translated_string_index,
               &register_argument, sizeof (char));
        translated_string_index += sizeof (char);
    }

    if (function_number & STANDART_MODE)
    {
        memcpy (translated_string + translated_string_index,
               &decimal_argument, sizeof (int));
        translated_string_index += sizeof (int);
    }

    // When function has to have args but none valid received.
    if (((function_number & STANDART_MODE) |
         (function_number & REGISTER_MODE)) == 0)
    {
        fprintf (stderr, "Wrong arguments for function number %d\n",
                 function_number);
    }
}

#undef DEF_CMD

void SetLabel (const char  * const original_line,
                     Label * const labels_array,
                     size_t*       n_labels)
{
    assert (original_line);
    assert (labels_array);
    assert (n_labels);

    char new_label_name[LABEL_MAX_LENGTH] = {};

    sscanf (original_line, ":%s", new_label_name);

    for (size_t i = 0; i < *n_labels; ++i)
    {
        if (strcmp (new_label_name, labels_array[i].label_name) == 0)
        {
            labels_array[i].label_address = translated_string_index;
            return;
        }
    }

    strcpy (labels_array[*n_labels].label_name, new_label_name);
    labels_array[(*n_labels)++].label_address = translated_string_index;
}

void GetLabel (const int           function_number,
               const char  * const need_label_name,
                     Label * const labels_array,
                     size_t*       n_labels)
{
    assert (need_label_name);
    assert (labels_array);
    assert (n_labels);

    *(translated_string + translated_string_index) = (char) function_number;
    translated_string_index += sizeof (char);

    for (size_t cur_label = 0; cur_label < *n_labels; ++cur_label)
    {
        if (strcmp (labels_array[cur_label].label_name, need_label_name) == 0)
        {
            memcpy (translated_string + translated_string_index,
                   &labels_array[cur_label].label_address, sizeof (int));
            translated_string_index += sizeof (int);

            return;
        }
    }

    SetUnknownLabel (need_label_name, labels_array, n_labels);
}

void SetUnknownLabel (const char  * const need_label_name,
                            Label * const labels_array,
                            size_t*       n_labels)
{
    strcpy (labels_array[*n_labels].label_name, need_label_name);
    labels_array[*n_labels].label_address = FILE_MAX_SIZE;

    label_fixups_array[n_label_fixups].label_address = translated_string_index;
    label_fixups_array[n_label_fixups++].label_id    = (*n_labels)++;

    memcpy (translated_string + translated_string_index,
           &FILE_MAX_SIZE, sizeof (int));
    translated_string_index += sizeof (int);
}
