#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "translator.h"

void DecodeFile (const char* const translated_file_name,
                 const char* const decoded_file_name);

/// @brief This function decodes file (translates from assembler language to human).
/// @param translated_file Pointer to struct with info about file on asm language.
/// @return Pointer to decoded file.
FILE* Decoder (file_input* const translated_file,
               FILE*       const decoded_file);

#endif
