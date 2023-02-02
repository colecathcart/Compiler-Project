golf: main.o scanner.o
	g++ -Wall main.o scanner.o -o golf

main.o: main.cpp
	g++ -Wall -c main.cpp

scanner.o: scanner.cpp
	g++ -Wall -c scanner.cpp

clean:
	rm *.o golf