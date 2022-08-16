//
//  my_malloc-driver.c
//  my_malloc-driver
//
//  Created by Zain 👻 on 17/10/2021.
//

#include <stdio.h>
#include "my_malloc.h"
#include <stddef.h>

void print_free_list(FreeListNode header)
{
    printf("Free List :  ");
    if (NULL == header)
    {
        printf("NULL\n");
    }
    else
    {
        FreeListNode fnode = header;
        while (fnode)
        {
            printf("%lu ", fnode->size);
            fnode = fnode->flink;
        }
        printf("\n");
    }
}
int main(int argc, char* agrv[])
{
    int Array[3] = {10, 50, 100};
    int *ArrayPtr[3];
    FreeListNode dummyHead = free_list_begin(); // Header should be null here
    for (int i = 0; i < 3; i++)
    {
        ArrayPtr[i] = (int*) my_malloc(sizeof(int) * Array[i]);
        dummyHead = free_list_begin();
        print_free_list(dummyHead);
        my_free(ArrayPtr[i]);
        dummyHead = free_list_begin();
        print_free_list(dummyHead);
    }
    coalesce_free_list();
    return 0;
    
}
