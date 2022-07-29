/******************************************************************************************
 * Filename:List.h
 * Description:Declaration and implementation of class List.
 * Author  :Albert Zhang
 * Time	   :20/7/2022
 * Copyright (c) . All rights reserved.
 ******************************************************************************************/
#pragma once
#ifndef HAS_DEFIEND_CLASS_NODE_AND_LIST
#define HAS_DEFIEND_CLASS_NODE_AND_LIST
#endif 
#include "Node.h"

template <class T>
class Node;

///General Programming Model for Lists.
template <class T>
class List
{
protected:
	int _size;
	Node<T>* _head;  Node<T>* _tail;

	void init();

	/*Assert: You must use valid() to TEST STARTPOS VALID before calling this function.*/
	void copyNodes(Node<T>* startPos, int num);


	void mergeSort(Node<T>*& myStart, int n);
	/*Internal method for mergeSort().
	 NOTION: Merges an ALREADY SORTED sub-list of some list into an ALREADY SORTED sub-list of this list  to form a composite new list.*/
	void mergeList(Node<T>*& myListStartNode, int myListNum, List<T>& anotherList, Node<T>* anotherListStartNode, int otherListNum);



	/*sort  sub-list List[startPos,startPos+ num2Bsort)*/
	void insertionSort(Node<T>* startPos, int num2Bsort);
	/*Internal function , core of method "insertionSort"
	return the address of the hindmost node  WITHIN [pos-searchBarWidth,pos) that is no more than(<=) given targetData.*/
	Node<T>* searchBackward(const T& targetData, int searchBarWidth, Node<T>* pos);

	void selectionSort(Node<T>* startPos, int num2Bsort);
	/*Insert as first/last node and return its address.*/

	Node<T>* prepend_internal(const T& data2Binserted);
	Node<T>* append_internal(const T& data2Binserted);

public:

	List() { this->init(); };

	/*Duplicate  another list "source" entirely to construct this list.
	Assert:Must use valid() to TEST STARTPOS VALID before calling this function.*/
	List(const List<T>& source);

	/*Duplicate n-nodes from startNode to create a new list.(startNode included).
	Assert:Must use valid() to TEST STARTPOS VALID before calling this function.*/
	List(Node<T>* startNode, int n);

	/*Duplicate another list "source" [rank,rank+num) to construct this list.
	Assert: You must use valid() to TEST STARTPOS VALID before calling this function.*/
	List(const List<T>& source, int rank, int num);

public://Read Only Interfaces

	int size()const;
	bool isEmpty()const;

	/*Test whether node address(position) pos is a valid position.
	That is, pos is not nullptr , head nor tail*/
	bool valid(Node<T>* pos);
	T& operator[] (int rank) const;

	/*note: when rank is too big or is negative, operator[] still returns value but migh have be not accurate.*/
	bool operator==(List<T>&);

	/*WARNING: returns nullptr when rank is not valid!*/
	Node<T>* getNodeAddress(int rank)const;
	/*Get the address of its head node's predecessor or tail node's successor.*/
	Node<T>* first()const;
	Node<T>* last() const;
	/// Summary:Find the maximum node within [startPos,startPos+n)
	/// Para:Node<T>* startPos: startPos: a VALID (real, can seen from outside) node position.
	/// Para:num: number of nodes that will be traversed(==total length of comparison region)
	/// RetV:if operation successful, return max 's position.
	///		 if operation failed(input an INVALID position), return nullptr;
	///	WARNING: THIS METHOD MIGHT RETURN A nullptr! MUST TEST RETURN VAL AFTER USE!
	Node<T>* max(Node<T>* startPos, int num);
	/*Find the maximum node from all the nodes.*/
	Node<T>* max() { return this->max(this->_head->_successor, this->_size); }

