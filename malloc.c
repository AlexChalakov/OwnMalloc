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
static dataBlocks *addr = &head;
struct dataBlocks *freeList = (void*)SMALL_BLOCK_MAX_SIZE;
static int* tail;

void initialise()
{   }

void coalesce() //merging consecutive free blocks 
{
    struct dataBlocks *currBlock; //a metadata pointer to the current block
    currBlock = freeList; //pointing it to the memory
    while ((currBlock->nextBlock)!= NULL) //traverses through the data blocks
    {
        printf("Free traverse initiated.");
        if ((currBlock->used == 0) && (currBlock->nextBlock->used == 0)) //if the current and the next one are not used, execute
        {
            currBlock->length += (currBlock->nextBlock->length) + sizeof(struct dataBlocks); //gets the current block length and adds the next one to it
            currBlock->nextBlock = currBlock -> nextBlock -> nextBlock; //going to the next next block
        }
        currBlock->prevBlock = currBlock; //making the previous block the current block
        currBlock = currBlock -> nextBlock; //going to the next one and so on
    }
}

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

               int allocation = size + sizeof(addr->length);
               unsigned long int tempAddr = (unsigned long int)addr;

               newAddr += allocation;
               addr = (void *)tempAddr;
           }

           if ((((void*)addr->nextBlock) + addr->length) > (void*)tail) //if address goes beyond the tail, add more memory
           {
               printf("Getting more memory!\n");
               tail = sbrk(SMALL_BLOCK_MAX_SIZE); //assign 1 more block of 8192 bytes
           }

           if (addr->nextBlock == NULL)
           {
               printf("Next Block is NULL.\n");
               addr->nextBlock = addr + addr->length + 7; //taking the end of the memory block, if removed gives segmentation fault
               
               addr->nextBlock->used = 0; //making the new block unused
               addr->nextBlock->length = 0; //making it 0 length

               addr->prevBlock = addr; //making the new block a previous block
               addr->prevBlock->used = 1; //making the previous block used
               addr->prevBlock->length = addr->length;//assigning the length
           }
       }
    } // else if (mmap)
};

void new_free(void * ptr)   //
{
    if (ptr == NULL)
    {
        return;
    }
    
    dataBlocks* freeBlock = (dataBlocks*) ptr; //making the pointer be a free block
    //--freeBlock; //going back by 1
    freeBlock->used = 0;    //making the free block false, as not used
    //coalesce(); //merging free blocks
};

int main()
{
    char letter;    //initialising the needed character for the scanf 
    void* ptr;

    addr = sbrk(0); //getting the current address;
    sbrk(SMALL_BLOCK_MAX_SIZE); //adding initial memory
    printf("Initial Address: %p\n", addr);

    for(;;)
    {
        //printf("Initial Address: %p\n", addr);
        scanf("%c", &letter);
        if (letter == 'A' || letter == 'F')
        {
            if (letter == 'A')
            {
                int sizeBytes;
                scanf("%d", &sizeBytes);
                ptr = new_malloc(sizeBytes);
                printf("Address after allocation: %p\n", addr);
            } else if (letter == 'F')
            {
                ptr = addr;
                new_free(ptr);
                printf("Used: %d,so address is freed.\n", addr->used);
                //printf("Address after freeing: %p\n", addr);
            }
        }
    }
    //void *ptr = new_malloc(2);
    //printf("Address after allocation: %p\n", addr);
}