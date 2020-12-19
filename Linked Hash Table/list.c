#include "list.h"                                                               //libraries
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef struct node{                                                            //node structure
        void *data;
        struct node *next;
        struct node *prev;
}NODE;

typedef struct list{                                                            //list structure
        int count;
        NODE* head;
        int (*compare)();
}LIST;

LIST *createList(int (*compare)()){                                             //createList function   Average:O(1)    Worst:O(1)
        LIST *lp = malloc(sizeof(LIST));                                        //allocate memory for list
        assert(lp != NULL);

        lp->count = 0;

        lp->head = malloc(sizeof(NODE));                                        //allocate memory for head node
        assert(lp->head != NULL);

        lp->compare = compare;
        lp->head->next = lp->head;                                              //adjust pointers
        lp->head->prev = lp->head;

        return lp;                                                              //return list
}

void destroyList(LIST *lp){                                                     //destroyList function  Average:O(n)    Worst:O(n)
        assert(lp != NULL);

        NODE *pDel;                                                             //initialize delete and prev nodes
        NODE *pPrev = lp->head->prev;

        do{
                pDel = pPrev;
                pPrev = pDel->prev;                                             //traverse list and destroy each element
                free(pDel);
        } while(pDel != lp->head);

        free(lp);                                                               //free list
        return;
}

int numItems(LIST *lp){                                                         //numItems function     Average:O(1)    Worst:O(1)
        assert(lp != NULL);

        return lp->count;                                                       //return count
}

void addFirst(LIST *lp, void *item){                                            //addFirst function     Average:O(1)    Worst:O(1)
        assert(lp != NULL);

        NODE *pAdd = malloc(sizeof(NODE));                                      //allocate memory for node
        assert(pAdd != NULL);

        pAdd->data = item;
        lp->head->next->prev = pAdd;
        pAdd->next = lp->head->next;                                            //adjust pointers
        pAdd->prev = lp->head;
        lp->head->next = pAdd;

        lp->count++;
        return;
}

void addLast(LIST *lp, void *item){                                             //addLast function      Average:O(1)    Worst:O(1)
        assert(lp != NULL);

        NODE *addNode = malloc(sizeof(NODE));                                   //allocate memory for node
        assert(addNode != NULL);

        addNode->data = item;
        lp->head->prev->next = addNode;
        addNode->next = lp->head;                                               //adjust pointers
        addNode->prev = lp->head->prev;
        lp->head->prev = addNode;
        lp->count++;

        return;
}

void *removeFirst(LIST *lp){                                                    //removeFirst function  Average:O(1)    Worst:O(1)
        assert(lp != NULL && lp->count>0);

        NODE *deleteNode = lp->head->next;                                      //initialize node to be deleted
        void *cp = deleteNode->data;                                            //create copy of node data

        lp->head->next = deleteNode->next;                                      //adjust pointers
        deleteNode->next->prev = lp->head;

        free(deleteNode);
        lp->count--;

        return cp;                                                              //return copy data
}

void *removeLast(LIST *lp){                                                     //removeLast function   Average:O(1)    Worst:O(1)
        assert(lp != NULL && lp->count>0);

        NODE *deleteNode = lp->head->prev;                                      //initialize node to be deleted
        void *cp = deleteNode->data;                                            //create copy of node data

        lp->head->prev = deleteNode->prev;                                      //adjust pointers
        deleteNode->prev->next = lp->head;

        free(deleteNode);
        lp->count--;

        return cp;                                                              //return copy data
}

void *getFirst(LIST *lp){                                                       //getFirst function     Average:O(1)    Worst:O(1)
        assert(lp != NULL && lp->count>0);

        return lp->head->next->data;                                            //return first node data
}

void *getLast(LIST *lp){                                                        //getLast function      Average:O(1)    Worst:O(1)
        assert(lp != NULL && lp->count>0);

        return lp->head->prev->data;                                            //return last node data
}

void removeItem(LIST *lp, void *item){                                          //removeItem function   Average:O(n)    Worst:O(n)
        assert(lp != NULL);

        if(lp->count>0){
                NODE *trav = lp->head->next;
                while(trav != lp->head){
                        if((*lp->compare)(trav->data, item) == 0){              //compare node data with item
                                trav->prev->next = trav->next;                  //adjust pointers
                                trav->next->prev = trav->prev;

                                free(trav);                                     //free traversal node
                                lp->count--;

                                break;
                        }

                        else{
                                trav = trav->next;                              //move traversal node
                        }
                }
        }

        return;
}

void *findItem(LIST *lp, void *item){                                           //findItem function     Average:O(n)    Worst:O(n)
        assert(lp != NULL);

        if(lp->count>0){
                NODE *trav = lp->head->next;
                while(trav != lp->head){                                        //traverse list
                        if((*lp->compare)(trav->data, item) == 0)               //compare node data with item
                                return trav->data;                              //return node data
                        else{
                                trav = trav->next;
                        }
                }
        }

        return NULL;
}

void *getItems(LIST *lp){                                                       //getItems function     Average:O(n)    Worst:O(n)
        assert(lp != NULL);

        void **array = malloc(sizeof(void*) * lp->count);                       //allocate memory for array
        assert(array != NULL);

        int i = 0;
        NODE *trav = lp->head->next;
        while(trav != lp->head){                                                //traverse array and add data to array
                array[i++] = trav->data;
                trav = trav->next;
        }

        return array;                                                           //return array
}
