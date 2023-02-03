golf: main.o scanner.o token.o logger.o
	g++ -Wall main.o scanner.o logger.o token.o -o golf

main.o: main.cpp
	g++ -Wall -c main.cpp

scanner.o: scanner.cpp
	g++ -Wall -c scanner.cpp

token.o: token.cpp
	g++ -Wall -c token.cpp

logger.o: logger.cpp
	g++ -Wall -c logger.cpp

clean:
	rm *.o golf