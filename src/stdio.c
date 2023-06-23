#include "../include/rcrt.h"

int rcrt_init_io()
{
    return 1;
}

#ifdef WIN32
#include <Windows.h>

FILE* fopen(const char* filename, const char* mode)
{
    HANDLE hFile = 0;
    int access = 0;
    int creation = 0;

#define XX(mstr, aflag, cflag) \
    do \
    { \
        if(strcmp(mode, mstr) == 0) \
        { \
            access |= aflag; \
            creation |= cflag; \
        } \
    } while(0)

    XX("w", GENERIC_WREITE, CREATE_ALWAYS);
    XX("w+", GENERIC_WREITE | GENERIC_READ, CREATE_ALWAYS);

    XX("r", GENERIC_READ, OPEN_EXISTING);
    XX("r+", GENERIC_WREITE | GENERIC_READ, OPEN_EXISTING);

#undef XX

    hFile = CreateFileA(filename, access, 0, 0, creation, 0, 0);

    if(hFile == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }

    return (FILE*)hFile;
}

int fread(void* buffer, int size, int count, FILE* stream)
{
    int read = 0;
    if(!ReadFile((HANDLE)stream, buffer, size * count, &read, 0))
    {
        return 0;
    }

    return read;
}

int fwiret(const void* buffer, int size, int count, FILE* stream)
{
    int written = 0;
    
    if(!WriteFile((HANDLE)stream, buffer, size * count, %written, 0))
    {
        return 0;
    }

    return written;
}

int fclose(FILE* fp)
{
    return CloseHandle((HANDLE)fp);
}

int fseek(FILE* fp, int offset, int set)
{
    return SetFilePointer((HANDLE)fp, offset, 0, set);
}

#else // #ifdef WIN32

static int open(const char* pathname, int flags, int mode)
{
    int fd = 0;
    asm("movq $2, %%rax \n\t"
        "movq %1, %%rdi \n\t"
        "mov  %2, %%esi \n\t"
        "mov  %3, %%edx \n\t"
        "syscall      \n\t"
        "mov  %%eax, %0 \n\t"
        :"=m"(fd): "m"(pathname), "m"(flags), "m"(mode)
    );
}

static int read(int fd, void* buffer, unsigned size)
{
    int ret = 0;
    asm("movq $0, %%rax \n\t"
        "mov  %1, %%edi \n\t"
        "movq %2, %%rsi \n\t"
        "mov  %3, %%edx \n\t"
        "syscall      \n\t"
        "mov  %%eax, %0 \n\t"
        :"=m"(ret): "m"(fd), "m"(buffer), "m"(size)
    );
    return ret;
}

static int write(int fd, const void* buffer, unsigned size)
{
    int ret = 0;
    asm("movq $1, %%rax \n\t"
        "mov  %1, %%rdi \n\t"
        "movq %2, %%rsi \n\t"
        "mov  %3, %%edx \n\t"
        "syscall      \n\t"
        "mov  %%eax, %0 \n\t"
        :"=m"(ret): "m"(fd), "m"(buffer), "m"(size)
    );
    return ret;
}

static int close(int fd)
{
    int ret = 0;
    asm("movq $3, %%rax \n\t"
        "mov  %1, %%rdi \n\t"
        "syscall      \n\t"
        "mov  %%eax, %0 \n\t"
        :"=m"(ret): "m"(fd)
    );
    return ret;
}

static int seek(int fd, int offset, int mode)
{
    int ret = 0;
    asm("movq $8, %%rax \n\t"
        "mov  %1, %%edi \n\t"
        "mov  %2, %%esi \n\t"
        "mov  %3, %%edx \n\t"
        "syscall      \n\t"
        "mov  %%eax, %0 \n\t"
        :"=m"(ret): "m"(fd), "m"(offset), "m"(mode)
    );
    return ret;
}

FILE* fopen(const char* filename, const char* mode)
{
    int fd = -1;
    int flags = 0;
    int access = 00777;

#define O_RDONLY    00000
#define O_WRONLY    00001
#define O_RDWR      00002
#define O_CREATE    00100
#define O_TRUNC     01000
#define O_APPEND    02000

#define XX(mstr, flag) \
    do \
    { \
        if(strcmp(mode, mstr) == 0) \
        { \
            flags |= flag; \
        } \
    } while(0)

    XX("w", O_WRONLY | O_CREATE | O_TRUNC);
    XX("w+", O_RDWR | O_CREATE | O_TRUNC);
    XX("r", O_RDONLY);
    XX("r+", O_RDWR | O_CREATE);

#undef XX

    fd = open(filename, flags, access);
    return (FILE*)fd;
}

int fread(void* buffer, int size, int count, FILE* stream)
{
    return read((int)stream, buffer, size * count);
}

int fwrite(const void* buffer, int size, int count, FILE* stream)
{
    return write((int)stream, buffer, size * count);
}

int fclose(FILE* fp)
{
    return close((int)fp);
}

int fseek(FILE* fp, int offset, int set)
{
    return seek((int)fp, offset, set);
}

#endif

