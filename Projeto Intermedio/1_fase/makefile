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



#
# Compilation and linking
#
all: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJ)
	$(CC) $^ -o $@

./%.o: ./%.c ./%.h
	$(CC) $< $(FLAGS) -o $@


./campista.o: ./campista.c $(H_SOURCE)
	$(CC) $< $(FLAGS) -o $@


clean:
	@ $(RM) ./*.o $(PROJECT_NAME) *~

.PHONY: all clean