	/*Find the hindmost node that matches given data from the place RIGHT BEFORE a given node position(excluding this node)
	all the way back to n-nodes before it(total traversal number is n).
	If match is found on the way, immediately return that nodes's address.
	If match is not found, return nullptr.*/
	/*NOTION: this function needs valid() RIGHT AFTER CALLING IT to test whether it succeeds.*/
	Node<T>* find(const T& targetData, int searchBarWidth, Node<T>* startPos);
	///Find the hindmost node in this list that matches given data and then return its address
	//NOTION: this function needs to use valid()RIGHT AFTER CALLING IT to test whether it succeeds.
	Node<T>* find(const T& targetData);

	/// searchBy: Find the hindmost node that satisfies condition(targetData, thisNode)
	/// para   " VisitingFunctor& condition": a functor with a reloaded "bool operator()(thisNodeData,targetData)" that receive 2 variables.
	///	and returns true/false, meaning whether thisNode statisfies certain condition in relation with target or not.
	/// ReturnVal:  If condition can be matched, return the address of the HINDMOST node that satisfies this condition.    
	///				If condition not matched, RETURN NULLPLR.
	/// NOTION: this function needs valid() RIGHT AFTER CALLING IT to test whether it succeeds.
	template <class VisitingFunctor>
	Node<T>* searchBy(VisitingFunctor& condition, const T& targetData);
	/// searchBy: Find the hindmost node that satisfies condition(thisNodeData,targetData) WITHIN [pos-searchBarWidth,pos)
	/// For Detailed descriptions, check another reload: searchBy(VisitingFunctor& condition,const T& targetData)
	/// NOTION: this function needs valid() RIGHT AFTER CALLING IT to test whether it succeeds.
	template <class VisitingFunctor>
	Node<T>* searchBy(VisitingFunctor& condition, const T& targetData, int searchBarWidth, Node<T>* pos);

public://Write Interfaces

	List<T>& prepend(const T& data2Binserted);
	List<T>& append(const T& data2Binserted);
	/*Insert a data as a node BEFORE a given node position and return new node's address AFTER insertion.*/
	Node<T>* insertBefore(const T& data2Binserted, Node<T>* originalNodePos);
	/*Insert a data as a node AFTER a given node position and return new node's address AFTER insertion.*/
	Node<T>* insertAfter(Node<T>* originalNodePos, const T& data2Binserted);
	/*Merge two lists.*/
	void mergeList(List<T>& AnotherList);

	/*Test whether list has been sorted.*/
	bool isSorted()const;
	/// sort(Node<T>* startPos, int sortMode): Use a certain mode to sort the list from node startPos.
	/// Para: sortMode==0, insertionSort;
	///		  sortMode==1, selectionSort;
	///		  sortMode==2, mergeSort;
	///		  startPos, address of the starting position(should be a real node);
	///		  Defualt value==first();
	/// ReturnV: none.
	void sort(Node<T>* startPos, int sortLength, int sortMode = 2);
	///Sort the entire list.
	void sort(int mode = 1) { this->sort(this->first(), this->_size, mode); }

	void reverse();

	/// Traverse the list to add a certain operation(method) to every REAL node's data(first to last).
	/// This operation is represented by a functional pointer whose parameter is T&
	/// Para: void (*)(T&);  Return: None.
	void for_each(void(*operation)(T&));
	/// Traverse the list to add a certain operation(method) to every REAL node's data(first to last).
	/// Para: a reference to a functor which belongs to class VisitFunctor
	template <class VisitingFunctor>
	void for_each(VisitingFunctor& functor);

	/// Para:place to pop. WARNING : YOU MUST use valid() TO TEST POS!!!!!THIS FUNCTION DOESN'T SUPPORT EMBEDED SAFE-CHECK!
	/// PLUG A NODE: delete its data and relation with suc and pred nodes.(delete pos->suc and pos->pred)
	/// HEAL THE WOUND : sew its suc and pred back together
	/// PUMP DATA OUT : RETURN data in the node plugged just then.
	T pop(Node<T>* pos);
	/*Delete all the duplicates in an unsorted list and return number of nodes deleted.
	Complexity: O(n^2)*/
	int removeDuplicates();
	/*clear all nodes from first to last and return number of nodes just cleaned.(only deletes the nodes that can be seen outside.)*/
	int clear();
	/*Destruct all the nodes .(including head and tail.)*/
	~List();
};

