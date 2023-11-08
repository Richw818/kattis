/**
 *      Author:  Richw818
 *      Created: 04.17.2023 00:13:05
**/

#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct fenwick {
    int N;
    vector<T> sum;
    fenwick(int _N) {
        N = _N + 5;
        sum.resize(N);
    }
    void update(int i, T v) {
        for (; i < N; i += i & (-i)) sum[i] += v;
    }
    T query(int i) {
        T res = 0;
        for (; i; i -= i & (-i)) res += sum[i];
        return res;
    }
    T query(int l, int r) {
        return query(r) - query(l - 1);
    }
};

struct segH {
    int row;
    int s, e;
};

struct segV {
    int col;
    int s, e;
};

struct evtH {
    int row, x;
    int type;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int r, c, m, n;
    int tt = 1;
    while (cin >> r >> c >> m >> n) {
        map<int, vector<int>> by_row, by_col;
        map<pair<int, int>, char> mirror;
        for (int i = 0; i < m; i++) {
            int x, y;
            cin >> x >> y;
            by_row[x].push_back(y);
            by_col[y].push_back(x);
            mirror[{x, y}] = '/';
        }
        for (int i = 0; i < n; i++) {
            int x, y;
            cin >> x >> y;
            by_row[x].push_back(y);
            by_col[y].push_back(x);
            mirror[{x, y}] = '\\';
        }
        by_row[r].push_back(c + 1);
        by_col[c + 1].push_back(r);
        for (auto it = by_row.begin(); it != by_row.end(); it++) {
            sort(it->second.begin(), it->second.end());
        }
        for (auto it = by_col.begin(); it != by_col.end(); it++) {
            sort(it->second.begin(), it->second.end());
        }
        bool hits = false;
        auto dfs = [&](int row, int col, int d, vector<segH>& h, vector<segV>& v, auto&& dfs) -> void {
            // cerr << "Vis " << row << ' ' << col << ' ' << d << endl;
            if (d == 0) {
                // Right
                auto it = upper_bound(by_row[row].begin(), by_row[row].end(), col);
                if (it == by_row[row].end()) {
                    h.push_back({row, col + 1, c});
                    return;
                } else if (*it == c + 1) {
                    hits = true;
                    return;
                } else {
                    h.push_back({row, col + 1, *it - 1});
                    int nr = row, nc = *it;
                    if (mirror[{nr, nc}] == '/') {
                        d ^= 1;
                    } else {
                        d = 3 - d;
                    }
                    dfs(nr, nc, d, h, v, dfs);
                }
            } else if (d == 1) {
                // Up
                auto it = lower_bound(by_col[col].begin(), by_col[col].end(), row);
                if (it == by_col[col].begin()) {
                    v.push_back({col, 1, row - 1});
                    return;
                } else {
                    it--;
                    v.push_back({col, *it + 1, row - 1});
                    int nr = *it, nc = col;
                    if (mirror[{nr, nc}] == '/') {
                        d ^= 1;
                    } else {
                        d = 3 - d;
                    }
                    dfs(nr, nc, d, h, v, dfs);
                }
            } else if (d == 2) {
                // Left
                auto it = lower_bound(by_row[row].begin(), by_row[row].end(), col);
                if (it == by_row[row].begin()) {
                    h.push_back({row, 1, col - 1});
                    return;
                } else {
                    it--;
                    h.push_back({row, *it + 1, col - 1});
                    int nr = row, nc = *it;
                    if (mirror[{nr, nc}] == '/') {
                        d ^= 1;
                    } else {
                        d = 3 - d;
                    }
                    dfs(nr, nc, d, h, v, dfs);
                }
            } else {
                // Down
                auto it = upper_bound(by_col[col].begin(), by_col[col].end(), row);
                if (it == by_col[col].end()) {
                    v.push_back({col, row + 1, r});
                    return;
                } else {
                    v.push_back({col, row + 1, *it - 1});
                    int nr = *it, nc = col;
                    if (mirror[{nr, nc}] == '/') {
                        d ^= 1;
                    } else {
                        d = 3 - d;
                    }
                    dfs(nr, nc, d, h, v, dfs);
                }
            }
        };
        vector<segH> hf, hs;
        vector<segV> vf, vs;
        dfs(1, 0, 0, hf, vf, dfs);
        if (hits) {
            cout << "Case " << tt++ << ": " << 0 << '\n';
            continue;
        }
        dfs(r, c + 1, 2, hs, vs, dfs);
        int sr = 1e9, sc = 1e9;
        auto sweep = [&](vector<segH> h, vector<segV> v) -> long long {
            multiset<int> rows;
            fenwick<int> ft(r);
            vector<evtH> events;
            long long intersections = 0;
            for (segH hor : h) {
                events.push_back({hor.row, hor.s, 1});
                events.push_back({hor.row, hor.e + 1, -1});
            }
            sort(events.begin(), events.end(), [](evtH& a, evtH& b) {
                if (a.x == b.x) {
                    return a.type > b.type;
                }
                return a.x < b.x;
            });
            sort(v.begin(), v.end(), [](segV& a, segV& b) {
                return a.col < b.col;
            });
            int p = 0;
            for (int i = 0; i < (int) v.size(); i++) {
                while (p < (int) events.size() && events[p].x <= v[i].col) {
                    ft.update(events[p].row, events[p].type);
                    if (events[p].type == 1) {
                        rows.insert(events[p].row);
                    } else {
                        rows.erase(rows.find(events[p].row));
                    }
                    p++;
                }
                intersections += ft.query(v[i].s, v[i].e);
                auto it = rows.lower_bound(v[i].s);
                if (it != rows.end() && *it <= v[i].e) {
                    tie(sr, sc) = min(make_pair(sr, sc), make_pair(*it, v[i].col));
                }
            }
            return intersections;
        };
        long long total = sweep(hf, vs) + sweep(hs, vf);
        if (total == 0) {
            cout << "Case " << tt++ << ": impossible\n";
        } else {
            cout << "Case " << tt++ << ": " << total << ' ' << sr << ' ' << sc << '\n';
        }
    }

    return 0;
}
