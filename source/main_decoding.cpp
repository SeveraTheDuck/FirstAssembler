#include "../headers/disassembler.h"

static const size_t MAX_FILE_NAME_LENGTH = 0x100;

int main (int /*argc*/, char** /*argv*/)
{
    char translated_file_name [MAX_FILE_NAME_LENGTH] = {};
    char decoded_file_name    [MAX_FILE_NAME_LENGTH] = {};

    printf ("Type file you need to decode (in binary):\n");
    scanf  ("%s", translated_file_name);

    printf ("Type file where to decode:\n");
    scanf  ("%s", decoded_file_name);

    DecodeFile (translated_file_name, decoded_file_name);
}