template <class T>
bool List<T>::operator==(List<T>& L)
{
	int matchingNum = 0;
	if (this->_size != L._size)
		return false;
	else if (&L == this)
		return true;
	else
	{
		Node<T>* myPos = this->first();
		Node<T>* anotherPos = L.first();

		for (; valid(myPos) == true && valid(anotherPos) == true; myPos = myPos->_successor, anotherPos = anotherPos->_successor)
		{
			if (myPos->_node_data == anotherPos->_node_data)
				matchingNum++;
			else
				return false;

		}
	}
	return (matchingNum == this->_size);
}

template <class T>
T& List<T>::operator[](int rank)const
{
	Node<T>* p = this->first();
	if (rank < 0)
	{
		rank = rank * (-1) - 1;
		rank %= this->_size;

		p = this->last();
		while (0 < rank--)
		{
			if (p == nullptr)
			{
				p = new Node<T>;
				break;
			}
			p = p->_predecessor;
		}
	}
	else if (rank >= 0)
	{
		rank %= this->_size;
		while (0 < rank--)
		{
			p = p->_successor;
			if (p == nullptr)
			{
				p = new Node<T>;
				break;
			}
		}
	}
	return p->_node_data;
}

template <class T>
Node<T>* List<T>::getNodeAddress(int rank)const
{
	if (rank < 0 || rank >= this->_size)
		return nullptr;
	Node<T>* p = this->first();
	while (0 < rank--)
		p = p->_successor;
	return p;
}

template <class T>
void List<T>::init()
{
	this->_head = new Node<T>;
	this->_tail = new Node<T>;						///				  ��---------pre-----------|
	this->_head->_predecessor = nullptr;			/// nullptr��-----head				     tail---------��nullptr
	this->_head->_successor = this->_tail;			//				  |---------suc---------->��
	this->_tail->_predecessor = this->_head;
	this->_tail->_successor = nullptr;
	this->_size = 0;
}

template <class T>
int List<T>::size() const { return this->_size; }

template <class T>
bool List<T>::isEmpty()const { return (this->_size <= 0); }

template <class T>
Node<T>* List<T>::first()const { return _head->_successor; }

template <class T>
Node<T>* List<T>::last()const { return _tail->_predecessor; }

template <class T>
bool List<T>::valid(Node<T>* pos) { return (pos && pos != this->_head && pos != this->_tail); }

template <class T>
bool List<T>::isSorted()const {/*Needs further implementation.*/ return false; }

template <class T>
Node<T>* List<T>::find(const T& targetData, int searchBarWidth, Node<T>* pos)
{													//note that we will iterate the parameters and make them move.
													//they received initial values from outside but then becomes volatile
	while (0 < searchBarWidth--)					//searBarWidth==how many nodes are there ahead that we can search.
	{												//As long as we still can move(0<searchBarWidth), 
		if (targetData == (pos = pos->_predecessor)->_node_data) //move a step forward (which spontaneously reduced steps we can take laters(searchBarWidth--))
			return pos;						            //by replacing the node being checked currently to its predecessor(pos = pos->_predecessor)
	}												//find this node's data:(pos = pos->_predecessor)->_node_data)
	return nullptr;									//and test whether it matches target data: (if (targetData == (pos = pos->_predecessor)->_node_data))
													//If they match, return the node being tested just then.
													//If they don't match, move forward. When searchBarWidth becomes 0(on the margin!),result is no found.
													///											 startPos
													/// 											|
													///					    |	searchBarWidth    | |
													/// _ _ _ _ _ _ _ _ _ _ _ _ _ * _ _ _ _ _ _ _ _ _
													///							  |
													///							targetData
}

template <class T>
Node<T>* List<T>::find(const T& targetData)
{
	return(this->find(targetData, this->_size, this->_tail));
}

