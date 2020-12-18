#include <stdlib.h>                                                             //defining libraries used
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"

#define EMPTY 1                                                                 //defining empty, filled, deleted
#define FILLED 2
#define DELETED 3

static unsigned strhash(char *s);                                               //function prototypes
static int search(SET *sp, char *elt, bool *found);

typedef struct set{                                                             //declare structure and variables
        char** data;
        int length;
        int count;
        int *flags;
        }SET;


//O(n)
SET *createSet(int maxElts){                                                    //create set function
        SET *sp;
        sp = (SET*)malloc(sizeof(SET));                                         //allocate memory for set
        assert(sp != NULL);

        sp->length = maxElts;
        sp->count = 0;

        sp->data = (char **)malloc(sizeof(char*) * maxElts);                    //allocate memory for data
        assert(sp->data != NULL);

        sp->flags = malloc(sizeof(int*) * maxElts);                             //allocate memory for flags
        assert(sp->flags != NULL);

        for(int i=0; i < sp->length; i++)                                       //set all indexes of flags to empty
                sp->flags[i] = EMPTY;

        return sp;
}


//O(n)
void destroySet(SET *sp){                                                       //destroy set function
        assert(sp != NULL);

        for(int i=0; i<sp->count; i++){                                         //free data marked as filled
                if(sp->flags[i] == FILLED)
                        free(sp->data[i]);
        }

        free(sp->data);                                                         //free data array
        free(sp->flags);                                                        //free flags array
        free(sp);                                                               //free set

        return;
}


//O(1)
int numElements(SET *sp){                                                       //number of elements function
        assert(sp != NULL);
        return (sp->count);                                                     //return count
}

//O(n)
void addElement(SET *sp, char *elt){                                            //add element function
        assert(sp != NULL && elt != NULL);

        if(sp->count == sp->length)                                             //check if set full
                return;

        bool found = false;
        int index = search(sp, elt, &found);

        if(found == true || index == -1)                                        //check if new element is already in set
                return;

        sp->data[index] = strdup(elt);                                          //add element to data
        sp->flags[index] = FILLED;                                              //update flags to filled
        sp->count++;                                                            //update count

        return;
}


//O(n)
void removeElement(SET *sp, char *elt){                                         //remove element function
        assert(sp != NULL && elt != NULL);

        bool found = false;
        int index = search(sp, elt, &found);                                    //set index value

        if(found == true){                                                      //free data and flags if found
                free(sp->data[index]);
                sp->flags[index] = DELETED;
                sp->count--;                                                    //update count
        }

        return;
}


//O(n)
char *findElement(SET *sp, char *elt){                                          //find element function
        assert(sp != NULL && elt != NULL);

        bool found = false;
        int index = search(sp, elt, &found);                                    //set index value

        if(found == true)                                                       //return element at index if found
                return (sp->data[index]);

        return NULL;                                                            //return null if element not found
}

//O(n)
char **getElements(SET *sp){                                                    //get elements function
        assert(sp != NULL);
        int i, j;

        char **elts = malloc(sizeof(char*) * sp->count);                        //allocate memory for elts array
        assert(elts != NULL);

        for(i=0, j=0; i<sp->length; i++){                                       //traverse flag and data array
                if(sp->flags[i] == FILLED)                                      //add to elts if index marked filled
                        elts[j++] = sp->data[i];
        }

        return elts;
}


//O(n)
static int search (SET *sp, char *elt, bool *found){                            //search function using hash table
        int home = -1;
        assert(sp != NULL && elt != NULL);

        *found = false;
        unsigned key = strhash(elt);                                            //initialize key

        for(int i=0; i<sp->length; i++){                                        //traverse flag array checking for filled, deleted, and empty conditions
                int index = (key + i) % sp->length;

                if(sp->flags[index] == FILLED){                                 //filled condition
                        if(strcmp(elt, sp->data[index]) == 0){                  //check if element matches elt
                                *found = true;
                                return index;
                        }
                }

                if(sp->flags[index] == DELETED)                                 //deleted condition
                        home = index;                                           //update closest home location

                if(sp->flags[index] == EMPTY){                                  //empty condition
                        if(home > -1)                                           //return closest home location if valid
                                return home;

                        return index;                                           //return home location
                }
        }

        return -1;
}


//O(1)
static unsigned strhash(char *s){                                               //hash function determined by element provided
        unsigned hash = 0;

        while(*s != '\0')
                hash = 31 * hash + *s++;

        return hash;
}
