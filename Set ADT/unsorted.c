#include <stdlib.h>                                             //declaring libraries
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"

int static search(SET *sp, char *elt);

typedef struct set{                                             //declare structure
        char** data;                                            //address of character in the array
        int length;                                             //number of arrays
        int count;                                              //number of elements in array
        }SET;


SET *createSet(int maxElts){                                    //create set
        SET *sp = (SET*)malloc(sizeof(SET));
        assert(sp != NULL);
        sp->count = 0;                                          //declare values of struct variables
        sp->length = maxElts;
        sp->data = (char**)malloc(sizeof(char*)*maxElts);       //allocate memory for data
        assert(sp->data != NULL);
        return sp;                                              //return pointer for set
}

void destroySet(SET *sp){                                       //destroy set
        assert(sp != NULL);
        for(int i = 0; i < sp->count; i++){                     //free characters within the set
                free(sp->data[i]);
        }
        free(sp->data);                                         //free character pointer
        free(sp);                                               //free set
        return;
}

int numElements(SET *sp){                                       //count number of elements in set
        assert(sp != NULL);
        return sp->count;                                       //returns count
}

void addElement(SET *sp, char *elt){                            //adds element to the set
        assert(sp != NULL && elt != NULL);

        int index = search(sp, elt);                            //search function returns index of element
        if(index < 0){
                sp->data[sp->count] = strdup(elt);              //allocates memory for the element
                sp->count++;
        }

        return;
}

void removeElement(SET *sp, char *elt){                         //remove element
        assert(sp != NULL && elt != NULL);

        int index = search(sp, elt);                            //search function returns index of element
        if(index < 0)
                return;

        for(int i = index; i < sp->count; i++){                 //deletes element at index by shifting all elements
                sp->data[i] = sp->data[i+1];
        }
        sp->count--;
        return;
}

char *findElement(SET *sp, char *elt){                          //find element
        assert(sp != NULL && elt != NULL);

        int index = search(sp, elt);                            //search function returns index of element
        if(index < 0)
                return NULL;

        return sp->data[index];                                 //returns character at index
}

char **getElements(SET *sp){                                    //get character array
        assert(sp != NULL);

        char** elements = (char**) malloc(sizeof(char*) * sp->count);   //allocates memory for character array
        elements = sp->data;
        return elements;

}


int static search(SET *sp, char *elt){                          //search function
        assert(sp != NULL && elt != NULL);
        int found = false;
        int index;

        for(int i=0;i<sp->count;i++){                           //comparing element to character in array
                if(strcmp(sp->data[i], elt) == 0){
                        found = true;
                        index = i;                              //if the same, index is defined
                        break;
                }
        }

        if(found == true){                                      //if the element is found, function returns index
                return index;
        }

        else{                                                   //if the element is not found, funciton returns false (-1)
                return -1;
        }
}
