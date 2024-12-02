from math import sin

def rec(n):
    if n == 0:
        return 0
    return sin(n)/ (2 * n - 1) + rec(n - 1)

s = input("enter n(5 by default): ")
n = int(s) if s.isdigit() else 5
print(rec(n))
