OBJS = readwrite.o discretize_spectrum.o
OBJ_PATHS = $(addprefix ../../obj/, $(OBJS))

HEADERS = readwrite.h 
HEADER_PATHS = $(addprefix ../../headers/, $(HEADERS))

INC_DIR = /home/sano/include headers /home/sano/.local/include ../../headers
INC_PATHS = $(addprefix -I, $(INC_DIR))

LIB_DIR = /home/sano/lib /home/sano/.local/lib
LIB_PATHS = $(addprefix -L, $(LIB_DIR))
comma := ,
RPATH_PATHS = $(addprefix -Wl$(comma)-rpath$(comma), $(LIB_DIR))
LIB_OPTS = -lnrlmsise00 -lnlopt -lm -lbayesopt $(RPATH_PATHS)
OPTS = -Wall -O3

vpath %.cpp ../../src:../../src/Observed:../../src/coordinate:../../src/execute:../../src/interface:../../src/fit:../../src/wrapper:../../src/golden_section_search:../../src/readwrite

.PHONY: all clean

all: discretize_spectrum

discretize_spectrum: $(OBJ_PATHS)
	g++ $(OBJ_PATHS) -o $@ $(LIB_PATHS) $(LIB_OPTS)
# "$@"はターゲット（目的物）

../../obj/%.o: %.cpp
	g++ -c $< $(OPTS) -o $@ $(INC_PATHS)
# $(HEADERS)
# "$<"は、:(コロン)の一つ右

clean:
	rm ../../obj/*.o	discretize_spectrum
