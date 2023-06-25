
extern "C" {
#include "../include/rcrt.h"
}

#include "../include/string.h"
#include "../include/iostream.h"

class Foo
{
public:
    Foo(int data)
    {
        m_val = data;
        rstd::cout << "Foo(" << m_val << ")" << rstd::endl;
    }
    ~Foo()
    {
        rstd::cout << "~Foo(" << m_val << ")" << rstd::endl;
    }

    void test()
    {
        rstd::cout << "Foo::test(" << m_val << ")" << rstd::endl;
    }

private:
    int m_val;
};

Foo f(114);

int main(int argc, char** argv)
{
    rstd::cout << "gcc version: " << __GNUC__ << "." <<  __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << rstd::endl;
    char** v = (char**)malloc(argc * sizeof(char*));
    char lenstr[16] = {0};

    print("argc = ");
    println(itoa(argc, lenstr, 10));

    for(int i = 0; i < argc; ++i)
    {
        v[i] = (char*)malloc(strlen(argv[i]) + 1);
        strcpy(v[i], argv[i]);
    }

    FILE* fp = fopen(".test.txt", "w");
    for(int i = 0; i < argc; ++i)
    {
        int len = strlen(v[i]);
        fwrite(&len, sizeof(int), 1, fp);
        fwrite(v[i], len, 1, fp);
    }
    fclose(fp);

    fp = fopen(".test.txt", "r");
    for(int i = 0; i < argc; ++i)
    {
        int len = 0;
        char* buf = NULL;

        fread(&len, sizeof(int), 1, fp);
        buf = (char*)malloc(len + 1);
        fread(buf, len, 1, fp);
        buf[len] = '\0';

        print("size = ");
        print(itoa(len, lenstr, 10));
        print(", arg = ");
        println(buf);

        free(buf);
        free(v[i]);
    }
    fclose(fp);

    println("tesing cpp");

    f.test();
    Foo g(233);
    g.test();

    rstd::string* msg = new rstd::string("hello world!");
    rstd::cout << *msg << rstd::endl;
    delete msg;

    return 0;
}
