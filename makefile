CC=g++
HEADERS=headers/
FLAGS=-D_DEBUG -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -fsanitize=address,alignment -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla
RUN_DIR=executers/
BIN_DIR:=object/
SOURCE_DIR=source/

run_program: $(BIN_DIR)main.o
	@$(CC) $(FLAGS) $(BIN_DIR)main.o -o $@

run_translation: $(BIN_DIR)main_translator.o $(BIN_DIR)translator.o $(BIN_DIR)filestruct.o $(HEADERS)commands.h $(HEADERS)constants.h $(HEADERS)translator.h config.h
	@$(CC) $(FLAGS) $(BIN_DIR)main_translator.o $(BIN_DIR)translator.o $(BIN_DIR)filestruct.o -o $(RUN_DIR)$@

run_decoding: $(BIN_DIR)main_decoding.o $(BIN_DIR)disassembler.o $(HEADERS)commands.h $(HEADERS)constants.h $(HEADERS)disassembler.h config.h
	@$(CC) $(FLAGS) $(BIN_DIR)main_decoding.o $(BIN_DIR)disassembler.o -o $(RUN_DIR)$@

run_processor: $(BIN_DIR)main_processor.o $(BIN_DIR)SPU.o $(BIN_DIR)videomemory.o $(BIN_DIR)stack.o $(BIN_DIR)hash.o $(BIN_DIR)errors.o $(HEADERS)commands.h $(HEADERS)constants.h $(HEADERS)SPU.h config.h
	@$(CC) $(FLAGS) $(BIN_DIR)main_processor.o $(BIN_DIR)SPU.o $(BIN_DIR)videomemory.o $(BIN_DIR)stack.o $(BIN_DIR)errors.o $(BIN_DIR)hash.o -o $(RUN_DIR)$@

$(BIN_DIR)main.o: $(SOURCE_DIR)main.cpp
	@$(CC) $(FLAGS) $(SOURCE_DIR)main.cpp -c -o $(BIN_DIR)main.o

$(BIN_DIR)main_translator.o: $(SOURCE_DIR)main_translator.cpp
	@$(CC) $(FLAGS) $(SOURCE_DIR)main_translator.cpp -c -o $(BIN_DIR)main_translator.o

$(BIN_DIR)main_decoding.o: $(SOURCE_DIR)main_decoding.cpp
	@$(CC) $(FLAGS) $(SOURCE_DIR)main_decoding.cpp -c -o $(BIN_DIR)main_decoding.o

$(BIN_DIR)main_processor.o: $(SOURCE_DIR)main_processor.cpp
	@$(CC) $(FLAGS) $(SOURCE_DIR)main_processor.cpp -c -o $(BIN_DIR)main_processor.o

$(BIN_DIR)translator.o: $(SOURCE_DIR)translator.cpp $(HEADERS)translator.h $(HEADERS)constants.h $(HEADERS)commands.h $(HEADERS)jumps.h
	@$(CC) $(FLAGS) $(SOURCE_DIR)translator.cpp -c -o $(BIN_DIR)translator.o

$(BIN_DIR)disassembler.o: $(SOURCE_DIR)disassembler.cpp $(HEADERS)disassembler.h $(HEADERS)constants.h $(HEADERS)commands.h $(HEADERS)jumps.h
	@$(CC) $(FLAGS) $(SOURCE_DIR)disassembler.cpp -c -o $(BIN_DIR)disassembler.o

$(BIN_DIR)SPU.o: $(SOURCE_DIR)SPU.cpp $(SOURCE_DIR)videomemory.cpp $(HEADERS)SPU.h $(HEADERS)constants.h $(HEADERS)commands.h $(HEADERS)jumps.h $(HEADERS)videomemory.h
	@$(CC) $(FLAGS) $(SOURCE_DIR)SPU.cpp -c -o $(BIN_DIR)SPU.o

$(BIN_DIR)videomemory.o: $(SOURCE_DIR)videomemory.cpp $(HEADERS)videomemory.h
	@$(CC) $(FLAGS) $(SOURCE_DIR)videomemory.cpp -c -o $(BIN_DIR)videomemory.o

$(BIN_DIR)filestruct.o: FileOpenLib/filestruct.cpp
	@$(CC) $(FLAGS) FileOpenLib/filestruct.cpp -c -o $(BIN_DIR)filestruct.o

$(BIN_DIR)stack.o: stack/stack.cpp
	@$(CC) $(FLAGS) stack/stack.cpp -c -o $(BIN_DIR)stack.o

$(BIN_DIR)hash.o: stack/hash.cpp
	@$(CC) $(FLAGS) stack/hash.cpp -c -o $(BIN_DIR)hash.o

$(BIN_DIR)errors.o: stack/errors.cpp
	@$(CC) $(FLAGS) stack/errors.cpp -c -o $(BIN_DIR)errors.o

.PHONY: makedirs clean doxygen

makedirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(RUN_DIR)

clean:
	rm -rf $(BIN_DIR)
	mkdir -p $(BIN_DIR)

doxygen:
	doxygen ./doxygen
