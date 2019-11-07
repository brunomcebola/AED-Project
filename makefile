PROJECT_NAME=projeto-aed

CC=gcc
FLAGS=-c -Wall -Wextra -ansi -pedantic -std=c99 -g -Og #-O3 --> optimizes execution but might interfere with debug information

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
C_SOURCE=$(wildcard ./sources/*.c)
# .h files
H_SOURCE=$(wildcard ./headers/*.h)
# .o files
OBJ=$(subst .c,.o,$(subst sources,objects,$(C_SOURCE)))

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

./objects/%.o: ./sources/%.c ./headers/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(FLAGS) -o $@
	@ echo ' '


./objects/projeto_aed_intermedio.o: ./sources/projeto_aed_intermedio.c $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(FLAGS) -o $@
	@ echo ' '


objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJECT_NAME) *~
	@ rmdir objects

.PHONY: all clean

a:
	@ rm -r -f ./ans/A
	@ mkdir -p ./ans/A
	for F in ${FILES_A_IN}; do ./$(PROJECT_NAME) $${F} ; done
