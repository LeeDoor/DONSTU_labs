from random import *

def getFloat(x, default):
    try:
        a = float(x)
    except:
        a = default
    return a

def countSk(A):
    S = sum(x for x in A if x > 0)
    k = len([x for x in A if x > 0])
    return (S, k)

def method(x, y, A, B):
    S1, k1 = countSk(A)
    S2, k2 = countSk(B)
    U = (x ** k1 * y ** k2) / (S1 + S2)
    return U
    
A = []
B = []
N = 5
s = input("enter mass length (5 by default): ")
N = int(s) if s.isdigit() else 5
A = [randint(-10, 10) for _ in range(N)]
B = [randint(-10, 10) for _ in range(N)]
print ("A:", A)
print ("B:", B)
s = input("enter x (3 by default): ")
x = getFloat(s, 3)
s = input("enter y (3 by default): ")
y = getFloat(s, 3)

print("U:", method(x,y,A,B))