template <class T>
Node<T>* List<T>::searchBackward(const T& targetData, int searchBarWidth, Node<T>* pos)
{
	if (searchBarWidth<0 || searchBarWidth>this->_size || pos == nullptr || pos->_predecessor == nullptr || pos == this->_head) //Skip trivial conditions.
		return nullptr;

	while (pos != nullptr && 0 < searchBarWidth--)					//Note that sequence of "&&" conditions.if pos==null, break immediately.
	{																//SearchBarWidth will shorten only when pos isn't nullptr.

		if (((pos = pos->_predecessor)->_node_data <= targetData))
			return pos;
		else if (pos == this->_head)							//If has already got to the head BEFORE but didn't return, it means we will never find result anymore.
			break;
	}
	return nullptr;
}

template <class T>  template <class VisitingFunctor>
Node<T>* List<T>::searchBy(VisitingFunctor& condition, const T& targetData, int searchBarWidth, Node<T>* pos)/*[might have some problems...]*/
{
	if (searchBarWidth<0 || searchBarWidth>this->_size || pos == this->_head) //Skip trivial conditions.
		return nullptr;

	while (pos != nullptr && 0 < searchBarWidth--)					//Note that sequence of "&&" conditions.if pos==null, break immediately.
	{																//SearchBarWidth will shorten only when pos isn't nullptr.

		if (condition((pos = pos->_predecessor)->_node_data, targetData))
			return pos;
		else if (pos == this->_head)							//If has already got to the head BEFORE but didn't return, it means we will never find result anymore.
			break;
	}
	return nullptr;
}

template <class T>  template <class VisitingFunctor>
Node<T>* List<T>::searchBy(VisitingFunctor& condition, const T& targetData)
{
	return(this->searchBy(condition, targetData, this->_size, this->_tail));
}

/*find the maximum node within the range of [pos, pos+n).
  If input is invalid, return nullptr.
  Else, always return a valid Node<T>* pointer.*/
template <class T>
Node<T>* List<T>::max(Node<T>* pos, int n)
{
	/*skip invalid input when pos is not a REAL NODE or n is not POSITIVE.*/
	if (valid(pos) == false || n <= 0)
		return nullptr;
	/*skip trivial case when there's only one node.*/
	else if (n < 2 || pos->_successor == nullptr || pos == this->last())
		return pos;
	/*now we can guarantee that cursor is not nullptr ,thus it has _node_data to be accessed.So there are at least two nodes
	for comparison and this searching is well-defined.*/
	Node<T>* cursor = pos->_successor;

	/*count represents number of nodes that has been compared to pos.
	  We take pos itself as one since it was compared at the beginning with itself so count starts from 1.
	  Right after that, cursor is placed on the second node in [pos,pos+n) and is ready to test it.*/
	int count = 1; /*count==x means we have tested x nodes already and we are now testing the (x+1)-th node.*/
	while (count < n)/*count==n-1 means we have just start to test the last node(n-th). If count==n,it means we have tested all nodes and we should just break loop. */
	{
		if (pos->_node_data <= cursor->_node_data)
			pos = cursor;

		cursor = cursor->_successor;					//no matter what happens, cursor always moves forward to the tail.

		if (cursor == this->_tail || cursor == nullptr)		//end of comparison..
			return pos;

		count += 1;										//We have tested anther one node,so count++;Since count is for next round's reference,
	}													//if we have already come to an end(cursor == this->_tail||cursor == nullptr), we don't need "count" anymore.
	return pos;											//so "return pos" is placed before "count++";
}

template<class T>
Node<T>* List<T>::prepend_internal(const T& data2Binserted)//Insert as first node is equivalent to insert as the suc of head node.
{
	++this->_size;
	return(this->_head->append(data2Binserted));
}

template<class T>
Node<T>* List<T>::append_internal(const T& data2Binserted)
{
	++this->_size;

	return(this->_tail->prepend(data2Binserted));
}

template<class T>
List<T>& List<T>::prepend(const T& data2Binserted)
{
	++this->_size;
	this->_head->append(data2Binserted);
	return *this;
}

