CXX = g++
CXXFLAGS = -I $(HOME)/utilities/readwrite_cpp/ -I $(HOME)/include/
TARGET = pickup_b
OBJ = pickup_b.o
LIB_OBJ = $(HOME)/utilities/readwrite_cpp/readwrite.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LIB_OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)
