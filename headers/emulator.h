#ifndef EMULATOR_H
#define EMULATOR_H

#include "../FileOpenLib/filestruct.h"
#include "../stack/headers/stack.h"
#include "translator.h"

void ProcessSPU (const char* const spu_file_name);

void Processor (file_input* const asm_code);

#endif
