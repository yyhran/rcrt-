#pragma once
#include "iostream.h"

extern "C" {
#include "rcrt.h"
}

namespace rstd
{

class string
{
    unsigned m_len;
    char* m_pbuf;
    
public:
    explicit string(const char* str)
        : m_len(0)
        , m_pbuf(0)
    {
        *this = str;
    }

    string(const string& s)
        : m_len(0)
        , m_pbuf(0)
    {
        *this = s;
    }

    ~string()
    {
        if(m_pbuf)
        {
            delete[] m_pbuf;
            m_pbuf = 0;
        }
    }

    string& operator=(const string& s)
    {
        if(&s == this)
        {
            return *this;
        }

        this->~string();
        m_len = s.m_len;
        m_pbuf = strcpy(new char[m_len + 1], s.m_pbuf);
        return *this;
    }

    string& operator=(const char* s)
    {
        this->~string();
        m_len = strlen(s);
        m_pbuf = strcpy(new char[m_len + 1], s);
        return *this;
    }

    const char& operator[](unsigned idx) const { return m_pbuf[idx]; }
    char& operator[](unsigned idx) { return m_pbuf[idx]; }

    const char* c_str() const { return m_pbuf; }
    unsigned length() const { return m_len; }
    unsigned size() const { return m_len; }
};

ofstream& operator<<(ofstream& o, const string& s)
{
    return o << s.c_str();
}

} // namespace rstd
