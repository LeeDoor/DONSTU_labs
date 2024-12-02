s = input("enter your string: ")
d = {}
for c in s:
    d[c] = 0
for c in s:
    d[c] += 1

for pair in d:
    print(pair, "=>", d[pair])

while True:
    c = input('enter character to search (enter to exit): ')
    if len(c) == 1:
        if c in d:
            print(c, "=>", d[c])
        else:
            print("no", c, "in dictionary")
    elif len(c) == 0:
        break
    else:
        print('not a character')
