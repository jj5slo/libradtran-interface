OBJS = fit_result.o fit_readwrite.o mean.o squareerr.o leastsquare.o apply_fitting.o running_mean.o smooth_trials.o
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
