CXX = g++
CXXFLAGS = -I $(HOME)/utilities/readwrite_cpp/ -I $(HOME)/utilities/linear_regression/ -I $(HOME)/include/
TARGET = calc_b
OBJ = calc_b.o
LIB_OBJ = $(HOME)/utilities/readwrite_cpp/readwrite.o $(HOME)/utilities/linear_regression/linear_regression.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LIB_OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)
