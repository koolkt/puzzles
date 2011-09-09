#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <vector>


using namespace std;


struct Edge {
  int id;
  int tail, head;
  uint64_t cost;

  Edge(int i, int t, int h, uint64_t c) : id(i), tail(t), head(h), cost(c) { }
};

int n = 0;
vector<Edge> machines;

uint64_t min_cost = UINT64_MAX;
vector<int> best_sol;

uint64_t cost = 0;
vector<int> sol;

vector< vector<int> > neighbours;
vector< vector<bool> > used_edge;

vector<bool> visited;


vector< vector<bool> > useful_edge;
vector< vector<uint64_t> > dist;
vector< vector<int> > next;

vector< vector<bool> > got_path;

int getpath(int i, int j, bool return_front=false)
{
  if (!got_path[i][j]) {
    int k = next[i][j];
    if (k == 0)
      useful_edge[i][j] = true;
    else {
      int u = getpath(i, k);
      int v = getpath(k, j, true);
      useful_edge[u][k] = useful_edge[k][v] = true;
    }
    got_path[i][j] = true;
  }

  if (return_front)
    return i;
  return j;
}

inline void prune()
{
  dist.resize(n + 1, vector<uint64_t>(n + 1, UINT64_MAX));
  for (int i = 1; i <= n; ++i)
    dist[i][i] = 0;

  for (int i = 0; i < machines.size(); ++i)
    dist[machines[i].tail][machines[i].head] = machines[i].cost;

  next.resize(n + 1, vector<int>(n + 1));

  for (int k = 1; k <= n; ++k)
    for (int i = 1; i <= n; ++i)
      for (int j = 1; j <= n; ++j)
        if (dist[i][j] > dist[i][k] + dist[k][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
          next[i][j] = max(next[i][j], k);
        }

  got_path.resize(n + 1, vector<bool>(n + 1));

  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n; ++j)
      getpath(i, j);

  vector<Edge> useful_machines;
  for (int i = 0; i < machines.size(); ++i)
    if (useful_edge[machines[i].tail][machines[i].head])
      useful_machines.push_back(machines[i]);

  machines = useful_machines;
}

bool reachable(int u, int t)
{
  visited[u] = true;

  if (u == t)
    return true;

  for (int i = 0; i < neighbours[u].size(); ++i) {
    int v = neighbours[u][i];
    if (!visited[v] && used_edge[u][v] && reachable(v, t))
      return true;
  }
  return false;
}

inline bool strong()
{
  for (int i = 1; i <= n; ++i)
    for (int j = i + 1; j <= n; ++j) {
      fill(visited.begin(), visited.end(), false);
      if (!reachable(i, j))
        return false;
      fill(visited.begin(), visited.end(), false);
      if (!reachable(j, i))
        return false;
    }

  return true;
}

inline void generate(int i=0)
{
  if (cost > min_cost)
    return;

  if (strong()) {
    min_cost = cost;
    best_sol = sol;
    return;
  }

  if (i >= machines.size())
    return;

  cost += machines[i].cost;
  sol.push_back(machines[i].id);
  used_edge[machines[i].tail][machines[i].head] = true;
  generate(i + 1);
  cost -= machines[i].cost;
  sol.pop_back();
  used_edge[machines[i].tail][machines[i].head] = false;

  generate(i + 1);
}

int main(int argc, char **argv)
{
  FILE *f = fopen(argv[1], "r");
  char m[100], c1[100], c2[100];
  uint64_t p; 
  while (true) {
    m[0] = ' ';
    fscanf(f, "%s", m);
    if (m[0] != 'M')
      break;
    fscanf(f, "%s", c1);
    fscanf(f, "%s", c2);
    fscanf(f, "%llu", &p);
    int c1i = atoi(&c1[1]);
    int c2i = atoi(&c2[1]);
    machines.push_back(Edge(atoi(&m[1]), c1i, c2i, p));
    n = max(n, max(c1i, c2i));
  }
  fclose(f);

  if (machines.empty()) {
    printf("0\n");
    return 0;
  }

  neighbours.resize(n + 1);
  for (int i = 0; i < machines.size(); ++i)
    neighbours[machines[i].tail].push_back(machines[i].head);
  used_edge.resize(n + 1, vector<bool>(n + 1));
  visited.resize(n + 1);

  useful_edge.resize(n + 1, vector<bool>(n + 1));

  prune();

  generate();

  printf("%llu\n", min_cost);
  for (int i = 0; i < best_sol.size() - 1; ++i)
    printf("%d ", best_sol[i]);
  printf("%d\n", best_sol.back());

  return 0;
}
