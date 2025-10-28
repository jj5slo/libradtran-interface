OBJS = fit_result.o fit_readwrite.o mean.o squareerr.o main.o
HEADERS = fit.h

all: main

main: $(OBJS)
	g++ $(OBJS) -o $@ -L/lhome/sano2/lib -lnlopt -lm

%.o: %.cpp
	g++ -c $< -Wall -O3 -o $@ -I/lhome/sano2/include

clean: 
	rm -f *.o
	rm main
