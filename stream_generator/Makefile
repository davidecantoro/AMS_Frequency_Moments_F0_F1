# Nome del programma da generare
TARGET = generate_stream

# Lista dei file sorgente nella cartella corrente
SOURCES = $(wildcard *.cpp)

# Lista dei file oggetto generati durante la compilazione
OBJECTS = $(SOURCES:.cpp=.o)

# Compilatore
CC = g++

# Regola di default, compila il programma
default: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

# Regola generica per la combinazione dei file oggetto
%.o: %.cpp
	$(CC) -c -o $@ $<

# Pulisce i file oggetti e l'eseguibile 
clean:
	rm -f $(OBJECTS) $(TARGET)
