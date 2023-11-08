/**
 *    Author:  Richw818
 *    Created: 10.13.2023 01:00:54
**/

#include <bits/stdc++.h>
using namespace std;

struct segtree {
  struct node {
    int l = 0, r = 0;
    array<int, 4> val;
    node(int _l = 0, int _r = 0) {
      l = _l, r = _r;
      val = {0, 0, 0, 0};
    }
  };
  int N = 1;
  vector<node> tree;
  segtree(int _n) {
    while(N < _n) N <<= 1;
    tree.resize(2 * N + 5);
    build();
  }
  void apply(array<int, 4>& a, int v, int c) {
    if (v > a[0]) {
      if (c != a[1]) {
        a[2] = a[0], a[3] = a[1];
        a[0] = v, a[1] = c;
      } else {
        a[0] = v;
      }
    } else if (v > a[2]) {
      if (c != a[1]) {
        a[2] = v, a[3] = c;
      }
    }
  }
  array<int, 4> comb(array<int, 4> a, array<int, 4> b) {
    array<int, 4> c = a;
    apply(c, b[0], b[1]);
    apply(c, b[2], b[3]);
    return c;
  }
  void build() {
    for (int i = 0; i < N; i++) tree[i + N] = node(i, i + 1);
    for (int i = N - 1; i > 0; i--) tree[i] = node(tree[2 * i].l, tree[2 * i + 1].r);
  }
  void update(int i, int v, int c) {
    i += N;
    apply(tree[i].val, v, c);
    while (i > 1) {
      i >>= 1;
      tree[i].val = comb(tree[2 * i].val, tree[2 * i + 1].val);
    }
  }
  array<int, 4> query(int v, int l, int r) {
    if(l <= tree[v].l && tree[v].r <= r) return tree[v].val;
    if(tree[v].l >= r || tree[v].r <= l) return {0, 0, 0, 0};
    return comb(query(2 * v, l, r), query(2 * v + 1, l, r));
  }
  array<int, 4> query(int l, int r) {
    return query(1, l, r);
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vector<int> c(n), s(n);
  for (int i = 0; i < n; i++) {
    cin >> c[i] >> s[i];
  }
  vector<int> sorted = s;
  sort(sorted.begin(), sorted.end());
  sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());
  int x = sorted.size();
  segtree st(x);
  vector<int> dp(n);
  for (int i = 0; i < n; i++) {
    int ind = lower_bound(sorted.begin(), sorted.end(), s[i]) - sorted.begin();
    auto best = st.query(0, ind);
    dp[i] = (best[1] != c[i] ? best[0] : best[2]) + 1;
    st.update(ind, dp[i], c[i]);
  }
  vector<int> ans;
  int start = max_element(dp.begin(), dp.end()) - dp.begin();
  while (true) {
    ans.push_back(start + 1);
    if (dp[start] == 1) {
      break;
    }
    for (int i = start - 1; i >= 0; i--) {
      if (dp[i] == dp[start] - 1 && s[i] < s[start] && c[i] != c[start]) {
        start = i;
        break;
      }
    }
  }
  reverse(ans.begin(), ans.end());
  int k = ans.size();
  cout << k << '\n';
  for (int i = 0; i < k; i++) {
    cout << ans[i] << " \n"[i == k - 1];
  }
  return 0;
}
