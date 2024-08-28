# sorgente
SOURCES = $(wildcard *.c)

# file oggetto generati durante la compilazione
OBJECTS = $(SOURCES:.c=.o)

# compiler
CC = gcc

# flags di compilazione
CFLAGS = -Wall -Wextra -g -c

# file .c
TARGETS = $(SOURCES:.c=)

# default rule: compila
all: $(TARGETS)
%: %.c
	$(CC) -o $@ $^ 


# rule: combina i file .o
%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

# rule clean
clean:
	rm -f $(OBJECTS) $(TARGETS)
