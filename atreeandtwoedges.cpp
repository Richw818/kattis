/**
 *    Author:  Richw818
 *    Created: 05.31.2023 20:16:23
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

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  vector<vector<int>> adj(n);
  dsu d(n);
  vector<pair<int, int>> saved;
  for (int i = 0; i <= n; i++) {
    int u, v;
    cin >> u >> v;
    u--, v--;
    if (d.unite(u, v)) {
      adj[u].push_back(v);
      adj[v].push_back(u);
    } else {
      saved.emplace_back(u, v);
    }
  }

  vector<bool> vis(n);
  auto markNodes = [&](int node, int dest, vector<bool>& vb, auto&& markNodes) -> bool {
    vis[node] = true;
    if (node == dest) {
      vb[node] = true;
      return true;
    }
    bool ret = false;
    for (int next : adj[node]) {
      if (!vis[next]) {
        ret = ret || markNodes(next, dest, vb, markNodes);
      }
    }
    vb[node] = ret;
    return ret;
  };
  vector<bool> c1(n), c2(n);
  markNodes(saved[0].first, saved[0].second, c1, markNodes);
  fill(vis.begin(), vis.end(), false);
  markNodes(saved[1].first, saved[1].second, c2, markNodes);

  fill(vis.begin(), vis.end(), false);
  adj[saved[0].first].push_back(saved[0].second);
  adj[saved[0].second].push_back(saved[0].first);
  adj[saved[1].first].push_back(saved[1].second);
  adj[saved[1].second].push_back(saved[1].first);
  vector<int> p(n);
  auto fillParent = [&](int node, int par, auto&& fillParent) -> void {
    vis[node] = true;
    p[node] = par;
    for (int next : adj[node]) {
      if (!vis[next] && !(c1[next] || c2[next])) {
        fillParent(next, par, fillParent);
      }
    }
  };
  for (int i = 0; i < n; i++) {
    if (c1[i] || c2[i]) {
      fillParent(i, i, fillParent);
    }
  }
  
  int inBoth = 0;
  for (int i = 0; i < n; i++) {
    inBoth += c1[i] && c2[i];
  }

  if (inBoth == 0) {
    vector<bool> atBorder(n);
    vector<int> path, curr;
    fill(vis.begin(), vis.end(), false);
    auto findPath = [&](int node, auto&& findPath) -> void {
      curr.push_back(node);
      if (c2[node]) {
        path = curr;
        return;
      }
      vis[node] = true;
      for (int next : adj[node]) {
        if (!vis[next] && !c1[next]) {
          findPath(next, findPath);
        }
      }
      curr.pop_back();
    }; 
    for (int i = 0; i < n; i++) {
      if (c1[i]) {
        findPath(i, findPath);
      }
    }
    atBorder[path[0]] = atBorder[path.back()] = true;
    for (int i = 0; i < q; i++) {
      int a, b;
      cin >> a >> b;
      a--, b--;
      a = p[a], b = p[b];
      if (a == b) {
        cout << 1 << '\n';
      } else if (atBorder[a] && atBorder[b]) {
        cout << 1 << '\n';
      } else if (atBorder[a] || atBorder[b]) {
        cout << 2 << '\n';
      } else if ((c1[a] && c1[b]) || (c2[a] && c2[b])) {
        cout << 2 << '\n';
      } else {
        cout << 4 << '\n';
      }
    }
  } else if (inBoth == 1) {
    for (int i = 0; i < q; i++) {
      int a, b;
      cin >> a >> b;
      a--, b--;
      a = p[a], b = p[b];
      bool ct1 = c1[a] && c2[a];
      bool ct2 = c1[b] && c2[b];
      if (a == b) {
        cout << 1 << '\n';
      } else if (ct1 || ct2) {
        cout << 2 << '\n';
      } else if ((c1[a] && c1[b]) || (c2[a] && c2[b])) {
        cout << 2 << '\n';
      } else {
        cout << 4 << '\n';
      }
    }
  } else {
    vector<bool> border(n);
    fill(vis.begin(), vis.end(), false);
    auto findBorder = [&](int node, auto&& findBorder) -> void {
      vis[node] = true;
      int cnt = 0;
      for (int next : adj[node]) {
        if (c1[next] || c2[next]) {
          cnt++;
          if (!vis[next]) {
            findBorder(next, findBorder);
          }
        }
      }
      border[node] = cnt >= 3;
    };
    for (int i = 0; i < n; i++) {
      if (c1[i] || c2[i]) {
        findBorder(i, findBorder);
      }
    }
    for (int i = 0; i < q; i++) {
      int a, b;
      cin >> a >> b;
      a--, b--;
      a = p[a], b = p[b];
      bool ct1 = c1[a] && c2[a];
      bool ct2 = c1[b] && c2[b];
      if (a == b) {
        cout << 1 << '\n';
      } else if ((border[a] || ct1) && (border[b] || ct2)) {
        cout << 3 << '\n';
      } else if (border[a] || border[b]) {
        cout << 3 << '\n';
      } else if (ct1 || ct2) {
        cout << 4 << '\n';
      } else if ((c1[a] && c1[b]) || (c2[a] && c2[b])) {
        cout << 3 << '\n';
      } else {
        cout << 4 << '\n';
      }
    }
  }
  return 0;
}
