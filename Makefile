headers = global.hpp lexer.hpp logger.hpp main.hpp symbol.hpp parser.hpp error.hpp
objects = error.o lexer.o main.o parser.o logger.o symbol.o emitter.o
flags = -Wall 

output: $(objects)
	g++ $(flags) $(objects) -o output 

parser.cpp parser.hpp: parser.y
	bison -d -o parser.cpp parser.y

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

lexer.o: lexer.cpp $(headers)
	g++ -c $(flags) lexer.cpp

parser.o: parser.cpp $(headers)
	g++ -c $(flags) parser.cpp

emitter.o: emitter.cpp $(headers)
	g++ -c $(flags) emitter.cpp

clean:
	rm *.o output lexer.cpp parser.hpp parser.cpp

.PHONY: clean
