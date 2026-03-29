#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
using ui64 = unsigned long long;
ui64 gerar(ui64 lo, ui64 hi) { return uniform_int_distribution<ui64>(lo,hi)(rng); }
using namespace chrono;

int main(int argc, char* argv[]) {

  int vertices = stoi(argv[1]);
 // cin >> vertices;

  vector<int> dp(vertices + 1, 0);

  dp[0] = 1;
  for (int i = 1; i <= vertices; i++) {
    if (i - 2 >= 0) dp[i] |= dp[i - 2];
    if (i - 4 >= 0) dp[i] |= dp[i - 4];
  }

  assert(dp[vertices]);
  
  const int ST = gerar(0, 20);
  vector< pair<int, int> > segs;
  segs.emplace_back( 0, ST );

  vertices -= 4;
  //exit(0);
  while (vertices > 0) {

    int LO = segs.back().first;
    int HI = segs.back().second;

    int rng = gerar(0, 5);
    assert(dp[vertices]);
    if (rng == 0) segs.emplace_back(segs.back());
    else if (rng <= 3) {
      if (vertices - 2 >= 0 && dp[vertices - 2]) {
        // decrease just one
        int side = gerar(0, 1);
        if (side == 0) {
          int x = LO;
          int y = gerar(HI + 1, (HI + 1 + gerar(1, 8)));
          segs.emplace_back(x, y);
          vertices -= 2;
        } else if (LO < HI) {
          int x = gerar(LO + 1, min(HI, (int) (LO + 1 + gerar(1, 8))));
          int y = HI;
          segs.emplace_back(x, y);
          vertices -= 2;
        } 
        
      }
    }  else {
      if (vertices - 4 >= 0 && dp[vertices - 4] && LO < HI) {
        // both
        int x = gerar(LO + 1, min(HI, (int) (LO + 1 + gerar(1, 8))));
        int y = gerar(HI + 1, (int) (HI + 1 + gerar(1, 8)));
        vertices -= 4;
        segs.emplace_back(x, y);
      }
    }
  }
 

  int n = (int) segs.back().second + 1;
  int m = (int) segs.size();
  cout << n << ' ' << m << '\n';
  vector< vector<char> > mat(n, vector<char>(m, '.'));

  for (int j = 0; j < m; j++) {
    auto [l, r] = segs[j];
    for (int i = l; i <= r; i++) {
      mat[i][j] = '#';
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cout << mat[i][j];
    }
    cout << '\n';
  }
  return 0;
}
