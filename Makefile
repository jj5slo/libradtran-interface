OBJS = translate_coordinate.o LookingDirection.o Geocoordinate.o cross_point_at_altitude.o across_point_atmosphere.o solar_direction.o sensor_direction.o interface_stdin.o obsDateTime.o Observed.o execute.o read.o read_obs.o main.o
HEADERS = coordinate.h interface.h solar_direction.h execute.h obsDateTime.h Observed.h
OPTS = -Wall -O3

.PHONY: all clean

all: main

main: $(OBJS)
	g++ $(OBJS) -o $@ -L/home/sano/lib -lsample
# "$@"はターゲット（目的物）
# ちゃんと全部はいっている

%.o: %.cpp
	g++ -c $< $(OPTS) -I/home/sano/include
# $(HEADERS)
# "$<"は、:(コロン)の一つ右

clean:
	rm *.o
