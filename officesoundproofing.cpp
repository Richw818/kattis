/**
 *    author:  Richw818
 *    created: 11.07.2026 13:05:44
**/

#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using db = double;
using ld = long double;
using pii = pair<int, int>;

#define sz(x) int((x).size())
#define all(x) (x).begin(), (x).end()
#define pb push_back
#define pf push_front
#define eb emplace_back
#define ef emplace_front
#define ins insert
#define lb lower_bound
#define ub upper_bound
#define mp make_pair

template <class T> using V = vector<T>;
template <class T, class K> bool ckmin(T& a, const K& b) { return b < a ? a = b, 1 : 0; }
template <class T, class K> bool ckmax(T& a, const K& b) { return a < b ? a = b, 1 : 0; }

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

#ifdef LOCAL
#include "debug.h"
#else
#define dbg(...) "Lisa"
#endif

struct FlowEdge {
    int v, u;
    long long cap, flow = 0;
    FlowEdge(int v, int u, long long cap) : v(v), u(u), cap(cap) {}
};

struct Dinic {
    const ll flow_inf = 1e18;
    V<FlowEdge> edges;
    V<V<int>> adj;
    int n, m = 0;
    int s, t;
    V<int> level, ptr;
    queue<int> q;

    Dinic(int n, int s, int t) : n(n), s(s), t(t) {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    void add_edge(int v, int u, ll cap) {
        edges.eb(v, u, cap);
        edges.eb(u, v, 0);
        adj[v].pb(m);
        adj[u].pb(m + 1);
        m += 2;
    }

    bool bfs() {
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int id : adj[v]) {
                if (edges[id].cap == edges[id].flow) continue;
                if (level[edges[id].u] != -1) continue;
                level[edges[id].u] = level[v] + 1;
                q.push(edges[id].u);
            }
        }
        return level[t] != -1;
    }

    ll dfs(int v, ll pushed) {
        if (pushed == 0) return 0;
        if (v == t) return pushed;
        for (int& cid = ptr[v]; cid < sz(adj[v]); cid++) {
            int id = adj[v][cid];
            int u = edges[id].u;
            if (level[v] + 1 != level[u]) continue;
            ll tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
            if (tr == 0) continue;
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }

    ll flow() {
        ll f = 0;
        while (true) {
            fill(all(level), -1);
            level[s] = 0;
            q.push(s);
            if (!bfs()) break;
            fill(all(ptr), 0);
            while (ll pushed = dfs(s, flow_inf)) {
                f += pushed;
            }
        }
        return f;
    }
};

void solve() {
    int n; cin >> n;
    V<string> grid(2 * n + 1);
    for (int i = 0; i < 2 * n + 1; i++) cin >> grid[i];

    V<int> x, y;
    V<V<int>> id(2 * n + 1, V<int>(2 * n + 1, -1));
    for (int i = 0; i < 2 * n + 1; i++) {
        for (int j = 0; j < 2 * n + 1; j++) {
            if (grid[i][j] == 'o') {
                id[i][j] = sz(x);
                x.pb(i);
                y.pb(j);
            }
        }
    }

    V<pii> constraints;
    for (int i = 0; i < 2 * n + 1; i++) {
        for (int j = 0; j < 2 * n + 1; j++) {
            if (j + 2 < 2 * n + 1 && id[i][j] != -1 && id[i][j + 2] != -1) constraints.eb(id[i][j], id[i][j + 2]);
            if (i + 2 < 2 * n + 1 && id[i][j] != -1 && id[i + 2][j] != -1) constraints.eb(id[i][j], id[i + 2][j]);
        }
    }

    int k = sz(x), c = sz(constraints);
    Dinic d(k + c + 2, k + c, k + c + 1);
    for (int i = 0; i < c; i++) {
        d.add_edge(k + i, constraints[i].first, 1);
        d.add_edge(k + i, constraints[i].second, 1);
        d.add_edge(k + c, k + i, 1);
    }
    for (int i = 0; i < k; i++) d.add_edge(i, k + c + 1, 1);

    if (d.flow() != c) {
        cout << "impossible\n";
        return;
    }

    for (int i = k; i < k + c; i++) {
        auto [u, v] = constraints[i - k];
        for (int eid : d.adj[i]) {
            if (d.edges[eid].flow > 0) {
                int to = d.edges[eid].u;
                if (x[u] == x[v]) {
                    grid[x[u]][y[u] + 1] = (to == u ? '<' : '>');
                } else {
                    grid[x[u] + 1][y[u]] = (to == u ? '^' : 'v');
                }
            }
        }
    }

    cout << c << "\n";
    for (int i = 0; i < 2 * n + 1; i++) cout << grid[i] << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}
