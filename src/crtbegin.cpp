
#ifndef WIN32
typedef void (*ctor_func)(void);

#if (__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ < 7))
ctor_func ctors_begin[1] __attribute__ ((section(".ctors"))) = { ((ctor_func)-1) };
#else
ctor_func ctors_begin[1] __attribute__ ((section(".init_array"))) = { ((ctor_func)-1) };
#endif

void run_hooks()
{
    const ctor_func* list = ctors_begin;
    while(((long)*++list) != -1)
    {
        (**list)();
    }
}
#endif
