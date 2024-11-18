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

# fill
for i in range(M):
    mx.append([])
    for j in range(N):
        mx[i].append(randint(-100, 100))
        print(mx[i][j], end='\t')
    print()

maxes = []
# array of max elements
print ('max elements: ', end='')
for i in range(M):
    maxes.append([0, mx[i][0]])
    for j in range(1, N):
        if maxes[-1][1] < mx[i][j]:
            maxes[-1][1] = mx[i][j]
            maxes[-1][0] = j
    print(maxes[-1][1], end = ' ')
print()

# min element from max elements
mn = maxes[0][1]
mi = 0
for i in range(1, M):
    if maxes[i][1] < mn:
        mn = maxes[i][1]
        mi = i
print("otvet: (" + str(mi) + ", " + str(maxes[mi][0]) + ")")
