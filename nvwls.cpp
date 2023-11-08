/**
 *      Author:  Richw818
 *      Created: 04.22.2023 16:56:37
**/

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    const int P = 29, MOD = 1e9 + 7, N = 300005;
    auto modpow = [&](int n, int p) -> int {
        int ans = 1;
        for (; p; p >>= 1) {
            if (p & 1) {
                ans = (1LL * ans * n) % MOD;
            }
            n = (1LL * n * n) % MOD;
        }
        return ans;
    };
    vector<int> inv(N);
    inv[0] = 1;
    int minv = modpow(P, MOD - 2);
    for (int i = 1; i < N; i++) {
        inv[i] = (1LL * inv[i - 1] * minv) % MOD;
    }
    auto getHash = [&](string s) -> int {
        int n = s.size();
        int ans = 0, curr = 1;
        for (int i = 0; i < n; i++) {
            int v = s[i] - 'A' + 1;
            int add = (1LL * v * curr) % MOD;
            ans += add;
            if (ans >= MOD) {
                ans -= MOD;
            }
            curr = (1LL * curr * P) % MOD;
        }
        return ans;
    };
    int n;
    cin >> n;
    vector<string> words(n), wnv(n);
    vector<int> vowels(n);
    vector<unordered_map<int, int>> byLength(100005);
    unordered_set<int> lengths;
    for (int i = 0; i < n; i++) {
        cin >> words[i];
        for (char c : words[i]) {
            if (c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
                wnv[i] += c;
            } else {
                vowels[i]++;
            }
        }
        int h = getHash(wnv[i]);
        int l = wnv[i].size();
        if (!byLength[l].count(h) || vowels[i] > vowels[byLength[l][h]]) {
            byLength[l][h] = i;
        }
        lengths.insert(l);
    }
    vector<int> l(lengths.begin(), lengths.end());
    string message;
    cin >> message;
    int m = message.size();
    vector<int> hash(m + 1);
    int curr = 1;
    for (int i = 1; i <= m; i++) {
        int v = message[i - 1] - 'A' + 1;
        hash[i] = (1LL * curr * v) % MOD;
        hash[i] += hash[i - 1];
        if (hash[i] >= MOD) {
            hash[i] -= MOD;
        }   
        curr = (1LL * curr * P) % MOD;
    }
    auto getRange = [&](int l, int r) -> int {
        int ans = hash[r + 1] - hash[l];
        if (ans < 0) {
            ans += MOD;
        }
        ans = (1LL * ans * inv[l]) % MOD;
        return ans;
    };
    vector<int> dp(m + 1, -1), p(m + 1, -1);
    dp[0] = 0;
    for (int i = 1; i <= m; i++) {
        for (int L : l) {
            if (i >= L && dp[i - L] != -1) {
                int h = getRange(i - L, i - 1);
                if (byLength[L].count(h) && dp[i - L] + vowels[byLength[L][h]] > dp[i]) {
                    dp[i] = dp[i - L] + vowels[byLength[L][h]];
                    p[i] = byLength[L][h];
                }
            }
        }
    }
    int start = m;
    vector<int> inds;
    while (p[start] != -1) {
        inds.push_back(p[start]);
        start -= (int) wnv[p[start]].size();
    }
    reverse(inds.begin(), inds.end());
    for (int i = 0; i < (int) inds.size(); i++) {
        cout << words[inds[i]] << " \n"[i == (int) inds.size() - 1];
    }
    return 0;
}
