CC = g++
default: searchengine clean
searchengine: searchengine.o
	$(CC) -o searchengine searchengine.o
searchengine.o: searchengine.cpp header/searchengine.hpp
	$(CC) -c searchengine.cpp
clean:
	del *.o