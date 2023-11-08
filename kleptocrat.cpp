/**
 *    Author:  Richw818
 *    Created: 11.02.2023 15:02:11
**/

#include <bits/stdc++.h>
using namespace std;

struct dsu {
  int N;
  vector<int> p, s;
  dsu(int _n) {
    N = _n + 5;
    p.resize(N); s.resize(N);
    iota(p.begin(), p.end(), 0);
    fill(s.begin(), s.end(), 1);
  }
  int size(int a) {
    return s[find(a)];
  }
  int find(int a) {
    if(p[a] == a) return a;
    p[a] = find(p[a]);
    return p[a];
  }
  bool unite(int a, int b) {
    a = find(a), b = find(b);
    if(a == b) return false;
    if(s[a] > s[b]) swap(a, b);
    s[b] += s[a];
    p[a] = b;
    return true;
  }
  bool same(int a, int b) {
    return find(a) == find(b);
  }
};

struct edge {
  int u, v;
  long long w;
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  const int LOG = 60;
  int n, m, q;
  cin >> n >> m >> q;
  vector<vector<pair<int, long long>>> adj(n);
  vector<edge> extra;
  dsu d(n);
  for (int i = 0; i < m; i++) {
    int u, v;
    long long w;
    cin >> u >> v >> w;
    u--, v--;
    if (d.unite(u, v)) {
      adj[u].emplace_back(v, w);
      adj[v].emplace_back(u, w);
    } else {
      extra.push_back({u, v, w});
    }
  } 
  vector<long long> x(n), basis(LOG);
  auto dfs = [&](int node, int p, auto&& dfs) -> void {
    for (auto [next, w] : adj[node]) {
      if (next != p) {
        x[next] = x[node] ^ w;
        dfs(next, node, dfs);
      }
    }
  };
  dfs(0, -1, dfs);
  auto insert = [&](long long v) -> void {
    for (int i = LOG - 1; i >= 0; i--) {
      if ((1LL << i) & v) {
        if (basis[i] == 0) {
          basis[i] = v;
          return;
        } else {
          v ^= basis[i];
        }
      }
    }
  };
  for (auto [u, v, w] : extra) {
    insert(x[u] ^ x[v] ^ w);
  }
  for (int i = 0; i < q; i++) {
    int u, v;
    cin >> u >> v;
    u--, v--;
    long long ans = x[u] ^ x[v];
    for (int b = LOG - 1; b >= 0; b--) {
      if ((1LL << b) & ans) {
        ans ^= basis[b];
      }
    }
    cout << ans << '\n';
  }
  return 0;
}
