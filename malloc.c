#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#define SMALL_BLOCK_MAX_SIZE 8192 //defining the max size for the small blocks - maximum of sbrk call

typedef struct dataBlocks {
    //data about current block
    int used;
    size_t length;

    //data about next block
    int nextUsed;
    size_t nextLength;

    //defining struct pointers
    struct dataBlocks* prevBlock;
    struct dataBlocks* nextBlock;
} dataBlocks;

static dataBlocks head;
static dataBlocks* addr = &head;
static int* tail;

void * new_malloc(size_t size)
{
    if (size <= 0) //making sure that the size cant be less or equal to 0
    {
        printf("Size CANNOT be 0! ERROR!\n");
        return NULL;
    }
    
    if(tail == NULL){
        printf("Tail has gone to hell! Program break.\n");
        tail = (int*) sbrk(0);  //assign the tail as the program break - end
    }

    if (size < SMALL_BLOCK_MAX_SIZE) //define max size, should be less than 8192
    {
        printf("Size is smaller than 8192.\n");
       if (addr->nextUsed == 0)  //if not used
       {
           if (size > addr->nextLength) //check if there is enough memory for block to fit, if not
           {
               tail = sbrk(((int*)addr->nextBlock + addr->nextLength) - tail); //assign more memory
           } else if (addr->nextLength == 0 || size < addr->nextLength) //if the block fits and there is enough memory
           {
               addr->nextUsed = 1; //make it used
               addr->nextLength = size; //assign the size at the lengths head adress
           }
       }
    } // else if (mmap)
};

void new_free(void * ptr)
{
    ptr = ptr - 7; //getting the head of our pointer
    dataBlocks freeBlock = (dataBlocks) ptr; //making the pointer be a free block
    freeBlock->nextUsed = 0;    //making the free block false, as not used
};

int main(){
    
}