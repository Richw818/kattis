/**
 *    author:  Richw818
 *    created: 09.18.2025 20:16:57
**/

#include <bits/stdc++.h>

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

#ifdef LOCAL
#include "debug.h"
#else
#define dbg(...) "Lisa"
#endif

using ld = long double;

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
	ld dist() const { return sqrtl((ld)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	ld angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

using P = Point<ld>;

vector<P> circleLine(P c, ld r, P a, P b) {
	P ab = b - a, p = a + ab * (c-a).dot(ab) / ab.dist2();
	ld s = a.cross(b, c), h2 = r*r - s*s / ab.dist2();
	if (h2 < 0) return {};
	if (h2 == 0) return {p};
	P h = ab.unit() * sqrtl(h2);
	return {p - h, p + h};
}

std::vector<P> circleInter(P a,P b,ld r1,ld r2) {
	if (a == b) { return {}; }
	P vec = b - a;
	ld d2 = vec.dist2(), sum = r1+r2, dif = r1-r2,
	       p = (d2 + r1*r1 - r2*r2)/(d2*2), h2 = r1*r1 - p*p*d2;
	if (sum*sum < d2 || dif*dif > d2) return {};
	P mid = a + vec*p, per = vec.perp() * sqrtl(fmax(0, h2) / d2);
	return {mid + per, mid - per};
}

constexpr ld INF = 1e9;
constexpr ld EPS = 1e-9;

struct Node {
    int id;
    ld dist;
};

struct cmp {
    bool operator()(const Node& a, const Node& b) const {
        return a.dist > b.dist;
    }
};

void solve() {
    int n;
    std::cin >> n;
    std::vector<P> points;
    points.emplace_back(0.0, 0.0);
    points.emplace_back(-0.2, 0.0);
    points.emplace_back(0.2, 0.0);
    for (int i = 0; i < n; i++) {
        ld x, y;
        std::cin >> x >> y;
        points.emplace_back(x, y);
    }
    n += 3;

    std::vector<P> p = points;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            std::vector<P> ci = circleInter(points[i], points[j], 1.0, 1.0);
            for (auto pp : ci) {
                p.push_back(pp);
            }
        }
    }

    int k = (int) p.size();
    
    auto getLocation = [&](const P& a, const P& b, const P& c) -> ld {
        P ba = b - a, ca = c - a;
        ld ans = ba.dot(ca) / ba.dist2();
        ans = std::max(ans, 0.0l);
        ans = std::min(ans, 1.0l);
        return ans;
    };
    
    auto validate = [&](const P& a, const P& b) -> bool {
        std::vector<std::pair<ld, int>> evts;
        for (int i = 0; i < n; i++) {
            std::vector<P> inters = circleLine(points[i], 1.0, a, b);
            if (!inters.empty()) {
                ld l = getLocation(a, b, inters[0]);
                ld r = getLocation(a, b, inters.back());
                if (l > r) {
                    std::swap(l, r);
                }
                evts.emplace_back(l - EPS, 1);
                evts.emplace_back(r + EPS, -1);
            }
        }
        std::sort(evts.begin(), evts.end());
        if (evts.empty()) {
            return false;
        }

        bool ok = false;
        ld prev = -EPS, total = 0.0;
        int count = 0;
        for (auto [pos, add] : evts) {
            count += add;
            if (ok) {
                total += pos - prev;
            }
            ok = count >= 3;
            prev = pos;
        }
        return total >= 1.0 - EPS;
    };

    std::vector<std::vector<ld>> dist(k, std::vector<ld>(k, INF));
    for (int i = 0; i < k; i++) {
        dist[i][i] = 0;
        for (int j = i + 1; j < k; j++) {
            if (validate(p[i], p[j])) {
                dist[i][j] = dist[j][i] = (p[i] - p[j]).dist();
            }
        }
    }

    std::vector<ld> dists(k, INF);
    dists[0] = 0;
    std::priority_queue<Node, std::vector<Node>, cmp> pq;
    std::vector<bool> vis(k);
    pq.push({0, 0.});
    while (!pq.empty()) {
        auto [id, d] = pq.top();
        pq.pop();
        if (vis[id]) {
            continue;
        }
        vis[id] = true;
        for (int next = 0; next < k; next++) {
            if (!vis[next] && d + dist[id][next] < dists[next]) {
                dists[next] = d + dist[id][next];
                pq.push({next, dists[next]});
            }
        }
    }

    P dst = p[n - 1];
    ld ans = INF;
    for (int i = 0; i < k; i++) {
        if (vis[i]) {
            P diff = p[i] - dst;
            ld d = diff.dist();
            if (d <= 1) {
                ans = std::min(ans, dists[i]);
            } else {
                P to = dst + diff * (1.0 / d);
                if (validate(p[i], to)) {
                    ans = std::min(ans, dists[i] + d - 1);
                }
            }
        }
    }

    if (ans < INF / 2) {
        std::cout << std::fixed << std::setprecision(12) << ans << "\n";
    } else {
        std::cout << -1 << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();

    return 0;
}
