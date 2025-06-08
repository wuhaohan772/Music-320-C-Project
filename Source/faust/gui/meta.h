#ifndef __meta__
#define __meta__

struct Meta
{
    virtual ~Meta() {}
    virtual void declare(const char* key, const char* value) = 0;
};

#endif 