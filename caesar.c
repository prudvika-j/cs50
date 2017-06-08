#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>

int main(int argc, string argv[])
{
    if( argc == 1 || argc > 2)
    {
        printf("Please specify one command line argument\n");
        return 1;
    }
    
    int k = atoi(argv[1]);
    
    // reading plain text
    printf("plaintext:");
    string plaintext = get_string();
    
    int ch;
   
    printf("ciphertext:");
    
    for(int i = 0,n = strlen(plaintext); i < n; i++)
    {
        // check whether the next letter is an alphabet or not
        // if alphabet identify the corresponding cipher text 
        if(isalpha(plaintext[i]))
        {
        
            ch = plaintext[i] + k;
            if(isupper(plaintext[i]))
            {
                ch=ch-65;
                ch = ch % 26;
                printf("%c",ch+65);
            }
            else
            {
                ch=ch-97;
                ch = ch % 26;
                printf("%c",ch+97);
            }
        }
        // else print the letter as it is
        else
        {
            printf("%c",plaintext[i]);
        }
    }
    
    printf("\n");
    return 0;
}