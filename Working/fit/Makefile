OBJS = fit_result.o fit_readwrite.o mean.o leastsquare.o main.o
HEADERS = fit.h

main: $(OBJS)
	g++ $(OBJS) -o $@ -L/lhome/sano2/lib -lnlopt -lm

%.o: %.cpp
	g++ -c $< -Wall -O3 -g3 $@ -I/lhome/sano2/include

