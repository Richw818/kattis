#include <bits/stdc++.h>
using namespace std;

int dp[101][101][2][2];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  string s;
  cin >> s;
  vector<int> groundhog(n);
  auto getGroundhog = [&](int i, int j) -> void {
    for (int l = j; l >= i; l--) {
      for (int r = l; r <= j; r++) {
        for (int c1 = 0; c1 < 2; c1++) {
          for (int c2 = 0; c2 < 2; c2++) {
            if (l == r) {
              continue;
            } else if (l + 1 == r) {
              char x = s[l], y = s[r];
              if (c1) {
                x ^= ('B' ^ 'W');
              }
              if (c2) {
                y ^= ('B' ^ 'W');
              }
              dp[l][r][c1][c2] = (x == y ? 0 : 1);
            } else {
              unordered_set<int> seen;
              for (int m = l; m < r; m++) {
                char from = s[m], to = s[m + 1];
                if (m == l && c1) {
                  from ^= ('B' ^ 'W');
                }
                if (m + 1 == r && c2) {
                  to ^= 'B' ^ 'W';
                }
                if (from != to) {
                  int left = (m == l ? 0 : dp[l][m - 1][c1][0]);
                  int right = dp[m + 1][r][1][c2];
                  seen.insert(left ^ right);
                }
              }
              for (int m = l + 1; m <= r; m++) {
                char from = s[m], to = s[m - 1];
                if (m == r && c2) {
                  from ^= ('B' ^ 'W');
                }
                if (m - 1 == l && c1) {
                  to ^= ('B' ^ 'W');
                }
                if (from != to) {
                  int left = dp[l][m - 1][c1][1];
                  int right = (m == r ? 0 : dp[m + 1][r][0][c2]);
                  seen.insert(left ^ right);
                }
              }
              while (seen.count(dp[l][r][c1][c2])) {
                dp[l][r][c1][c2]++;
              }
            }
            // cerr << "dbg: " << l << " " << r << " " << c1 << " " << c2 << " : " << dp[l][r][c1][c2] << endl;
          }
        }
      }
    }
  };
  int x = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] != '.') {
      string curr = "";
      int j = i;
      while (j < n && s[j] != '.') {
        curr += s[j++];
      }
      getGroundhog(i, j - 1);
      x ^= dp[i][j - 1][0][0];
      i = j - 1;
    }
  }
  vector<pair<int, int>> ans;
  for (int i = 0; i < n - 1; i++) {
    if (s[i] != '.' && s[i + 1] != '.' && s[i] != s[i + 1]) {
      int lb = i, rb = i;
      while (lb >= 0 && s[lb] != '.') {
        lb--;
      }
      lb++;
      while (rb < n && s[rb] != '.') {
        rb++;
      }
      rb--;
      // cerr << "dbg2: " << i << " " << lb << " " <<  rb << endl;
      int newx = x ^ dp[lb][rb][0][0];
      int left = (i == lb ? 0 : dp[lb][i - 1][0][0]);
      int right = dp[i + 1][rb][1][0];
      newx ^= (left ^ right);
      if (newx == 0) {
        ans.emplace_back(i, i + 1);
      }
    }
  }
  for (int i = 1; i < n; i++) {
    if (s[i] != '.' && s[i - 1] != '.' && s[i] != s[i - 1]) {
      int lb = i, rb = i;
      while (lb >= 0 && s[lb] != '.') {
        lb--;
      }
      lb++;
      while (rb < n && s[rb] != '.') {
        rb++;
      }
      rb--;
      int newx = x ^ dp[lb][rb][0][0];
      int left = dp[lb][i - 1][0][1];
      int right = (i == rb ? 0 : dp[i + 1][rb][0][0]);
      newx ^= left ^ right;
      if (newx == 0) {
        ans.emplace_back(i, i - 1);
      }
    }
  }
  sort(ans.begin(), ans.end());
  cout << (int) ans.size() << '\n';
  for (auto [x, y] : ans) {
    cout << x + 1 << ' ' << y + 1 << '\n';
  }
  return 0;
}
