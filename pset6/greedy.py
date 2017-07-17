import cs50

def convert(c):
    return round(c*100);
    
change = 0
while True:
    print("O hai! How much change is owed?")
    change = cs50.get_float()
    if change > 0:
        break
coins = 0
cents = convert(change)
while cents >= 25:
    coins = coins + 1
    cents = cents - 25
while cents >= 10:
    coins = coins + 1
    cents = cents - 10
while cents >=5:
    coins = coins + 1
    cents-=5
while cents >=1:
    coins = coins + 1
    cents-=1
print(coins)
