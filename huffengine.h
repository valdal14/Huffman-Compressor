#ifndef HUFFENGINE_H
#define HUFFENGINE_H

#include <stddef.h>

#define FILENAME_SIZE 32
#define BUFFER_SIZE 256

struct HuffNode
{
    unsigned char data;
    int freq;
    struct HuffNode *left;
    struct HuffNode *right;
};

#endif
