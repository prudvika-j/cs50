import cs50
def get_Height():
    while True:
        print("Height: ",end="")
        h = cs50.get_int()
        if h > 0 and h <= 23:
            return h   
     
h = get_Height();
for i in range(h):
    for j in range(0,h-i):
        print(" ",end="")
    for j in range(i+2):
        print("#",end="")
    print("")