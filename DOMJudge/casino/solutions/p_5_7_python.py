G=int(input())
for i in range(G):
    g=input()
    print(1)
    while True:
        L=input().split(' ')
        if(len(L)==2):
            if float(L[1]) > 0.57:
                print(0)
                input()
                break
            else:
                print(1)
        else:
            input()
            break
