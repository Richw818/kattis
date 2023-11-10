/**
 *    Author:  Richw818
 *    Created: 11.09.2023 20:10:44
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

const int MOD = 1e9+7;
const db PI = acos((db)-1);
mt19937 rng(0); // or mt19937_64

tcT> bool ckmin(T& a, const T& b) {
	return b < a ? a = b, 1 : 0; } // set a = min(a,b)
tcT> bool ckmax(T& a, const T& b) {
	return a < b ? a = b, 1 : 0; } // set a = max(a,b)

struct MCMF { 
	using F = ll; using C = double; // flow type, cost type
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
  int n;
  cin >> n;
  vector<vector<int>> xv(n), yv(n);
  for (int i = 0; i < n; i++) {
    int m;
    cin >> m;
    for (int j = 0; j < m; j++) {
      int a, b;
      cin >> a >> b;
      xv[i].push_back(a);
      yv[i].push_back(b);
    }
  }
  double dp[16][65536];
  auto tsp = [&](const vector<int>& x, const vector<int>& y) -> double {
    int n = x.size();
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < (1 << n); j++) {
        dp[i][j] = 1e9;
      }
    }
    vector<vector<double>> dists(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        dists[i][j] = hypot(x[i] - x[j], y[i] - y[j]);
      }
    }
    dp[0][0] = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
      if (mask & 1) {
        continue;
      }
      for (int i = 0; i < n; i++) {
        for (int next = 0; next < n; next++) {
          if (~mask & (1 << next)) {
            dp[next][mask | (1 << next)] = min(dp[next][mask | (1 << next)], dp[i][mask] + dists[i][next]);
          }
        }
      }
    }
    return dp[0][(1 << n) - 1];
  };
  double initial = 0.0;
  for (int i = 0; i < n; i++) {
    initial += tsp(xv[i], yv[i]);
  }
  MCMF m;
  m.init(n + 3);
  for (int i = 0; i < n / 2; i++) {
    for (int j = n / 2; j < n; j++) {
      vector<int> a = xv[i], b = yv[i];
      for (int v : xv[j]) {
        a.push_back(v);
      }
      for (int v : yv[j]) {
        b.push_back(v);
      }
      double d = tsp(a, b);
      m.ae(i, j, 1, d);
    }
  }
  for (int i = 0; i < n / 2; i++) {
    m.ae(n, i, 1, 0);
    m.ae(i + n / 2, n + 1, 1, 0);
  }
  m.ae(n + 2, n, n / 2, 0);
  auto ans = m.calc(n + 2, n + 1);
  cout << fixed << setprecision(10) << initial << ' ' << ans.second << '\n';
  return 0;
}
