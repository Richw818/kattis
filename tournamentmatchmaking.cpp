/**
 *    Author:  Richw818
 *    Created: 02.25.2024 19:41:48
**/

#include <bits/stdc++.h>
using namespace std;

struct FlowEdge {
  int v, u;
  long long cap, flow = 0;
  FlowEdge(int _v, int _u, long long _cap) : v(_v), u(_u), cap(_cap) {}
};

struct Dinic {
  const long long flow_inf = 1e18;
  vector<FlowEdge> edges;
  vector<vector<int>> adj;
  int n, m = 0;
  int s, t;
  vector<int> level, ptr;
  queue<int> q;

  Dinic(int _n, int _s, int _t) : n(_n), s(_s), t(_t) {
    adj.resize(n);
    level.resize(n);
    ptr.resize(n);
  }

  void add_edge(int v, int u, long long cap) {
    edges.emplace_back(v, u, cap);
    edges.emplace_back(u, v, 0);
    adj[v].push_back(m);
    adj[u].push_back(m + 1);
    m += 2;
  }

  bool bfs() {
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int id : adj[v]) {
        if (edges[id].cap - edges[id].flow < 1)
          continue;
        if (level[edges[id].u] != -1)
          continue;
        level[edges[id].u] = level[v] + 1;
        q.push(edges[id].u);
      }
    }
    return level[t] != -1;
  }

  long long dfs(int v, long long pushed) {
    if (pushed == 0)
      return 0;
    if (v == t)
      return pushed;
    for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
      int id = adj[v][cid];
      int u = edges[id].u;
      if (level[v] + 1 != level[u] || edges[id].cap - edges[id].flow < 1)
        continue;
      long long tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
      if (tr == 0)
        continue;
      edges[id].flow += tr;
      edges[id ^ 1].flow -= tr;
      return tr;
    }
    return 0;
  }

  long long flow() {
    long long f = 0;
    while (true) {
      fill(level.begin(), level.end(), -1);
      level[s] = 0;
      q.push(s);
      if (!bfs())
        break;
      fill(ptr.begin(), ptr.end(), 0);
      while (long long pushed = dfs(s, flow_inf)) {
        f += pushed;
      }
    }
    return f;
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<vector<pair<int, int>>> groups(n);
  vector<int> count(15);
  for (int i = 0; i < n; i++) {
    int k;
    cin >> k;
    count[k]++;
    for (int j = 0; j < k; j++) {
      int a, b;
      cin >> a >> b;
      a--, b--;
      groups[i].emplace_back(a, b);
    }
  }
  vector<pair<int, int>> edges;
  auto check = [&](int i, int j) -> void {
    Dinic d(34, 30, 33);
    d.add_edge(30, 31, (int) groups[i].size());
    d.add_edge(30, 32, (int) groups[j].size());
    int curr = 0;
    for (auto [a, b] : groups[i]) {
      d.add_edge(31, curr, 1);
      d.add_edge(curr, 15 + a, 1);
      d.add_edge(curr, 15 + b, 1);
      curr++;
    }
    for (auto [a, b] : groups[j]) {
      d.add_edge(32, curr, 1);
      d.add_edge(curr, 15 + a, 1);
      d.add_edge(curr, 15 + b, 1);
      curr++;
    }
    for (int i = 0; i < 15; i++) {
      d.add_edge(15 + i, 33, 1);
    }
    if (d.flow() == 15) {
      edges.emplace_back(i, j);
    }
  };
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if ((int) groups[i].size() + (int) groups[j].size() != 15) {
        continue;
      }
      check(i, j);
    }
  }
  Dinic d(2 * n + 2, 2 * n, 2 * n + 1);
  for (int i = 0; i < n; i++) {
    if ((int) groups[i].size() % 2) {
      d.add_edge(2 * n, i, 1);
    } else {
      d.add_edge(n + i, 2 * n + 1, 1);
    }
  }
  for (auto [i, j] : edges) {
    if ((int) groups[j].size() % 2) {
      swap(i, j);
    }
    d.add_edge(i, n + j, 1);
  }
  cout << d.flow() << '\n';
  return 0;
}
