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
  
  int rows, cols;
  cin >> rows >> cols;
  
  vector< vector<char> > mat(rows, vector<char>(cols));
  
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      cin >> mat[i][j];
    }
  }

  const int N = 3000;
  vector< vector<int> > lattice(N, vector<int>(N));
  
  const int OFF = 550;
  
  for(int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (mat[i][j] == '.') continue;
      
      int my_row = 3 * (OFF + i);
      int my_col = 3 * (OFF + j);
      lattice[my_row][my_col] = 1;
      lattice[my_row][my_col + 1] = 1;
      lattice[my_row][my_col + 2] = 1;
      
      lattice[my_row + 1][my_col] = 1;
      lattice[my_row + 1][my_col + 1] = 1;
      lattice[my_row + 1][my_col + 2] = 1;
      
      lattice[my_row + 2][my_col] = 1;
      lattice[my_row + 2][my_col + 1] = 1;
      lattice[my_row + 2][my_col + 2] = 1;
    }
  }
  
  transpose(lattice);
  invert(lattice);

  
  queue< pair<int, int> > q;
  vector< vector<int> > vis(N, vector<int>(N));
  
  vector<int> dr = {+1, -1, +0, +0};
  vector<int> ddr = {+1, -1, +0, +0, +1, -1, +1, -1};
  vector<int> dc = {+0, +0, +1, -1};
  vector<int> ddc = {+0, +0, +1, -1, +1, -1, -1, +1};
  
  q.emplace(0, 0);
  vis[0][0] = 1;
  
  int vertices = 0;
  
  while (!q.empty()) {
    auto [r, c] = q.front();
    q.pop();
    for (int i = 0; i < 8; i++) {
      int rr = r + ddr[i];
      int cc = c + ddc[i];
      if (rr < 0 || rr >= N || cc < 0 || cc >= N) continue;
      if (vis[rr][cc]) continue;
      if (lattice[rr][cc] >= 1) {
        if (lattice[rr][cc] == 1) ++vertices;
        lattice[rr][cc] = 2;
        continue;
      }
      vis[rr][cc] = 1;
      q.emplace(rr, cc);
    }
  }
  
    // for (int i = 0; i < N; i++) {
    // for (int j = 0; j < N; j++) {
      // cout << lattice[i][j] << ' ';
    // }
    // cout << '\n';
  // }
  // exit(0);
  
  
  cout << vertices << '\n';
  
  auto getPath = [&](int sr, int sc) {
    
    vector< vector<int> > vis(N, vector<int>(N));
    
    
    vector<int> dor = {+0, +1, +0, -1};
    vector<int> doc = {+1, +0, -1, +0};
    
    int dir = 0;
    
    for (int i = 0; i < vertices - 1; i++) {
      
      // cout << sr << ' ' << sc << endl;
      vis[sr][sc] = 1;
      
      cout << sr - 3 * OFF - 300 << ' ' << sc - 3 * OFF + 850 << endl;
      
      // int rr = sr + dor[dir];
      // int cc = sc + doc[dir];
      
      auto valid = [&](int rrr, int ccc) {
        if (vis[rrr][ccc]) return 0;
        if (lattice[rrr][ccc] != 2) return 0;
        
        return 1;
      };
      
      
      while (!valid(sr + dor[dir], sc + doc[dir])) {
        ++dir;
        dir %= 4;
        // rr = sr + dor[dir];
        // cc = sc + doc[dir];
      }
      
      sr += dor[dir];
      sc += doc[dir];
      
      // if (!vis[rr][cc] && lattice[rr][cc] == 2) {
        // sr = rr;
        // sc = cc;
        // vis[rr][cc] = 1;
        // continue;
      // } else {
        // ++dir;
        // dir %= 4;
      // }
      
      
      // for (int i = 0; i < 4; i++) {
        
        // int rr = sr + dr[i];
        // int cc = sc + dc[i];
        
        // if (rr < 0 || rr >= N || cc < 0 || cc >= N) continue;
        // if (vis[rr][cc]) continue;
        // if (lattice[rr][cc] != 2) continue;
        // sr = rr;
        // sc = cc;
        // vis[sr][sc] = 1;
        // break;
      // }
    }
    cout << sr - 3 * OFF - 300 << ' ' << sc - 3 * OFF + 850 << endl;
    
  };
  
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (lattice[i][j] == 2) {
        getPath(i, j);
        exit(0);
      }
    }
  }
  return 0;
}

  // 2 2 2 2 2 2 
  // 0 0 0 0 2 2
  // 0 0 0 0 2 2
  // 2 2 2 2 2 2
  // 2 2 2 2 2 2
