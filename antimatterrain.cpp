/**
 *    Author:  Richw818
 *    Created: 05.26.2023 14:35:47
**/

#include <bits/stdc++.h>
using namespace std;

struct node {
  int l, r;
  pair<int, int> val;
  node(int _l = 0, int _r = 0) {
    l = _l, r = _r;
    val = {INT_MAX, -1};
  }
};

struct segtree {
  int N;
  vector<node> tree;
  segtree(int _N) {
    N = 1;
    while (N < _N) {
      N <<= 1;
    }
    tree.resize(2 * N);
    build();
  }
  void build() {
    for (int i = 0; i < N; i++) {
      tree[i + N] = node(i, i + 1);
    }
    for (int i = N - 1; i >= 1; i--) {
      tree[i] = node(tree[2 * i].l, tree[2 * i + 1].r);
    }
  }
  void update(int i, int v, int id) {
    i += N;
    tree[i].val = {v, id};
    while (i > 1) {
      i >>= 1;
      tree[i].val = comb(tree[2 * i].val, tree[2 * i + 1].val);
    }
  }
  pair<int, int> comb(pair<int, int> a, pair<int, int> b) {
    return min(a, b);
  }
  pair<int, int> query(int v, int l, int r) {
    if (tree[v].l >= r || tree[v].r <= l) {
      return {INT_MAX, -1};
    }
    if (l <= tree[v].l && tree[v].r <= r) {
      return tree[v].val;
    }
    return comb(query(2 * v, l, r), query(2 * v + 1, l, r));
  }
};  

struct event {
  int y, type, x1, x2, id;
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vector<event> events, drops;
  set<int> xv;
  for (int i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y;
    xv.insert(x);
    event e = {y, 0, x, -1, i};
    events.push_back(e);
    drops.push_back(e);
  }
  for (int i = 0; i < m; i++) {
    int x1, x2, y;
    cin >> x1 >> x2 >> y;
    xv.insert(x1);
    xv.insert(x2);
    events.push_back({y, 1, x1, x2, i});
  }
  vector<int> x(xv.begin(), xv.end());
  auto getIndex = [&](int i) -> int {
    return lower_bound(x.begin(), x.end(), i) - x.begin();
  };
  sort(events.begin(), events.end(), [](event& a, event& b) {
    if (a.y == b.y) {
      return a.type > b.type;
    }
    return a.y > b.y;
  });
  int k = x.size();
  segtree st(k);
  vector<int> ans(n);
  map<int, vector<pair<int, int>>> byX;
  for (event e : events) {
    if (e.type == 0) {
      byX[e.x1].emplace_back(e.y, e.id);
      int ind = getIndex(e.x1);
      st.update(ind, e.y, e.id);
    } else {
      int l = getIndex(e.x1), r = getIndex(e.x2) + 1;
      pair<int, int> smallest = st.query(1, l, r);
      if (smallest.first == INT_MAX) {
        continue;
      }
      while (true) {
        pair<int, int> cv = st.query(1, l, r);
        if (cv.first != smallest.first) {
          break;
        }
        ans[cv.second] = e.y;
        int tx = drops[cv.second].x1;
        int ind = getIndex(tx);
        st.update(ind, INT_MAX, -1);
        byX[tx].pop_back();
        if (!byX[tx].empty()) {
          st.update(ind, byX[tx].back().first, byX[tx].back().second);
        }
      }
    }
  }
  for (int i = 0; i < n; i++) {
    cout << ans[i] << '\n';
  }
  return 0;
}
