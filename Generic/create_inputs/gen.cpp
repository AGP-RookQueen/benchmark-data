#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

using namespace chrono;


class Polyomino {
private:

static vector< pair<int, int> > P; 
static vector< int > rows; 
static vector< tuple<int, int, int> > H; 
static vector< tuple<int, int, int> > V; 
static int cells;

static vector< pair<int, int> > oldP; 
static vector< int > oldRows; 
static vector< tuple<int, int, int> > oldH; 
static vector< tuple<int, int, int> > oldV; 
static int oldCells;

//static int leftTurn(int ax, int ay, int bx, int by) {
  //return (ax * by) - (ay * bx) > 0;
//}

static void debug() {
  cout << "P = { ";
  for (auto [x, y] : P) cout << "{" << x << ',' << y << "}," << '\n';
  cout << " }\n";
  
  cout << "rows = { ";
  for (auto row : rows) cout << row << ",";
   cout << "}\n";
   
  //~ for (auto [y, l, r] : H) cout << y << ' ' << l <<  ' ' << r << '\n';
   //~ cout << '\n';
   
   //~ for (auto [x, l, r] : V) cout << y << ' ' << l <<  ' ' << r << '\n';
   //~ cout << '\n';
   
   cout << cells << '\n';
 //~ }
}

//static int countReflexVertices() {
  //int count = 0;
  //int numberVertices = (int) P.size();
  //for (int i = 0; i < numberVertices; i++) {
    //count += leftTurn( 
    
    //P[ i ].first -  P[ (i - 1 + numberVertices) % numberVertices  ].first,
    //P[ i ].second - P[ (i - 1 + numberVertices) % numberVertices ].second,
    
    //P[ (i + 1) % numberVertices ].first - P[ i ].first,
    //P[ (i + 1) % numberVertices ].second - P[ i ].second
    //);
    
