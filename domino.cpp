/**
 *    Author:  Richw818
 *    Created: 11.05.2023 15:03:02
**/

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using db = long double; // or double if tight TL
using str = string;

using pi = pair<int,int>;
#define mp make_pair
#define f first
#define s second

#define tcT template<class T
tcT> using V = vector<T>; 
tcT, size_t SZ> using AR = array<T,SZ>;
using vi = V<int>;
using vb = V<bool>;
using vpi = V<pi>;

#define sz(x) int((x).size())
#define all(x) begin(x), end(x)
#define sor(x) sort(all(x))
#define rsz resize
#define pb push_back
#define ft front()
#define bk back()

#define FOR(i,a,b) for (int i = (a); i < (b); ++i)
#define F0R(i,a) FOR(i,0,a)
#define ROF(i,a,b) for (int i = (b)-1; i >= (a); --i)
#define R0F(i,a) ROF(i,0,a)
#define rep(a) F0R(_,a)
#define each(a,x) for (auto& a: x)

tcT> bool ckmin(T& a, const T& b) {
    return b < a ? a = b, 1 : 0; } // set a = min(a,b)
tcT> bool ckmax(T& a, const T& b) {
    return a < b ? a = b, 1 : 0; } // set a = max(a,b)

struct domino {
  int x1, y1, x2, y2;
  int sum;
};

struct cmp {
  bool operator()(const domino& a, const domino& b) const {
    if (a.sum == b.sum) {
      return make_tuple(a.x1, a.y1, a.x2, a.y2) < make_tuple(b.x1, b.y1, b.x2, b.y2);
    }
    return a.sum > b.sum;
  }
};

struct MCMF { 
    using F = ll; using C = ll; // flow type, cost type
    struct Edge { int to; F flo, cap; C cost; };
    int N; V<C> p, dist; vi pre; V<Edge> eds; V<vi> adj;
    void init(int _N) { N = _N;
        p.rsz(N), dist.rsz(N), pre.rsz(N), adj.rsz(N); }
    void ae(int u, int v, F cap, C cost) { assert(cap >= 0); 
        adj[u].pb(sz(eds)); eds.pb({v,0,cap,cost}); 
        adj[v].pb(sz(eds)); eds.pb({u,0,0,-cost});
    } // use asserts, don't try smth dumb
    bool path(int s, int t) { // find lowest cost path to send flow through
        const C inf = numeric_limits<C>::max(); F0R(i,N) dist[i] = inf;
        using T = pair<C,int>; priority_queue<T,vector<T>,greater<T>> todo; 
        todo.push({dist[s] = 0,s}); 
        while (sz(todo)) { // Dijkstra
            T x = todo.top(); todo.pop(); if (x.f > dist[x.s]) continue;
            each(e,adj[x.s]) { const Edge& E = eds[e]; // all weights should be non-negative
                if (E.flo < E.cap && ckmin(dist[E.to],x.f+E.cost+p[x.s]-p[E.to]))
                    pre[E.to] = e, todo.push({dist[E.to],E.to});
            }
        } // if costs are doubles, add some EPS so you 
        // don't traverse ~0-weight cycle repeatedly
        return dist[t] != inf; // return flow
    }
    pair<F,C> calc(int s, int t) { assert(s != t);
        F0R(_,N) F0R(e,sz(eds)) { const Edge& E = eds[e]; // Bellman-Ford
            if (E.cap) ckmin(p[E.to],p[eds[e^1].to]+E.cost); }
        F totFlow = 0; C totCost = 0;
        while (path(s,t)) { // p -> potentials for Dijkstra
            F0R(i,N) p[i] += dist[i]; // don't matter for unreachable nodes
            F df = numeric_limits<F>::max();
            for (int x = t; x != s; x = eds[pre[x]^1].to) {
                const Edge& E = eds[pre[x]]; ckmin(df,E.cap-E.flo); }
            totFlow += df; totCost += (p[t]-p[s])*df;
            for (int x = t; x != s; x = eds[pre[x]^1].to)
                eds[pre[x]].flo += df, eds[pre[x]^1].flo -= df;
        } // get max flow you can send along path
        return {totFlow,totCost};
    }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, k;
  cin >> n >> k;
  set<domino, cmp> d;
  long long ans = 0;
  vector<vector<int>> grid(n, vector<int>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      cin >> grid[i][j];
      ans += grid[i][j];
    }
  }
  auto insert = [&](int x1, int y1, int x2, int y2) -> void {
    int sum = grid[x1][y1] + grid[x2][y2];
    d.insert({x1, y1, x2, y2, sum});
    if ((int) d.size() > 50) {
      d.erase(--d.end());
    }
  };
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i < n - 1) {
        insert(i, j, i + 1, j);
      }
      if (j < n - 1) {
        insert(i, j, i, j + 1);
      }
    }
  }
  set<pair<int, int>> coords;
  for (auto dom : d) {
    coords.insert({dom.x1, dom.y1});
    coords.insert({dom.x2, dom.y2});
  }
  vector<pair<int, int>> sorted(coords.begin(), coords.end());
  int l = sorted.size();
  MCMF m;
  m.init(l + 3);
  m.ae(l + 2, l, k, 0);
  set<int> left, right;
  for (auto dom : d) {
    int i = lower_bound(sorted.begin(), sorted.end(), make_pair(dom.x1, dom.y1)) - sorted.begin();
    int j = lower_bound(sorted.begin(), sorted.end(), make_pair(dom.x2, dom.y2)) - sorted.begin();
    if ((dom.x1 + dom.y1) % 2) {
      swap(i, j);
    }
    if (!left.count(i)) {
      m.ae(l, i, 1, 0);
      left.insert(i);
    }
    m.ae(i, j, 1, -dom.sum);
    if (!right.count(j)) {
      m.ae(j, l + 1, 1, 0);
      right.insert(j);
    }
  }
  auto best = m.calc(l + 2, l + 1);
  ans += best.second;
  cout << ans << '\n';
  return 0;
}
