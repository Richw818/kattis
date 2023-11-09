/**
 *    Author:  Richw818
 *    Created: 11.08.2023 23:08:42
**/

#include <bits/stdc++.h>
using namespace std;

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	long double dist() const { return sqrt((long double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	long double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

using P = Point<long double>;

vector<P> circleLine(P c, long double r, P a, P b) {
	P ab = b - a, p = a + ab * (c-a).dot(ab) / ab.dist2();
	long double s = a.cross(b, c), h2 = r*r - s*s / ab.dist2();
	if (h2 < 0) return {};
	if (h2 == 0) return {p};
	P h = ab.unit() * sqrt(h2);
	return {p - h, p + h};
}

vector<P> circleInter(P a,P b,double r1,double r2) {
	if (a == b) return {};
	P vec = b - a;
	double d2 = vec.dist2(), sum = r1+r2, dif = r1-r2,
	       p = (d2 + r1*r1 - r2*r2)/(d2*2), h2 = r1*r1 - p*p*d2;
	if (sum*sum < d2 || dif*dif > d2) return {};
	P mid = a + vec*p, per = vec.perp() * sqrt(fmax(0, h2) / d2);
	return {mid + per, mid - per};
}

struct node {
  int id;
  long double dist;
};

struct cmp {
  bool operator()(const node& a, const node& b) const {
    return a.dist > b.dist;
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  const long double INF = 1e18;
  const long double EPS = 1e-13;
  int x1, y1, x2, y2;
  cin >> x1 >> y1 >> x2 >> y2;
  int n;
  cin >> n;
  vector<P> c;
  vector<int> r(n);
  for (int i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y >> r[i];
    c.emplace_back(x, y);
  }
  vector<P> pts;
  pts.emplace_back(x1, y1);
  pts.emplace_back(x2, y2);
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      auto p = circleInter(c[i], c[j], r[i] + EPS / 2, r[j] + EPS / 2);
      for (auto pt : p) {
        pts.push_back(pt);
      }
    }
  }
  auto goodSegm = [&](P a, P b) -> bool {
    vector<pair<long double, long double>> intervals;
    P d = b - a;
    for (int i = 0; i < n; i++) {
      auto is = circleLine(c[i], r[i] + EPS, a, b);
      if ((int) is.size() == 2) {
        P d1 = is[0] - a;
        P d2 = is[1] - a;
        long double l = (d.x > EPS ? d1.x / d.x : d1.y / d.y);
        long double r = (d.x > EPS ? d2.x / d.x : d2.y / d.y);
        l = max(l, 0.0l);
        r = max(r, 0.0l);
        l = min(l, 1.0l);
        r = min(r, 1.0l);
        if (l > r) {
          swap(l, r);
        }
        intervals.emplace_back(l - EPS, r + EPS);
      }
    }
    long double r = 0.0;
    sort(intervals.begin(), intervals.end());
    int m = intervals.size();
    int p = 0;
    while (r < 1.0) {
      long double most = -INF;
      while (p < m && intervals[p].first <= r) {
        most = max(most, intervals[p].second);
        p++;
      }
      if (most <= r) {
        return false;
      }
      r = most;
    }
    return true;
  };
  int k = pts.size();
  // for (int i = 0; i < k; i++) {
  //   cerr << pts[i].x << ' ' << pts[i].y << endl;
  // }
  vector<vector<long double>> dist(k, vector<long double>(k, INF));
  for (int i = 0; i < k; i++) {
    dist[i][i] = 0.0;
    for (int j = i + 1; j < k; j++) {
      if (goodSegm(pts[i], pts[j])) {
        long double d = (pts[i] - pts[j]).dist();
        dist[i][j] = dist[j][i] = d;
      }
    }
  }
  vector<bool> vis(k);
  vector<long double> dists(k, INF);
  dists[0] = 0.0;
  priority_queue<node, vector<node>, cmp> pq;
  pq.push({0, 0.0});
  while (!pq.empty()) {
    auto [id, d] = pq.top();
    pq.pop();
    if (vis[id]) {
      continue;
    }
    vis[id] = true;
    for (int next = 0; next < k; next++) {
      if (dists[next] > dists[id] + dist[id][next]) {
        dists[next] = dists[id] + dist[id][next];
        pq.push({next, dists[next]});
      }
    }
  }
  if (dists[1] > 1e17) {
    cout << "impossible\n";
  } else {
    cout << fixed << setprecision(10) << dists[1] << '\n';
  }
  return 0;
}
