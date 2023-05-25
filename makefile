.PHONY: all test clean graph1 graph2 render

CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

SRC = noise.cpp noise/noise1d.cpp noise/noise2d.cpp noise/noise3d.cpp
TARGET = noise.so
RTARGET = render/noise.so
TTARGET = test/noise.so

# build noise library at root folder
all: $(TARGET)

# test library
test: $(TTARGET)
	python3 -m pytest test/ -v

# show 1D noise testing graph
graph1: $(TTARGET)
	python3 test/test_1d.py

# show 2D noise testing graph
graph2: $(TTARGET)
	python3 test/test_2d.py

# show 2D noise testing graph
graph3: $(TTARGET)
	python3 test/test_3d.py

# render a terrain
render: $(RTARGET)
	python3 render/render.py
	

clean:
	rm -rf *.o *.out *.so __pycache__/ .pytest_cache/

# build .so file at root folder
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) `python3 -m pybind11 --includes` $(SRC) -o $(TARGET) `python3-config --includes --ldflags`

# build .so file for 'render' folder
$(RTARGET): $(SRC)
	$(CC) $(CFLAGS) `python3 -m pybind11 --includes` $(SRC) -o $(RTARGET) `python3-config --includes --ldflags`

# build .so file for 'test' folder
$(TTARGET): $(SRC)
	$(CC) $(CFLAGS) `python3 -m pybind11 --includes` $(SRC) -o $(TTARGET) `python3-config --includes --ldflags`