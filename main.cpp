#include "headers/translator.h"
#include "headers/disassembler.h"
#include "headers/SPU.h"

int main (int /*argc*/, char** /*argv*/)
{
// QUADRATIC EQUATION
    TranslateFile ("square_original.txt", "square_translated.txt");
    SPU_process   ("square_translated.txt");
// QUADRATIC EQUATION

// FIBONACCI
    // TranslateFile ("fibonacci_original.txt", "fibonacci_translated.txt");
    // SPU_process   ("fibonacci_translated.txt");
// FIBONACCI END

// TEST
    // TranslateFile ("original.txt", "translated.txt");
    // SPU_process   ("translated.txt");
// TEST END
    return 0;
}
