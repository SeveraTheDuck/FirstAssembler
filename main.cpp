#include "headers/translator.h"
#include "headers/disassembler.h"
#include "headers/emulator.h"

int main (int /*argc*/, char** /*argv*/)
{
    // Translating from English to asm.
    TranslateFile ("original.txt", "translated.txt");

    // Translating from asm to English.
    DecodeFile ("translated.txt", "decoded.txt");

    // SPU
    ProcessSPU ("translated.txt");

    return 0;
}
