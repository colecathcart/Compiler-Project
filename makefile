golf: main.o scanner.o token.o logger.o parser.o ast.o
	g++ -Wall main.o scanner.o logger.o token.o parser.o ast.o -o golf

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

clean:
	rm *.o golf