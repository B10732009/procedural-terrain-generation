.PHONY: all clean

CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

SRC = noise.cpp noise/noise1d.cpp
TARGET = noise.so

all: $(TARGET)

test: $(TARGET)
	python3 -m pytest -v

clean:
	rm -rf *.o *.out *.so __pycache__/ .pytest_cache/

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) `python3 -m pybind11 --includes` $(SRC) -o $(TARGET) `python3-config --includes --ldflags` ${MKL_LIBS}