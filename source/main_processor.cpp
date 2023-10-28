#include "../headers/SPU.h"

static const size_t MAX_FILE_NAME_LENGTH = 0x100;

int main (int /*argc*/, char** /*argv*/)
{
    char translated_file_name[MAX_FILE_NAME_LENGTH] = {};

    printf ("Type file you need to run (in binary):\n");
    scanf  ("%s", translated_file_name);

    SPU_process (translated_file_name);
}
