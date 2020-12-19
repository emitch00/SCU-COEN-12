#include <stdlib.h>                                                             //libraries
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "set.h"
#define alpha 20                                                                //alpha value given by assignment

static int search(SET *sp, void *elt, bool *found);                             //search function prototype

struct set{                                                                     //set structure
        int count;
        int length;
        LIST **lists;
        int (*compare)();
        unsigned (*hash)();
};

SET *createSet(int maxElts, int (*compare)(), unsigned(*hash)()){               //createSet function    Average:O(1)            Worst:O(n)
        int i;
        SET *sp;

        assert(compare != NULL && hash != NULL);

        sp = malloc(sizeof(SET));                                               //allocate memory for set
        assert(sp != NULL);

        sp->length = maxElts/alpha;
        sp->compare = compare;
        sp->hash = hash;                                                        //assign pointers
        sp->count = 0;

        sp->lists = malloc(sizeof(LIST*) * sp->length);                         //allocate memory for lists
        assert(sp->lists != NULL);

        for(i=0; i<sp->length; i++)
                sp->lists[i] = createList(compare);                             //create a list for each item in the list

        return sp;                                                              //return set
}

void destroySet(SET *sp){                                                       //destroySet function   Average:O(n)    Worst:O(n)
        int i;
        assert(sp != NULL);

        for(i=0; i<sp->length; i++)                                             //traverse list and delete each list
                destroyList(sp->lists[i]);

        free(sp->lists);                                                        //free list and set
        free(sp);

        return;
}

int numElements(SET *sp){                                                       //numElements function  Average:O(1)    Worst:O(1)
        assert(sp != NULL);

        return sp->count;                                                       //return count
}

void addElement(SET *sp, void *elt){                                            //addElement function   Average:O(n)    Worst:O(n)
        assert(sp != NULL && elt != NULL);

        bool found = false;
        int index = search(sp, elt, &found);                                    //search if element is within the set

        if(found == false){                                                     //if element is unique, add to end of list
                addLast(sp->lists[index], elt);
                sp->count++;                                                    //update count
        }

        return;
}

void removeElement(SET *sp, void *elt){                                         //removeElement function        Average:O(n)    Worst:O(n)
        assert(sp != NULL && elt != NULL);

        bool found = false;
        int index = search(sp, elt, &found);                                    //search if element is within the set

        if(found == false)
                return;

        removeItem(sp->lists[index], elt);                                      //if element is within the set, remove item
        sp->count--;                                                            //update count

        return;
}

void *findElement(SET *sp, void *elt){                                          //findElement function  Average:O(n)    Worst:O(n)
        assert(sp != NULL && elt != NULL);

        bool found = false;
        int index = search(sp, elt, &found);                                    //search if element is within the set

        if(found == false)
                return NULL;

        return findItem(sp->lists[index], elt);                                 //if element is found, return item
}

void *getElements(SET *sp){                                                     //getElements function  Average:O(n^2)  Worst:O(n^2)
        assert(sp != NULL);

        void **elts = malloc(sizeof(void*) * sp->count);                        //allocate memory for elts
        assert(elts != NULL);

        int traverse = 0;                                                       //traversal variables
        int i, j;

        for(i=0; i<sp->length; i++){
                void **cpList = getItems(sp->lists[i]);                         //add elements to a copy of the list
                for(j=0; j<numItems(sp->lists[i]); j++){
                        elts[traverse] = cpList[j];                             //add elements in copy of list to elts
                        traverse++;
                }
        }

        return elts;                                                            //return elts
}

static int search(SET *sp, void *elt, bool *found){                             //search function       Average:O(logn)   Worst:O(n)
        assert(sp != NULL && elt != NULL);

        int index = (*sp->hash)(elt) % sp->length;                              //hash to find index

        if(findItem(sp->lists[index], elt) != NULL){                            //if element is found
                *found = true;
                return index;                                                   //return index
        }

        *found = false;

        return index;
}
