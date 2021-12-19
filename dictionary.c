// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

int g_quantity = 0;
int g_countfree = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10001;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int index = 0;
    //get the word index
    index = hash(word);
    node *cursor = NULL;

    if (table[index] != NULL)
    {
        cursor = table[index];
        while (cursor != NULL)
        {
            if (!strcasecmp(cursor->word, word))
            {
                return true;
            }
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number.
// I used a function from the book: The C Programming Language K&R
//(Kernighan, Brian; Ritchie, Dennis M. (March 1988). The C Programming Language (2nd ed.). Englewood Cliffs, NJ: Prentice Hall. ISBN 0-13-110362-8.)
unsigned int hash(const char *word)
{
    unsigned int hashval;
    unsigned int wordvalue;

    for (hashval = 0; *word != '\0'; word++)
    {
        wordvalue = tolower(*word);
        hashval = wordvalue + 31 * hashval;
    }
    return hashval % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    int index = 0;
    char word[LENGTH + 1];

    node *n = NULL;
    //open dictionary
    FILE *dictfile = fopen(dictionary, "r");
    if (dictfile != NULL)
    {

        // get each word from the dictionary
        while (fscanf(dictfile, "%s", word) != EOF)
        {
            // create new node
            n = malloc(sizeof(node));
            if (n == NULL)
            {
                return false;
            }
            strcpy(n->word, word);
            // use hash for each word and indexate table
            index = hash(word);
            // add word to table
            if (table[index] != NULL)
            {
                n->next = table[index];
                table[index] = n;
            }
            else
            {
                n->next = NULL;
                table[index] = n;
            }
            g_quantity++;
        }
        fclose(dictfile);
        return true;
    }
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return (g_quantity);
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cursor = NULL;
    node *tmp = NULL;
    int index = 0;

    while (index < N)
    {
        if (table[index] == NULL)
        {
            ;
        }
        else
        {
            cursor = table[index];
            while (cursor != NULL)
            {
                tmp = cursor;
                cursor = cursor->next;
                free(tmp);
                g_countfree++;
            }
        }
        index++;
    }
    if (g_countfree == g_quantity)
    {
        return true;
    }
    else
    {
        return false;
    }
}
