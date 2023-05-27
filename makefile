.PHONY: all test graph1 graph2 graph3 render clean

CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC

SRC = $(wildcard noise/*.cpp)
LIB = noise/noise.so
RLIB = render/$(notdir $(LIB))
TLIB = test/$(notdir $(LIB))

CLEAN_FILES = *.o *.so *.gif __pycache__/ .pytest_cache/

# build library
all: $(LIB)

# test library
test: $(TLIB)
	python3 -m pytest test/ -v

# show 1D noise testing graph
graph1: $(TLIB)
	python3 test/test_1d.py

# show 2D noise testing graph
graph2: $(TLIB)
	python3 test/test_2d.py

# show 3D noise testing graph
graph3: $(TLIB)
	python3 test/test_3d.py

# render a terrain
render: $(RLIB)
	python3 render/render.py
	
clean:
	rm -rf $(CLEAN_FILES)
	cd noise; rm -rf $(CLEAN_FILES)
	cd render; rm -rf $(CLEAN_FILES) heightmap.png colormap.png
	cd test; rm -rf $(CLEAN_FILES)

$(LIB): $(SRC)
	$(CC) $(CFLAGS) `python3 -m pybind11 --includes` $(SRC) -o $(LIB) `python3-config --includes --ldflags`

$(RLIB): $(LIB)
	cp $(LIB) $(RLIB)

$(TLIB): $(LIB)
	cp $(LIB) $(TLIB)