from random import * 

M = 5
N = 6

s = input("enter M (5 by default): ")
if s.isdigit() and int(s) != 0:
    M = int(s)
s = input("enter N (6 by default): ")
if s.isdigit() and int(s) != 0:
    N = int(s)

mx = []

ms = 0
mi = 0

print('matrix: ')
for i in range(M):
    mx.append([])
    cs = 0
    for j in range(N):
        mx[-1].append(randint(-100, 100))
        print(mx[-1][-1], end='\t')
        cs += mx[-1][-1]
    if cs > ms:
        ms = cs
        mi = i
    print(" ::: sum is " + str(cs))

print("line with max sum: " + str(mi))