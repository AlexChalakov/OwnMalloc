#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

typedef struct dataBlocks {
    //data about previous block
    int prevUsed;
    size_t prevLength;

    //data about next block
    int nextUsed;
    size_t nextLength;

    //defining struct pointers
    struct dataBlocks* prevBlock;
    struct dataBlocks* nextBlock;
} dataBlocks;

static dataBlocks head;
static int* tail;

//void * new_malloc(size_t size); //declare functions
//void new_free(void * ptr); 

void * new_malloc(size_t size)
{
    if (head.nextUsed == NULL) //if the head of the next block is null
    {
        head.prevUsed -= 1; //decrease by 1 as we jump off the previous one
        head.prevLength -= 1;

        head.nextUsed = 0; //define the new block
        head.nextLength = 0;
    }
    
    if (tail == NULL) //if tail is null
    {
        tail = (int*) sbrk(0);  //request more memory space at the tail
    } 

    if (size < 8192)
    {
        
    }
    
};

void new_free(void * ptr)
{

};

int main(){
    void * printOne = new_malloc(0);
    printf("Address: %p\n", printOne);
}