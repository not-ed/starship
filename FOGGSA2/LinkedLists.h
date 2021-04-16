#pragma once
#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H

#include "Structures.h"
#include <iostream>
#include <iomanip>
#include "SceneObject.h"

class LinkedLists
{
public:
	// Append a new node to the end of the linked list.
	ListNode* MakeNode(ListNode** head,SceneObject* obj);
	// Insert a node as the first one in a linked list.
	ListNode* InsertFirst(ListNode** head, SceneObject* obj);
	// Insert a node after a specified point in the linked list.
	void InsertAfter(ListNode* lastNode, SceneObject* obj);

	// Delete all nodes in the list
	void DeleteList(ListNode** node);
	// Delete a node after a given point if it exists
	void DeleteAfter(ListNode* node);
	// Delete ALL nodes after a specific point
	void DeleteAllAfter(ListNode* node);
	// Delete a specific node at a given position if it exists.
	void DeleteAt(ListNode** node, int pos);

	// Get the node found at a specific position, if it exists at all.
	ListNode* GetNode(ListNode* node, int pos);
	// Find a specifc node by object, if it exists.
	ListNode* Find(ListNode* node, SceneObject* obj);

	void PrintList(ListNode* node);
	void PrintListBackwards(ListNode* node);

	// Destroy the associated object of a node
	// This cannot be called inside the node destructor as the sceneobject's (and its derivatives) destructor will not call if done due to forward declarations.
	void DestroyNodeObject(ListNode* node);
private:
	ListNode* head = nullptr;
};

#endif