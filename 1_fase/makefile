PROJECT_NAME=campista

CC=gcc
FLAGS=-c -Wall -std=c99 -O3 #-O3 --> optimizes execution but might interfere with debug information

#extra debug flags:
#-fasynchronous-unwind-tables
#-Werror=format-security
#-Wformat-security
#-Werror=implicit-function-declaration
#-Wl,-z,defs
#-Wundef
#-Wshadow
#-Wcast-align
#-Wstrict-prototypes
#-Wswitch-default***
#-Wconversion***
#-Wunreachable-code
#-Wformat=2
#-Winit-self
#-Wuninitialized


#extra otimization flags:
#-flto
#-march=native --> produces optimized code for the chip it's running on, might be dangerous


# .c files
C_SOURCE=$(wildcard ./*.c)
# .h files
H_SOURCE=$(wildcard ./*.h)
# .o files
OBJ=$(subst .c,.o,$(C_SOURCE))

RM = rm -rf


FILES_A_IN = $(shell ls ./camps/A/*.camp0)
FILES_B_IN = $(shell ls ./camps/B/*.camp0)
FILES_C_IN = $(shell ls ./camps/C/*.camp0)
FILES_MIX_IN = $(shell ls ./camps/MIX/*.camp0)


#
# Compilation and linking
#
all: objFolder $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@
	@ echo 'Finished building binary: $@'
	@ echo ' '

./%.o: ./%.c ./%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(FLAGS) -o $@
	@ echo ' '


./campista.o: ./campista.c $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(FLAGS) -o $@
	@ echo ' '


objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./*.o $(PROJECT_NAME) *~
	@ rmdir objects

.PHONY: all clean

# test different modes
