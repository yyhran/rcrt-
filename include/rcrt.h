#pragma once

#ifdef __cplusepluse
extern "C" {
#endif

// malloc
#ifndef NULL
#define NULL (0)
#endif

void free(void* ptr);
void* malloc(unsigned size);
static int brk(void* end_data_segment);
int rcrt_init_heap();

// string
char* itoa(int n, char* str, int radix);
int strcmp(const char* src, const char* dst);
char* strcpy(char* dest, const char* src);
unsigned strlen(const char* str);

// file and I/O
typedef int FILE;

#define EOF (-1)

#ifdef WIN32
#define stdin ((FILE*)(GetStdHandle(STD_INPUT_HANDLE)))
#define stdout ((FILE*)(GetStdHandle(STD_OUTPUT_HANDLE)))
#define stderr ((FILE*)(GetStdHandle(STD_ERROR_HANDLE)))
#else
#define stdin ((FILE*)0)
#define stdout ((FILE*)1)
#define stderr ((FILE*)2)
#endif

int rcrt_init_io();
FILE* fopen(const char* filename, const char* mode);
int fread(void* buffer, int size, int count, FILE* stream);
int fwrite(const void* buffer, int size, int count, FILE* stream);
int fclose(FILE* fp);
int fseek(FILE* fp, int offset, int set);

// printf
int fputc(int c, FILE* stream);
int fputs(const char* str, FILE* stream);
int print(const char* format);
int println(const char* format);
int printf(const char* format, ...);
int fprintf(FILE* stream, const char* format, ...);

// internal
void do_global_ctors();
void rcrt_call_exit_routine();

// atexit
typedef void (*atexit_func_t)(void);
int atexit(atexit_func_t func);
    
#ifdef __cplusepluse
}
#endif

