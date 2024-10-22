A = 0.0
N = 0
while True:
    print('enter A:')
    try:
        A = float(input())
        break
    except ValueError:
        print('да я сделал проверку блин')
while True:
    print('enter N:')
    s = input()
    if s.isdigit() and int(s) > 0:
        N = int(s)
        break
    else:
        print('wrong number')

print('A:' + str(A), 'N:' + str(N))
        
for i in range(1, N + 1):
    print('A ^ ' + str(i) + ' = ' + str(A**i))
