/**
 *    Author:  Richw818
 *    Created: 05.16.2023 18:28:33
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
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<string> names(n), party(n);
    unordered_set<string> P, C;
    vector<vector<string>> partOf(n);
    for (int i = 0; i < n; i++) {
      cin >> names[i] >> party[i];
      P.insert(party[i]);
      int k;
      cin >> k;
      for (int j = 0; j < k; j++) {
        string c;
        cin >> c;
        C.insert(c);
        partOf[i].push_back(c);
      }
    }
    vector<string> parties(P.begin(), P.end());
    vector<string> clubs(C.begin(), C.end());
    unordered_map<string, int> clubID, partyID;
    int p = parties.size(), c = clubs.size();
    for (int i = 0; i < p; i++) {
      partyID[parties[i]] = i;
    }
    for (int i = 0; i < c; i++) {
      clubID[clubs[i]] = i;
    }
    int most = (c - 1) / 2;
    Dinic d(n + p + c + 2, 0, n + p + c + 1);
    for (int i = 0; i < p; i++) {
      d.add_edge(0, i + 1, most);
    }
    for (int i = 0; i < n; i++) {
      int pt = partyID[party[i]];
      d.add_edge(pt + 1, p + i + 1, 1);
      for (string clb : partOf[i]) {
        int cb = clubID[clb];
        d.add_edge(p + i + 1, p + n + cb + 1, 1);
      }
    }
    for (int i = 0; i < c; i++) {
      d.add_edge(p + n + i + 1, p + n + c + 1, 1);
    }
    long long can = d.flow();
    if (can != c) {
      cout << "Impossible.\n";
    } else {
      for (int i = 0; i < n; i++) {
        for (int id : d.adj[i + 1 + p]) {
          if (id % 2 == 0 && d.edges[id].flow > 0) {
            int to = d.edges[id].u;
            to -= (p + n + 1);
            cout << names[i] << ' ' << clubs[to] << '\n';
          }
        }
      }
    }
    cout << '\n';
  }
  return 0;
}
