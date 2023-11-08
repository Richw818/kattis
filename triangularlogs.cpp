/**
 *    Author:  Richw818
 *    Created: 09.29.2023 21:47:33
**/

#include <bits/stdc++.h>
using namespace std;

struct tree {
  int x, y, h;
};

struct node {
  int l, r;
  vector<pair<int, int>> vals;
  node(int _l = 0, int _r = 0) {
    l = _l, r = _r;
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  vector<tree> trees;
  vector<int> sorted;
  for (int i = 0; i < n; i++) {
    int x, y, h;
    cin >> x >> y >> h;
    trees.push_back({x, y, h});
    sorted.push_back(x);
  }
  sort(sorted.begin(), sorted.end());
  sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());
  int N = 1;
  while (N < (int) sorted.size()) {
    N <<= 1;
  }
  vector<node> st(2 * N);
  for (int i = 0; i < N; i++) {
    st[i + N] = node(i, i + 1);
  }
  for (int i = 0; i < n; i++) {
    int id = lower_bound(sorted.begin(), sorted.end(), trees[i].x) - sorted.begin();
    st[id + N].vals.emplace_back(trees[i].y, trees[i].h);
  }
  for (int i = 0; i < N; i++) {
    sort(st[i + N].vals.begin(), st[i + N].vals.end());
  }
  for (int i = N - 1; i > 0; i--) {
    st[i] = node(st[2 * i].l, st[2 * i + 1].r);
    st[i].vals = st[2 * i].vals;
    for (auto v : st[2 * i + 1].vals) {
      st[i].vals.push_back(v);
    }
    sort(st[i].vals.begin(), st[i].vals.end());
  }
  auto query = [&](int v, int l, int r, int ylo, int yhi, auto&& query) -> vector<int> {
    if (st[v].r <= l || st[v].l >= r) {
      return {};
    } else if (st[v].r <= r && st[v].l >= l) {
      vector<int> ans;
      int start = lower_bound(st[v].vals.begin(), st[v].vals.end(), make_pair(ylo, 0)) - st[v].vals.begin();
      for (int i = start; i < (int) st[v].vals.size(); i++) {
        if ((int) ans.size() == 50 || st[v].vals[i].first > yhi) {
          break;
        }
        ans.push_back(st[v].vals[i].second);
      }
      return ans;
    }
    vector<int> left = query(2 * v, l, r, ylo, yhi, query);
    vector<int> right = query(2 * v + 1, l, r, ylo, yhi, query);
    vector<int> ans = left;
    for (int v : right) {
      if ((int) ans.size() < 50) {
        ans.push_back(v);
      }
    }
    return ans;
  };
  auto check = [&](vector<int> h) -> bool {
    if ((int) h.size() == 50) {
      return true;
    }
    sort(h.begin(), h.end());
    for (int i = 2; i < (int) h.size(); i++) {
      if (h[i] < h[i - 1] + h[i - 2]) {
        return true;
      }
    }
    return false;
  };
  for (int i = 0; i < q; i++) {
    int xlo, ylo, xhi, yhi;
    cin >> xlo >> ylo >> xhi >> yhi;
    int l = lower_bound(sorted.begin(), sorted.end(), xlo) - sorted.begin();
    int r = upper_bound(sorted.begin(), sorted.end(), xhi) - sorted.begin();
    cout << check(query(1, l, r, ylo, yhi, query)) << '\n';
  }
  return 0;
}
