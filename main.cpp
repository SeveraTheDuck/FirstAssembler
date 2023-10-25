#include "headers/constants.h"
#include "headers/translator.h"
#include "headers/disassembler.h"
#include "headers/SPU.h"

int main (int /*argc*/, char** /*argv*/)
{
// QUADRATIC EQUATION
    TranslateFile ("tests/square_original.txt", "tests/square_translated.txt");
    SPU_process   ("tests/square_translated.txt");
    DecodeFile    ("tests/square_translated.txt", "tests/square_decoded.txt");

// QUADRATIC EQUATION

// FIBONACCI
    // TranslateFile ("tests/fibonacci_original.txt", "tests/fibonacci_translated.txt");
    // DecodeFile    ("tests/fibonacci_translated.txt", "tests/fibonacci_decoded.txt");
    // SPU_process   ("tests/fibonacci_translated.txt");
// FIBONACCI END

// TEST
    // TranslateFile ("original.txt", "translated.txt");
    // SPU_process   ("translated.txt");
// TEST END
    return 0;
}
