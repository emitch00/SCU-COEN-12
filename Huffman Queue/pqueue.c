#include <stdio.h>                                                                                              //libraries
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "pqueue.h"

#define INIT_LENGTH 10
#define p(x) (((x) - 1) / 2)                                                                                    //calculate index of parent
#define l(x) ((x) * 2 + 1)                                                                                      //calculate index of left child
#define r(x) ((x) * 2 + 2)                                                                                      //calculate index of right child

typedef struct pqueue{                                                                                          //priority queue structure
        int (*compare)();                                                                                       //compare function
        void **data;                                                                                            //data array
        int count;                                                                                              //number of elements in array
        int length;                                                                                             //length of array

}PQ;

PQ *createQueue(int (*compare)()){                                                                              //createQueue function: O(1)
        PQ *pq = malloc(sizeof(PQ));                                                                            //allocate memory for priority queue
        assert(pq != NULL);

        pq->count = 0;                                                                                          //set count to 0
        pq->length = INIT_LENGTH;                                                                               //set length to 10
        pq->compare = compare;                                                                                  //set compare to compare

        pq->data = malloc(sizeof(void*) * pq->length);                                                          //allocate memory for array
        assert(pq->data != NULL);

        return pq;                                                                                              //return priority queue
}

void destroyQueue(PQ *pq){                                                                                      //destroyQueue function: O(n)
        int i;
        assert(pq != NULL);

        for(i=0; i<pq->count; i++)                                                                              //free each index in array
                free(pq->data[i]);

        free(pq->data);                                                                                         //free array
        free(pq);                                                                                               //free priority queue
}

int numEntries(PQ *pq){                                                                                         //numEntries function: O(1)
        assert(pq != NULL);

        return pq->count;                                                                                       //return number of elements in array
}

void addEntry(PQ *pq, void *entry){                                                                             //addEntry function: O(logn)
        assert(pq != NULL && entry != NULL);

        if(pq->length == pq->count){                                                                            //checks if priority queue is full
                pq->length *=2;
                pq->data = realloc(pq->data, sizeof(void*) * pq->length);                                       //reallocates memory for an array that is twice as big
        }

        pq->data[pq->count] = entry;
        int index = pq->count;                                                                                  //index of new entry

        while(pq->count != 0 && (*pq->compare)(pq->data[p(index)], pq->data[index]) > 0){                       //if child is smaller than parent, swap
                void* child = pq->data[index];
                pq->data[index] = pq->data[p(index)];
                pq->data[p(index)] = child;
                index = p(index);
        }

        pq->count++;                                                                                            //increment count
}

void *removeEntry(PQ *pq){                                                                                      //removeEntry function: O(logn)
        assert(pq != NULL);

        void* root = pq->data[0];
        pq->data[0] = pq->data[pq->count - 1];                                                                  //swap root with last item
        int index = 0;

        while(l(index) <= pq->count - 1){                                                                       //heap down on the left
                int smallerchild = l(index);
                if(r(index) <= pq->count - 1){                                                                  //if right child exists
                        if((*pq->compare)(pq->data[r(index)], pq->data[l(index)]) < 0)                          //if right child is smaller, use its index
                                smallerchild = r(index);
                }

                if((*pq->compare)(pq->data[index], pq->data[smallerchild]) < 0){                                //if parent is smaller than child, break
                        break;
                }

                else{                                                                                           //swap parent with smaller child
                        void* child = pq->data[index];
                        pq->data[index] = pq->data[smallerchild];
                        pq->data[smallerchild] = child;
                }

                index = smallerchild;                                                                           //update index
        }

        pq->count--;                                                                                            //decrement count

        return root;                                                                                            //return root
}
