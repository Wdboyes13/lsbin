CPPFLAGS := -Iinclude -DNDEBUG

CXX ?= clang++
CXXFLAGS := $(CPPFLAGS) -fms-extensions -std=c++23 -DNDEBUG

OUT := lsbin

OBJ := src/elf32_main.o src/elf64_main.o src/macho_main.o \
	   src/pe_main.o src/main.o

$(OUT): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUT)
