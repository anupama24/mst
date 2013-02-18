#ifndef EULERLIST_H
#define EULERLIST_H
#endif


class EulerList
{

public:
	typedef stxxl::VECTOR_GENERATOR<EulerElem, PAGE_SIZE,NO_OF_PAGES,BLOCK_SIZE,stxxl::striping,PAGER>::result eulerListType;
	typedef typename eulerListType::iterator listItr;

public:
	eulerListType eulerList;


	EulerList() {}
	EulerList(unsigned int size): eulerList(2*size)
	{
		STXXL_MSG("Creating a euler list, addr=" << this);
		eulerList.clear();
	}

	void createList(DirectedGraph dag);
	void printList();	

};


