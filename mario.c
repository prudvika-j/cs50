#include<stdio.h>
#include<cs50.h>

int main(void)
{
     
     int h;
     do
     {
         printf("Height: ");  
         h=get_int();
       
     }while(h<0 || h>23);
     
     for(int i=2;i<=h+1;i++)
     {
         for(int j=1;j<=h-i+1;j++)
         {
             printf(" ");
         }
         for(int j=1;j<=i;j++)
         {
             printf("#");
         }
         printf("\n");
     }
}
