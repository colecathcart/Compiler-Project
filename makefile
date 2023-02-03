golf: main.o scanner.o token.o
	g++ -Wall main.o scanner.o token.o -o golf

main.o: main.cpp
	g++ -Wall -c main.cpp

scanner.o: scanner.cpp
	g++ -Wall -c scanner.cpp

token.o: token.cpp
	g++ -Wall -c token.cpp

clean:
	rm *.o golf