#include<math.h>
#include<iostream>
#include<ctime>
#include <limits>
#include <stxxl/vector>
#include <stxxl/priority_queue>
#include <stxxl/scan>
#include <stxxl/sort>
#include <stxxl/map>

// external sorting
#define INTERNAL_MEMORY_FOR_SORTING 650*1024*1024

//default template parameters of Edge & MST Vector
#define BLOCK_SIZE 2 * 1024 *1024
#define NO_OF_PAGES 1
#define PAGE_SIZE 1
#define PAGER stxxl::lru

//default template parameters of Vertex Vector
#define VER_BLOCK_SIZE 1024 *1024
#define VER_NO_OF_PAGES 1
#define VER_PAGE_SIZE 1

// ** PQUEUE IMPLEMENTATION **
#define PQUEUE_INTERNAL_MEMORY 10*1024*1024
#define PQUEUE_MAX_SIZE 100*1024*1024
#define PQUEUE_MEM_POOL 128 * 1024 * 1024

struct cmp : public std::less<unsigned int>
{
    unsigned int min_value() const
    {
        return std::numeric_limits<unsigned int>::min();
    }
    unsigned int max_value() const
    {
        return std::numeric_limits<unsigned int>::max();
    }
};
