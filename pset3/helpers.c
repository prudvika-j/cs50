/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    if(n <= 0)
    {
        return false;
    }
    
    int low = 0, high = n-1;
    int mid;
    
    while(low <= high)
    {
        mid = (low + high)/2;
        if(values[mid] == value)
        {
            return true;
        }
        else if(value < values[mid])
        {
            high  = mid-1;
        }
        else
        {
            low = mid+1;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    int temp;
    for(int i = 1 ; i < n; i++)
    {
        for(int j = 0; j < n - i; j++)
        {
            if(values[j] > values[j+1])
            {
               temp = values[j];
               values[j] = values[j+1];
               values[j+1] = temp;
            }
        }
    }
    return;
}