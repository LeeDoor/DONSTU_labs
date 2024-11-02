while True:
    c = input('enter character: ')
    if len(c) != 1 or not c.isalpha():
        print("поставьте зачет пожалуйста")
        continue
    break
if c in "eyuioaаеёиоуэюяы":
    print('glasnaya')
else:
    print('ne glasnaya')
