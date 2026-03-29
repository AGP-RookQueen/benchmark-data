 
/*---------------------------------------------------------------------------*\
 
   Copyright (c) 2025 Ana Paula Tomas  aptomas _AT_ fc.up.pt
    DCC-FCUP & CMUP, Universidade do Porto
 
#    This program is free software; you can redistribute it and/or modify
#      it under the terms of the GNU General Public License as published by
#      the Free Software Foundation; either version 2 of the License, or
#      (at your option) any later version.
#
#      This program is distributed in the hope that it will be useful,
#      but WITHOUT ANY WARRANTY; without even the implied warranty of
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#      GNU General Public License for more details.
#
#      You should have received a copy of the GNU General Public License
#      along with this program.
\*---------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------\
 
 
   To create convex grid-orthogonal polygons by Inflate-Paste
   (i.e., convex polyominoes induced by grid ogons)
 
   File: Convex.c
   Last changed: March 2025
 
   Input: the number of polygons that will be generated
          and their number of vertices
 
   Output: the number of vertices 
           and the coordinates in CCW order 
           --- wrt (O,(1,0),(0,1)) (i.e., (East,North)) coordinate system
 
      Usage:
 
         ./convex fileCoords fileLaTeX
          // pictures fo to fileLaTeX
 
     ./convex file
         // pictures and coordinates output to file
 
         ./convex 
         // all output goes to stdout
 
 
\-----------------------------------------------------------------------*/
 
 
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
 
 
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
using ui64 = unsigned long long;
ui64 gerar(ui64 lo, ui64 hi) { assert(lo <= hi); return uniform_int_distribution<ui64>(lo,hi)(rng); }
using namespace chrono;
 
typedef struct verts {
  int x, y;
  struct verts *nxt, *prev;
} *PVERTS, VERT;
 
 
PVERTS Polygon, LhsVert, RhsVert;
 
// To introduce collinearities
#define COLLINEAR
// first makes some horizontal edges collinear
// then rotates the polygon by 90º CW and does the same
 
// Preferentially choosing cells to the left or to the right of the top edge
#define BIASED
#define PERCENTAGE 3
// > 0.3 
 
// To rotate the permutomino by 90 or 180 CW
// (just after creating it) 
//#define ROTATE
 
// To draw the polygons using LaTeX
//#define TOLATEX
 
// To output the coordinates in CCW
#define COORDS
 
 
 
#define ABS(X) ((X) < 0 ? -(X) : (X))
#define MIN(A,B) ((A) < (B)? (A): (B))
#define MAX(A,B) ((A) > (B)? (A): (B))
#define VERTNXT(P)  ((P)->nxt)
#define VERTPREV(P)  ((P)->prev)
#define VERTX(P) ((P)->x)
#define VERTY(P) ((P)->y)
 
// to create a new vertex
PVERTS  newVertex(int, int, PVERTS, PVERTS);
// to create the initial square
void square1();
// to inflate the polygon
void inflate_polygon(int);
// to link the new chain
void paste(int flag, PVERTS top, PVERTS bot);
void convex_Paste(int n);
void genConvex(int n, FILE *,FILE *);
void rotateCW90(PVERTS,int n);
// frees dynamic memory
void free_polygon(PVERTS);
// writes a grid-ogon
void outputPolygon(FILE *,PVERTS,int);  
void drawPolygon(FILE *,PVERTS,int);  
void outputCoords(FILE *,PVERTS,int);  
 
#ifdef COLLINEAR
int collinear_horizontal(PVERTS p,int n);
#ifdef VERBOSE
void collinear(FILE *frLaTeX,int n);
#endif
#ifndef VERBOSE
void collinear(int n);
#endif
#endif
 
 
#ifdef TOLATEX
void outputCoordsLaTeX(FILE *,PVERTS,int); 
void showPol(FILE *,PVERTS,int);   
#endif
 
 
//=======================================================
//  Inflate-Paste to create a convex permutomino
//=======================================================
 
PVERTS  newVertex(int x, int y, PVERTS prevV, PVERTS nxtV)
{
  PVERTS newV = (PVERTS) malloc(sizeof(VERT));
  VERTPREV(newV) = prevV;  VERTNXT(newV) = nxtV;
  VERTX(newV) = x;   VERTY(newV) = y; 
  return newV;
}
 
