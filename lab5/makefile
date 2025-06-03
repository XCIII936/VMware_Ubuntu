CXX = g++
CXXFLAGS = -Wall -g -std=c++11
TARGET = vmrp
SRC = vmrp.cc
HEADER = vmrp.h

all: $(TARGET)

$(TARGET): $(SRC) $(HEADER)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
