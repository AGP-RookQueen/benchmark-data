#include <bits/stdc++.h>
using namespace std;

void transpose (vector<vector<char> > &g) {
  for (int i = 0; i < (int) g.size(); i++) {
     for (int j = i; j < (int) g[0].size(); j++) {
       swap (g[i][j], g[j][i]);
     }
   }
}

void invert (vector<vector<char> > &g) {
  for (int i = 0; i < (int) g.size(); i++) {
     for (int j = 0, k = (int) g[0].size() -1; j < k; j++, k--) {
       swap (g[i][j], g[i][k]);
     }
   }
}

int main() {
  int tt = 1;
  while (tt--) {
    int n;
    cin >> n;
    vector< pair<int, int> > vertices(n); // Given in CCW
    for (auto& [x, y] : vertices) cin >> x >> y;
    
     
    const int N = n / 2 + 5;
    
    vector< vector<char> > mat(N, vector<char>(N, '.'));
    
    auto is_inside = [&](int X, int lo, int hi) {
      
      for (int i = 0; i < 2 * n; i++) {
        auto [x1, y1] = vertices[i % n];
        auto [x2, y2] = vertices[(i + 1) % n];
        
        if (x1 == x2 && x1 == X) {
          
          int mn = min(y1, y2);
          int mx = max(y1, y2);
          
          if (mn <= lo && hi <= mx) {
            return 1;
          }
          
        }
        
      }
      
      return 0;
    };
    
    for (int i = 0; i < N; i++) {
      
      int crossings = 0;
      int column = -1;
      
      for (int j = 0; j < N; j++) {
        // move to right
        ++column;
        
        // cout << "Q: " << column << ' ' << i << ' ' << i + 1 << ' ' << is_inside(column, i, i + 1) << '\n';
        if (is_inside(column, i, i + 1)) {
          // cout << "YeES" << ' ' << i << ' ' << i + 1 << ' ' << column << '\n';
          ++crossings;
        }
        if (crossings & 1) {
          mat[i][j] = '#';
        }
        
      }
      // exit(0);
      
    }
    
    transpose(mat);
    invert(mat);
    transpose(mat);
    invert(mat);
    invert(mat);
    
    // cout << N << ' ' << N << '\n';
    // for (int i = 0; i < N; i++) {
      // for (int j = 0; j < N; j++) {
        // cout << mat[i][j];
      // }
      // cout << '\n';
    // }
    
    vector<int> rows;
    vector<int> cols;
    
    for (int i = 0; i < N; i++) {
      int ok = 0;
      for (int j = 0; j < N; j++) {
        ok |= mat[i][j] == '#';
      }
      if (ok) rows.emplace_back(i);
    }
    for (int j = 0; j < N; j++) {
      int ok = 0;
      for (int i = 0; i < N; i++) {
        ok |= mat[i][j] == '#';
      }
      if (ok) cols.emplace_back(j);
    }
    cout << (int) rows.size() << ' ' << cols.size() << '\n';
    for (auto& i : rows) {
      for (auto& j : cols) {
        cout << mat[i][j];
      }
      cout << '\n';
    }
  }
}