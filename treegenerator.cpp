/**
 *      Author:  Richw818
 *      Created: 01.20.2023 17:14:52
**/

#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    const int N = 2e5 + 5;
    int n, m, q; cin >> n >> m >> q;
    vector<long long> p10(N);
    p10[0] = (m == 1 ? 0 : 1);
    for(int i = 1; i < N; i++){
        p10[i] = (p10[i - 1] * 10) % m;
    }
    vector<vector<int>> adj(n);
    for(int i = 0; i < n - 1; i++){
        int u, v; cin >> u >> v;
        u--, v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> a(n); for(auto& A : a) cin >> A;
    vector<int> dep(n);
    vector<vector<int>> jmp(18, vector<int>(n, -1));
    vector<vector<long long>> up(18, vector<long long>(n)), down(18, vector<long long>(n));
    auto dfs = [&](int node, int p, int d, auto&& dfs) -> void {
        dep[node] = d;
        jmp[0][node] = p;
        up[0][node] = down[0][node] = a[node];
        for(int next : adj[node]){
            if(next != p) dfs(next, node, d + 1, dfs);
        }
    };
    dfs(0, -1, 0, dfs);
    for(int i = 1; i < 18; i++){
        for(int j = 0; j < n; j++){
            int next = jmp[i - 1][j];
            if(next != -1) jmp[i][j] = jmp[i - 1][next];
            if(dep[j] >= (1 << i) - 1){
                up[i][j] = p10[1 << (i - 1)] * up[i - 1][j] + up[i - 1][next];
                up[i][j] %= m;
                down[i][j] = p10[1 << (i - 1)] * down[i - 1][next] + down[i - 1][j];
                down[i][j] %= m;
            }
        }
    }
    auto lca = [&](int a, int b) -> int {
        if(dep[a] < dep[b]) swap(a, b);
        int diff = dep[a] - dep[b];
        for(int i = 0; i < 18; i++){
            if((1 << i) & diff) a = jmp[i][a];
        }
        if(a == b) return a;
        for(int i = 17; i >= 0; i--){
            if(jmp[i][a] != jmp[i][b]){
                a = jmp[i][a];
                b = jmp[i][b];
            }
        }
        return jmp[0][a];
    };
    auto dist = [&](int a, int b) -> int {
        return dep[a] + dep[b] - 2 * dep[lca(a, b)];
    };
    auto queryUp = [&](int node, int d) -> long long {
        int remaining = d;
        long long ans = 0;
        for(int i = 0; i < 18; i++){
            if((1 << i) & d){
                remaining -= (1 << i);
                ans += up[i][node] * p10[remaining];
                ans %= m;
                node = jmp[i][node];
            }
        }
        return ans;
    };
    auto queryDown = [&](int node, int d) -> long long {
        int curr = 0;
        long long ans = 0;
        for(int i = 0; i < 18; i++){
            if((1 << i) & d){
                ans += down[i][node] * p10[curr];
                ans %= m;
                curr += (1 << i);
                node = jmp[i][node];
            }
        }
        return ans;
    };
    while(q--){
        int x, y; cin >> x >> y;
        x--, y--;
        int l = lca(x, y);
        int d1 = dist(x, l), d2 = dist(y, l);
        long long first = queryUp(x, d1 + 1);
        long long sec = queryDown(y, d2 + 1);
        first = (first * p10[d2]) % m;
        sec -= (a[l] * p10[d2]);
        sec %= m;
        if(sec < 0) sec += m;
        cout << (first + sec) % m << '\n';
    }
    return 0;
}
