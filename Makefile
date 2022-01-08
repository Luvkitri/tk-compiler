headers = global.hpp lexer.hpp logger.hpp main.hpp
objects = main.o logger.o

output: $(objects)
	g++ -o output $(objects)

main.o: main.cpp $(headers)
	g++ -c -Wall main.cpp

logger.o: logger.cpp $(headers)
	g++ -c -Wall logger.cpp

.PHONY: clean

clean:
	rm *.o output