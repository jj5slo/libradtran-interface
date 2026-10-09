OBJS = interface_atmosphere.o Nair_to_atm.o
OBJ_PATHS = $(addprefix ../../obj/, $(OBJS))

HEADERS = coordinate.h interface.h solar_direction.h execute.h obsDateTime.h Observed.h save.h get_msis.h read_config.h fit.h wrapper.h
HEADER_PATHS = $(addprefix ../../headers/, $(HEADERS))

INC_DIR = /home/sano/include /home/sano/.local/include ../../headers
INC_PATHS = $(addprefix -I, $(INC_DIR))

LIB_DIR = /home/sano/lib /home/sano/.local/lib
LIB_PATHS = $(addprefix -L, $(LIB_DIR))
comma := ,
RPATH_PATHS = $(addprefix -Wl$(comma)-rpath$(comma), $(LIB_DIR))
LIB_OPTS = -lnrlmsise00 -lnlopt -lm -lbayesopt $(RPATH_PATHS)
OPTS = -Wall -O3

vpath %.cpp ../../src:../../src/Observed:../../src/coordinate:../../src/execute:../../src/interface:../../src/fit:../../src/wrapper

.PHONY: all clean

all: Nair_to_atm

Nair_to_atm: $(OBJ_PATHS)
	g++ $(OBJ_PATHS) -o $@ $(LIB_PATHS) $(LIB_OPTS)
# "$@"はターゲット（目的物）

../../obj/%.o: %.cpp
	g++ -c $< $(OPTS) -o $@ $(INC_PATHS)
# $(HEADERS)
# "$<"は、:(コロン)の一つ右

clean:
	rm ../../obj/*.o	
