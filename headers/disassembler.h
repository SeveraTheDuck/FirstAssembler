#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "constants.h"

const size_t MAX_TRANSLATED_FILE_SIZE = 0x1000;
const size_t MAX_WORD_LENGTH = 0x100;

void DecodeFile (const char* const translated_file_name,
                 const char* const decoded_file_name);

/// @brief This function decodes file (translates from assembler language to human).
/// @param translated_file Pointer to struct with info about file on asm language.
/// @return Pointer to decoded file.
void Decoder (const unsigned char* const spu_code,
              const size_t spu_code_length,
              const char* const decoded_file_name);

void WriteToFile (const char* const decoded_file_name,
                  const char* const decoded_file_array,
                  const int         decoded_file_index);

void PrintArgs (const unsigned char command,
                const unsigned char* const spu_code,
                size_t* const spu_code_index,
                char* const decoded_file_array,
                int* const decoded_file_index);

void PrintRegisterArgument (const unsigned char        command,
                            const unsigned char* const spu_code,
                                  size_t       * const spu_code_index,
                                  char         * const decoded_file_array,
                                  int          * const decoded_file_index,
                                  unsigned char* const reg_name);

void PrintNumericArgument  (const unsigned char        command,
                            const unsigned char* const spu_code,
                                  size_t       * const spu_code_index,
                                  char         * const decoded_file_array,
                                  int          * const decoded_file_index,
                                  double       * const float_argument);

void SetDisasmLabel (const int         label_argument,
                           char* const decoded_file_array,
                           int * const decoded_file_index);

#endif
