#include <bits/stdc++.h>
using namespace std;

void transpose (vector<vector<int> > &g) {
  for (int i = 0; i < (int) g.size(); i++) {
     for (int j = i; j < (int) g[0].size(); j++) {
       swap (g[i][j], g[j][i]);
     }
   }
}

void invert (vector<vector<int> > &g) {
  for (int i = 0; i < (int) g.size(); i++) {
     for (int j = 0, k = (int) g[0].size() -1; j < k; j++, k--) {
       swap (g[i][j], g[i][k]);
     }
   }
}

int main() {
  
  ios::sync_with_stdio(0);
  cin.tie(0);
  
  int n;
  cin >> n;
  vector< pair<int, int> > points(n);
  for (auto& [x, y] : points) cin >> x >> y;
  int count = 0;
  vector< pair<int, int> > final_points;
  for (int i = 0; i < n; i++) {
    auto [x1, y1] = points[(i - 1 + n) % n];
    auto [x2, y2] = points[(i + 1) % n];
    
    auto [x, y] = points[i];
    
    int del = 0;
    
    if (x1 == x2) {
      assert( min(y1, y2) <= y && max(y1, y2) >= y );
      del = 1;
    }
    if (y1 == y2) {
      assert( min(x1, x2) <= x && max(x1, x2) >= x );
      del = 1;
    }
    if (!del) {
      ++count;
      final_points.emplace_back(points[i]);
    }
  }
  cout << (int) final_points.size() << '\n';
  for (auto& [x, y] : final_points) {
    cout << x << ' ' << y << '\n';
  }
}

  // 2 2 2 2 2 2 
  // 0 0 0 0 2 2
  // 0 0 0 0 2 2
  // 2 2 2 2 2 2
  // 2 2 2 2 2 2
