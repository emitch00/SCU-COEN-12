#include <stdlib.h>                                                             //defining libraries used
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"

#define EMPTY 1                                                                 //defining empty, filled, deleted
#define FILLED 2
#define DELETED 3

static int search(SET *sp, void *elt, bool *found);                             //function prototypes

typedef struct set{                                                             //declare structure and variables
        void** data;
        int length;
        int count;
        int *flags;
        int (*compare) ();
        unsigned (*hash) ();
        }SET;


//O(n)
SET *createSet(int maxElts, int (*compare) (), unsigned (*hash) ()){            //create set function
        SET *sp;
        sp = (SET*)malloc(sizeof(SET));                                         //allocate memory for set
        assert(sp != NULL);

        sp->length = maxElts;
        sp->count = 0;

        sp->data = (void **)malloc(sizeof(void*) * maxElts);                    //allocate memory for data
        assert(sp->data != NULL);

        sp->flags = malloc(sizeof(int*) * maxElts);                             //allocate memory for flags
        assert(sp->flags != NULL);

        for(int i=0; i < sp->length; i++)                                       //set all indexes of flags to empty
                sp->flags[i] = EMPTY;

        sp->compare = compare;
        sp->hash = hash;
        return sp;
}

//O(1)
void destroySet(SET *sp){                                                       //destroy set function
        assert(sp != NULL);

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
void addElement(SET *sp, void *elt){                                            //add element function
        assert(sp != NULL && elt != NULL);

        if(sp->count == sp->length)                                             //check if set full
                return;

        bool found = false;
        int index = search(sp, elt, &found);

        if(found == true || index == -1)                                        //check if new element is already in set
                return;

        sp->data[index] = elt;                                                  //add element to data
        sp->flags[index] = FILLED;                                              //update flags to filled
        sp->count++;                                                            //update count

        return;
}

//O(n)
void removeElement(SET *sp, void *elt){                                         //remove element function
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
void *findElement(SET *sp, void *elt){                                          //find element function
        assert(sp != NULL && elt != NULL);

        bool found = false;
        int index = search(sp, elt, &found);                                    //set index value

        if(found == true)                                                       //return element at index if found
                return (sp->data[index]);

        return NULL;                                                            //return null if element not found
}


//O(n)
void *getElements(SET *sp){                                                    //get elements function
        assert(sp != NULL);
        int i, j;

        void **elts = malloc(sizeof(void*) * sp->count);                        //allocate memory for elts array
        assert(elts != NULL);

        for(i=0, j=0; i<sp->length; i++){                                       //traverse flag and data array
                if(sp->flags[i] == FILLED)                                      //add to elts if index marked filled
                        elts[j++] = sp->data[i];
        }

        return elts;
}


//O(n)
static int search (SET *sp, void *elt, bool *found){                            //search function using hash table
        int home = -1;
        assert(sp != NULL && elt != NULL);

        *found = false;
        unsigned key = sp->hash(elt);                                           //initialize key

        for(int i=0; i<sp->length; i++){                                        //traverse flag array checking for filled, deleted, and empty conditions
                int index = (key + i) % sp->length;

                if(sp->flags[index] == FILLED){                                 //filled condition
                        if((*sp->compare) (elt, sp->data[index]) == 0){                  //check if element matches elt
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