  //}
  //return count;
//}

static int randomNumber(int n) { // inclusive
  return uniform_int_distribution<int>(1, n)(rng); 
}

static int sign(int x) {
  if (x > 0) return 1;
  if (x < 0) return -1;
  return 0;
}

static pair<int, int> findCellCoordinates(int C) {
  //assert(C <= (int) P.size());
  // Extract row and col of cell
  //cout << "TARGET:" << C << ' ' << cells << '\n';
  double row = 1;
  
  while (C - rows[row] > 0) {
    C -= rows[row];
    assert(rows[row] > 0);
    //assert(rows[row] != 0);
    ++row;
    
  }
  assert(C - rows[row] <= 0);
  //assert(row < (int) rows.s
  row += 0.5;
  int col = -1;
  
  //assert(P[0].first == 1);
  //assert(P[0].second == 1);
  //cout << "ROW" << ' ' << row << '\n';
  // Now find col
  int last = -1;
  //int check = 0;
  for (auto [x, l, r] : V) {
    if (l <= row && r >= row) {
      //cout << x << ' '0 << l << ' ' << r << '\n';
      if (last == -1) {
        last = x;
      } else {
        if (C - (x - last) > 0) {
          C -= (x - last);
        } else {
          col = last + C - 1;
          last = -1;
          break;
        }
        last = -1;
      }
    }
  }
  
    assert(last == -1);
  assert(col != -1);
  
  {
    last = -1;
    int check = 0;
    for (auto [x, l, r] : V) {
      if (l <= row && r >= row) {
        //cout << x << ' '0 << l << ' ' << r << '\n';
        if (last == -1) {
          last = x;
        } else {
          if (C - (x - last) > 0) {
            check += x - last;
          } else {

          }
        }
      }
    }
    //cout << "!!!" << check << ' ' << rows[row] << '\n';
    //assert(check == rows[row]);
  }

  return {col, row};
}

static int count(double fx, double fy, double ox, double oy) {
  int count = 0;
  double mnX = min(fx, ox);
  double mxX = max(fx, ox);
  double mnY = min(fy, oy);
  double mxY = max(fy, oy);
  for (int i = 0; i < (int) P.size(); i++) {
    if (P[i].first >= mnX && P[i].first <= mxX && P[i].second >= mnY && P[i].second <= mxY) ++count;
  }
  return count;
}

static vector< pair<int, int> > findCorners(double ox, double oy) {

 //for (int i = 0; i < (int) P.size(); i++) {
    //if (count(P[i].first, P[i].second, ox, oy) == 1) {
      //corner.emplace_back(P[i].first, P[i].second);
    //}
  //}
  // horizontal
  
  vector< pair<int, int> > corner;
  // horizontal
  {
    pair<int, int> p1 = {-1, -1};
    pair<int, int> p2 = {-1, -1};
    for (int i = 0; i < (int) H.size(); i++) {
      auto [y, l, r] = H[i];
      
      if (l <= ox && ox <= r && y < oy) {
        p1 = {l , y};
        p2 = {r , y};
      }
    }
    assert(p1.first != -1 && p2.first != -1);
    if (count(p1.first, p1.second, ox, oy) == 1) corner.emplace_back(p1);
    if (count(p2.first, p2.second, ox, oy) == 1) corner.emplace_back(p2);
  }
  
  {
    pair<int, int> p1 = {-1, -1};
    pair<int, int> p2 = {-1, -1};
    for (int i = H.size() - 1; i >= 0; i--) {
      auto [y, l, r] = H[i];
      
      if (l <= ox && ox <= r && y > oy) {
        p1 = {l , y};
        p2 = {r , y};
      }
    }
    assert(p1.first != -1 && p2.first != -1);
    if (count(p1.first, p1.second, ox, oy) == 1) corner.emplace_back(p1);
    if (count(p2.first, p2.second, ox, oy) == 1) corner.emplace_back(p2);
  }
  
  // vertical
  {
    pair<int, int> p1 = {-1, -1};
    pair<int, int> p2 = {-1, -1};
    for (int i = 0; i < (int) V.size(); i++) {
      auto [x, l, r] = V[i];
      
      if (l <= oy && oy <= r && x < ox) {
        p1 = {x , l};
        p2 = {x , r};
      }
    }
    assert(p1.first != -1 && p2.first != -1);
    if (count(p1.first, p1.second, ox, oy) == 1) corner.emplace_back(p1);
    if (count(p2.first, p2.second, ox, oy) == 1) corner.emplace_back(p2);
  }
  
  {
    pair<int, int> p1 = {-1, -1};
    pair<int, int> p2 = {-1, -1};
    for (int i = (int) V.size() - 1; i >= 0; i--) {
      auto [x, l, r] = V[i];
      
      if (l <= oy && oy <= r && x > ox) {
        p1 = {x , l};
        p2 = {x , r};
      }
    }
    assert(p1.first != -1 && p2.first != -1);
    if (count(p1.first, p1.second, ox, oy) == 1) corner.emplace_back(p1);
    if (count(p2.first, p2.second, ox, oy) == 1) corner.emplace_back(p2);
  }

  
  return corner;
}

static void inflatePoint(int& x, int &y, double p, double q) {
  if (x > p) x++;
  if (y > q) y++;
}

static void inflate(double p, double q) {
  for (auto& point : P) {
    inflatePoint(point.first, point.second, p, q);
  }
  //for (auto& 
  // must update row and counters!
  
//~ for (auto& row : rows) cout << row << ' ';
  //~ cout << '\n';
  
  int last = -1;
  for (auto [y, l, r] : H) {
    if (l <= p && p <= r) {
      if (last == -1) {
        last = y;
      } else {
        for (int j = last; j < y; j++) {
          //~ //cout << last << ' ' << y << '\n';
          ++cells;
          ++rows[j];
        }
        
        last = -1; 
      }
    }
  }
  assert(last == -1);
  //~ for (auto& row : rows) cout << row << ' ';
  //~ cout << '\n';
  
  //~ exit(0);
  //~ exit(0);
    // Fix H
  for (auto& [y, x1, x2] : H) {
    if (y > q) ++y;
    if (x1 > p) ++x1;
    if (x2 > p) ++x2;
  }
  
  // Fix V
  for (auto& [x, y1, y2] : V) {
    
    if (x > p) ++x;
    if (y1 > q) ++y1;
    if (y2 > q) ++y2;
    
  }
  //~ p += 1;
  //~ cout << rows[p] << '\n';
  cells += rows[q];
  //~ cout << cells << '\n';
  //~ cout << rows[p] << '\n';
  //~ cout << fixed << setprecision(2) << p << '\n';
  rows.insert(rows.begin() + q, rows[q]);  
  //~ for (auto& row : rows) cout << row << ' ';
  //~ cout << '\n';
}
  
  
  //for (auto& row : rows) cout << row << '\n';
  //exit(0);
  
static void rem (int x1, int y1, int x2, int y2) {
  if (x1 == x2) {
    assert(find(V.begin(), V.end(), make_tuple(x1, min(y1, y2), max(y1, y2))) != V.end());
    V.erase(find(V.begin(), V.end(), make_tuple(x1, min(y1, y2), max(y1, y2))));
  } else {
    assert(find(H.begin(), H.end(), make_tuple(y1, min(x1, x2), max(x1, x2))) != H.end());
    H.erase(find(H.begin(), H.end(), make_tuple(y1, min(x1, x2), max(x1, x2))));
  }
}

static void add (int x1, int y1, int x2, int y2) {
  if (x1 == x2) {
    assert(is_sorted(V.begin(), V.end()));
    V.insert
        (upper_bound( V.begin(), V.end(),  make_tuple(x1, min(y1, y2), max(y1, y2)) ),
          make_tuple(x1, min(y1, y2), max(y1, y2))
        );
    assert(is_sorted(V.begin(), V.end()));
  } else {
    
    //cout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << '\n';
    assert(is_sorted(H.begin(), H.end()));
    
    H.insert
        (upper_bound( H.begin(), H.end(),  make_tuple(y1, min(x1, x2), max(x1, x2)) ),
          make_tuple(y1, min(x1, x2), max(x1, x2))
        );
    assert(is_sorted(H.begin(), H.end()));
  }
}


public:
  static void printPolyomino() {
    int numberVertices = (int) P.size();
    cout << "\\documentclass{article}" << '\n';
    cout << "\\setlength{\\unitlength}{0,5cm}" << '\n';
    cout << "\\begin{document}" << '\n';
    cout << "\\begin{picture}(200,200)" << '\n'; // change later
    for (int i = 0; i < numberVertices; i++) P[i].second *= -1;
    for (int i = 0; i < numberVertices; i++) {
      int dx = P[ (i + 1) % numberVertices ].first - P[i].first;
      int dy = P[ (i + 1) % numberVertices ].second - P[i].second;
      cout << "\\put(" << P[i].first << "," << P[i].second << ")";
      cout << "{\\line(" << sign(dx) << "," << sign(dy) << "){" << max( abs(dx), abs(dy) )<< "}}" << '\n';
    }
    cout << "\\end{picture}" << '\n';
    cout << "\\end{document}" << '\n';
  }
  
