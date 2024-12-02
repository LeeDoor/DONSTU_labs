def AddLeftDigit(D, K):
    K = int(str(D) + str(K))
    return K

s = input("enter number K (123 default): ")
K = int(s) if s.isdigit() else 123
while True:
    D = 0
    while True:
        s = input("enter 1 digit D:")
        if not s.isdigit() or len(s) != 1:
            print("nepravilno")
            continue
        D = int(s)
        break
    K = AddLeftDigit(D, K)
    print("K:", K)
    