template<class T>
List<T>& List<T>::append(const T& data2Binserted)
{
	++this->_size;

	this->_tail->prepend(data2Binserted);

	return *this;
}

template<class T>
Node<T>* List<T>::insertBefore(const T& data2Binserted, Node<T>* originalNodePos)
{
	++this->_size;
	return (originalNodePos->prepend(data2Binserted));
}

template<class T>
Node<T>* List<T>::insertAfter(Node<T>* originalNodePos, const T& dataInsertedBack)
{
	++this->_size;


	Node<T>* answer = originalNodePos->append(dataInsertedBack);


	return (answer);
}

template<class T>
T List<T>::pop(Node<T>* pos)						//Reverse process of insert.
{
	if (this->valid(pos) == false)
		throw - 1;
	T popValue = pos->_node_data;
	pos->_predecessor->_successor = pos->_successor;
	pos->_successor->_predecessor = pos->_predecessor;
	delete pos;
	this->_size--;
	return popValue;
}
/* |---- �� pre-------||-----<<pre-------|       |---��---pre---------|
  [pre]			   [*pos]		      [suc]   [pre]					[suc]
   |--------suc>>-----||------suc��------|       |-------suc-----��---|	    */

template<class T>
int List<T>::clear()
{
	int numCleaned = this->_size;					//copy a memory of size as ReturnVal.
	while (0 < this->_size)
	{
		pop(this->_head->_successor);
		this->_size--;
	}//Note that pop will automatically reduce _size so _size is implicitly used as a counter.

	/*sew head and tail back together to close this list.
	If you don't do this double check, bugs will come out but it seems that pop was right.*/
	this->_head->_predecessor = nullptr;
	this->_head->_successor = this->_tail;			//				  ��<--------pre-----------|
	this->_tail->_predecessor = this->_head;		/// nullptr��-----head				     tail---------��nullptr
	this->_tail->_successor = nullptr;				//				  |---------suc---------->��

	return numCleaned;
}

template <class T>
List<T>::~List()
{
	this->clear();		//Clear nodes can be seen first(coz it needs the _head as an anchor.)
	delete this->_head;
	delete this->_tail;
}

template <class T>
int List<T>::removeDuplicates()
{
	if (this->size < 2)								//Trivial lists don't have remove duplicates.
		return 0;
	int formerSize = this->_size;					//memorize.
	Node<T>* pos = this->_head;
	Node<T>* toBremoved = nullptr;
	int searchBandWidth = 0;						//bandwidth==real nodes BEFORE pos(first to pos-1) that will be searched to find duplicate of this node.
	while ((pos = pos->_successor) != this->_tail)	//pos becomes first at the begining and "last" will be the last node whose former nodes will be checked
	{
		if (toBremoved = this->find(pos->_node_data, searchBandWidth, pos))
			pop(toBremoved);
		else
			searchBandWidth++;						//searchBandWidth++  and pos=pos->_successor pushes cursor and searching bar forward.
	}												//When searching ends(pos==last), searchBandWidth becomes the new _size of the list.
	return formerSize - searchBandWidth;			//The difference is nodes removed using this method.
}
/*			  ____bandwidth____       |
			 |				   |	  ��
 [head]---[first]------------[pos]--[suc]-------[last]--[tail]
 NoData    Data              Data   Target       Data   NoData  */

template <class T>
void List<T>::copyNodes(Node<T>* pos, int num)
{
	init();
	int actualSize = 0;
	while (num--)									//Num manifests number of nodes left to be copied.
	{
		this->append_internal(pos->_node_data);
		actualSize++;
		if ((pos = pos->_successor) == nullptr)     //end when cursor move onto a tail, whose successor is nullptr.
			return;									//This is designed to handle a too-big input of num.
	}
	this->_size = actualSize;
}

template <class T>
List<T>::List(const List<T>& source)
{
	this->copyNodes(source.first(), source._size);
}

