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
    for(int i = 0; i < strlen(argv[1]); i++)
    {
        if(!isalpha(argv[1][i]))
        {
            printf("Please specify command line argument with alphabetical characters\n");
            return 1;
        }
    }
    // reading plain text
    printf("plaintext:");
    string plaintext = get_string();
    printf("ciphertext:");
    int ch, n = strlen(argv[1]);
    for(int i = 0,j = 0; i < strlen(plaintext); i++)
    {
        if(j==n)
        {
            j=0;
        }
        if(isalpha(plaintext[i]))
        {
            ch = plaintext[i] + argv[1][j];
            if(isupper(argv[1][j]))
            {
                ch = ch - 65;
            }
            else
            {
                ch = ch - 97;
            }
            if(isupper(plaintext[i]))
            {
                
                ch = ch - 65;
                ch = ch % 26;
                printf("%c",ch+65);
            }
            else
            {
                
                ch = ch - 97;
                ch = ch % 26;
                printf("%c",ch+97);
            }
            j++;
        }
        else
        {
            printf("%c",plaintext[i]);
        }
        
    }
    printf("\n");
    return 0;
}
    