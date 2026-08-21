# Phase 0 build helper.
#
# Compile by hand at least a dozen times before using this. A build command
# that feels like magic is a gap that reappears as soon as nvcc shows up --
# and Module 07 is entirely about not letting that happen.
#
# Usage:
#   make FILE=01-cpp-basics/exercises/exercise-01.cpp        build one file
#   make run FILE=...                                        build and run
#   make san FILE=...                                        with AddressSanitizer
#   make ubsan FILE=...                                      with UBSanitizer
#   make opt FILE=...                                        -O2, for timing runs
#   make asm FILE=...                                        emit assembly
#   make clean

CXX      := g++
STD      := -std=c++17
WARN     := -Wall -Wextra
DEBUG    := -g -O0
BUILD    := build

# Strip directories and the extension to name the output binary.
NAME     := $(notdir $(basename $(FILE)))
OUT      := $(BUILD)/$(NAME)

.PHONY: all run san ubsan opt asm clean check

all: check
	@mkdir -p $(BUILD)
	$(CXX) $(STD) $(WARN) $(DEBUG) -o $(OUT) $(FILE)
	@echo "built $(OUT)"

run: all
	@echo "---"
	@./$(OUT)

# Catches out-of-bounds access, use-after-free and leaks, with line numbers.
# Use this by default for Modules 04 and 06.
san: check
	@mkdir -p $(BUILD)
	$(CXX) $(STD) $(WARN) $(DEBUG) -fsanitize=address -o $(OUT)-asan $(FILE)
	@echo "---"
	@./$(OUT)-asan

# Catches signed overflow, bad shifts, misaligned access.
ubsan: check
	@mkdir -p $(BUILD)
	$(CXX) $(STD) $(WARN) $(DEBUG) -fsanitize=undefined -o $(OUT)-ubsan $(FILE)
	@echo "---"
	@./$(OUT)-ubsan

# Never benchmark at -O0. It measures unoptimized code that nobody runs.
opt: check
	@mkdir -p $(BUILD)
	$(CXX) $(STD) $(WARN) -O2 -o $(OUT)-O2 $(FILE)
	@echo "built $(OUT)-O2"

# Compare -O0 and -O2 output. Rehearsal for reading PTX and SASS later.
asm: check
	@mkdir -p $(BUILD)
	$(CXX) $(STD) -S -O0 -o $(BUILD)/$(NAME)-O0.s $(FILE)
	$(CXX) $(STD) -S -O2 -o $(BUILD)/$(NAME)-O2.s $(FILE)
	@wc -l $(BUILD)/$(NAME)-O0.s $(BUILD)/$(NAME)-O2.s

check:
ifndef FILE
	$(error set FILE, e.g. make FILE=01-cpp-basics/exercises/exercise-01.cpp)
endif

clean:
	$(RM) -r $(BUILD)
