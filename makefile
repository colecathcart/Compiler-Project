golf: main.o scanner.o token.o logger.o parser.o ast.o checker.o symbol.o stable.o
	g++ -Wall main.o scanner.o logger.o token.o parser.o ast.o checker.o symbol.o stable.o -o golf

main.o: main.cpp
	g++ -Wall -c main.cpp

scanner.o: scanner.cpp
	g++ -Wall -c scanner.cpp

token.o: token.cpp
	g++ -Wall -c token.cpp

logger.o: logger.cpp
	g++ -Wall -c logger.cpp

parser.o: parser.cpp
	g++ -Wall -c parser.cpp

ast.o: ast.cpp
	g++ -Wall -c ast.cpp

checker.o: checker.cpp
	g++ -Wall -c checker.cpp

symbol.o: symbol.cpp
	g++ -Wall -c symbol.cpp

stable.o: stable.cpp
	g++ -Wall -c stable.cpp

clean:
	rm *.o golf