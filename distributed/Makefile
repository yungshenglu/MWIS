main: main.o MWIS.o
	g++ -o main main.o MWIS.o

main.o: main.cpp header.h
	g++ -c main.cpp

MWIS.o: MWIS.cpp MWIS.h header.h
	g++ -c MWIS.cpp

clean:
	rm main main.o MWIS.o