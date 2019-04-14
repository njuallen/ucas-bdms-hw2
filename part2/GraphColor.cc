/**
 * @file GraphColor.cc
 * @author  Zhigang Liu
 * @version 0.1
 *
 * @section LICENSE 
 * 
 * Copyright 2016 Zhigang Liu (liuzhigang@ict.ac.cn)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * @section DESCRIPTION
 * 
 * This file implements the GraphColor algorithm using graphlite API.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <set>

using std::set;


#include "GraphLite.h"

int v0_id = 0;
int num_color = 0;

#define VERTEX_CLASS_NAME(name) GraphColor##name

class VERTEX_CLASS_NAME(InputFormatter): public InputFormatter {
  public:
    int64_t getVertexNum() {
      unsigned long long n;
      sscanf(m_ptotal_vertex_line, "%lld", &n);
      m_total_vertex= n;
      return m_total_vertex;
    }
    int64_t getEdgeNum() {
      unsigned long long n;
      sscanf(m_ptotal_edge_line, "%lld", &n);
      m_total_edge= n;
      return m_total_edge;
    }
    int getVertexValueSize() {
      m_n_value_size = sizeof(int);
      return m_n_value_size;
    }
    int getEdgeValueSize() {
      m_e_value_size = sizeof(int);
      return m_e_value_size;
    }
    int getMessageValueSize() {
      m_m_value_size = sizeof(int);
      return m_m_value_size;
    }
    void loadGraph() {
      unsigned long long last_vertex;
      unsigned long long from;
      unsigned long long to;
      // in graph color, we do not use weight
      int weight = 0;

      // at the very beginning, every vertex has color -1
      int defaultColor = -1;

      int outdegree = 0;

      const char *line = getEdgeLine();

      sscanf(line, "%lld %lld", &from, &to);
      addEdge(from, to, &weight);

      last_vertex = from;
      ++outdegree;

      for (int64_t i = 1; i < m_total_edge; ++i) {
        line = getEdgeLine();

        sscanf(line, "%lld %lld", &from, &to);
        if (last_vertex != from) {
          addVertex(last_vertex, &defaultColor, outdegree);
          last_vertex = from;
          outdegree = 1;
        } else {
          ++outdegree;
        }
        addEdge(from, to, &weight);
      }
      addVertex(last_vertex, &defaultColor, outdegree);
    }
};

class VERTEX_CLASS_NAME(OutputFormatter): public OutputFormatter {
  public:
    void writeResult() {
      int64_t vid;
      int value;
      char s[1024];

      for (ResultIterator r_iter; ! r_iter.done(); r_iter.next() ) {
        r_iter.getIdValue(vid, &value);
        int n = sprintf(s, "%lld: %d\n", (unsigned long long)vid, value);
        writeNextResLine(s, n);
      }
    }
};

class VERTEX_CLASS_NAME(): public Vertex <int, int, int> {
  public:
    void compute(MessageIterator* pmsgs) {
      int64_t vertexId = getVertexId();
      int superStep = getSuperstep();
      int currColor = *mutableValue();
      int newColor = -1;
      bool colorChanged = false;

      // debug
      printf("step: %d vertex: %ld\n", superStep, vertexId);

      // in the first super step,
      // we let v0 has color 0
      // and all worker choose a different random number seed
      if (superStep == 0) {
        if (vertexId == v0_id) {
          // v0 id has color 0
          newColor = 0;
          colorChanged = true;
        }

        srand(time(NULL) + vertexId);
      }

      if (superStep > 0) {
        // in later steps
        // we first recv messages from last step
        // check whether our color is valid or collides with neighbours' colors
        set<int> neighbourColors;
        for ( ; ! pmsgs->done(); pmsgs->next() ) {
          neighbourColors.insert(pmsgs->getValue());
          // debug
          printf("recvMessage: step: %d vertex: %ld value: %d\n",
              superStep, vertexId, pmsgs->getValue());
        }

        // if we haven't been assigned a color
        // or our color collides with our neighbours' color
        // we need to randomly choose a new color
        if (currColor == -1 || neighbourColors.find(currColor) != neighbourColors.end()) {
          colorChanged = true;
          while (true) {
            int random = rand();
            // debug, see whether random numbers are really random
            printf("generateRandom: step: %d vertex: %ld value: %d\n",
                superStep, vertexId, random);
            newColor = random % num_color;
            // no conflicts exists, this color is OK
            if (neighbourColors.find(newColor) == neighbourColors.end())
              break;
          }
        }
      }

      // if our color changed, we needs to notify all our neighbours
      // and, if our color changed, we needs to vote for halt?
      if (colorChanged) {
        // debug
        printf("ChangeColor: step: %d vertex: %ld value: %d\n",
            superStep, vertexId, newColor);

        *mutableValue() = newColor;
        sendMessageToAllNeighbors(newColor);
      } else
        voteToHalt();
    }
};

class VERTEX_CLASS_NAME(Graph): public Graph {
  public:
    // argv[0]: GraphColor.so
    // argv[1]: <input path>
    // argv[2]: <output path>
    // argv[3]: <v0 id>
    // argv[4]: <num color>
    void init(int argc, char* argv[]) {
      setNumHosts(5);
      setHost(0, "localhost", 1411);
      setHost(1, "localhost", 1421);
      setHost(2, "localhost", 1431);
      setHost(3, "localhost", 1441);
      setHost(4, "localhost", 1451);

      if (argc != 5) {
        printf ("Usage: %s <input path> <output path> <v0 id> <num color>\n", argv[0]);
        exit(1);
      }

      m_pin_path = argv[1];
      m_pout_path = argv[2];
      sscanf(argv[3], "%d", &v0_id);
      sscanf(argv[4], "%d", &num_color);

      // debug
      printf ("in_path: %s out_path: %s v0_id: %d num_color: %d\n",
          argv[1], argv[2], v0_id, num_color);
    }
};

/* STOP: do not change the code below. */
extern "C" Graph* create_graph() {
  Graph* pgraph = new VERTEX_CLASS_NAME(Graph);

  pgraph->m_pin_formatter = new VERTEX_CLASS_NAME(InputFormatter);
  pgraph->m_pout_formatter = new VERTEX_CLASS_NAME(OutputFormatter);
  pgraph->m_pver_base = new VERTEX_CLASS_NAME();

  return pgraph;
}

extern "C" void destroy_graph(Graph* pobject) {
  delete ( VERTEX_CLASS_NAME()* )(pobject->m_pver_base);
  delete ( VERTEX_CLASS_NAME(OutputFormatter)* )(pobject->m_pout_formatter);
  delete ( VERTEX_CLASS_NAME(InputFormatter)* )(pobject->m_pin_formatter);
  delete ( VERTEX_CLASS_NAME(Graph)* )pobject;
}
