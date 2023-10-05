#ifndef EMULATOR_H
#define EMULATOR_H

#include "../FileOpenLib/filestruct.h"
#include "../stack/headers/stack.h"

void EmulateCPU (file_input* const asm_code);

#endif
