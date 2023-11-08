/**
 *    Author:  Richw818
 *    Created: 09.21.2023 15:28:18
**/

#include <bits/stdc++.h>
using namespace std;

struct node {
  int x1, y1, x2, y2;
  int moves, hits;
};

struct cmp {
  bool operator()(const node& a, const node& b) const {
    return make_pair(a.moves, a.hits) > make_pair(b.moves, b.hits);
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  const int N = 55;
  int n, m, e;
  cin >> n >> m >> e;
  pair<int, int> dist[N][N][N][N];
  for (int x1 = 0; x1 < N; x1++) {
    for (int y1 = 0; y1 < N; y1++) {
      for (int x2 = 0; x2 < N; x2++) {
        for (int y2 = 0; y2 < N; y2++) {
          dist[x1][y1][x2][y2] = {-1, -1};
        }
      }
    }
  }
  auto getDir = [&](char c) -> int {
    if (c == 'W') {
      return 0;
    } else if (c == 'N') {
      return 1;
    } else if (c == 'E') {
      return 2;
    }
    return 3;
  };
  int sx1, sy1, sx2, sy2;
  char d1, d2;
  cin >> sx1 >> sy1 >> d1 >> sx2 >> sy2 >> d2;
  int a = getDir(d1), b = getDir(d2);
  bool hwall[N][N], vwall[N][N];
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      hwall[i][j] = vwall[i][j] = false;
    }
  }
  int w;
  cin >> w;
  for (int i = 0; i < w; i++) {
    int x, y;
    cin >> x >> y;
    hwall[x][y] = true;
  }
  cin >> w;
  for (int i = 0; i < w; i++) {
    int x, y;
    cin >> x >> y;
    vwall[x][y] = true;
  }
  auto getMove = [&](int x, int y, int d) -> array<int, 3> {
    if (x == 0) {
      // Ignore command
      return {0, 0, 0};
    }
    if (d == 0) {
      // Go West
      if (x == 1 || vwall[x - 1][y]) {
        return {x, y, 1};
      } else {
        return {x - 1, y, 0};
      }
    } else if (d == 1) {
      // Go North
      if (y == m || hwall[x][y]) {
        return {x, y, 1};
      } else {
        return {x, y + 1, 0};
      }
    } else if (d == 2) {
      // Go East
      if (x == n || vwall[x][y]) {
        return {x, y, 1};
      } else {
        return {x + 1, y, 0};
      }
    } else {
      // Go South
      if (x == e && y == 1) {
        return {0, 0, 0};
      } else {
        if (y == 1 || hwall[x][y - 1]) {
          return {x, y, 1};
        } else {
          return {x, y - 1, 0};
        }
      }
    }
  };
  priority_queue<node, vector<node>, cmp> pq;
  pq.push({sx1, sy1, sx2, sy2, 0, 0});
  while (!pq.empty()) {
    auto x = pq.top();
    pq.pop();
    if (dist[x.x1][x.y1][x.x2][x.y2].first != -1) {
      continue;
    }
    dist[x.x1][x.y1][x.x2][x.y2] = {x.moves, x.hits};
    for (int dd = 0; dd < 4; dd++) {
      int nd1 = (a + dd) % 4;
      int nd2 = (b + dd) % 4;
      array<int, 3> nc1 = getMove(x.x1, x.y1, nd1);
      array<int, 3> nc2 = getMove(x.x2, x.y2, nd2);
      if (nc1[0] == nc2[0] && nc1[1] == nc2[1] && nc1[0] != 0) {
        continue;
      }
      if (dist[nc1[0]][nc1[1]][nc2[0]][nc2[1]].first == -1) {
        pq.push({nc1[0], nc1[1], nc2[0], nc2[1], x.moves + 1, x.hits + nc1[2] + nc2[2]});
      }
    }
  }
  auto ans = dist[0][0][0][0];
  cout << ans.first << ' ' << ans.second << '\n';
  return 0;
}
