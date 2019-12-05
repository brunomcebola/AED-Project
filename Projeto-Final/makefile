PROJECT_NAME=projeto-aed-final

CC=gcc
FLAGS= -c -g -Wall -Wextra -ansi -pedantic -std=c99 #-O3 --> optimizes execution but might interfere with debug information

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


FILES_A_IN = $(shell ls ./Tests/Boring/*.camp)
FILES_B_IN = $(shell ls ./Tests/Deserto/*.camp)
FILES_C_IN = $(shell ls ./Tests/Amazonia/*.camp)
FILES_MIX_IN = $(shell ls ./Tests/Enunciado/*.camp)
FILES_D_IN = $(shell ls ./Tests/Surpresa/*.camp)

VALG = valgrind --leak-check=full

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


./objects/projeto-aed-final.o: ./sources/projeto-aed-final.c $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(FLAGS) -o $@
	@ echo ' '


objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJECT_NAME) *~
	@ rmdir objects
	@ rm -r -f ./ans/Boring
	@ rm -r -f ./ans/Deserto
	@ rm -r -f ./ans/Amazonia
	@ rm -r -f ./ans/Enunciado
	@ rm -r -f ./ans/Surpresa

.PHONY: all clean

# test different modes

movea:
	for F in $(shell ls ./*.camp0); do mv $${F} ans/Boring ; done

moveb:
	for F in $(shell ls ./*.camp0); do mv $${F} ans/Deserto ; done

movec:
	for F in $(shell ls ./*.camp0); do mv $${F} ans/Amazonia ; done

movemix:
	for F in $(shell ls ./*.camp0); do mv $${F} ans/Enunciado ; done

moved:
	for F in $(shell ls ./*.camp0); do mv $${F} ans/Surpresa ; done

a:
	@ rm -r -f ./ans/Boring
	@ mkdir -p ./ans/Boring
	for F in ${FILES_A_IN}; do ./$(PROJECT_NAME) $${F} ; done
	$(MAKE) movea
	#@ diff -qiBbw -r ans/Boring Check/Boring

b:
	@ rm -r -f ./ans/Deserto
	@ mkdir -p ./ans/Deserto
	for F in ${FILES_B_IN}; do ./$(PROJECT_NAME) $${F} ; done
	$(MAKE) moveb
	#@ diff -qiBbw -r ans/Deserto Check/Deserto

c:
	@ rm -r -f ./ans/Amazonia
	@ mkdir -p ./ans/Amazonia
	for F in ${FILES_C_IN}; do ./$(PROJECT_NAME) $${F} ; done
	$(MAKE) movec
	#@ diff -qiBbw -r ans/Amazonia Check/Amazonia

m:
	@ rm -r -f ./ans/Enunciado
	@ mkdir -p ./ans/Enunciado
	for F in ${FILES_MIX_IN}; do ./$(PROJECT_NAME) $${F} ; done
	$(MAKE) movemix
	#@ diff -qiBbw -r ans/Enunciado Check/Enunciado

d:
	@ rm -r -f ./ans/Surpresa
	@ mkdir -p ./ans/Surpresa
	for F in ${FILES_D_IN}; do ./$(PROJECT_NAME) $${F} ; done
	$(MAKE) moved
	#@ diff -qiBbw -r ans/Surpresa Check/Surpresa

t:
	$(MAKE) a
	$(MAKE) b
	$(MAKE) c
	$(MAKE) m
	$(MAKE) d
