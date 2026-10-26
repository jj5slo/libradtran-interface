OBJS = linear_interpolation.o readwrite.o interface_atmosphere.o atmNair_to_temperature.o ylog_interpolation.o save_101km_atm.o
OBJ_PATHS = $(addprefix ../../obj/, $(OBJS))

HEADERS = interface.h atmNair_to_temperature.h
HEADER_PATHS = $(addprefix ../../headers/, $(HEADERS))

INC_DIR = /lhome/sano2/include ../../headers
INC_PATHS = $(addprefix -I, $(INC_DIR))

LIB_DIR = /lhome/sano2/lib
LIB_PATHS = $(addprefix -L, $(LIB_DIR))
LIB_OPTS = -lnrlmsise00 -lnlopt -lm -Wl,-rpath,$(LIB_DIR)
OPTS = -Wall -O3

vpath %.cpp ../../src:../../src/Observed:../../src/coordinate:../../src/execute:../../src/interface:../../src/fit:../../src/wrapper:../../src/readwrite:../../src/linear_interpolation

.PHONY: all clean

all: save_101km_atm

save_101km_atm: $(OBJ_PATHS)
	g++ $(OBJ_PATHS) -o $@ $(LIB_PATHS) $(LIB_OPTS)
# "$@"はターゲット（目的物）

../../obj/%.o: %.cpp
	g++ -c $< $(OPTS) -o $@ $(INC_PATHS)
# $(HEADERS)
# "$<"は、:(コロン)の一つ右

clean:
	rm ../../obj/*.o	
