chat: Des.o chat.o
	g++ -o chat Des.o chat.o

des:
	g++ -o des Des.cpp

chat.o: chat.cpp Des.h
	g++ -c chat.cpp

Des.o: Des.cpp
	g++ -c Des.cpp


clean:
	rm chat.o Des.o chat des
