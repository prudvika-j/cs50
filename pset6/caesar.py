import sys
import cs50

if len(sys.argv) == 1 or len(sys.argv) > 2:
    exit(1)

key = int(sys.argv[1])

print("plaintext: ",end="")
plaintext = cs50.get_string()

print("ciphertext: ",end="")
ch = 0
for c in plaintext:
    if c.isalpha():
        ch = ord(c) + key
        if c.isupper():
            ch = ch - 65;
            ch = ch % 26;
            print(chr(ch+65),end="")
        else:
            ch = ch - 97;
            ch = ch % 26;
            print(chr(ch+97),end="")
    else:
        print(c,end="")

print("")
exit(0)
        
        
        
