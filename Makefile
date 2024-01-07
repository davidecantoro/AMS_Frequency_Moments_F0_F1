# Nome del programma da generare
TARGET = eseguibile

# Lista dei file sorgente nella cartella corrente
SOURCES = $(wildcard *.c)

# Lista dei file oggetto generati durante la compilazione
OBJECTS = $(SOURCES:.c=.o)

# Compilatore
CC = gcc

# Opzioni di compilazione
CFLAGS = -Wall -Wextra -g -c

# Regola di default, compila il programma
default: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

# Regola generica per la combinazione dei file oggetto
%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Pulisce i file oggetti e l'eseguibile 
clean:
	rm -f $(OBJECTS) $(TARGET)
