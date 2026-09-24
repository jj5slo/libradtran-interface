OBJS = squareerr.o readwrite.o atmNair_to_temperature.o linear_interpolation.o interface_atmosphere.o temperature_fitting_by_top_pressure.o
OBJ_PATHS = $(addprefix ../../obj/, $(OBJS))

HEADERS = coordinate.h interface.h solar_direction.h execute.h obsDateTime.h Observed.h save.h get_msis.h read_config.h fit.h wrapper.h
HEADER_PATHS = $(addprefix ../../headers/, $(HEADERS))

INC_DIR = /lhome/sano2/include ../../headers
INC_PATHS = $(addprefix -I, $(INC_DIR))

LIB_DIR = /lhome/sano2/lib
LIB_PATHS = $(addprefix -L, $(LIB_DIR))
LIB_OPTS = -lnrlmsise00 -lnlopt -lm -Wl,-rpath,$(LIB_DIR)
OPTS = -Wall -O3

vpath %.cpp ../../src:../../src/Observed:../../src/coordinate:../../src/execute:../../src/interface:../../src/fit:../../src/wrapper

.PHONY: all clean

all: temperature_fitting_by_top_pressure

temperature_fitting_by_top_pressure: $(OBJ_PATHS)
	g++ $(OBJ_PATHS) -o $@ $(LIB_PATHS) $(LIB_OPTS)
# "$@"はターゲット（目的物）

../../obj/%.o: %.cpp
	g++ -c $< $(OPTS) -o $@ $(INC_PATHS)
# $(HEADERS)
# "$<"は、:(コロン)の一つ右

clean:
	rm ../../obj/*.o	
