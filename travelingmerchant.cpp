/**
 *      Author:  Richw818
 *      Created: 04.21.2023 22:52:49
**/

#include <bits/stdc++.h>
using namespace std;

struct item {
    bool set = true;
    long long pref = 0, suff = 0, sum = 0, ans = 0;
};

template <typename T>
struct segtree {
    struct node {
        int l = 0, r = 0;
        T val;
        node(int _l = 0, int _r = 0) {
            l = _l, r = _r;
        }
    };
    int N = 1;
    item ret;
    vector<node> tree;
    segtree(int _n) {
        while(N < _n) N <<= 1;
        ret.set = false;
        tree.resize(2 * N + 5);
        build();
    }
    T comb(T a, T b) {
        if (!a.set) {
            return b;
        } else if (!b.set) {
            return a;
        } else {
            item i;
            i.pref = max(a.pref, a.sum + b.pref);
            i.suff = max(b.suff, b.sum + a.suff);
            i.sum = a.sum + b.sum;
            i.ans = max({a.ans, b.ans, a.suff + b.pref});
            return i;
        }
    }
    void build() {
        for (int i = 0; i < N; i++) tree[i + N] = node(i, i + 1);
        for (int i = N - 1; i > 0; i--) tree[i] = node(tree[2 * i].l, tree[2 * i + 1].r);
    }
    void update(int i, int v) {
        i += N;
        if (v >= 0) {
            tree[i].val = {true, v, v, v, v};
        } else {
            tree[i].val = {true, 0, 0, v, 0};
        }
        while (i > 1) {
            i >>= 1;
            tree[i].val = comb(tree[2 * i].val, tree[2 * i + 1].val);
        }
    }
    T query(int v, int l, int r) {
        if(l <= tree[v].l && tree[v].r <= r) return tree[v].val;
        if(tree[v].l >= r || tree[v].r <= l) return ret;
        return comb(query(2 * v, l, r), query(2 * v + 1, l, r));
    }
    T query(int l, int r) {
        return query(1, l, r);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    using st = segtree<item>;
    vector<int> dt = {0, 1, 2, 3, 2, 1, 0};
    int n;
    cin >> n;
    vector<int> v(n), d(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i] >> d[i];
    }
    vector<st> forward(7, st(n)), backward(7, st(n));
    for (int start = 0; start < 7; start++) {
        int prev = 0;
        for (int i = 0; i < n; i++) {
            int val = v[i] + dt[(start + i) % 7] * d[i];
            forward[start].update(i, val - prev);
            prev = val;
        }
        prev = 0;
        for (int i = n - 1; i >= 0; i--) {
            int val = v[i] + dt[(start + (n - 1 - i)) % 7] * d[i];
            backward[start].update(i, val - prev);
            prev = val;
        }
    }
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;
        if (r > l) {
            int need = (7 - ((l - 1) % 7)) % 7;
            cout << forward[need].query(l, r).ans << '\n';
        } else {
            int need = (7 - ((n - l) % 7)) % 7;
            cout << backward[need].query(r - 1, l - 1).ans << '\n';
        }
    }
    return 0;
}
