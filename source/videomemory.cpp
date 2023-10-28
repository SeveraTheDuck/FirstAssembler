#include "../headers/constants.h"

static const size_t WIDTH = 0x10;

void PrintMemory (const Processor_t* const ram_array)
{
    for (size_t i = 0; i < RAM_AMOUNT / WIDTH; ++i)
    {
        for (size_t j = 0; j < WIDTH; ++j)
        {
            if (*(ram_array + WIDTH * i + j))
            {
                printf ("* ");
            }
            else
            {
                printf (". ");
            }
        }

        printf ("\n");
    }
}
