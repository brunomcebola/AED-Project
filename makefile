PROJECT_NAME=projeto-aed

CC=gcc
FLAGS=-c -Wall -Wextra -ansi -pedantic -std=c99 -g -Og #-O3

# .c files
C_SOURCE=$(wildcard ./sources/*.c)
# .h files
H_SOURCE=$(wildcard ./headers/*.h)
# .o files
OBJ=$(subst .c,.o,$(subst sources,objects,$(C_SOURCE)))

RM = rm -rf


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
