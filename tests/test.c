#include "../include/rcrt.h"

int main(int argc, char** argv)
{
    char** v = malloc(argc * sizeof(char*));

    for(int i = 0; i < argc; ++i)
    {
        v[i] = malloc(strlen(argv[i]) + 1);
        strcpy(v[i], argv[i]);
    }

    FILE* fp = fopen("__test.txt", "w");
    for(int i = 0; i < argc; ++i)
    {
        int len = strlen(v[i]);
        fwrite(&len, sizeof(int), 1, fp);
        fwrite(v[i], len, 1, fp);
    }
    fclose(fp);

    fp = fopen("__test.txt", "r");
    char lenstr[16] = {0};
    for(int i = 0; i < argc; ++i)
    {
        int len = 0;
        char* buf = NULL;

        fread(&len, sizeof(int), 1, fp);
        buf = malloc(len + 1);
        fread(buf, len, 1, fp);
        buf[len] = '\0';

        print("size = ");
        itoa(len, lenstr, 10);
        print(lenstr);
        print(", arg = ");
        println(buf);

        free(buf);
        free(v[i]);
    }
    fclose(fp);
}
