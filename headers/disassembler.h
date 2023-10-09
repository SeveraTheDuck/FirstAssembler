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

void DecodePush (file_input* const translated_file,
                 FILE* const decoded_file,
                 const size_t n_line,
                 const int n_operation);

void DecodePop (file_input* const translated_file,
                FILE* const decoded_file,
                const size_t n_line,
                int n_operation);

#endif
