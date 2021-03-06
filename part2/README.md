# GraphLite Usage

  @see /home/bdms/setup/GraphLite-0.20/README.md


# Homework 2 Part 2 Requirements

Note: Please use only English in comments.
First line: /* group, studentId, nameInEnglish */

0. Group 0

   #define VERTEX_CLASS_NAME(name) SSSP##name

   command line:
   $ start-graphlite example/your_program.so <input path> <output path> <v0 id>

   input file:  fields are separated by a single space
                note: modify InputFormatter slightly to read the distance

     num_vertex_in_this_partition
     num_edge_in_this_partition
     src_vertex_id dest_vertex_id distance
     src_vertex_id dest_vertex_id distance
     ... ...

   output file: fields are separated by a single space

     vertexid: length
     vertexid: length
     ...
  
1. Group 1

   #define VERTEX_CLASS_NAME(name) KCore##name

   command line:
   $ start-graphlite example/your_program.so <input path> <output path> <K>

   input file: fields are separated by a single space

     num_vertex_in_this_partition
     num_edge_in_this_partition
     src_vertex_id dest_vertex_id
     src_vertex_id dest_vertex_id
     ... ...

   output file: fields are separated by a single space

     vertexid
     vertexid
     ...

2. Group 2

   #define VERTEX_CLASS_NAME(name) GraphColor##name

   command line:
   $ start-graphlite example/your_program.so <input path> <output path> <v0 id> <num color>

   input file: fields are separated by a single space

     num_vertex_in_this_partition
     num_edge_in_this_partition
     src_vertex_id dest_vertex_id
     src_vertex_id dest_vertex_id
     ... ...

   output file: fields are separated by a single space

     vertexid: colorid
     vertexid: colorid
     ...
  
3. Group 3

   #define VERTEX_CLASS_NAME(name) DirectedTriangleCount##name

   command line:
   $ start-graphlite example/your_program.so <input path> <output path>

   input file:

     num_vertex_in_this_partition
     num_edge_in_this_partition
     src_vertex_id dest_vertex_id
     src_vertex_id dest_vertex_id
     ... ...

   output file: fields are separated by a single space

     in: num_in
     out: num_out
     through: num_through
     cycle: num_cycle
  
------------------------------------------------------------
Homework 2 Part 2 Test Commands
------------------------------------------------------------

SSSP:
$ start-graphlite example/your_program.so ${GRAPHLITE_HOME}/part2-input/SSSP-graph0_4w ${GRAPHLITE_HOME}/out 0
$ start-graphlite example/your_program.so ${GRAPHLITE_HOME}/part2-input/SSSP-graph1_4w ${GRAPHLITE_HOME}/out 0

KCore:
$ start-graphlite example/your_program.so ${GRAPHLITE_HOME}/part2-input/KCore-graph0_4w ${GRAPHLITE_HOME}/out 7
$ start-graphlite example/your_program.so ${GRAPHLITE_HOME}/part2-input/KCore-graph1_4w ${GRAPHLITE_HOME}/out 8

Graph Coloring:
$ start-graphlite example/your_program.so ${GRAPHLITE_HOME}/part2-input/Color-graph0_4w ${GRAPHLITE_HOME}/out 0 5
$ start-graphlite example/your_program.so ${GRAPHLITE_HOME}/part2-input/Color-graph1_4w ${GRAPHLITE_HOME}/out 0 5

Triangle Counting:
$ start-graphlite example/your_program.so ${GRAPHLITE_HOME}/part2-input/Triangle-graph0_4w ${GRAPHLITE_HOME}/out
$ start-graphlite example/your_program.so ${GRAPHLITE_HOME}/part2-input/Triangle-graph1_4w ${GRAPHLITE_HOME}/out

------------------------------------------------------------
Undirected graph vs. directed graph
------------------------------------------------------------

The following is an example directed graph

  0 --> 1 --> 2 --> 3
        |          /|\
        |           |
        + --------- +

The input will look like the following:

  4
  4
  0 1
  1 2
  1 3
  2 3
  
Consider the following undirected graph, where each edge in
the above graph is now an undirected edge.

  0 --- 1 --- 2 --- 3
        |           | 
        |           |
        + --------- +

The input will look like the following:

  4
  8
  0 1
  1 0
  1 2
  1 3
  2 1
  2 3
  3 2
  3 1

Note that every undirected edge is shown as TWO directed edge.
So the vertex compute() method can receive messages from all
the neighbors in the original undirected graph, and can send
messages to all the neighbors in the original undirected graph.

