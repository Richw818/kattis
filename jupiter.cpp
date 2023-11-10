/**
 *    Author:  Richw818
 *    Created: 05.17.2023 23:31:40
**/

#include <bits/stdc++.h>
using namespace std;

struct FlowEdge {
  int v, u;
  long long cap, flow = 0;
  FlowEdge(int v, int u, long long cap) : v(v), u(u), cap(cap) {}
};

struct Dinic {
  const long long flow_inf = 1e18;
  vector<FlowEdge> edges;
  vector<vector<int>> adj;
  int n, m = 0;
  int s, t;
  vector<int> level, ptr;
  queue<int> q;

  Dinic(int n, int s, int t) : n(n), s(s), t(t) {
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
  int n, q, s;
  cin >> n >> q >> s;
  vector<int> feed(s), cap(q);
  for (int i = 0; i < s; i++) {
    cin >> feed[i];
    feed[i]--;
  }
  for (int i = 0; i < q; i++) {
    cin >> cap[i];
  }
  int curr = 1;
  vector<vector<int>> limiters(n, vector<int>(q));
  vector<vector<int>> ques(n, vector<int>(q));
  vector<int> down(n), end(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < q; j++) {
      limiters[i][j] = curr++;
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < q; j++) {
      ques[i][j] = curr++;
    }
  }
  for (int i = 0; i < n; i++) {
    down[i] = curr++;
  }
  for (int i = 0; i < n; i++) {
    end[i] = curr++;
  }
  Dinic d(curr + 1, 0, curr);
  long long totalData = 0;
  for (int i = 0; i < n; i++) {
    int D;
    cin >> D;
    long long data = 0;
    for (int j = 0; j < s; j++) {
      int a;
      cin >> a;
      int to = feed[j];
      d.add_edge(down[i], limiters[i][to], a);
      data += a;
    }
    d.add_edge(0, down[i], data);
    for (int j = 0; j < q; j++) {
      d.add_edge(ques[i][j], end[i], cap[j]);
      d.add_edge(limiters[i][j], ques[i][j], cap[j]);
      if (i < n - 1) {
        d.add_edge(ques[i][j], limiters[i + 1][j], cap[j]);
      }
    }
    d.add_edge(end[i], curr, D);
    totalData += data;
  }
  cout << (d.flow() == totalData ? "possible" : "impossible") << '\n';
  return 0;
}
