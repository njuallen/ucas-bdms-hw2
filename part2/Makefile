# ----------------------------------------------------------------------
# compiler options
# ----------------------------------------------------------------------

CXX= g++

CFLAGS_COMMON=-std=c++0x -g -O2 -Wall -I${HADOOP_HOME}/include -I${GRAPHLITE_HOME}/include
LIB_GRAPHALGO=-fPIC -shared

# ----------------------------------------------------------------------
# target
# ----------------------------------------------------------------------

all : example VerifyGraphColor

# ----------------------------------------------------------------------
# example graph algorithms
# ----------------------------------------------------------------------

EXAMPLE_ALGOS=GraphColor.so

example: ${EXAMPLE_ALGOS}

GraphColor.so : GraphColor.cc
	${CXX} ${CFLAGS_COMMON} $< ${LIB_GRAPHALGO} -o $@

VerifyGraphColor : VerifyGraphColor.cc
	${CXX} -std=c++0x -g -O2 -Wall $< -o $@

run: GraphColor.so
	start-graphlite GraphColor.so part2-input/Color-graph0_4w part2-output/Color-graph0 0 20
	./VerifyGraphColor part2-input/Color-graph0_4w part2-output/Color-graph0 0 20
	start-graphlite GraphColor.so part2-input/Color-graph1_4w part2-output/Color-graph1 0 20
	./VerifyGraphColor part2-input/Color-graph1_4w part2-output/Color-graph1 0 20

test: GraphColor.so
	start-graphlite GraphColor.so ${GRAPHLITE_HOME}/Input/tinygraph_4w part2-output/tinygraph 0 9
	./VerifyGraphColor ${GRAPHLITE_HOME}/Input/tinygraph_4w part2-output/tinygraph 0 9

# ----------------------------------------------------------------------
# clean up
# ----------------------------------------------------------------------

clean :
	rm -rf *.so
