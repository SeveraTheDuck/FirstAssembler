// #include "headers/disassembler.h"
//
// void DecodeFile (const char* const translated_file_name,
//                  const char* const decoded_file_name)
// {
//     assert (translated_file_name);
//     assert (decoded_file_name);
//
//     file_input translated_file = {};
//     GetFileInput (translated_file_name, &translated_file, PARTED);
//
//     FILE* decoded_file = fopen (decoded_file_name, "wb");
//     assert  (decoded_file);
//     Decoder (&translated_file, decoded_file);
//
//     FreeFileInput (&translated_file);
//     fclose (decoded_file);
// }
//
// FILE* Decoder (file_input* const translated_file,
//                FILE*       const decoded_file)
// {
//     assert (translated_file);
//     assert (decoded_file);
//
//     int n_operation = 0;
//
//     for (size_t n_line = 0;
//                 n_line < translated_file->number_of_lines;
//               ++n_line)
//     {
//         sscanf  (translated_file->lines_array[n_line].line,
//                  "%d", &n_operation);
//         fprintf (decoded_file, "%s", operations_array[n_operation]);
//
//         if (n_operation == ASM_PUSH)
//         {
//             DecodePush (translated_file, decoded_file,
//                         n_line, n_operation);
//         }
//
//         else if (n_operation == ASM_POP)
//         {
//             DecodePop (translated_file, decoded_file,
//                         n_line, n_operation);
//         }
//
//         fprintf (decoded_file, "\n");
//     }
//
//     return decoded_file;
// }
//
// void DecodePush (file_input* const translated_file,
//                  FILE* const decoded_file,
//                  const size_t n_line,
//                  int n_operation)
// {
//     assert (translated_file);
//     assert (decoded_file);
//
//     int push_regime = STANDART_REGIME;
//     int push_value  = 0;
//
//     sscanf (translated_file->lines_array[n_line].line,
//             "%d %d %d", &n_operation, &push_regime, &push_value);
//
//     if (push_regime == REGISTER_REGIME)
//     {
//         fprintf (decoded_file, " r%cx", push_value + 'a');
//     }
//     else
//     {
//         fprintf (decoded_file, " %d", push_value);
//     }
// }
//
// void DecodePop (file_input* const translated_file,
//                 FILE* const decoded_file,
//                 const size_t n_line,
//                 int n_operation)
// {
//     assert (translated_file);
//     assert (decoded_file);
//
//     int pop_value  = 0;
//
//     sscanf (translated_file->lines_array[n_line].line,
//             "%d %d", &n_operation, &pop_value);
//
//     fprintf (decoded_file, " r%cx", pop_value + 'a');
// }
