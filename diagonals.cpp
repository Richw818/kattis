/**
 *      Author:  Richw818
 *      Created: 02.23.2023 00:27:40
**/

#include <bits/stdc++.h>
using namespace std;
struct update{
    int a, parA;
    int b, sizeB;
};
struct dsuroll{
    vector<int> p, s;
    vector<update> updates;
    dsuroll(int _n){
        p.resize(_n); s.resize(_n);
        iota(p.begin(), p.end(), 0);
        fill(s.begin(), s.end(), 1);
    }
    int size(int a){
        return s[find(a)];
    }
    int time(){
        return (int)updates.size();
    }
    void rollback(int t){
        for(int i = time() - 1; i >= t; i--){
            update u = updates[i];
            p[u.a] = u.parA;
            s[u.b] = u.sizeB;
        }
        updates.resize(t);
    }
    int find(int a){
        if(p[a] == a) return a;
        return find(p[a]);
    }
    bool unite(int a, int b){
        a = find(a), b = find(b);
        if(a == b) return false;
        if(s[a] > s[b]) swap(a, b);
        updates.push_back({a, p[a], b, s[b]});
        s[b] += s[a];
        p[a] = b;
        return true;
    }
    bool same(int a, int b){
        return find(a) == find(b);
    }
};
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<vector<int>> intersections(n + 1, vector<int>(n + 1));
    int check = 0, good = 0;
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= n; j++){
            char c; cin >> c;
            intersections[i][j] = (c == '+' ? -1 : c - '0');
            check += c != '+';
            good += intersections[i][j] == 0;
        }
    }
    int all = (n + 1) * (n + 1);
    vector<vector<int>> touched(n + 1, vector<int>(n + 1));
    vector<string> ans(n, string(n, '.'));
    dsuroll d(all);
    auto add_edge = [&](int x1, int y1, int x2, int y2) -> void {
        int a = (n + 1) * x1 + y1;
        int b = (n + 1) * x2 + y2;
        d.unite(a, b);
    };
    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= n; j++){
            if(intersections[i][j] == 4){
                touched[i][j] = 4;
                good++;
                touched[i - 1][j - 1]++;
                good += touched[i - 1][j - 1] == intersections[i - 1][j - 1];
                touched[i - 1][j + 1]++;
                good += touched[i - 1][j + 1] == intersections[i - 1][j + 1];
                touched[i + 1][j - 1]++;
                good += touched[i + 1][j - 1] == intersections[i + 1][j - 1];
                touched[i + 1][j + 1]++;
                good += touched[i + 1][j + 1] == intersections[i + 1][j + 1];
                ans[i - 1][j - 1] = '\\';
                ans[i - 1][j] = '/';
                ans[i][j - 1] = '/';
                ans[i][j] = '\\';
                add_edge(i, j, i - 1, j - 1);
                add_edge(i, j, i - 1, j + 1);
                add_edge(i, j, i + 1, j - 1);
                add_edge(i, j, i + 1, j + 1);
            }
        }
    }
    auto backtrack = [&](int x, int y, int satisfied, auto&& backtrack) -> bool {
        if(x == n){
            // cerr << satisfied << endl;
            if(satisfied != check) return false;
            for(int i = 0; i < n; i++) cout << ans[i] << '\n';
            return true;
        }
        int nx = x, ny = y + 1;
        if(ny == n) nx++, ny = 0;
        if(ans[x][y] != '.'){
            if(intersections[x][y] != -1 && touched[x][y] != intersections[x][y]) return false;
            return backtrack(nx, ny, satisfied, backtrack);
        }
        bool ret = false;
        ans[x][y] = '/';
        int add = 0;
        touched[x + 1][y]++;
        touched[x][y + 1]++;
        add += touched[x + 1][y] == intersections[x + 1][y];
        add += touched[x][y + 1] == intersections[x][y + 1];
        bool ok = true;
        if(intersections[x][y + 1] != -1 && touched[x][y + 1] > intersections[x][y + 1]) ok = false;
        if(intersections[x + 1][y] != -1 && touched[x + 1][y] > intersections[x + 1][y]) ok = false;
        if(intersections[x][y] != -1 && touched[x][y] != intersections[x][y]) ok = false;
        int t = d.time();
        int a = (n + 1) * (x + 1) + y, b = (n + 1) * x + (y + 1);
        if(ok && !d.same(a, b)){
            d.unite(a, b);
            ret = ret || backtrack(nx, ny, satisfied + add, backtrack);
            d.rollback(t);
        }
        touched[x + 1][y]--;
        touched[x][y + 1]--;
        ans[x][y] = '\\';
        ok = true;
        add = 0;
        touched[x][y]++;
        touched[x + 1][y + 1]++;
        add += touched[x][y] == intersections[x][y];
        add += touched[x + 1][y + 1] == intersections[x + 1][y + 1];
        if(intersections[x][y] != -1 && touched[x][y] != intersections[x][y]) ok = false;
        if(intersections[x + 1][y + 1] != -1 && touched[x + 1][y + 1] > intersections[x + 1][y + 1]) ok = false;
        a = (n + 1) * x + y, b = (n + 1) * (x + 1) + y + 1;
        if(ok && !d.same(a, b)){
            d.unite(a, b);
            ret = ret || backtrack(nx, ny, satisfied + add, backtrack);
            d.rollback(t);
        }
        touched[x][y]--;
        touched[x + 1][y + 1]--;
        ans[x][y] = '.';
        return ret;
    };
    backtrack(0, 0, good, backtrack);
    return 0;
}
