

#include "trie.h"


#include <cassert>
#include <stack>
#include <queue>
#include <deque>
#include <iostream>


using namespace trie;

static int bitmask[] = {
	0x00000001, 0x00000002, 0x00000004, 0x00000008,
	0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800,
	0x00001000, 0x00002000, 0x00004000, 0x00008000,

	0x00010000, 0x00020000, 0x00040000, 0x00080000,
	0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000,
};


Trie::Node * trie::Trie::GetNode()
{
	return new Node();
}

void trie::Trie::GiveBack(Node * p)
{
	if (nullptr != p)
		delete p;
}

Trie::LinkedNode * trie::Trie::GetLinkedNode()
{
	return new Trie::LinkedNode();
}

void trie::Trie::GiveBack(LinkedNode * p)
{
	if (nullptr != p)
		delete p;
}

void trie::Trie::Insert(const char * str, size_t len)
{
	if (0 == len || nullptr == str)
		return;

	Node *n = &root;
	LinkedNode *child = nullptr;
	LinkedNode *first = nullptr;
	int offset = 5;
	int mask = (1 << offset) - 1;
	byte b;

	int v;
	for (size_t i = 0; i < len; i++) {
		b = str[i];
		v = b;
		if ((n->children.childrenMap[b >> offset] & bitmask[b & mask]) == 0) {
			n->children.childrenMap[b >> offset] |= bitmask[b & mask];
			n->children.childcount++;

			child = GetLinkedNode();
			child->value = b;
			child->next = n->children.node;
			n->children.node = child;

			child->child = GetNode();
		}
		else {
			child = n->children.node;
			while (child != nullptr && child->value != b) {
				child = child->next;
			}
			
			// cannot find existing one
			assert(child != nullptr);
		}

		// get node error
		assert(child != nullptr);

		n = child->child;
	}

	assert(n != nullptr);

	n->isEnd = true;
}

bool trie::Trie::Search(const char * str, size_t len)
{
	if (0 == len || nullptr == str)
		return false;

	Node *n = &root;
	LinkedNode *child = nullptr;
	int b;
	int offset = 5;
	int mask = (1 << offset) - 1;
	for (size_t i = 0; i < len; i++) {
		b = str[i];
		if ((n->children.childrenMap[b >> offset] & bitmask[b & mask]) == 0) {
			return false;
		}
		child = n->children.node;
		while (child != nullptr && child->value != b) {
			child = child->next;
		}

		if (nullptr == child)
			return false;
		n = child->child;
	}

	return nullptr != n && n->isEnd;
}

void trie::Trie::Delete(const char * str, size_t len)
{
	if (!Search(str, len))
		return;

	std::stack<Node *> parents;
	std::stack<char> chars;

	Node *n = &root;
	LinkedNode *child = nullptr;
	byte b;
	int offset = 5;
	int mask = (1 << offset) - 1;
	for (size_t i = 0; i < len; i++) {
		b = str[i];

		child = n->children.node;
		while (child != nullptr && child->value != b) {
			child = child->next;
		}

		parents.push(n);
		chars.push(b);

		n = child->child;
	}

	// check find str or not
	assert(n != nullptr && n->isEnd);

	// the path is other words' prefix
	if (n->children.childcount != 0) {
		n->isEnd = false;
		return;
	}
	
	while (parents.size() > 0) {
		n = parents.top();
		parents.pop();
		b = chars.top();
		chars.pop();

		if (n->isEnd || n->children.childcount > 1) {
			break;
		}

	}
	
	child = n->children.node;
	LinkedNode *childpre = nullptr;
	while (child != nullptr && child->value != b) {
		childpre = child;
		child = child->next;
	}

	if (nullptr != childpre) {
		childpre->next = child->next;
	}
	else {
		n->children.node = child->next;
	}

	n->children.childrenMap[b >> offset] &= ~bitmask[b & mask];
	n->children.childcount--;
	child->next = nullptr;
	GiveBack(child);
}

void trie::Trie::PrintString(struct Node *n, std::deque<char> &words)
{
	if (n->isEnd) {

		std::string outstr(words.begin(), words.end());
		std::cout << outstr << std::endl;
	}

	LinkedNode *child = n->children.node;
	while (nullptr != child) {
		words.push_back(child->value);
		PrintString(child->child, words);
		words.pop_back();
		child = child->next;
	}
	
}

void trie::Trie::PrintAllString()
{
	std::deque<char> words;
	PrintString(&root, words);
}
