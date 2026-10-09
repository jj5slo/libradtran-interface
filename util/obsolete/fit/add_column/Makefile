OBJS = fit_result.o fit_readwrite.o mean.o squareerr.o apply_fitting.o add_column.o
HEADERS = fit.h

vpath %.cpp ../

all: main

main: $(OBJS)
	g++ $(OBJS) -o $@ -L/lhome/sano2/lib -lnlopt -lm

%.o: %.cpp
	g++ -c $< -Wall -O3 -o $@ -I/lhome/sano2/include

clean: 
	rm -f *.o
	rm main
