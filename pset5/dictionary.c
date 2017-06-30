/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"


struct node
{
    char word[LENGTH+1];
    struct node *next;
};

struct node *hash_table[HASHTABLE_SIZE];

int count_of_words = 0;
/**
 * hash function referred from reddit.com
 * https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/
 */
int get_hash(char* word)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
        hash = (hash << 2) ^ word[i];
    return hash % HASHTABLE_SIZE;
}
/**
 * Returns true if word is in dictionary else false.
 */

bool check(const char *word)
{
    int length = strlen(word);
    char copy[length+1];
    int i;
    // create a copy of the word argument
    for( i = 0; i < length; i++ )
    {
        copy[i] = tolower(word[i]);
    }
    copy[i]='\0';
    
    // find the hash bucket to which the word belongs to
    int index = get_hash(copy);
    
    // assign a pointer to the linked list associated with the hash bucket
    struct node *temp = hash_table[index];
    
    // if hash bucket is empty return false
    if( temp == NULL )
    {
        return false;
    }
    else
    {  
        // iterate over the linked list
        while( temp != NULL )
        {
            // compare data in the linked list with the word
            // if a match occurs return true
            if(strcmp(temp->word, copy) == 0)
            {
                return true;
            }
            // else move to the next node in the linked list
            temp = temp->next;
        }
    }
    
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open the dictionary
    FILE *fp = fopen(dictionary, "r");
    if(fp == NULL)
    {
    return false;
    }
    // intialize the hash table with NULL
    for( int i = 0; i < HASHTABLE_SIZE; i++)
    {
        hash_table[i] = NULL;
    }
    
    char word[LENGTH+1];
    
    // read each word from the dictionary
    while(fscanf(fp, "%s",word) != EOF)
    {
        // find a hash bucket for the word
        int index = get_hash(word);
        
        // create a new node
        struct node *nn = malloc(sizeof(struct node));
        strcpy(nn->word, word);
        nn->next = NULL;
        
        // if the hash bucket is empty insert a new node 
        if(hash_table[index] == NULL)
        {
            
            hash_table[index] = nn;
        }
        // else if the hash bucket is not empty then append the new node to the existing list 
        else
        {
            nn->next = hash_table[index];
            hash_table[index] = nn;
        }
        
        // increment count of words in dictionary
        count_of_words++; 
    }
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return count_of_words;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // for each hash bucket
    for( int i = 0; i < HASHTABLE_SIZE; i++)
    {
      struct node *temp = hash_table[i];
      struct node *cursor;
      // iterate over linked list attached to hash bucket
      while( temp != NULL )
      {
          // free each node in linked list
          cursor = temp;
          temp = temp->next;
          free(cursor);
      }
    }
    
    return true;
}
