OBJS = readwrite.o save_params.o interface_stdin.o interface_atmosphere.o atmNair_to_temperature.o filematch.o execute.o read.o smoothing.o LookingDirection.o across_point_atmosphere.o cross_point_at_altitude.o Geocoordinate.o sensor_direction.o translate_coordinate.o avoid_dupe.o shot_loop.o
OBJ_PATHS = $(addprefix ../../obj/, $(OBJS))

#HEADERS = interface.h atmNair_to_temperature.h filematch.h
#HEADER_PATHS = $(addprefix ../../headers/, $(HEADERS))

INC_DIR = /lhome/sano2/include ../../headers
INC_PATHS = $(addprefix -I, $(INC_DIR))

LIB_DIR = /lhome/sano2/lib
LIB_PATHS = $(addprefix -L, $(LIB_DIR))
LIB_OPTS = -lnrlmsise00 -lnlopt -lm -Wl,-rpath,$(LIB_DIR)
OPTS = -Wall -O3

vpath %.cpp ../../src:../../src/Observed:../../src/coordinate:../../src/execute:../../src/interface:../../src/fit:../../src/wrapper:../../src/readwrite:../../src/filematch

.PHONY: all clean

all: shot_loop

shot_loop: $(OBJ_PATHS)
	g++ $(OBJ_PATHS) -o $@ $(LIB_PATHS) $(LIB_OPTS)
# "$@"はターゲット（目的物）

../../obj/%.o: %.cpp
	g++ -c $< $(OPTS) -o $@ $(INC_PATHS)
# $(HEADERS)
# "$<"は、:(コロン)の一つ右

clean:
	rm ../../obj/*.o	
