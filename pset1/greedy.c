#include<stdio.h>
#include<cs50.h>
#include<math.h>

int convert(float c)
{
    return round(c*100);
}
int main(void)
{
     printf("O hai! How much change is owed?\n");
     float change=get_float();
     while(change<0)
     {
         printf("How much change is owed?\n");
         change=get_float();
     }
    
    int coins=0;
    int cents=convert(change);
    
         while(cents >= 25)
         {
             coins++;
             cents-=25;
         }
         while(cents >= 10 )
         {
             coins++;
             cents-=10;
         }
         
         while(cents >= 5)
         {
             coins++;
             cents-=5;
         }
          while(cents >= 1)
         {
             coins++;
             cents-=1;
         }
        
         
         printf("%d\n",coins);
     
}