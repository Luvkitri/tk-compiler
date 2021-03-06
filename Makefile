headers = global.hpp emitter.hpp lexer.hpp logger.hpp main.hpp symbol.hpp parser.hpp error.hpp 
objects = emitter.o error.o lexer.o main.o parser.o logger.o symbol.o 
flags = -Wall -g

output: $(objects)
	g++ $(flags) $(objects) -o output 

parser.cpp parser.hpp: parser.y
	bison -g -d -o parser.cpp parser.y

lexer.cpp: lexer.l $(headers)
	flex -o lexer.cpp lexer.l

main.o: main.cpp $(headers)
	g++ -c $(flags) main.cpp

logger.o: logger.cpp $(headers)
	g++ -c $(flags) logger.cpp

error.o: error.cpp $(headers)
	g++ -c $(flags) error.cpp

symbol.o: symbol.cpp $(headers)
	g++ -c $(flags) symbol.cpp

emitter.o: emitter.cpp $(headers)
	g++ -c $(flags) emitter.cpp

lexer.o: lexer.cpp $(headers)
	g++ -c $(flags) lexer.cpp

parser.o: parser.cpp $(headers)
	g++ -c $(flags) parser.cpp

clean:
	rm *.o output lexer.cpp parser.hpp parser.cpp

.PHONY: clean
