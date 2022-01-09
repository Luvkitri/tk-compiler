headers = global.hpp lexer.hpp logger.hpp main.hpp symbol.hpp parser.hpp error.hpp
objects = main.o logger.o lexer.o parser.o symbol.o error.o
flags = -Wall 

output: $(objects)
	g++ -o output $(objects)

parser.cpp parser.hpp: parser.y
	bison -d -o parser.cpp parser.y

lexer.cpp: lexer.l
	flex -o lexer.cpp lexer.l

main.o: main.cpp $(headers)
	g++ -c $(flags) main.cpp

logger.o: logger.cpp $(headers)
	g++ -c $(flags) logger.cpp

error.o: error.cpp $(headers)
	g++ -c $(flags) error.cpp

symbol.o: symbol.cpp $(headers)
	g++ -c $(flags) symbol.cpp

lexer.o: lexer.cpp lexer.hpp
	g++ -c $(flags) lexer.cpp

parser.o: parser.cpp $(headers)
	g++ -c $(flags) parser.cpp

.PHONY: clean

clean:
	rm *.o output lexer.cpp parser.hpp parser.cpp