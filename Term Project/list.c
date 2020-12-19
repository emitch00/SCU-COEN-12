#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct node{                                                                            //node structure
        void **data;                                                                            //data array
        int length;                                                                             //length of array
        int first;                                                                              //position of first element in array
        int count;                                                                              //number of elements in the array

        struct node *next;                                                                      //next and prev pointers
        struct node *prev;
}NODE;


typedef struct list{                                                                            //list structure - doubly linked list
        NODE* tail;                                                                             //tail pointer
        NODE* head;                                                                             //head pointer
        int count;                                                                              //number of nodes in list
}LIST;

NODE *createNode(int length);

LIST *createList(void){                                                                         //createList function: O(1)
        LIST *lp = malloc(sizeof(LIST));                                                        //allocate memory for list pointer
        lp->head = lp->tail = createNode(8);                                                    //set head to empty node of size 8
        lp->count = 0;

        return lp;                                                                              //return list pointer
}

void destroyList(LIST *lp){                                                                     //destroyList function: O(n)
        assert(lp != NULL);

        NODE* pDel;
        while(lp->head != NULL){                                                                //traverse list and free each node and its data
                pDel = lp->head;
                lp->head = lp->head->next;
                free(pDel->data);
                free(pDel);
        }

        free(lp);                                                                               //free list
}

int numItems(LIST *lp){                                                                         //numItems function: O(1)
        assert(lp != NULL);

        return lp->count;                                                                       //returns number of items in list
}

void addFirst(LIST *lp, void *item){                                                            //addFirst function: O(1)
        assert(lp != NULL && item != NULL);

        if(lp->head->count == lp->head->length){                                                //check if first node is full
                NODE* pFirst = createNode(lp->head->length * 2);                                //create node with double array length
                pFirst->next = lp->head;                                                        //fix pointers
                lp->head = pFirst;
                pFirst->next->prev = pFirst;
        }

        int index = ((lp->head->first + lp->head->length - 1) % lp->head->length);              //calculate first index of array
        lp->head->data[index] = item;                                                           //insert item into first slot
        lp->head->first = index;

        lp->head->count++;                                                                      //increment counts
        lp->count++;
}

void addLast(LIST *lp, void *item){                                                             //addLast function: O(1)
        assert(lp != NULL && item != NULL);

        if(lp->tail->count == lp->tail->length){                                                //check if last node is full
                NODE* pLast = createNode(lp->tail->length * 2);                                 //create node with double array length
                pLast->prev = lp->tail;                                                         //fix pointers
                lp->tail->next = pLast;
                lp->tail = pLast;
        }

        int index = ((lp->tail->first + lp->tail->count) % lp->tail->length);                   //calculate last index of array
        lp->tail->data[index] = item;                                                           //insert item into last slot

        lp->tail->count++;                                                                      //increment counts
        lp->count++;
}

void *removeFirst(LIST *lp){                                                                    //removeFirst function: O(1)
        assert(lp != NULL);

        if(lp->head->count == 0){                                                               //check if array is empty
                lp->head = lp->head->next;
                free(lp->head->prev);                                                           //free node
                lp->head->prev = NULL;                                                          //fix pointers
        }

        int index = lp->head->first;                                                            //calculate next index to old first
        int newindex = (index+1) % lp->head->length;
        lp->head->first = newindex;                                                             //set new first to the index

        void *copy = lp->head->data[index];                                                     //copy array data

        lp->head->count--;                                                                      //decrement counts                                                              
        lp->count--;

        return copy;                                                                            //return copy
}

void *removeLast(LIST *lp){                                                                     //removeLast function: O(1)
        assert(lp != NULL);

        if(lp->tail->count == 0){                                                               //check if array is empty
                NODE* pDel = lp->tail;
                lp->tail = lp->tail->prev;                                                      //fix pointers
                lp->tail->next = NULL;
                free(pDel->data);                                                               //free node and node data
                free(pDel);
        }

        int index = ((lp->tail->first + lp->tail->count) % lp->tail->length);                   //calculate index of last item

        void* copy = lp->tail->data[index];                                                     //copy array data

        lp->tail->count--;                                                                      //decrement counts
        lp->count--;

        return copy;                                                                            //return copy
}

void *getItem(LIST *lp, int index){                                                             //getItem function: O(logn)
        assert(lp != NULL);
        assert(index >= 0 && index < lp->count);                                                //assert that given index is valid

        NODE* pTrav = lp->head;                                                                 //initialize traversal node

        while(index >= pTrav->count){                                                           //decrement index while traversing list
                index -= pTrav->count;
                pTrav = pTrav->next;
        }

        void* copy = pTrav->data[(pTrav->first + index) % pTrav->length];                       //copy array data of traversal node

        return copy;                                                                            //return copy
}

void setItem(LIST *lp, int index, void *item){                                                  //setItem funciton: O(logn)
        assert(lp != NULL && item != NULL);
        assert(index >= 0 && index < lp->count);                                                //assert that given index is valid

        NODE* pTrav = lp->head;                                                                 //initialize traversal node

        while(index >= pTrav->count){                                                           //decrement index while traversing list
                index -= pTrav->count;
                pTrav = pTrav->next;
        }

        pTrav->data[(pTrav->first + index) % pTrav->length] = item;                             //insert item at index of traversal node
}

NODE *createNode(int length){                                                                   //createNode function: O(1)
        NODE *np = malloc(sizeof(NODE));                                                        //allocate memory for node

        assert(np != NULL);

        np->data = malloc(sizeof(void**) * length);                                             //allocate memory for array
        np->count = 0;
        np->first = 0;

        np->length = length;
        np->next = NULL;
        np->prev = NULL;

        return np;                                                                              //return node pointer
}
