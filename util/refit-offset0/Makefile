OBJS = mean.o obtain_fitting_coefficient.o squareerr.o apply_fitting.o fit_readwrite.o leastsquare.o readwrite.o refit.o
OBJ_PATHS = $(addprefix ../../obj/, $(OBJS))

HEADERS = coordinate.h interface.h solar_direction.h execute.h obsDateTime.h Observed.h save.h get_msis.h read_config.h fit.h wrapper.h
HEADER_PATHS = $(addprefix ../../headers/, $(HEADERS))

INC_DIR = /lhome/sano2/include ../../headers
INC_PATHS = $(addprefix -I, $(INC_DIR))

LIB_DIR = /lhome/sano2/lib
LIB_PATHS = $(addprefix -L, $(LIB_DIR))
LIB_OPTS = -lnrlmsise00 -lnlopt -lm -Wl,-rpath,$(LIB_DIR)
OPTS = -Wall -O3

vpath %.cpp ../../src:../../src/Observed:../../src/coordinate:../../src/execute:../../src/interface:../../src/fit:../../src/wrapper:../../src/readwrite

.PHONY: all clean

all: refit

refit: $(OBJ_PATHS)
	g++ $(OBJ_PATHS) -o $@ $(LIB_PATHS) $(LIB_OPTS)
# "$@"はターゲット（目的物）

../../obj/%.o: %.cpp
	g++ -c $< $(OPTS) -o $@ $(INC_PATHS)
# $(HEADERS)
# "$<"は、:(コロン)の一つ右

clean:
	rm ../../obj/*.o	
