from random import *

arr = []
while True:
    s = input("enter n: ")
    if s.isdigit():
       break 
n = int(s)
mm = 0
mn = 10**10
mmi = 0
mni = 0
for i in range(n):
    arr.append(randint(-100, 100))
    if arr[i] > mm:
        mm = arr[i]
        mmi = i
    if arr[i] < mn:
        mn = arr[i]
        mni = i
ss = 0
for i in range(mmi):
    if arr[i] > 0:
        ss += arr[i]
print(arr)
print('1. sum: ' + str(ss))
for i in range(len(arr) // 2):
    arr[i], arr[len(arr) - i - 1] = arr[len(arr) - i - 1], arr[i] 
print('2. reverse ')
print(arr)