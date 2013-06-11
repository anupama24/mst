#include<math.h>
#include<iostream>
#include<fstream>
#include<ctime>
#include <climits>
#include <stxxl/vector>
#include <stxxl/priority_queue>
#include <stxxl/scan>
#include <stxxl/sort>
#include <stxxl/map>
#include <stxxl/io>
#include <stxxl/mng>
#include <stxxl/stack>

stxxl::stats_data stats_total;

// external sorting
#define INTERNAL_MEMORY_FOR_SORTING 650*1024*1024

//default template parameters of Edge & MST Vector
#define BLOCK_SIZE 2 * 1024 *1024
#define NO_OF_PAGES 5
#define PAGE_SIZE 1
#define PAGER stxxl::lru

//default template parameters of Vertex Vector
#define VER_BLOCK_SIZE 2 * 1024 *1024
#define VER_NO_OF_PAGES 5
#define VER_PAGE_SIZE 1

// ** PQUEUE IMPLEMENTATION **
#define PQUEUE_INTERNAL_MEMORY 10*1024*1024
#define PQUEUE_MAX_SIZE 100*1024*1024
#define PQUEUE_MEM_POOL 128 * 1024 * 1024

// template parameters of the Stack
#define STACK_BLOCK_SIZE 1024*1024
#define STACK_PAGE_SIZE 1
#define STACK_PREFETCH_POOL 4
#define STACK_WRITE_POOL 4

typedef std::pair<unsigned int,unsigned int> pairType;



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

struct cmpFirst
{
    pairType min_value() const
    {
        return pairType(std::numeric_limits<unsigned int>::min(),0);
    }
    pairType max_value() const
    {
        return pairType(std::numeric_limits<unsigned int>::max(),0);
    }
    bool operator () (pairType a, pairType b) const {
		return a.first < b.first || (a.first==b.first && a.second < b.second);
    }
};
