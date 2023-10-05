#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdlib.h>
#include "../FileOpenLib/filestruct.h"
#include "translator.h"

/// @brief This function decodes file (translates from assembler language to human).
/// @param translated_file Pointer to struct with info about file on asm language.
/// @return Pointer to decoded file.
FILE* DecodeFile (file_input* const translated_file);

#endif