  static void printPoints() {
    // cout << 1 << '\n';
    cout << P.size() << '\n';
    for (auto& [x, y] : P) {
      cout << x << ' ' << y << '\n';
    } 
  }

  static void generatePolyomino(int n) {
    assert(n >= 4 && (n % 2) == 0);
    // Unit Square, in CCW order - northwest corner has coordinates (1,1).
    P = { {1, 1}, {1, 2}, {2, 2}, {2, 1} };
    //~ P = { {2,1},
//~ {2,2},
//~ {1,2},
//~ {1,3},
//~ {3,3},
//~ {3,1}
 //~ };
//~ rows = { 0,1,2 };
    //~ rows = {0, 1};
    //P = { {1, 1}, {1, 6}, {2, 6}, {2, 4}, {4, 4}, {4, 3}, {3, 3}, {3, 2}, {5, 2}, {5, 5}, {6, 5}, {6, 1} };
    cells = 1;
    
    //~ printPolyomino();
    //~ exit(0);
    //cells = 15;
    rows = {0, 1};
    //rows = {0, 5, 3, 4, 2, 1};
    
    V.clear();
    H.clear();
    int numberVertices = (int) P.size();
    for (int i = 0; i < (int) P.size(); i++) {
      if (P[i].first == P[ ((i - 1) + numberVertices) % numberVertices].first) {
        V.emplace_back(P[i].first, min(P[i].second, P[((i - 1) + numberVertices) % numberVertices].second), max(P[i].second, P[((i - 1) + numberVertices) % numberVertices].second));
      } else {
        assert(P[i].second == P[((i - 1) + numberVertices) % numberVertices].second);
        H.emplace_back(P[i].second, min(P[i].first, P[((i - 1) + numberVertices) % numberVertices].first), max(P[i].first, P[((i - 1) + numberVertices) % numberVertices].first));
      }
    }
    sort(V.begin()  , V.end());
    sort(H.begin(), H.end());
    
    //for (auto& [a, b, c] : V) cout << a << ' ' << b << ' ' << c << '\n';
    //exit(0);
    
    //inflate(3, 3);
    //printPolyomino();
    //exit(0);
    
    int r = n / 2 - 2;
    while (r > 0) {
      
      oldRows = rows;
      oldH = H;
      oldV = V;
      oldP = P;
      
      numberVertices = (int) P.size();
      int p, q;
      int xm, ym;
      
      //~ int choices = 4;
      //~ int C;
      while (true) {
        //~ int C = 2;
        //~ C = 2;
        int C = randomNumber(cells);
        auto [x, y] = findCellCoordinates(C);
        
        p = x; q = y;
        vector< pair<int, int> > vm = findCorners(x + 0.5, y + 0.5);
        sort(vm.begin(), vm.end());
        vm.erase(unique(vm.begin(), vm.end()), vm.end());
        if (vm.empty()) continue;
        //~ choices = randomNumber( (int) vm.size() ) - 1;
        auto [xx, yy] = vm[ randomNumber( (int) vm.size() ) - 1 ];
        
        //~ for (auto [x, y] : vm) cout << x << ' ' << y << '\n';
        //~ exit(0);
        
        //auto [xx, yy] = vm[ 0 ];
        xm = xx; ym = yy;
        break;
      }
      //~ cout << C << ' ' << choices << '\n';
      int i = find(P.begin(), P.end(), make_pair(xm, ym)) - P.begin();
      
      //exit(0);
      
      inflate(p + 0.5, q + 0.5);
      inflatePoint(xm, ym, p + 0.5, q + 0.5);
      //~ printPolyomino();   
      //~ for (auto& row : rows) cout << row << '\n';
      //~ exit(0);
      
      //cout << xm << ' ' << ym << '\n';
      
      rem(P[i].first, P[i].second, P[(i - 1 + numberVertices) % numberVertices].first, P[ (i - 1 + numberVertices) % numberVertices].second);
      rem(P[(i + 1) % numberVertices].first, P[(i + 1) % numberVertices].second, P[i].first, P[i].second);
      //exit(0);
      // now check which point to replace
      i = find(P.begin(), P.end(), make_pair(xm, ym)) - P.begin();
      assert(P[i].first == xm);
      assert(P[i].second == ym);
      
      
      if ( P[ (i - 1 + numberVertices) % numberVertices].second == ym) { // horizontal
        
          
          
          
          
          //
          //add(xm, q + 1, p + 1, q + 1);
          //add(p + 1, q + 1, p + 1, ym);
          //add(p + 1, ym, P[i + 1].first, P[i + 1].second);
          
          add(P[(i - 1 + numberVertices) % numberVertices].first, P[(i - 1 + numberVertices) % numberVertices].second, p + 1, ym);
          add(p + 1, ym, p + 1, q + 1);
          add(p + 1, q + 1, xm, q + 1);
          add(xm, q + 1, P[(i + 1) % numberVertices].first, P[(i + 1) % numberVertices].second);
          
          //exit(0);
          
          P.erase(P.begin() + i);
          auto it = P.begin() + i;
          it = P.insert(it, {xm, q + 1});
          it = P.insert(it, {p + 1, q + 1});
          P.insert(it, {p + 1, ym});
          
          // update cell count
          for (int i = min(ym, q + 1); i < max(ym, q + 1); i++) {
            assert(rows[i] > 0);
            rows[i] -= max(xm, p + 1) - min(xm, p + 1);
            //~ if (rows[i] == 0) {
              //~ cout << xm << ' ' << ym << ' ' << p + 1 << ' ' << q + 1 << '\n';
              //~ cout << max(xm, p + 1) - min(xm, p + 1) << '\n';
              //~ exit(0);
            //~ }
            //~ assert(rows[i] > 0);
            cells -= max(xm, p + 1) - min(xm, p + 1);
            //cout << i << ' ' << max(xm, p + 1) - min(xm, p + 1) << '\n';
          }
          
      } else {
        
          
          
          add(P[(i - 1 + numberVertices) % numberVertices].first, P[(i - 1 + numberVertices) % numberVertices].second, xm, q + 1);
          add( xm, q + 1, p + 1, q + 1);
          add(p + 1, q + 1, p + 1, ym);
          add(p + 1, ym, P[(i + 1) % numberVertices].first, P[(i + 1) % numberVertices].second);
          
          P.erase(P.begin() + i);
          auto it = P.begin() + i;
          it = P.insert(it, {p + 1, ym});
          it = P.insert(it, {p + 1, q + 1});
          P.insert(it, {xm, q + 1});
          
          
          // update cell count
          //cout << "CELLS: " << cells << '\n';
          for (int i = min(ym, q + 1); i < max(ym, q + 1); i++) {
            rows[i] -= max(xm, p + 1) - min(xm, p + 1);
            cells -= max(xm, p + 1) - min(xm, p + 1);
            
            //cout << max(xm, p + 1) - min(xm, p + 1) << '\n';
            
            //cout << i << ' ' << max(xm, p + 1) - min(xm, p + 1) << '\n';
            //cout << i << ' ' << max(xm, p + 1) - min(xm, p + 1) << '\n';
          }
      }
      assert(accumulate(rows.begin(), rows.end(), 0) == cells);
      
      
       //V.clear();
      //H.clear();
      
      vector< tuple<int, int, int> > H1; 
      vector< tuple<int, int, int> > V1; 
      
      int numberVertices = (int) P.size();
      for (int i = 0; i < (int) P.size(); i++) {
        if (P[i].first == P[ ((i - 1) + numberVertices) % numberVertices].first) {
          V1.emplace_back(P[i].first, min(P[i].second, P[((i - 1) + numberVertices) % numberVertices].second), max(P[i].second, P[((i - 1) + numberVertices) % numberVertices].second));
        } else {
          assert(P[i].second == P[((i - 1) + numberVertices) % numberVertices].second);
          H1.emplace_back(P[i].second, min(P[i].first, P[((i - 1) + numberVertices) % numberVertices].first), max(P[i].first, P[((i - 1) + numberVertices) % numberVertices].first));
        }
      }
      sort(V1.begin()  , V1.end());
      sort(H1.begin(), H1.end());
    
      //~ if (V1 != V) {
        //~ cout << "BLAH" << '\n';
      //~ }
      //~ if (H1 != H) {
        //~ cout << "BLAH" << '\n';
      //~ }
      assert(V1 == V);
      assert(H1 == H);
      //cout << cells << '\n';
      //for (auto row : rows) cout << "!" << row << '\n';
      //cout << '\n';
      
      //for (auto [x, y] : P) cout << "!" << x << ' ' << y << '\n';
      //cout << '\n';
      
      //for (auto [y, l, r] : H) cout << "!" << y << ' ' << l << ' ' << r << '\n';
      //cout << '\n';
      
      //for (auto [y, l, r] : V) cout << "!" << y << ' ' << l << ' ' << r << '\n';
      //cout << '\n';
      
      //for (auto [x, y] : V) cout << "!" << x << ' ' << y << '\n';
      //cout << '\n';
      //exit(0);
      //break;
      
      r -= 1;
      for (int i = 1; i < (int) rows.size(); i++) {
        assert(rows[i] > 0);
        //~ if (rows[i] == 0) {
          //~ assert(oldRows[i] != 0);
          //~ cout << "WRONG" << '\n';
          //cout << "YES" << '\n';
          //~ P = oldP;
          //~ V = oldV;
          //~ H = oldH;
          //~ rows = oldRows;
          //~ cells = oldCells;
          //~ cout << C << ' ' << choices << '\n';
          //~ cout << "GOT" << '\n';
          //~ printPolyomino();
          //~ debug();
          
          //~ printPolyomino();
          //~ cout << xm << ' ' << ym << '\n';
          //~ cout << p + 1 << ' ' << q + 1 << '\n';
          //~ cout << cells << '\n';
          //~ for (auto rr : rows) cout << rr << ' ';
          //~ cout << '\n';
          //~ exit(0);
        //~ }
        //assert(rows[i] != 0);
        //assert(rows[i] != 0);
      }
      //~ exit(0);
      //for (auto& row : rows) {
        //assert(row != 0);
        //cout << row << '\n';
      //}
    }
    //~ cout << "OK!" << '\n';
    assert(P.size() == n);
    //~ cout << n << '\n';
    //~ printPolyomino();
    printPoints();
    //~ exit(0);
  }
};

