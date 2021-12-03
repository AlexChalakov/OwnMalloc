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

static dataBlocks *head; //head of our data
static dataBlocks *tail; //tail of our data
struct dataBlocks *addr;  //an address pointer 
//struct dataBlocks *freeList; //FREELIST POINTER VARIABLE
static int memoryRem = SMALL_BLOCK_MAX_SIZE;   //initialising the memory thats remaining with the whole memory, it starts from the max
static void* startAddress; //to record my starting address

void initialise() //initialising the variables
{   
    addr->used = 0;
    addr->length = 0;
}

void traverseFreeList()
{
    struct dataBlocks *freelist = head; //making the freelist point to our head
    while (freelist->nextBlock != NULL) //looping until the end
    {
            printf("LOOP1\n");
        if (freelist->used==0) //if its free
        {
            printf("LOOP2\n");
            printf("%p - %ld\n", (void*) &freelist, freelist->length); //print it out
        }
        freelist = freelist->nextBlock; //goes through the loop all over again
    }
}

/*void coalesce() //merging consecutive free blocks - not working rn
{
    struct dataBlocks *currBlock; //a metadata pointer to the current block
    printf("FIRST");
    currBlock = freeList; //pointing it to the memory
    printf("SECOND");
    while ((currBlock->nextBlock)!= NULL) //traverses through the data blocks
    {
        printf("Free traverse initiated.");
        if ((currBlock->used == 0) && (currBlock->nextBlock->used == 0)) //if the current and the next one are not used, execute
        {
            printf("THIRD");
            currBlock->length += (currBlock->nextBlock->length) + sizeof(struct dataBlocks); //gets the current block length and adds the next one to it
            currBlock->nextBlock = currBlock -> nextBlock -> nextBlock; //going to the next next block
        }
        printf("FOURTH");

        currBlock->prevBlock = currBlock; //making the previous block the current block
        currBlock = currBlock -> nextBlock; //going to the next one and so on
    }
}*/

void * new_malloc(size_t size)
{
    initialise(); //initiliasing the variables
    if (size <= 0) //making sure that the size cant be less or equal to 0
    {
        printf("Size CANNOT be 0! ERROR!\n");
        return NULL;
    }

    if (size > memoryRem) //if the size is bigger than 8192
    {
        sbrk(SMALL_BLOCK_MAX_SIZE); //we request one more block of 8192 memory
        size  = size - SMALL_BLOCK_MAX_SIZE; //seeing whats left for the new block
        printf("Getting more memory!\n");
    }

    memoryRem = memoryRem - size; //making space less and less, allocating size until 8192

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

               int allocation = size + sizeof(addr->length); //where the allocation happens
               unsigned long int tempAddr = (unsigned long int)addr;

               tempAddr += allocation; //puts the size + length in the temporary address
               addr = (void *)tempAddr; //which points to the new address after allocation

               if (head == NULL)    //whenever head is null, only goes once in this loop at the end
               {    //double linked list implementation
                   head = addr; 
                   tail = addr;
               } else {
                   tail->nextBlock = addr; //whenever we add a new item, we chain the next to the tail
                   addr->prevBlock = tail; //and the previous to the new address
                   tail = addr;
               }
           }
       }
    } // else if (mmap)
};

void new_free(void * ptr) 
{
    if (ptr == NULL) //can't be null
    {
        return;
    }
    
    dataBlocks* freeBlock = (dataBlocks*) ptr; //making the pointer be a free block
    printf("Used: %d,so address is NOT FREE.\n", freeBlock->used);
    freeBlock->used = 0;  //making the free block false, as not used
    traverseFreeList();
    //coalesce();
};

int main()
{
    char letter;    //initialising the needed character for the scanf 
    void* ptr;

    addr = sbrk(0); //getting the current address;
    startAddress = addr; //making the addr the starting address
    sbrk(SMALL_BLOCK_MAX_SIZE); //adding initial memory
    printf("Initial Address: %p\n", startAddress);

    for(;;) //loop
    {
        scanf("%c", &letter); //scanning for the letter for each operation
        if (letter == 'A' || letter == 'F')
        {
            if (letter == 'A')
            {
                int sizeBytes; //just for the scanf
                scanf("%d", &sizeBytes);
                ptr = new_malloc(sizeBytes); //puts the malloc operation in the pointer
                printf("Address after allocation: %p\n", addr); //see the allocated address
            } else if (letter == 'F')
            {
                scanf("%p", &ptr);
                //printf("%p\n", ptr);
                new_free(ptr);
                printf("Used: %d,so address is freed.\n", addr->used);
            }
        }
    }
}