
extern "C" void* malloc(unsigned int);
extern "C" void free(void*);

typedef unsigned int size_t;

void* operator new(unsigned long size)
{
    return malloc(size);
}

void operator delete(void* p, unsigned long size)
{
    free(p);
}

void* operator new[](unsigned long size)
{
    return malloc(size);
}

void operator delete[](void* p)
{
    free(p);
}