vector< pair<int, int> > Polyomino::P;
vector< int > Polyomino::rows;
vector< tuple<int, int, int> > Polyomino::H;
vector< tuple<int, int, int> > Polyomino::V;
int Polyomino::cells;



vector< pair<int, int> > Polyomino::oldP;
vector< int > Polyomino::oldRows;
vector< tuple<int, int, int> > Polyomino::oldH;
vector< tuple<int, int, int> > Polyomino::oldV;
int Polyomino::oldCells;


int main(int argc, char *argv[]) {
  assert(argc == 2); // assert that size was provided
  int n = atoi(argv[1]);
  Polyomino::generatePolyomino(n);
}

//for i in {0..200}; do ./inflate_cut >> a; done;


//\documentclass{article}
//\setlength{\unitlength}{0,5cm}
//\begin{document}
//\begin{picture}(200,30)
//\put(1,-1){\line(0,-1){3}}
//\put(1,-4){\line(1,0){1}}
//\put(2,-4){\line(0,1){1}}
//\put(2,-3){\line(1,0){1}}
//\put(3,-3){\line(0,1){1}}
//\put(3,-2){\line(1,0){1}}
//\put(4,-2){\line(0,1){1}}
//\put(4,-1){\line(-1,0){3}}
//\end{picture}
//\end{document}
//0 3 0 1 