void square1()
{ 
  PVERTS aux;
  Polygon = aux = newVertex(0,0,NULL,NULL);
  VERTNXT(aux) = newVertex(1,0,aux,NULL);
  aux = VERTNXT(aux);
  VERTNXT(aux) = newVertex(1,1,aux,NULL);
  aux = VERTNXT(aux); RhsVert = aux;
  VERTNXT(aux) = newVertex(0,1,aux,Polygon);
  VERTPREV(Polygon) = LhsVert = VERTNXT(aux); 
}
 
void inflate_polygon(int jcell)
{ // updating vertices due to the insertion of new column
  PVERTS pv = Polygon;
  do {
    if (VERTX(pv) > jcell) VERTX(pv)++;
    pv = VERTNXT(pv);
  } while (pv != Polygon);
}
 
void paste(int rhs, PVERTS top, PVERTS bot)
{
  if (rhs) {
    VERTY(RhsVert)++;
    VERTNXT(RhsVert) = top; VERTPREV(top) = RhsVert;
    VERTNXT(top) = bot; VERTPREV(bot) = top;
    VERTNXT(bot) = LhsVert; VERTPREV(LhsVert) = bot;
    LhsVert = top;
  } else {
    VERTY(LhsVert)++;
    VERTNXT(RhsVert) = bot; VERTPREV(bot) = RhsVert;
    VERTNXT(bot) = top; VERTPREV(top) = bot;
    VERTNXT(top) = LhsVert;  VERTPREV(LhsVert) = top;
    RhsVert = top;
  }
}
 
 
 
