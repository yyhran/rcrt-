#include "../include/rcrt.h"

typedef struct _heap_header
{
    enum
    {
        HEAP_BLOCK_FREE = 0xABABABAB,
        HEAP_BLOCK_USED = 0xCDCDCDCD,
    } type;

    unsigned size;
    struct _heap_header* next;
    struct _heap_header* prev;
} heap_header;

#define ADDR_ADD(a, o) (((char*)(a)) + o)
#define HEAPER_SIZE (sizeof(heap_header))

static heap_header* g_list_head = NULL;

void free(void* ptr)
{
    heap_header* header = (heap_header*)ADDR_ADD(ptr, -HEAPER_SIZE);
    if(header->type != HEAP_BLOCK_USED)
    {
        return;
    }

    header->type = HEAP_BLOCK_FREE;

    if(header->prev != NULL && header->prev->type == HEAP_BLOCK_FREE)
    {
        // merge
        header->prev->next = header->next;
        if(header->next != NULL)
        {
            header->next->prev = header->prev;
        }
        header->prev->size += header->size;
        header = header->prev;
    }

    if(header->next != NULL && header->next->type == HEAP_BLOCK_FREE)
    {
        // merge
        header->size += header->next->size;
        header->next = header->next->next;
    }
}

void* malloc(unsigned size)
{
    if(size == 0)
    {
        return NULL;
    }

    heap_header* header = g_list_head;
    while(header != NULL)
    {
        if(header->type == HEAP_BLOCK_USED)
        {
            header = header->next;
            continue;
        }

        if(header->size > size + HEAPER_SIZE &&
           header->size <= size + HEAPER_SIZE * 2)
        {
            header->type = HEAP_BLOCK_USED;
            return ADDR_ADD(header, HEAPER_SIZE);
        }

        if(header->size > size + HEAPER_SIZE * 2)
        {
            // split
            heap_header* next = (heap_header*)ADDR_ADD(header, size + HEAPER_SIZE);
            next->prev = header;
            next->next = header->next;
            next->type = HEAP_BLOCK_FREE;
            next->size = header->size - (size - HEAPER_SIZE);

            header->next = next;
            header->size = size + HEAPER_SIZE;
            header->type = HEAP_BLOCK_USED;
            return ADDR_ADD(header, HEAPER_SIZE);
        }

        header = header->next;
    }

    return NULL;
}

#ifndef WIN32
static int brk(void* end_data_segment)
{
    int ret = 0;
    asm("movq $12, %%rax \n\t"
        "mov  %1, %%rdi  \n\t"
        "syscall         \n\t"
        "mov  %%eax, %0  \n\t"
        :"=r"(ret): "m"(end_data_segment)
    );
    return ret;
}
#else
#include <Windows.h>
#endif

int rcrt_init_heap()
{
    void * base = NULL;
    heap_header* header = NULL;
    // just 64MB
    unsigned heap_size = 1024 * 1024 * 64;

#ifdef WIN32
    base = VirtualAlloc(0, heap_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(base == NULL)
    {
        return NULL;
    }
#else
    base = (void*)brk(0);
    void* end = ADDR_ADD(base, heap_size);
    end = (void*)brk(end);
    if(end == NULL)
    {
        return NULL;
    }
#endif

    header = (heap_header*)base;
    header->size = heap_size;
    header->type = HEAP_BLOCK_FREE;
    header->next = NULL;
    header->prev = NULL;
    g_list_head = header;

    return 1;
}
