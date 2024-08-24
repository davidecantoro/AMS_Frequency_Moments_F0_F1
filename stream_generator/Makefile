# output
TARGET = generate_stream

# sorgente
SOURCES = $(wildcard *.cpp)

# file oggetto generati durante la compilazione
OBJECTS = $(SOURCES:.cpp=.o)

# compiler
CC = g++

# default rule: compila
default: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^

# rule: combina i file .o
%.o: %.cpp
	$(CC) -c -o $@ $<

# rule clean
clean:
	rm -f $(OBJECTS) $(TARGET)
