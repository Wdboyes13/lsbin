DEV ?= FALSE

ifeq ($(DEV),TRUE)
CPPFLAGS := \
	-Iinclude \
	-Iinclude/platform/windows \
	-Iinclude/platform/macos   \
	-Iinclude/platform/linux   \
	-DWINCROSS
else
CPPFLAGS := -Iinclude -DNDEBUG
endif

CXX ?= clang++
CXXFLAGS := $(CPPFLAGS) -fms-extensions -std=c++23

OUT := lsbin

OBJ := src/elf32_main.o src/elf64_main.o src/macho_main.o \
	   src/pe_main.o src/main.o

$(OUT): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUT)
