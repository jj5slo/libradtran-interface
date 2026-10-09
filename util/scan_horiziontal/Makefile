OBJS = Geocoordinate.o LookingDirection.o translate_coordinate.o readwrite.o scan_horizontal.o
OBJ_PATHS = $(addprefix ../../obj/, $(OBJS))

HEADERS = coordinate.h readwrite.h FixedParams.h
HEADER_PATHS = $(addprefix ../../headers/, $(HEADERS))

INC_DIR = /home/sano/include ../../headers /home/sano/.local/include
INC_PATHS = $(addprefix -I, $(INC_DIR))

LIB_DIR = /home/sano/lib /home/sano/.local/lib
LIB_PATHS = $(addprefix -L, $(LIB_DIR))
comma := ,
RPATH_PATHS = $(addprefix -Wl$(comma)-rpath$(comma), $(LIB_DIR))
LIB_OPTS = -lnrlmsise00 -lnlopt -lm -lbayesopt $(RPATH_PATHS)
OPTS = -Wall -O3

vpath %.cpp ../../src:../../src/Observed:../../src/coordinate:../../src/execute:../../src/interface:../../src/fit:../../src/wrapper:../../src/readwrite

.PHONY: all clean

all: scan_horizontal

scan_horizontal: $(OBJ_PATHS)
	g++ $(OBJ_PATHS) -o $@ $(LIB_PATHS) $(LIB_OPTS)
# "$@"はターゲット（目的物）

../../obj/%.o: %.cpp
	g++ -c $< $(OPTS) -o $@ $(INC_PATHS)
# $(HEADERS)
# "$<"は、:(コロン)の一つ右

clean:
	rm ../../obj/*.o	
