#pragma once
/******************************************************************************************
 * Filename:AlbertSTL_Node.h
 * Description:Declaration and implementation of class Node.
 * Author  :Albert Zhang
 * Time	   :20/7/2022
 * Copyright (c) . All rights reserved.
 ******************************************************************************************/
#pragma once
#ifndef HAS_DEFIEND_CLASS_NODE
#define HAS_DEFIEND_CLASS_NODE
#endif 
#include "AlbertSTL_List.h"

template<class T>
class List;

template<class T>
class Node
{
public:
	T		 _node_data;
	Node<T>* _predecessor;
	Node<T>* _successor;
	Node();
	Node(const T& element, Node<T>* pre = nullptr, Node<T>* suc = nullptr) :_node_data(element), _predecessor(pre), _successor(suc) {};
protected:
	friend List<T>;
	Node<T>* prepend(const T& dataInserted);
	Node<T>* append(const T& dataInserted);
};

template <class T>
Node<T>::Node()
{
	this->_successor = this->_predecessor = nullptr;
}

template <class T>
Node<T>* Node<T>::prepend(const T& dataInserted)
{
	Node<T>* _newNode = new Node<T>(dataInserted, this->_predecessor, this);
	this->_predecessor->_successor = _newNode;
	this->_predecessor = _newNode;
	return(_newNode);
}

template <class T>
Node<T>* Node<T>::append(const T& dataInserted)
{
	Node<T>* _newNode = new Node<T>(dataInserted, this, this->_successor);
	this->_successor->_predecessor = _newNode;
	this->_successor = _newNode;
	return _newNode;
}