void genConvex(int n,FILE *frCoords,FILE *frLaTeX) {
  convex_Paste(n);
 
#ifdef ROTATE
  rotateCW90(Polygon,n);
  if (rand()%2) rotateCW90(Polygon,n);
#endif
  
#ifdef COLLINEAR
#ifdef VERBOSE
  collinear(frLaTeX,n);
#endif
#ifndef VERBOSE
  collinear(n);
#endif
#endif
 
#ifdef TOLATEX
  showPol(frLaTeX,Polygon,n);  
#endif
 
#ifdef COORDS
  outputCoords(frCoords,Polygon,n);  
#endif
 
  free_polygon(Polygon); 
}
 
 
void convex_Paste(int n)
{
  int r, cell;
  int xlhs, xrhs;
  square1();
  PVERTS v0, v1;
  int side, yv, xc;
  int okLeft = 1, okRight = 1;  // not blocked
  
  for (r=0; r<n/2-2; r++) {
    // select a cell
    xlhs = VERTX(LhsVert);  xrhs = VERTX(RhsVert);
    yv = VERTY(LhsVert);   
 
#ifdef BIASED
    if (okLeft && okRight && gerar(0, 9) > PERCENTAGE)
      if ( gerar(0, 1) == 0) cell = r+1;
      else cell = 0;
    else if (okLeft && gerar(0, 9) > PERCENTAGE) cell = r+1-xlhs;
    else if (okRight && gerar(0, 9) > PERCENTAGE) cell = 2*(xrhs-xlhs)+okLeft+okRight-1;
    else cell = gerar(0, (2*(xrhs-xlhs)+okLeft+okRight) - 1);
#else
    cell = rand() % (2*(xrhs-xlhs)+okLeft+okRight);
#endif
    
    if (okLeft) {  // without problems to expand left node  (xrhs = r+1)
      if (cell <= r+1-xlhs) {  // xrhs == r+1
    // lhsvertex
    xc = xlhs + cell;    
    side = 0;
    if (cell < r+1-xlhs) okLeft = 0;
      } else {
    // rhsvertex
    xc = (xrhs-1) - (cell-(r+2-xlhs));
    side = 1;
    if (xc >= xlhs) okRight = 0;
      }
    } else if (okRight) {
      if (cell <= xrhs-1-xlhs) {
    // lhsvertex
    xc = xlhs + cell;
    side = 0;
      } else {
    // rhsvertex
    xc = (xrhs-1) - (cell-(xrhs-xlhs));
    side = 1;
    if (xc >= xlhs) okRight = 0;
      }
    } else {
      if (cell <= xrhs-1-xlhs) {
    // lhsvertex
    xc = xlhs + cell;
    side = 0;
      } else {
    // rhsvertex
    xc = (xrhs-1) - (cell-(xrhs-xlhs));
    side = 1;
      }
    }
  
    inflate_polygon(xc);
    v0 = newVertex(xc+1,r+2,NULL,NULL);
    v1 = newVertex(xc+1,yv,NULL,NULL);
    paste(side,v0,v1);
  }
}
 
 
void rotateCW90(PVERTS pol,int n)
{  
  PVERTS inicio = pol;
  int aux;
  do {
    aux = VERTY(pol);
    VERTY(pol) = n/2-1-VERTX(pol);
    VERTX(pol) = aux;
    pol = VERTNXT(pol);
  } while (pol != inicio);
}
 
 
 
 
//========================================================
// Transforms a permutomino to make some edges collinear 
//========================================================
 
 
#ifdef COLLINEAR
int collinear_horizontal(PVERTS p,int n) {
  int dy, s, y, xright;
 
  // requires v_p = (x0,0) and v_{p+1} = (x1,0) with x1 > x0
  while (VERTY(p) != 0)
    p = VERTNXT(p);
  if (VERTY(VERTNXT(p)) != 0)
    p = VERTPREV(p);
  // Polygon=p;      // not to be done
 
 
  PVERTS right = VERTNXT(p), left = p;
  PVERTS r0, r1, rightmost;
 
  int changes = 0;
 
  do {
    r0 = VERTNXT(right);  // r0--r1  define next H-edge on right chain
    r1 = VERTNXT(r0);
    dy = VERTY(r0) - VERTY(right);
    if (dy > 1) {
#ifdef VERBOSE
      printf("Edge (%d,%d)--(%d,%d)\n\n",  VERTX(r0),VERTY(r0), VERTX(r1),VERTY(r1));
#endif
      xright = MIN(VERTX(r0),VERTX(r1));
      rightmost = left;   // righmost vertex below edge (r0,r1) (to the left of xright)
      while (VERTY(VERTPREV(left)) < VERTY(r1)) {
    left = VERTPREV(left);
    if (VERTX(left) >= xright) 
      rightmost = left;
      } 
      dy = MIN(dy, VERTY(r1)-VERTY(rightmost));
#ifdef VERBOSE
      printf("Rightmost (%d,%d) dy = %d\n\n",VERTX(rightmost),VERTY(rightmost),dy);
#endif
      if (dy > 1) {
    //s = rand() % dy; 
    s = gerar(0, dy - 1);
    if (s > 0) {
      changes++;
#ifdef VERBOSE
      printf("Changing (%d,%d)--(%d,%d) by s=%d\n\n",  VERTX(r0),VERTY(r0), VERTX(r1),VERTY(r1),s);
      fflush(stdout);
#endif
      VERTY(r0) -= s;
      VERTY(r1) -=  s;
    }
      }
    }
    y = VERTY(r0);
    right = r1;
  } while (VERTY(VERTNXT(right)) > y);
 
  return changes;
 
}
#endif
 
 
#ifdef TOLATEX
void showPol(FILE *frL,PVERTS start,int n){
  drawPolygon(frL,Polygon,n);  
  fputc('\n',frL);
  fputc('\n',frL);
  outputCoordsLaTeX(frL,Polygon,n);  
  fputc('\n',frL); fputc('\n',frL);
  fprintf(frL,"\\hrule\n\n");
  fprintf(frL,"\\medskip\n\n\n");
}
#endif
 
