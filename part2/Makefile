# ----------------------------------------------------------------------
# compiler options
# ----------------------------------------------------------------------

CXX= g++

CFLAGS_COMMON=-std=c++0x -g -O2 -Wall -I${HADOOP_HOME}/include -I${GRAPHLITE_HOME}/include
LIB_GRAPHALGO=-fPIC -shared

# ----------------------------------------------------------------------
# target
# ----------------------------------------------------------------------

all : example

# ----------------------------------------------------------------------
# example graph algorithms
# ----------------------------------------------------------------------

EXAMPLE_ALGOS=GraphColor.so

example: ${EXAMPLE_ALGOS}

%.so : %.cc
	${CXX} ${CFLAGS_COMMON} $< ${LIB_GRAPHALGO} -o $@

GraphColor.so : GraphColor.cc

run: GraphColor.so
	start-graphlite GraphColor.so part2-input/Color-graph0_4w part2-output/Color-graph0 0 1000
	start-graphlite GraphColor.so part2-input/Color-graph1_4w part2-output/Color-graph1 0 1000

test: GraphColor.so
	start-graphlite GraphColor.so ${GRAPHLITE_HOME}/Input/tinygraph_4w part2-output/tinygraph 0 3

# ----------------------------------------------------------------------
# clean up
# ----------------------------------------------------------------------

clean :
	rm -rf *.so
