#include "headers/translator.h"
#include "headers/disassembler.h"
#include "headers/emulator.h"

int main (int /*argc*/, char** /*argv*/)
{
    // Translating from English to asm.
    file_input original_file = {};
    GetFileInput ("original.txt", &original_file, PARTED);

    FILE* translated_file = TranslateFile (&original_file);

    FreeFileInput (&original_file);
    fclose (translated_file);

    // Translating from asm to English.
    file_input tr_file = {};
    GetFileInput ("translated.txt", &tr_file, PARTED);

    FILE* decoded_file = DecodeFile (&tr_file);

    FreeFileInput (&tr_file);
    fclose (decoded_file);
//
//     // CPU
//     file_input asm_code = {};
//     GetFileInput ("translated.txt", &asm_code, PARTED);
//     EmulateCPU (&asm_code);
//
//     FreeFileInput (&asm_code);

    return 0;
}
