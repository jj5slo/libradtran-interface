OBJS = readwrite.o leastsquare.o fit_shot_loop.o
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

all: fit_shot_loop

fit_shot_loop: $(OBJ_PATHS)
	g++ $(OBJ_PATHS) -o $@ $(LIB_PATHS) $(LIB_OPTS)
# "$@"はターゲット（目的物）

../../obj/%.o: %.cpp
	g++ -c $< $(OPTS) -o $@ $(INC_PATHS)
# $(HEADERS)
# "$<"は、:(コロン)の一つ右

clean:
	rm ../../obj/*.o	
