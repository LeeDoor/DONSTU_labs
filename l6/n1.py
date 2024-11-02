text = input("enter text to make replacement ")

A = input("enter word kotoroe zamenyaem ")
B = input("enter word na kotoroe zamenyaem ")
print (text)
splitted = []
prev = 0
for i in range(len(text)):
    if text[i] == ' ':
        splitted.append(text[prev:i])
        prev = i + 1
for word in splitted: 
    if word == A:
        word = B
    print (word, end=' ')