import turtle
from turtle import *

import time
import sys

if len(sys.argv) != 2:
    print("Output file missing")
    sys.exit(0)

X_OFFSET=300
Y_OFFSET=105

def drawPolyContour(m, S):
  """
  Print dots
  """ 
  t.begin_fill()  
#  m = int(input())

 # S=input().split(" ")
 # S=[int(a) for a in S]
  #  lst=["grey"]
  t.color("black")
  t.fillcolor("grey")
    
  Pol=[]
  for i in range(m):
    Pol.append((S[2*i],S[2*i+1]))

  for i in range(len(Pol)):
   # Pol[i]=(Pol[i][0]*fact-X_OFFSET,Pol[i][1]*fact-Y_OFFSET)
    Pol[i]=(Pol[i][0]*fact+X_OFFSET,Pol[i][1]*fact-Y_OFFSET)


  #t.pencolor("green")
  t.width(1.50) 
  t.penup()
  for i in range(len(Pol)):
    t.goto(Pol[(i+1)%len(Pol)])
    if (i == 0):
      t.pendown()
   # time.sleep(0.1)

  t.goto(Pol[1])
  t.end_fill()

t=turtle.Turtle()

#tt=int(input())
n=int(input())

#S=input()
#S=S.split(" ")

#S=[int(a) for a in S]

S = []
for i in range(n):
  a, b = map(int,input().split())
  S.append(a)
  S.append(b)
 

#print(S)
#exit(0)
Pol=[]
for i in range(n):
  Pol.append((S[2*i],S[2*i+1]))

fact=4

for a in Pol:
  M=max(abs(a[0]),abs(a[1]))
  if M>0:
    fact=min(fact,max(685//M,1))
for i in range(len(Pol)):
 # Pol[i]=(Pol[i][0]*fact-X_OFFSET,Pol[i][1]*fact-Y_OFFSET)
  Pol[i]=(Pol[i][0]*fact+X_OFFSET,Pol[i][1]*fact-Y_OFFSET)

t.speed(0)

curpos=None
for i in range(len(Pol)):
  p=Pol[(i+1)%len(Pol)]
  t.pendown()
  
  if p[0]==-fact:
    t.penup()
  if curpos==None or curpos[0]==-fact:
    print(curpos)
    t.penup()
    
  t.goto(p)
  curpos=p
  print(curpos)
 
t.speed(0)
drawPolyContour(n, S)
t.hideturtle()
#time.sleep(600)
ts = t.getscreen()
ts.getcanvas().postscript(file=sys.argv[1])