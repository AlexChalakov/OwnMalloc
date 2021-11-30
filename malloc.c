#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#define SMALL_BLOCK_MAX_SIZE 8192 //defining the max size for the small blocks - maximum of sbrk call

typedef struct dataBlocks {
    //data about current block
    int used;
    size_t length;

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
        //printf("Tail has gone to hell! Program break.\n");
        tail = (int*) sbrk(0);  //assign the tail as the program break - end
    }

    if (size < SMALL_BLOCK_MAX_SIZE) //define max size, should be less than 8192
    {
        printf("Used: %d\n", addr->used);
        printf("Length: %ld\n", addr->length);
        printf("Size is smaller than 8192.\n");
       if (addr->used == 0)  //if not used
       {
           printf("Not used.\n");
           if (addr->length == 0 || size < addr->length) //if the block fits and there is enough memory
           {
               printf("Allocating memory!\n");
               addr->used = 1; //make it used
               addr->length = size; //assign the size at the lengths head adress
               printf("Used: %d\n", addr->used);
               printf("Length: %ld\n", addr->length);

               int add = size + sizeof(addr->length);
               unsigned long int newAddr = (unsigned long int)addr;
               newAddr += add;

               addr = (void *)newAddr;
           }

           if ((((void*)addr->nextBlock) + addr->length) > (void*)tail) //if address goes beyond the tail, add more memory
           {
               printf("Getting more memory!\n");
               tail = sbrk(SMALL_BLOCK_MAX_SIZE); //assign 1 more block of 8192 bytes
           }

           if (addr->nextBlock == NULL)
           {
               printf("Next Block is NULL.\n");
               addr->nextBlock = addr + addr->length + 7; //taking the end of the memory block
               addr->nextBlock->used = 0; //making the new block unused
               addr->nextBlock->length = 0; //making it 0 length
               addr->nextBlock->prevBlock = addr; //making the new block a previous block
               addr->nextBlock->prevBlock->used = 1; //making the previous block used
               addr->nextBlock->prevBlock->length = addr->length;//assigning the length

               addr->nextBlock->prevBlock->length = tail - (int*) &(addr->nextBlock->nextBlock);
           } 
       }
    } // else if (mmap)
};

void new_free(void * ptr)
{
    ptr = ptr - 7; //getting the head of our pointer
    dataBlocks* freeBlock = (dataBlocks*) ptr; //making the pointer be a free block
    freeBlock->used = 0;    //making the free block false, as not used
};

int main(){
    char letter;    //initialising the needed character for the scanf 
    //void* ptr;

    addr = sbrk(0); //getting the current address;
    sbrk(SMALL_BLOCK_MAX_SIZE); //adding initial memory
    printf("Initial Address: %p\n", addr);
    /*scanf("%c", &letter);

    if (letter == 'A' || letter == 'F')
    {
        if (letter == 'A')
        {
            int size;
            scanf("%d", &size);
            ptr = new_malloc(size);
        } else if (letter == 'F')
        {
            new_free(ptr);
        }
        
        
    }*/
    void *ptr = new_malloc(2);
    printf("Address after allocation: %p\n", addr);
}