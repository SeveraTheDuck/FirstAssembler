#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int programm_argc, char** programm_argv)
{
    if (programm_argc == 2)
    {
        if (strncmp (programm_argv[1], "--translate",
                               sizeof ("--translate")) == 0)
        {
            system ("executers/run_translation");
            return 0;
        }

        else if (strncmp (programm_argv[1], "--decode",
                                    sizeof ("--decode")) == 0)
        {
            system ("executers/run_decoding");
            return 0;
        }

        else if (strncmp (programm_argv[1], "--spu",
                                    sizeof ("--spu")) == 0)
        {
            system ("executers/run_processor");
            return 0;
        }
    }

    fprintf (stderr, "Wrong process input. Choose one out of three:\n"
                        "1. --translate to translate file from assembly "
                        "to binary language. Asm file required.\n"
                        "2. --decode    to translate file from binary "
                        "to assembly language. Binary file required.\n"
                        "3. --spu       to run process. Binary file required."
            );
    return 1;
}
