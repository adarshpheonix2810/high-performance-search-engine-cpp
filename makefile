CC = g++

# Detect OS
ifeq ($(OS),Windows_NT)
	RM = del
	EXE_EXT = .exe
else
	RM = rm -f
	EXE_EXT =
endif

TARGET = searchengine$(EXE_EXT)

default: $(TARGET)

$(TARGET): searchengine.o
	$(CC) -o $(TARGET) searchengine.o

searchengine.o: searchengine.cpp header/searchengine.hpp
	$(CC) -c searchengine.cpp

clean:
	$(RM) searchengine.o $(TARGET)