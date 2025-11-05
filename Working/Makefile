OBJS = translate_coordinate.o LookingDirection.o Geocoordinate.o cross_point_at_altitude.o across_point_atmosphere.o solar_direction.o sensor_direction.o interface_stdin.o interface_atmosphere.o obsDateTime.o Observed.o execute.o read.o read_obs.o save.o get_msis.o read_config.o fit_readwrite.o fit_result.o squareerr.o mean.o Nair_to_atmosphere.o acquire_radiance.o set_stdin_direction.o wrapper.o msis_to_gm_e.o core.o
OBJ_PATHS = $(addprefix obj/, $(OBJS))

HEADERS = coordinate.h interface.h solar_direction.h execute.h obsDateTime.h Observed.h save.h get_msis.h read_config.h fit.h
HEADER_PATHS = $(addprefix ../headers/, $(HEADERS))

INC_DIR = /lhome/sano2/include ../headers wrapper
INC_PATHS = $(addprefix -I, $(INC_DIR))

LIB_DIR = /lhome/sano2/lib
LIB_PATHS = $(addprefix -L, $(LIB_DIR))
LIB_OPTS = -lnrlmsise00 -lnlopt -lm -Wl,-rpath,$(LIB_DIR)
OPTS = -Wall -O3

vpath %.cpp ../src:../src/Observed:../src/coordinate:../src/execute:../src/interface:../src/fit:wrapper

.PHONY: all clean

all: core

core: $(OBJ_PATHS)
	g++ $(OBJ_PATHS) -o $@ $(LIB_PATHS) $(LIB_OPTS)
# "$@"はターゲット（目的物）

obj/%.o: %.cpp
	g++ -c $< $(OPTS) -o $@ $(INC_PATHS)
# $(HEADERS)
# "$<"は、:(コロン)の一つ右

clean:
	rm obj/*.o	
