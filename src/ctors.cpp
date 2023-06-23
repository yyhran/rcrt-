
typedef void (*init_func)(void);

#ifdef WIN32
#pragma section(".CRT$CA", long, read)
#pragma section(".CRT$CZ", long, read)

__declspec(allocate(".CRT$CA")) init_func ctors_begin[] = {0};
__declspec(allocate(".CRT$CZ")) init_func ctors_end[] = {0};

eextern "C" void do_global_ctors()
{
    init_func* p = ctors_begin;
    while(p < ctors_end)
    {
        if(*p)
        {
            (**p)();
        }
        ++p;
    }
}
#else
void run_hooks();
extern "C" void do_global_ctors()
{
    run_hooks();
}
#endif
