#include "../include/iostream.h"

extern "C" {
#include "../include/rcrt.h"
}

#ifdef WIN32
#include <Windoes.h>
#endif

namespace rstd
{

stdout_stream::stdout_stream()
: ofstream()
{
    m_fp = stdout;
}

stdout_stream cout;

ofstream::ofstream()
: m_fp(NULL)
{ }

ofstream::ofstream(const char* filename, ofstream::openmode md)
: m_fp(NULL)
{
    open(filename, md);
}

ofstream::~ofstream()
{
    close();
}

ofstream& ofstream::operator<<(char c)
{
    fputc(c, m_fp);

    return *this;
}

ofstream& ofstream::operator<<(int n)
{
    char buf[16];
    itoa(n, buf, 10);
    fputs(buf, m_fp);

    return *this;
}

ofstream& ofstream::operator<<(const char* str)
{
    fputs(str, m_fp);

    return *this;
}

ofstream& ofstream::operator<<(ofstream& (*manip)(ofstream&))
{
    return manip(*this);
}

void ofstream::open(const char* filename, ofstream::openmode md)
{
    char mode[4];
    close();

    switch(md)
    {
        case (out | trunc):
            strcpy(mode, "w");
            break;
        case (out | in | trunc):
            strcpy(mode, "w+");
            break;
        case (out | in | binary):
            strcpy(mode, "wb");
            break;
        case (out | in | trunc | binary):
            strcpy(mode, "wb+");
            break;
    }

    m_fp = fopen(filename, mode);
}

void ofstream::close()
{
    if(m_fp)
    {
        fclose(m_fp);
        m_fp = NULL;
    }
}

ofstream& ofstream::write(const char* buf, unsigned size)
{
    fwrite(buf, size, 1, m_fp);
    return *this;
}

};