#ifdef COLLINEAR
#ifdef VERBOSE
void collinear(FILE *frL,int n){
  int changes;
  showPol(frL,Polygon,n);  
  fprintf(frL,"\\medskip \n\n\n Making edges collinear....\n\n");
  changes = collinear_horizontal(Polygon,n);
  if (changes != 0) 
    showPol(frL,Polygon,n); 
  fprintf(frL,"\n\n\\medskip \n\n\n %d changes\n\n",changes);
  fprintf(frL,"\n\n\\medskip \n\n\n Rotate and making edges collinear....\n\n");
  rotateCW90(Polygon,n);    // assumes coordinates in a grid with n/2 H/V lines 
  changes=collinear_horizontal(Polygon,n);
  if (changes != 0)  fprintf(frL,"\n\n\\medskip \n\n\n %d changes\n\n\n",changes);
  for(int i=0; i<3; i++)
    rotateCW90(Polygon,n);   
}
#endif
#ifndef VERBOSE
void collinear(int n){
  collinear_horizontal(Polygon,n);
  rotateCW90(Polygon,n);    // assumes coordinates in a grid n/2 x n/2
  collinear_horizontal(Polygon,n);
  for(int i=0; i<3; i++)
    rotateCW90(Polygon,n);   
}
#endif
#endif
 
 
#ifdef TOLATEX
void writePreamble(FILE *frLaTeX,int n) {
  fprintf(frLaTeX,"\\documentclass{article}\n");
  fprintf(frLaTeX,"\\setlength{\\unitlength}{0,%dcm}\n",(n > 20? 3:5));  // 0,2 or 0,5 
  fprintf(frLaTeX,"\\begin{document}\n\n\n");
}
#endif
 
 
 
 
int main(int argc,char *argv[])
{ 
  int n, npols;
  FILE *frLaTeX, *frCoords;
  frLaTeX = frCoords = stdout;

  if (argc != 2) {
    printf("Provide the number of vertices in the polygon\n");
    exit(0);
  }

// srand( (unsigned int)time( NULL));
    
  npols = 1;
  n = atoi(argv[1]);

  // printf("%d\n", n);
  if (n && npols) {
    // srand( (unsigned int)time( NULL));
 
#ifndef TOLATEX
#ifdef COORDS
    //fprintf(frCoords,"HERE: %d\n",npols);
#endif
#endif
 
#ifdef TOLATEX
    writePreamble(frLaTeX,n);
#ifdef COORDS
    fprintf(frCoords,"%d\n",npols);
#endif
#endif
 
    for (int instance=1; instance <= npols; instance++) {
 
#ifdef TOLATEX
      fprintf(frLaTeX,"\n\n \\bigskip \n\n Instance %d\n\n",instance);
#endif
 
      genConvex(n,frCoords,frLaTeX);
 
#ifdef TOLATEX
      if (instance < npols) fprintf(frLaTeX,"\n\n \\newpage \n\n");
#endif
    }
 
#ifdef TOLATEX
  fprintf(frLaTeX,"\n\n\\end{document}\n");
#endif
  }
 
  if (argc >= 2) {
    fclose(frCoords);
    if (argc == 3) fclose(frLaTeX);
  }
 
  return EXIT_SUCCESS;
}
 
#ifdef TOLATEX
void outputCoordsLaTeX(FILE *frL,PVERTS polygon, int n){
  PVERTS p = polygon;
  fprintf(frL,"%d\n\n",n);
  do {
    fprintf(frL,"(%d,%d) ", VERTX(p),VERTY(p));
    p = VERTNXT(p);
  } while (p!=polygon);
}
#endif
 
 
void outputCoords(FILE *frCoords,PVERTS polygon, int n)
{
  PVERTS p = polygon;
  fprintf(frCoords,"\n%d\n",n);
  do {
    fprintf(frCoords,"%d %d\n", VERTX(p),VERTY(p));
    p = VERTNXT(p);
  } while (p!=polygon);
}
 
 
void drawPolygon(FILE *fr,PVERTS p,int n)
{
  // supoe-se que p aponta um vértice dum polígono ortogonal
  // com n vertices; as coordenadas dos vertices são inteiros
  // de 0 a n/2
 
  int dx, dy, s;
  PVERTS inicio = p;
 
  fprintf(fr,"\\begin{picture}(%d,%d)\n",n/2+1,n/2+1);
 
  do {
    dx = VERTX(VERTNXT(p)) - VERTX(p);
    dy = VERTY(VERTNXT(p)) - VERTY(p);
    if (dx) {
      s = ABS(dx);
      dx = dx/s;
    } else {
      if (dy == 0) fprintf(stderr,"Wrong: (%d,%d)\n",VERTX(p),VERTY(p));
      else {
    s = ABS(dy);
    dy = dy/s;
      }
    }
    fprintf(fr,"\\put(%d,%d){\\line(%d,%d){%d}}\n",VERTX(p),VERTY(p),dx,dy,s);
    p = VERTNXT(p);
  } while (p != inicio);
 
  fprintf(fr,"\\end{picture}\n");
}
 
 
void free_polygon(PVERTS p)
{ PVERTS aux;
  VERTNXT(VERTPREV(p)) = NULL;
  do {
    aux = VERTNXT(p);
    free(p);
    p = aux;
  } while (p != NULL);
}
