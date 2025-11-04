OBJS = readwrite.o mean.o main.o
HEADERS = stat.h

all: main

main: $(OBJS)
	g++ $(OBJS) -o $@ -L/lhome/sano2/lib -lnlopt -lm

%.o: %.cpp
	g++ -c $< -Wall -g3 -o $@ -I/lhome/sano2/include

clean: 
	rm -f *.o
	rm main
