#include <stdio.h>
#define MAX_WORD_LENGTH 30                                              //defining library and word length constant



int main(int argc, char *argv[]){
        int wcount = 0;
        char word[MAX_WORD_LENGTH];
        FILE *fp;                                                       //initializing count variable, buffer, and file pointer

        if(argc == 1){                                                  //checking if a file is inputed
                printf("No file input.\n");
                return 0;
        }

        fp = fopen(argv[1], "r");                                       //opening file to read

        if(fp == NULL)                                                  //checking if file is valid
                printf("Not able to open file %s.\n", argv[1]);

        else{
                while((fscanf(fp, "%s", word)) == 1){                   //adds to count variable while there are scanned words
                        wcount++;
                }
                printf("%d total words\n", wcount);                     //prints final word count
        }

        fclose(fp);                                                     //closes file
}
