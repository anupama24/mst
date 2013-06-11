template <class VectorType>
class CommonPool
{
    typedef VectorType vector_type;
    typedef typename vector_type::size_type size_type;
    typedef typename vector_type::value_type value_type;
    typedef typename vector_type::block_type block_type;
    typedef typename vector_type::alloc_strategy_type alloc_strategy_type;

     enum {
       block_size = vector_type::block_size,
    };

    
    //typedef stxxl::typed_block<block_size, value_type> block_type;
    typedef stxxl::BID<block_size> bid_type;

    unsigned int cache_offset;
    block_type *cache;
    alloc_strategy_type alloc_strategy;
   


public:
    CommonPool() : cache_offset(0),cache(new block_type){}

    void push(const value_type & val)
    {
 

        assert(cache_offset < block_type::size);

        (*cache)[cache_offset] = val;
        ++cache_offset;

        assert(cache_offset > 0);
        assert(cache_offset <= block_type::size);
    }

    void push(const value_type & val,unsigned int pos)
    {
 
	assert(pos >= 0);
	assert(pos < block_type::size);
	assert(cache_offset >= 0);
        assert(cache_offset <= block_type::size);

        (*cache)[pos] = val;
        
    }


    bool isFull()
    {
	return (cache_offset == block_type::size);
    }
    bool checkSpace(unsigned int pos)
    {
	return (pos < block_type::size);	
    }
    bool isEmpty()
    {
	return (cache_offset == 0);
    }

    unsigned int getSize()
    {
	return cache_offset;
    }

    void setSize(unsigned int pos)
    {
	assert(pos >= 0);
	assert(pos <= block_type::size);
        //assert(pos <= cache_offset);
	cache_offset=pos;
    }

    value_type get(unsigned int pos)
    {
       
        assert(pos >= 0);
	assert(pos < block_type::size);
        assert(pos <= cache_offset);
        assert(cache_offset > 0);
        assert(cache_offset <= block_type::size);
       
        return (*cache)[pos];
    }

    void flush()
    {
	cache_offset=0;
    }
    virtual ~CommonPool()
    {
	cache_offset=0;
        delete cache;
        
    }
};
