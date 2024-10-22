import math
print('welcome to variant 22')

# Q = Q0 * e **(-1 * t * math.log(2)/p)

l = math.log(2)
t = 0
while 2 * math.e ** (-1 * t * l / 8) >= 0.15:
    t += 5
print ('required number of days: ' + str(t))
print(math.e ** (-1 * t * l / 8) == math.exp(-1 * t * l / 8))
