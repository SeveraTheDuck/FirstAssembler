#include "../headers/translator.h"

static const size_t MAX_FILE_NAME_LENGTH = 0x100;

int main (int /*argc*/, char** /*argv*/)
{
    char original_file_name  [MAX_FILE_NAME_LENGTH] = {};
    char translated_file_name[MAX_FILE_NAME_LENGTH] = {};

    printf ("Type file you need to translate:\n");
    scanf  ("%s", original_file_name);

    printf ("Type file where to translate:\n");
    scanf  ("%s", translated_file_name);

    TranslateFile (original_file_name, translated_file_name);
}
