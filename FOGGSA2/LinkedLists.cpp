#include "LinkedLists.h"

ListNode* LinkedLists::MakeNode(ListNode** head,SceneObject* obj) {
	ListNode* newNode = new ListNode;
	ListNode* last = *head;

	newNode->next = nullptr;
	newNode->object = obj;

	if (*head == nullptr)
	{
		*head = newNode;
		return newNode;
	}

	while (last->next != nullptr)
	{
		last = last->next;
	}
	last->next = newNode;

	return newNode;
}

ListNode* LinkedLists::InsertFirst(ListNode** head,SceneObject* obj) {
	ListNode* newNode = new ListNode;

	newNode->next = *head;
	newNode->object = obj;
	*head = newNode;

	return newNode;
}

void LinkedLists::InsertAfter(ListNode* lastNode, SceneObject* obj) {
	if (lastNode == nullptr)
	{
		return;
	}

	ListNode* newNode = new ListNode;
	newNode->next = lastNode->next;
	newNode->object = obj;
	lastNode->next = newNode;
}

void LinkedLists::DeleteList(ListNode** node) {
	ListNode* pTemp = *node;
	if (pTemp != nullptr && pTemp->next != nullptr) {
		DeleteList(&pTemp->next);
	}
	DestroyNodeObject(pTemp);
	delete pTemp;
	*node = nullptr;
}

void LinkedLists::DeleteAfter(ListNode* node) {
	ListNode* pTemp;
	if (node != nullptr && node->next != nullptr)
	{
		pTemp = node->next;
		node->next = pTemp->next;

		DestroyNodeObject(pTemp);
		delete pTemp;
	}
}

void LinkedLists::DeleteAllAfter(ListNode* node) {
	ListNode* pTemp = node;
	while (node->next != nullptr)
	{
		DeleteAfter(pTemp);
	}
}

void LinkedLists::DeleteAt(ListNode** node, int pos) {
	ListNode* nextNode = GetNode(*node, pos + 1);
	ListNode* targetNode = GetNode(*node, pos);
	ListNode* previousNode = GetNode(*node, pos - 1);

	if (targetNode != nullptr) {
		if (nextNode == nullptr && previousNode == nullptr) //There is only 1 node inside the whole list
		{
			DeleteList(node);
			return;
		}

		if (previousNode == nullptr) // It is the first in the array
		{
			*node = nextNode;
			nextNode = *node;

			DestroyNodeObject(targetNode);
			delete targetNode;
		}
		else {
			DeleteAfter(previousNode);
			previousNode->next = nextNode;
		}
	}
}

ListNode* LinkedLists::GetNode(ListNode* node, int pos) {
	int count = 0;
	while (node != nullptr)
	{
		if (count == pos)
		{
			return node;
		}
		count++;
		node = node->next;
	}
	return nullptr;
}

ListNode* LinkedLists::Find(ListNode* node, SceneObject* obj) {
	ListNode* result = nullptr;

	if (node != nullptr)
	{
		if (node->object == obj)
		{
			result = node;
		}
		else {
			result = Find(node->next, obj);
		}
	}

	return result;
}

void LinkedLists::PrintList(ListNode* node) {
	if (node == nullptr) {
		std::cout << "| NULL |" << std::endl;
	}
	else {
		std::cout << std::setw(5) << "| (" << node << ")|-->";
		PrintList(node->next);
	}
}

void LinkedLists::PrintListBackwards(ListNode* node) {
	if (node != nullptr)
	{
		PrintListBackwards(node->next);
		std::cout << std::setw(5) << "|| (" << node << ")||-->";
	}
}

void LinkedLists::DestroyNodeObject(ListNode* node) {
	if (node != nullptr && node->object != nullptr) {
		delete node->object;
	}
}