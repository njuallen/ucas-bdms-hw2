/**
 * @file VerifyGraphColor.cc
 * @author  Zhigang Liu
 * @version 0.1
 *
 * @section LICENSE 
 * 
 * Copyright 2019 Zhigang Liu (liuzhigang@ict.ac.cn)
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
 * This file verifies the output of graph color algorithm.
 *
 */

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>

using std::vector;

vector<int> load_colors(char *out_file, int v0_id, int num_color) {

  vector<int> v;

  // there are four partitions in total
  for (int i = 1; i <= 4; i++) {
    char buf[1024];
    // construct output file name
    sprintf(buf, "%s_%d", out_file, i);
    printf("Read %s\n", buf);

    FILE *f = fopen(buf, "r");
    assert(f);

    char *line;
    while ((line = fgets(buf, 1000, f)) != NULL) {
      int vertexId, color;

      sscanf(line, "%d: %d", &vertexId, &color);

      // verify that color is legal
      if (vertexId == v0_id)
        assert(color == 0);
      assert(color >= 0 && color < num_color);

      if (vertexId >= v.size())
        v.resize(vertexId + 1);
      v[vertexId] = color;
    }
  }
  return v;
}

int check_color_conflict(vector<int> colors, char *in_file) {

  int totalNVertices = colors.size();
  int max_out_degree = -1;

  // there are four partitions in total
  for (int i = 1; i <= 4; i++) {

    // construct input file name
    char buf[1024];
    sprintf(buf, "%s_%d", in_file, i);
    printf("Read %s\n", buf);

    FILE *f = fopen(buf, "r");
    assert(f);

    int nVertices, nEdges;
    char *line = fgets(buf, 1000, f);
    sscanf(line, "%d", &nVertices);
    line = fgets(buf, 1000, f);
    sscanf(line, "%d", &nEdges);

    int last_vertex = -1;
    int out_degree = -1;

    for (int j = 0; j < nEdges; j++) {
      int from, to;

      line = fgets(buf, 1000, f);
      sscanf(line, "%d %d", &from, &to);

      // check that vertex ids are legal
      assert(from >= 0 && from < totalNVertices);
      assert(to >= 0 && to < totalNVertices);

      // check that colors do not conflict
      assert(colors[from] != colors[to]);

      // count the out degree of each node
      if (from != last_vertex) {
        if (out_degree > max_out_degree)
          max_out_degree = out_degree;
        last_vertex = from;
        out_degree = 1;
      } else
        out_degree += 1;
    }
  }
  return max_out_degree;
}

int main(int argc, char * argv[]) {
  if (argc != 5) {
    printf ("Usage: %s <input path> <output path> <v0 id> <num color>\n", argv[0]);
    exit(1);
  }
  char *in_path = argv[1];
  char *out_path = argv[2];

  int v0_id, num_color;
  sscanf(argv[3], "%d", &v0_id);
  sscanf(argv[4], "%d", &num_color);

  // debug
  printf("in_path: %s out_path: %s v0_id: %d num_color: %d\n",
      in_path, out_path, v0_id, num_color);

  vector<int> colors = load_colors(out_path, v0_id, num_color);
  int max_out_degree = check_color_conflict(colors, in_path);
  printf("nVertices: %d maxOutDegree: %d\n",
      (int)colors.size(), max_out_degree);
  return 0;
};