template <class T>
List<T>::List(Node<T>* startNode, int num2Bcopied)
{
	this->copyNodes(startNode, num2Bcopied);
}

template <class T>
List<T>::List(const List<T>& source, int rank, int num)
{
	this->copyNodes(source->getNodeAddress(rank), num);
}

template <class T>
void List<T>::for_each(void(*operation)(T&))
{
	Node<T>* p = nullptr;
	for (p = this->_head->_successor; p != this->_tail; p = p->_successor)
		operation(p->_node_data);
}

template <class T> template<class VisitingFunctor>
void List<T>::for_each(VisitingFunctor& functor)
{
	Node<T>* p = nullptr;
	for (p = this->_head->_successor; p != this->_tail; p = p->_successor)
		functor(p->_node_data);
}

template<class T>
void List<T>::reverse()
{
	if (this->_size < 2)
		return;

	Node<T>* last = this->last();
	Node<T>* first = this->first();
	Node<T>* cursor = first;
	Node<T>* tempPre = nullptr;
	Node<T>* tempSuc = nullptr;

	this->_tail->_predecessor = first;
	this->_head->_successor = last;

	for (int i = 0; i < this->_size && valid(cursor) == true; i++)
	{
		tempPre = cursor->_predecessor;
		tempSuc = cursor->_successor;
		cursor->_predecessor = tempSuc;
		cursor->_successor = tempPre;
		cursor = cursor->_predecessor;
	}
}

template<class T>
void List<T>::sort(Node<T>* startPos, int sortLength, int sortMode)
{
	switch (sortMode)
	{
	case 0:
		this->insertionSort(startPos, sortLength);
		break;
	case 1:
		this->selectionSort(startPos, sortLength);
		break;
	case 2:
		this->mergeSort(startPos, sortLength);
		break;
	default:
		return;
	}
}

/*Algorithm: insertionSort
  Para:   we will sort List[pos,pos+n).If n is too big, we will sort from pos to end. */
template<class T>
void List<T>::insertionSort(Node<T>* pos, int n)
{
	if (n < 2 || this->_size < 2 || valid(pos) == false || valid(pos->_successor) == false || pos->_predecessor->_successor != pos || pos->_successor->_successor->_predecessor != pos->_successor)
		return;
	Node<T>* headAnchor = pos->_predecessor;
	Node<T>* cursor = pos->_successor;
	Node<T>* insertPos = nullptr;
	int sortedBandWidth = 1;

	while (cursor != this->_tail && sortedBandWidth < n)
	{
		insertPos = searchBackward(cursor->_node_data, sortedBandWidth, cursor);
		cursor = cursor->_successor;
		insertAfter(insertPos ? insertPos : headAnchor, pop(cursor->_predecessor));
		sortedBandWidth++;
	}
}

/// Algorithm selectionSort: select the max of unsorted region and throw it back to the end,  gradually forming a sorted region in the back.
/// Para: we will sort List[pos,pos+n) is n is well-defined. If n is too big, we wort List[pos, _tail)
/// At the begining, sorted region has 0 size and we suppose it's "sorted" already. 
template<class T>
void List<T>::selectionSort(Node<T>* pos, int n) {
#ifndef _OPEN_DETAILED_DESCRIPTION_FOR_LEARNING	

	if (n < 2 || this->_size < 2 || valid(pos) == false || pos->_successor->_predecessor != pos || pos->_predecessor->_successor != pos)
		return;

	Node<T>* unsortedHead = pos->_predecessor;

	Node<T>* unsortedTail = nullptr;
	int bandWidth = 0;// find the appropriate bandwidth and stop position no matter how ridiculously big n is.
	for (unsortedTail = pos; unsortedTail != this->_tail && bandWidth < n; ++bandWidth)
		unsortedTail = unsortedTail->_successor;

	while (1 < bandWidth)
	{
		//shortening tail gives us last node.//  //fixed head(Anchor) gives of first node//
		this->insertAfter(unsortedTail->_predecessor, pop(max(unsortedHead->_successor, bandWidth)));

		unsortedTail = unsortedTail->_predecessor;		//move tail forward 

		bandWidth--;									//and shorten unsorted region.
	}
#endif 
#ifndef OPEN_NOTES_ON_WHY_YOU_SHOULD_NOT_SHORTEN_CODES_LIKE_THIS
	//Note that you should not write like:
	//			while (1 < bandWidth--)
	//because this means test whether 1<bandwidth. If yes, FIRST REDUCE bandwidth	,THEN HEAD INTO WHILE(){}BODY.
	//BECAUSE (1 < bandWidth--)was executed before while{}
#endif
}

