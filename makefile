
CC = g++
CFLAGS = -std=c++11 -O2

all: res

res: main.o sound.o wavelet.o cwt.o gnuplot_i.o
	$(CC) main.o sound.o wavelet.o cwt.o gnuplot_i.o -o res -fno-elide-constructors

main.o sound.o wavelet.o cwt.o gnuplot_i.o: main.cpp sound.cpp wavelet.cpp cwt.cpp gnuplot_i.cpp
	$(CC) -c main.cpp sound.cpp wavelet.cpp cwt.cpp gnuplot_i.cpp $(CFLAGS)




