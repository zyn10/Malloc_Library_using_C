//
//  my_malloc.h
//  my_malloc
//
//  Created by Zain 👻 on 17/10/2021.
//

#ifndef my_malloc_h
#define my_malloc_h

#include <stdio.h>
#include <stdint.h>

//the size of the header for heap allocated memory chunks
#define CHUNKHEADERSIZE 8//------>for heap allocation

//error signaling
typedef enum {MYNOERROR, MYENOMEM, MYBADFREEPTR} MyErrorNo;
extern MyErrorNo error_number;

//my_malloc: returns a pointer to a chunk of heap allocated memory
void *my_malloc(size_t size);

//my_free: reclaims the previously allocated chunk referenced by ptr
void my_free(void *ptr);

//struct freelistnode: node for linked list of 'free' chunks
typedef struct freelistnode {
    struct freelistnode *flink; //pointer to next free chunk node
    size_t size; //size of current chunk
} * FreeListNode;

//free_list_begin(): returns pointer to first chunk in free list
FreeListNode free_list_begin(void);

//coalesce_free_list(): merge adjacent chunks on the free list
void coalesce_free_list(void);

#endif /* my_malloc_h */
