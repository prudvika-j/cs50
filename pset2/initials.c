#include<stdio.h>
#include<cs50.h>
#include<ctype.h>

int main(void)
{
    string s=get_string();
    int i=0;
    
    printf("%c",toupper(s[i]));
    i++;
    
    while(s[i]!='\0')
    {
        if(s[i]==' ')
        {
           printf("%c",toupper(s[i+1]));  
        }
       
        i++;
    }
    
    printf("\n");
}