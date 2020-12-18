#include <stdlib.h>                                                     //declaring libraries
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"

static int bosearch(SET *sp, char *elt, bool *found);

typedef struct set{                                                     //declare structure
        char** data;                                                    //address of character in the array
        int length;                                                     //number of arrays
        int count;                                                      //number of elements in array
        }SET;


SET *createSet(int maxElts){                                            //create set
        SET *sp = (SET*)malloc(sizeof(SET));
        assert(sp != NULL);
        sp->count = 0;                                                  //declare values of struct variables
        sp->length = maxElts;
        sp->data = (char**)malloc(sizeof(char*)*maxElts);               //allocate memory for data
        assert(sp->data != NULL);
        return sp;                                                      //return pointer for set
}

void destroySet(SET *sp){                                               //destroy set
        assert(sp != NULL);
        for(int i = 0; i < sp->count; i++){                             //free characters within the set
                free(sp->data[i]);
        }
        free(sp->data);                                                 //free character pointer
        free(sp);                                                       //free set
        return;
}

int numElements(SET *sp){                                               //count number of elements in set
        assert(sp != NULL);
        return sp->count;                                               //returns count
}

void addElement(SET *sp, char *elt){                                    //adds element to the set
        assert(sp != NULL && elt != NULL && sp->count <= sp->length);

        bool found = false;
        int index = bosearch(sp, elt, &found);                          //search function returns index of element and found pointer
        if(found == false){                                             //if word is unique
                for(int i = sp->count; i > index; i--){
                        sp->data[i] = sp->data[i-1];                    //shifts element down to allocate memory
                }
                sp->data[index] = strdup(elt);                          //allocates memory for element
                sp->count++;
        }

        return;
}

void removeElement(SET *sp, char *elt){                                 //remove element
        assert(sp != NULL && elt != NULL);

        bool found = false;
        int index = bosearch(sp, elt, &found);                          //search funciton returns index and found pointer
        if(found == false)
                return;

        for(int i = index; i < sp->count; i++){                         //shifts all elements due to deletion
                sp->data[i] = sp->data[i+1];
        }
        sp->count--;                                                    //adjust count
        return;
}

char *findElement(SET *sp, char *elt){                                  //find element
        assert(sp != NULL && elt != NULL);

        bool found = false;
        int index = bosearch(sp, elt, &found);                          //search funciton returns index and found pointer
        if(found == false)
                return NULL;

        return sp->data[index];
}

char **getElements(SET *sp){                                            //get element
        assert(sp != NULL);

        char** elements = (char**) malloc(sizeof(char*) * sp->count);   //allocate memory for element array
        elements = sp->data;
        return elements;

}


static int bosearch(SET *sp, char *elt, bool *found){                   //binary search function
        assert(sp != NULL && elt != NULL);
        int lo = 0;                                                     //define lo
        int mid;
        int hi = sp->count - 1;                                         //define hi
        while(lo <= hi){
                mid = (lo+hi)/2;                                        //set mid
                if(strcmp(sp->data[mid], elt) > 0)                      //bottom half of array
                        hi = mid-1;
                else if(strcmp(sp->data[mid], elt) < 0)                 //top half of array
                        lo = mid+1;
                else{
                        *found = true;
                        return mid;                                     //return mid as index of element
                }
        }

        *found = false;
        return lo;
}
