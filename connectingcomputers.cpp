
/**
 *    author:  Richw818
 *    created: 09.18.2025 22:23:42
**/

#include <bits/stdc++.h>

#ifdef LOCAL
#include "debug.h"
#else
#define dbg(...) "Lisa"
#endif

void solve() {
    int n, m, k;
    std::cin >> n >> m >> k;
    std::unordered_map<std::string, int> cable;
    for (int i = 0; i < k; i++) {
        std::string c;
        std::cin >> c;
        cable[c] = i;
    }

    std::vector<std::vector<int>> adj(n);
    std::vector<int> u(m), v(m), w(m);
    for (int i = 0; i < m; i++) {
        std::cin >> u[i] >> v[i];
        u[i]--, v[i]--;
        adj[u[i]].push_back(i);
        adj[v[i]].push_back(i);
        std::string c;
        std::cin >> c;
        w[i] = cable[c];
    }

    std::vector<std::vector<bool>> bad(k, std::vector<bool>(k));
    
    int must = 0;
    std::vector<int> prevEdge(n, -1), dep(n, -1);
    std::vector<bool> used(m);
    int time = 0;
    auto dfs = [&](int node, int p, auto&& self) -> void {
        dep[node] = time++;
        for (int eid : adj[node]) {
            int to = node ^ u[eid] ^ v[eid];
            if (to == p) {
                continue;
            }
            if (dep[to] != -1) {
                if (dep[to] < dep[node]) {
                    std::vector<int> count(k);
                    count[w[eid]] = 1;
                    used[eid] = true;
                    int curr = node;
                    while (curr != to) {
                        count[w[prevEdge[curr]]]++;
                        used[prevEdge[curr]] = true;
                        curr = curr ^ u[prevEdge[curr]] ^ v[prevEdge[curr]];
                    }
                    for (int i = 0; i < k; i++) {
                        if (count[i] > 1) {
                            must |= 1 << i;
                        }
                        if (count[i] == 1) {
                            for (int j = i + 1; j < k; j++) {
                                if (count[j] == 1) {
                                    bad[i][j] = bad[j][i] = true;
                                }
                            }
                        }
                    }
                }
            } else {
                prevEdge[to] = eid;
                self(to, node, self);
            }
        }
    };

    dfs(0, -1, dfs);

    for (int i = 0; i < m; i++) {
        if (!used[i]) {
            must |= 1 << w[i];
        }
    }

    std::vector<bool> indep(1 << k, true), good(1 << k);
    for (int mask = 0; mask < (1 << k); mask++) {
        int one = -1;
        for (int b = 0; b < k; b++) {
            if (mask & (1 << b)) {
                if (one == -1) {
                    one = b;
                    if (!indep[mask ^ (1 << b)]) {
                        indep[mask] = false;
                        break;
                    }
                } else {
                    if (bad[b][one]) {
                        indep[mask] = false;
                        break;
                    }
                }
            }
        }

        if (indep[mask]) {
            int remain = ((1 << k) - 1) ^ mask;
            remain |= must;
            good[remain] = true;
        }
    }

    int least = k, total = 0;
    for (int mask = 0; mask < (1 << k); mask++) {
        if (good[mask]) {
            least = std::min(least, __builtin_popcount(mask));
            total++;
        }
    }

    std::cout << least << "\n";
    std::cout << total << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();

    return 0;
}
