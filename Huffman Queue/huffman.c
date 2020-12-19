#include <stdlib.h>                                                                                                             //libraries
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "pqueue.h"
#include "pack.h"


typedef struct node NODE;                                                                                                       //declare given node structure

int counts[257];                                                                                                                //array to store character counts
NODE *nodes[257];                                                                                                               //array to store nodes in priority queue

static void readfile(char *filename);                                                                                           //function prototypes
static NODE *mknode(int data, NODE *left, NODE *right);
static int nodecmp(NODE *t1, NODE *t2);
static int depth(NODE *np);


int main(int argc, char *argv[]){                                                                                               //main function: O(n)
        int i, j;
        if(argc == 2){                                                                                                          //requires two files, input and output
                printf("Missing file\n");
                return 0;
        }

        readfile(argv[1]);                                                                                                      //reads input file
        PQ *pq = createQueue(nodecmp);                                                                                          //creates priority queue

        for(i=0; i<256; i++){
                if(counts[i] > 0){
                        nodes[i] = mknode(counts[i], NULL, NULL);                                                               //creates node for every character
                        addEntry(pq, nodes[i]);
                }
        }

        nodes[256] = mknode(0, NULL, NULL);                                                                                     //creates null node at end for EOF
        addEntry(pq, nodes[256]);

        while(numEntries(pq) > 1){                                                                                              //combine nodes
                NODE *left = removeEntry(pq);
                NODE *right = removeEntry(pq);
                NODE *parent = mknode(left->count + right->count, left, right);
                addEntry(pq, parent);
        }

        for(j=0; j<256; j++){
                if(nodes[j] != NULL){
                        printf(isprint(j) ? "'%c'" : "%03o", j);
                        printf(": %d x %d bits = %d bits\n", counts[j], depth(nodes[j]), counts[j]*depth(nodes[j]));            //print out bit values for each character
                }
        }

        pack(argv[1], argv[2], nodes);                                                                                          //pack function compresses file
        destroyQueue(pq);                                                                                                       //destroys the queue

        return 0;
}

static void readfile(char *filename){                                                                                           //readfile function: O(n)
        FILE *fp = fopen(filename, "r");                                                                                        //open file

        if(fp == NULL){
                printf("Can't open file");
                return;
        }

        int c = getc(fp);                                                                                                       //obtain character ASCII value
        while(c != EOF){
                counts[c]++;                                                                                                    //add frequency to each character
                c = getc(fp);
        }

        fclose(fp);                                                                                                             //close file
}

static NODE *mknode(int data, NODE *left, NODE *right){                                                                         //mknode function: O(1)
        NODE *np = malloc(sizeof(NODE));                                                                                        //allocate memory for node
        assert(np != NULL);

        np->parent = NULL;                                                                                                      //no parent for node
        np->count = data;

        if(left != NULL)
                left->parent = np;                                                                                              //nodes children

        if(right != NULL)
                right->parent = np;

        return np;                                                                                                              //return node
}

static int nodecmp(NODE *t1, NODE *t2){                                                                                         //nodecmp function: O(1)
        assert(t1 != NULL && t2 != NULL);

        return (t1->count < t2->count) ? -1 : (t1->count > t2->count);                                                          //return the greater count
}

static int depth(NODE *np){                                                                                                     //depth function: O(h)
        assert(np != NULL);

        if(np->parent == NULL)
                return 0;

        return depth(np->parent) + 1;                                                                                           //recursive, returns depth of node
}
