/**
 *      Author:  Richw818
 *      Created: 03.21.2023 18:54:12
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
        vector<string> games(n);
        for (int i = 0; i < n; i++) {
            cin >> games[i];
        }
        vector<int> win;
        auto check = [&](int player) -> void {
            vector<int> points(n);
            auto inst = games;
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (inst[i][j] == '1') {
                        points[i] += 2;
                    } else if (inst[i][j] == '0') {
                        points[j] += 2;
                    } else if (inst[i][j] == 'd') {
                        points[i]++;
                        points[j]++;
                    } else {
                        if (i == player) {
                            points[player] += 2;
                            inst[i][j] = '1';
                            inst[j][i] = '0';
                        } else if (j == player) {
                            points[player] += 2;
                            inst[i][j] = '0';
                            inst[j][i] = '1';
                        }
                    }
                }
            }
            int most = *max_element(points.begin(), points.end());
            if (most != points[player]) {
                return;
            }
            int unplayed = 0, curr = n + 1;
            Dinic d(n * n + 1, n, n * n);
            for (int i = 0; i < n; i++) {
                d.add_edge(i, n * n, most - points[i]);
            }
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (inst[i][j] == '.') {
                        d.add_edge(n, curr, 2);
                        d.add_edge(curr, i, 2);
                        d.add_edge(curr, j, 2);
                        unplayed++, curr++;
                    }
                }   
            }
            if (d.flow() == 2 * unplayed) {
                win.push_back(player + 1);
            }
        };
        for (int i = 0; i < n; i++) {
            check(i);
        }
        int m = win.size();
        for (int i = 0; i < m; i++) {
            cout << win[i] << " \n"[i == m - 1];
        }
    }
    return 0;
}
