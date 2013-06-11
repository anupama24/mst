#ifndef EULERLIST_H
#define EULERLIST_H
#endif


class EulerList
{

public:
	typedef stxxl::VECTOR_GENERATOR<EulerElem, PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result eulerListType;
	typedef typename eulerListType::iterator listItr;
	typedef typename eulerListType::const_iterator const_listItr;
private:

	eulerListType eulerList;

public:

	EulerList() {}
	EulerList(unsigned int size)
	{
		STXXL_MSG("Creating a euler list, addr=" << this);
		
	}

	
	inline bool checkEulerListEnd(listItr itr)
	{
		if(itr==eulerList.end())
			return true;
		else
			return false;
	}
	inline bool checkEulerListEnd(const_listItr itr)
	{
		if(itr==eulerList.end())
			return true;
		else
			return false;
	}
	inline listItr getFirstElem()
	{
		return eulerList.begin();
	}
	inline unsigned int getSize()
	{
		return eulerList.size();
	}
	void clear()
	{
		eulerList.clear();
	}
	EulerElem getEulerElem(unsigned int pos);
	void createList(DirectedGraph dag);
	void printList();	

};


