/**
 *    Author:  Richw818
 *    Created: 02.26.2024 13:16:44
**/

#include <bits/stdc++.h>
using namespace std;

struct segtree {
  struct node {
    int l = 0, r = 0;
    vector<int> val;
    node(int _l = 0, int _r = 0) {
      l = _l, r = _r;
    }
  };
  int N = 1;
  vector<node> tree;
  segtree(int _n) {
    while(N < _n) N <<= 1;
    tree.resize(2 * N + 5);
    build();
  }
  vector<int> comb(vector<int> a, vector<int> b) {
    vector<int> c((int) a.size() + (int) b.size());
    int p = 0, ind = 0;
    for (int i = 0; i < (int) a.size(); i++) {
      while (p < (int) b.size() && b[p] > a[i]) {
        c[ind++] = b[p++];
      }
      c[ind++] = a[i];
    }
    for (; p < (int) b.size(); p++) {
      c[ind++] = b[p];
    }
    if ((int) c.size() > 4) {
      c.resize(4);
    }
    return c;
  }
  void build() {
    for (int i = 0; i < N; i++) tree[i + N] = node(i, i + 1);
    for (int i = N - 1; i > 0; i--) tree[i] = node(tree[2 * i].l, tree[2 * i + 1].r);
  }
  void update(int i, int v) {
    i += N;
    tree[i].val = {v};
    while (i > 1) {
      i >>= 1;
      tree[i].val = comb(tree[2 * i].val, tree[2 * i + 1].val);
    }
  }
  vector<int> query(int v, int l, int r) {
    if(l <= tree[v].l && tree[v].r <= r) return tree[v].val;
    if(tree[v].l >= r || tree[v].r <= l) return {};
    return comb(query(2 * v, l, r), query(2 * v + 1, l, r));
  }
  vector<int> query(int l, int r) {
    return query(1, l, r);
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  const int LOG = 30;
  int n, k;
  cin >> n >> k;
  vector<int> x(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> x[i];
  }
  vector<int> coords = x;
  sort(coords.begin(), coords.end());
  auto getInd = [&](int pos) -> int {
    return lower_bound(coords.begin(), coords.end(), pos) - coords.begin();
  };
  vector<int> val(n + 1);
  for (int i = 1; i <= n; i++) {
    val[getInd(x[i])] = i;
  }
  segtree st(n + 1);
  for (int i = 0; i <= n; i++) {
    st.update(i, val[i]);
  }
  vector<int> ans(n + 1, -1);
  vector<vector<int>> dist(LOG, vector<int>(n + 1, -1));
  dist[1][getInd(0)] = 0;
  queue<pair<int, int>> q;
  q.emplace(1, getInd(0));
  while (!q.empty()) {
    auto [zoom, id] = q.front();
    q.pop();
    int want = val[id];
    if (ans[val[id]] == -1) {
      if (zoom == 0) {
        ans[want] = dist[zoom][id];
      } else {
        int l = coords[id] - (1 << (zoom - 1));
        int r = coords[id] + (1 << (zoom - 1));
        l = lower_bound(coords.begin(), coords.end(), l) - coords.begin();
        r = upper_bound(coords.begin(), coords.end(), r) - coords.begin();
        vector<int> in = st.query(l, r);
        if ((int) in.size() > k) {
          in.resize(k);
        }
        if (find(in.begin(), in.end(), want) != in.end()) {
          ans[want] = dist[zoom][id];
        }
      }
    }
    if (zoom + 1 < LOG && dist[zoom + 1][id] == -1) {
      dist[zoom + 1][id] = dist[zoom][id] + 1;
      q.emplace(zoom + 1, id);
    }
    if (zoom > 0 && dist[zoom - 1][id] == -1) {
      dist[zoom - 1][id] = dist[zoom][id] + 1;
      q.emplace(zoom - 1, id);
    } 
    if (zoom > 0) {
      int l = coords[id] - (1 << (zoom - 1));
      int r = coords[id] + (1 << (zoom - 1));
      l = lower_bound(coords.begin(), coords.end(), l) - coords.begin();
      r = upper_bound(coords.begin(), coords.end(), r) - coords.begin();
      vector<int> see = st.query(l, r);
      for (int i = 0; i < min(k, (int) see.size()); i++) {
        int nind = getInd(x[see[i]]);
        if (dist[zoom][nind] == -1) {
          dist[zoom][nind] = dist[zoom][id] + 1;
          q.emplace(zoom, nind);
        }
      }
    }
  }
  for (int i = 1; i <= n; i++) {
    cout << ans[i] << '\n';
  }
  return 0;
}
