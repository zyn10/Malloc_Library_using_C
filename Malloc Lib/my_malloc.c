//
//  my_malloc.c
//  my_malloc
//
//  Created by Zain 👻 on 17/10/2021.
//

#include <unistd.h>
#include <stddef.h>
#include "my_malloc.h"

FreeListNode list_ = NULL;
MyErrorNo error_number;

int size_Cal(size_t size)
{
    int remain = (size + 8) % 8;
    int dummy = remain + (size + 8);
    return dummy;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////          M   Y _ M    A    L    L    O    C      ////////////////////////////////////////
/////////////////////            returns a pointer to a chunk of heap allocated memory             /////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void *my_malloc(size_t size)
{
    if (size < 0)//-------->if it is a first node of linked list
    {
        error_number = MYENOMEM;
        return NULL;
    }
    if (list_ == NULL)//-------->if it is not first node of linked list
    {
        if (size_Cal(size) <= 8192)
        {
            list_ = (FreeListNode)sbrk(8192);//------->8192 is the minimum size
            list_->flink = NULL;
            if (8192 - size_Cal(size) >= 16)//-------->check size and return size
            {
                list_->size = 8192 - size_Cal(size);
                int *ret_ptr;
                ret_ptr = (int*)((void*)list_ + list_->size);
                *ret_ptr = size_Cal(size); //--------->putting size into address
                int *crosscheck;
                crosscheck = (int*)((void*)list_ + list_->size + 4);
                *crosscheck = 100;
                return (char *)list_ + list_->size + 8;
            }
            else//--------->nothing to go with
            {
                list_->size = 0;
                int *ret_ptr;  //--------->putting size into address
                ret_ptr = (int*)((void*)list_ + list_->size);
                *ret_ptr = size_Cal(size);
                int *crosscheck;
                crosscheck = (int*)((void*)list_ + list_->size + 4);
                *crosscheck = 100;
                list_ = NULL;
                return (char *)sbrk(0) + 8;
            }
        }
        else
        {
            list_ = (FreeListNode)sbrk(size_Cal(size));
            list_->size = 0;
            list_->flink = NULL;
            int *ret_ptr;  //--------->putting size into address
            ret_ptr = (int*)((void*)list_ + list_->size);
            *ret_ptr = size_Cal(size);
            int *crossCheck;
            crossCheck = (int*)((void*)list_ + list_->size + 4);
            *crossCheck = 100;
            list_ = NULL;
            return (char*)sbrk(0) + 8;
        }
    }
    else//---------->creation of linked list
    {
        struct freelistnode *newNode_;
        struct  freelistnode *ptr_prev;
        newNode_ = list_;
        ptr_prev = NULL;
        while (newNode_ != NULL)
        {
            int nodeSize = newNode_->size;
            int actualSize = size_Cal(size);
            if (nodeSize - actualSize >= 16)
            {
                newNode_->size = nodeSize - actualSize;//finding memory
                int *ret_ptr;//putting the size into ptr
                ret_ptr = (int*)((void*)newNode_ + newNode_->size);
                *ret_ptr = size_Cal(size);
                int *crosscheck;
                crosscheck = (int*)((void*)newNode_ + newNode_->size + 4);
                *crosscheck = 100;
                return (char *)list_ + list_->size + 8;
            }
            else if (nodeSize - actualSize > 0)
            {
                newNode_->size = 0;
                int *retPtr;//putting the size into ptr
                retPtr = (int*)((void*)newNode_ + newNode_->size);
                *retPtr = size_Cal(size);
                int *crosscheck;
                crosscheck = (int*)((void*)newNode_ + newNode_->size + 4);
                *crosscheck = 100;
                ptr_prev->flink = newNode_->flink;
                newNode_ = NULL;
                return (char *)sbrk(0) + 8;
            }
                ptr_prev = newNode_;
                newNode_ = newNode_->flink;//-------->move to next
        }
        if (size_Cal(size) <= 8192)//incasee of no space minimum size is 8192
        {
            newNode_ = (FreeListNode) sbrk(8192);
            newNode_->flink = NULL;
            if (8912 - size_Cal(size) >= 16)//creating free space
            {
                newNode_->size = 8192 - size_Cal(size);
                int *retPtr;//------------------>putting size back
                retPtr = (int *) ((void *) newNode_ + newNode_->size);
                *retPtr = size_Cal(size);
                int *crosscheck;
                crosscheck = (int *) ((void *) newNode_ + newNode_->size + 4);
                *crosscheck = 100;
                ptr_prev->flink = newNode_;
                return (char *)newNode_ + newNode_->size + 8;
            }
            else
            {
                newNode_->size = 0;
                int *retPtr;
                retPtr = (int *) ((void *) newNode_ + newNode_->size);
                *retPtr = size_Cal(size);
                int *check;
                check = (int *) ((void *) newNode_ + newNode_->size + 4);
                *check = 100;
                newNode_ = NULL;
                ptr_prev->flink = NULL;
                return (char *)sbrk(0) + 8;
            }
        }
        else
        {
            newNode_ = (FreeListNode)sbrk(size_Cal(size));
            newNode_->size = 0;
            newNode_->flink = NULL;
            int *retPtr;//------------------>putting size back
            retPtr = (int *) ((void *) newNode_ + newNode_->size);
            *retPtr = size_Cal(size);
            int *crosscheck;
            crosscheck = (int *) ((void *) newNode_ + newNode_->size + 4);
            *crosscheck = 100;
            newNode_ = NULL;
            ptr_prev->flink = NULL;
            return sbrk(0) + 8;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////              M   Y _ F    R    E    E               ////////////////////////////////////////
/////////////////////        reclaims the previously allocated chunk referenced by ptr            /////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void my_free(void *inputPtr)
{
    size_t sizeCheck;
    struct freelistnode *newNode_;
    struct freelistnode *dummyNode;
    struct  freelistnode *ptrPrv;
    dummyNode = list_;
    ptrPrv = NULL;
    if ((*(char*)(inputPtr - 4)) == 100)
    {
        sizeCheck = *(size_t*)(char*)(inputPtr - 8);
        if (list_ == NULL)
        {
            list_ = (FreeListNode) inputPtr;
            list_->size = sizeCheck;
            list_->flink = NULL;
            return;
        } else if (list_->flink == NULL)
        {
            if ((void*)inputPtr >= (void *)list_)
            {
                newNode_ = (FreeListNode) inputPtr;
                newNode_->size = sizeCheck;
                list_->flink = newNode_;
                newNode_->flink = NULL;
                return;
            }
            else
            {
                newNode_ = list_;
                list_ = (FreeListNode) inputPtr;
                list_->size = sizeCheck;
                list_->flink = newNode_;
                newNode_->flink = NULL;
                return;
            }
        }

        while (dummyNode != NULL)
        {
            ptrPrv = dummyNode;
            dummyNode = dummyNode->flink;
            if ((void*)inputPtr >= (void *)ptrPrv && inputPtr <= (void *)dummyNode)
            {
                newNode_ = (FreeListNode) inputPtr;
                newNode_->size = sizeCheck;
                newNode_->flink = dummyNode;
                ptrPrv->flink = newNode_;
                return;
            }
        }//null node
        newNode_ = (FreeListNode) inputPtr;
        newNode_->size = sizeCheck;
        newNode_->flink = NULL;
        ptrPrv->flink = newNode_;
        return;
    }
    else
    {
        error_number = MYBADFREEPTR;
        return;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////          F R E E _  L I S T _B E G I N      ////////////////////////////////////////
/////////////////////            returns pointer to first chunk in free list                                     /////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FreeListNode free_list_begin(void)
{
    return list_;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////         Coalesce Free List                       ////////////////////////////////////////
/////////////////////            rmerge adjacent chunks on the free list                                     /////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void coalesce_free_list(void)
{
    struct freelistnode *node;
    if (list_ == NULL)
    {
        return;
    }
    node = list_;
    while (node->flink != NULL)
    {    int current_size = node->size;
        int next_size = node->flink->size;
        if (node + node->size == node->flink)
        {
            node->size = current_size + next_size;
            if (node->flink->flink != NULL)
            {
                node->flink = node->flink->flink;
            }
            else
            {
                node->flink = NULL;
            }
        }
        else
        {
            node = node->flink;
        }
    }
    return;
}