template<class T>
void List<T>::mergeList(Node<T>*& myPos, int n, List<T>& L, Node<T>* otherPos, int m)
{
	Node<T>* anchorHead = myPos->_predecessor;
	if (L.valid(otherPos) == false || valid(myPos) == false || m < 1 || n < 1 || anchorHead->_successor != myPos)
		return;
	while (0 < n && 0 < m && myPos != this->_tail && otherPos != L._tail)
	{
		if (myPos->_node_data <= otherPos->_node_data)
		{
			n--;
			myPos = myPos->_successor;
		}
		else
		{
			insertBefore(pop((otherPos = otherPos->_successor)->_predecessor), myPos);
			m--;
		}
	}

	myPos = anchorHead->_successor;
}

/*MERGESORT basically uses divide and conquer. It first continuously cuts the entire linked-list by half untill we fall onto trialvial cases
  when len of sub-strings are 2.then we can sort these pieces and this is done by recursion and costs greate spcae complexity but a logrithmic time complexity.
  Then, with all the short pieces all sorted neatly, we will recurse and merge sorted lists to a bigger one using the algorithm "mergeList" shown below.
  Note that to save	space, we only memorize three things is this algorithm: mP, oP, and H. mP and oP moves up (and substring move down relatively)
  and checkes each node. H is the anchor(or tag) we use to re-locate the start of the newly merged sorted string.H has greate significance in
maintaing the stability of recursion.																										 10
This diagram shows how mergeList()works. in this graph, mP==myPos, oP==otherPos,H==anchorHead.								     			 9
|	 n==3  m==4 --(n--)-> n==2 m==4 --(m--)-> n==2 m==3 --(m--)-> n==2 m==2 --(n--)-> n==1 m==2 --(m--)-> n==1 m==1 --(m--)-> n==1 m==0-(BREA7K)->
|	       9		|	  	   9		|					|					|					|					|				|***R6WIND**
|	 10    7		|		   7		|			9		|					|					|					|				|	 4
|	 6     4		|	  10   4		|	  10	7		|		10	9		|			9		|					|				|	 3
|mP--2-----3--oP----| mP--6----3--oP----| mP--6----4--oP----|---mP--6---7--oP---|---mP-10---7--oP---|---mP-10---9--oP---|---mP-10		|-mP-2
|	 H				|	  2	��������		|	  3	��������		|		4	!!		|	   6 ��������		|	   7 ��������		|	   9		|	 H
|					|	  H	[pop&insert]|	  2	[pop&insert]|		3[my<other] |	   4[pop&insert]|	   6[pop&insert]|      7		|
											  H				|		2			|	   3			|      4			|      6		|
															|		H			|	   2			|	   3			|      4		|
																				|	   H			|	   2			|      3		|
																									|      H			|      2		|
																															   H		|	      */
template<class T>
void List<T>::mergeSort(Node<T>*& myStart, int n)
{
	if (n < 2 || valid(myStart) == false)
		return;

	Node<T>* secondHalfStart = myStart;

	int middleLine = n / 2;

	for (int i = 0; i < middleLine && secondHalfStart != nullptr; i++)
		secondHalfStart = secondHalfStart->_successor;

	mergeSort(myStart, middleLine);

	mergeSort(secondHalfStart, n - middleLine);

	mergeList(myStart, middleLine, *this, secondHalfStart, n - middleLine);
}