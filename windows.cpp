/**
 *      Author:  Richw818
 *      Created: 04.24.2023 21:10:11
**/

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int commandNumber = 1;
    int xmax, ymax;
    cin >> xmax >> ymax;
    string command;
    vector<int> x1, x2, y1, y2;
    while (cin >> command) {
        if (command == "OPEN") {
            int x, y, w, h;
            cin >> x >> y >> w >> h;
            bool hit = (x + w > xmax || y + h > ymax);
            for (int i = 0; i < (int) x1.size(); i++) {
                int lx = max(x1[i], x), rx = min(x2[i], x + w - 1);
                int ly = max(y1[i], y), ry = min(y2[i], y + h - 1);
                if (lx <= rx && ly <= ry) {
                    hit = true;
                }
            }
            if (hit) {
                cout << "Command " << commandNumber << ": " << command << " - " << "window does not fit\n";
            } else {
                x1.push_back(x);
                x2.push_back(x + w - 1);
                y1.push_back(y);
                y2.push_back(y + h - 1);
            }
        } else if (command == "RESIZE") {
            int x, y, w, h;
            cin >> x >> y >> w >> h;
            int win;
            for (win = 0; win < (int) x1.size(); win++) {
                if (x >= x1[win] && x <= x2[win] && y >= y1[win] && y <= y2[win]) {
                    break;
                }
            }
            if (win == (int) x1.size()) {
                cout << "Command " << commandNumber << ": " << command << " - " << "no window at given position\n";
            } else {
                x = x1[win], y = y1[win];
                bool hit = (x + w > xmax || y + h > ymax);
                for (int i = 0 ; i < (int) x1.size(); i++) {
                    if (i == win) {
                        continue;
                    }
                    int lx = max(x1[i], x), rx = min(x2[i], x + w - 1);
                    int ly = max(y1[i], y), ry = min(y2[i], y + h - 1);
                    if (lx <= rx && ly <= ry) {
                        hit = true;
                    }
                }
                if (hit) {
                    cout << "Command " << commandNumber << ": " << command << " - " << "window does not fit\n";
                } else {
                    x2[win] = x + w - 1;
                    y2[win] = y + h - 1;
                }
            }
        } else if (command == "CLOSE") {
            int x, y;
            cin >> x >> y;
            int win;
            for (win = 0; win < (int) x1.size(); win++) {
                if (x >= x1[win] && x <= x2[win] && y >= y1[win] && y <= y2[win]) {
                    break;
                }
            }
            if (win == (int) x1.size()) {
                cout << "Command " << commandNumber << ": " << command << " - " << "no window at given position\n";
            } else {
                x1.erase(x1.begin() + win);
                x2.erase(x2.begin() + win);
                y1.erase(y1.begin() + win);
                y2.erase(y2.begin() + win);
            }
        } else {
            int x, y, dx, dy;
            cin >> x >> y >> dx >> dy;
            int win;
            for (win = 0; win < (int) x1.size(); win++) {
                if (x >= x1[win] && x <= x2[win] && y >= y1[win] && y <= y2[win]) {
                    break;
                }
            }
            if (win == (int) x1.size()) {
                cout << "Command " << commandNumber << ": " << command << " - " << "no window at given position\n";
                commandNumber++;
                continue;
            }
            bool ymove = dy != 0;
            if (ymove) {
                swap(x1, y1);
                swap(x2, y2);
                swap(xmax, ymax);
                swap(dx, dy);
            }
            bool flipped = dx < 0;
            if (flipped) {
                for (int i = 0; i < (int) x1.size(); i++) {
                    x1[i] = xmax - x1[i] - 1;
                    x2[i] = xmax - x2[i] - 1;
                }
                swap(x1, x2);
                dx *= -1;
            }
            int wantToMove = dx;
            // Do the move
            vector<bool> inGroup((int) x1.size());
            inGroup[win] = true;
            while (true) {
                int nextAdd = -1, minGap = dx;
                for (int i = 0; i < (int) x1.size(); i++) {
                    if (inGroup[i]) {
                        minGap = min(minGap, xmax - x2[i] - 1);
                    }
                }
                for (int i = 0; i < (int) x1.size(); i++) {
                    if (inGroup[i]) {
                        for (int j = 0; j < (int) x1.size(); j++) {
                            if (!inGroup[j]) {
                                if (x1[j] > x2[i]) {
                                    int ly = max(y1[i], y1[j]), ry = min(y2[i], y2[j]);
                                    if (ly <= ry) {
                                        int gap = x1[j] - x2[i] - 1;
                                        if (gap < minGap) {
                                            minGap = gap;
                                            nextAdd = j;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                dx -= minGap;
                for (int i = 0; i < (int) x1.size(); i++) {
                    if (inGroup[i]) {
                        x1[i] += minGap;
                        x2[i] += minGap;
                    }
                }
                if (nextAdd == -1) {
                    break;
                } else {
                    inGroup[nextAdd] = true;
                }
            }
            if (dx != 0) {
                cout << "Command " << commandNumber << ": " << command << " - " << "moved " << (wantToMove - dx) << " instead of " << wantToMove << '\n';
            }
            if (flipped) {
                for (int i = 0; i < (int) x1.size(); i++) {
                    x1[i] = xmax - x1[i] - 1;
                    x2[i] = xmax - x2[i] - 1;
                }
                swap(x1, x2);
            }
            if (ymove) {
                swap(x1, y1);
                swap(x2, y2);
                swap(xmax, ymax);
            }
        }
        commandNumber++;
    }
    cout << (int) x1.size() << " window(s):\n";
    for (int i = 0; i < (int) x1.size(); i++) {
        cout << x1[i] << ' ' << y1[i] << ' ' << x2[i] - x1[i] + 1 << ' ' << y2[i] - y1[i] + 1 << '\n';
    }
    return 0;
}
