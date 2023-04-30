.PHONY: all test clean g1 g2

CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

SRC = noise.cpp noise/noise1d.cpp noise/noise2d.cpp
TARGET = noise.so

all: $(TARGET)

test: $(TARGET)
	cp $(TARGET) ./test/$(TARGET)
	python3 -m pytest test/ -v

clean:
	rm -rf *.o *.out *.so __pycache__/ .pytest_cache/

g1:
	cp $(TARGET) ./test/$(TARGET)
	python3 test/test_1d.py

g2:
	cp $(TARGET) ./test/$(TARGET)
	python3 test/test_2d.py

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) `python3 -m pybind11 --includes` $(SRC) -o $(TARGET) `python3-config --includes --ldflags` ${MKL_LIBS}