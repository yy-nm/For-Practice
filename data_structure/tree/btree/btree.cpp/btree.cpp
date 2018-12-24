

#include "btree.h"


#include <queue>
#include <iostream>


using namespace btree;


btree::BTree23::Node::~Node()
{
	for (int i = 0; keycount > 0 && i < keycount + 1; i++)
		GiveBack(childs[i]);
}

void btree::BTree23::Node::GiveBack(Node * n)
{
	if (nullptr != n)
		delete n;
}

BTree23::Node * btree::BTree23::Node::GetNode()
{
	return new Node();
}

BTree23 * btree::BTree23::CreateEmptyTree()
{
	BTree23 *t = new BTree23();

	t->root = Node::GetNode();
	t->root->isLeaf = true;

	return t;
}

bool btree::BTree23::Search(int key)
{
	if (nullptr == root)
		return false;

	return SearchNode(root, key);
}

void btree::BTree23::Insert(int key)
{
	if (nullptr == root) {
		root = Node::GetNode();
		root->isLeaf = true;
	}

	if (Search(key))
		return;

	Node *n = root;

	if (n->keycount == BTREE_23_NODE_KEY_COUNT) {
		Node *nroot = Node::GetNode();
		root = nroot;
		nroot->isLeaf = false;
		nroot->keycount = 0;
		nroot->childs[0] = n;
		SplitChildNode(nroot, 0, n);
		InsertNonFullNode(nroot, key);
	}
	else {
		InsertNonFullNode(n, key);
	}
}

void btree::BTree23::Delete(int key)
{
	if (nullptr == root) {
		return;
	}

	// not find key
	if (!Search(key))
		return;

	Node *n = root;
	DeleteNode(n, key);
}

void btree::BTree23::FreeTree()
{
	if (nullptr != root) {
		Node::GiveBack(root);
		root = nullptr;
	}
}

void btree::BTree23::TraverseTree()
{
	if (nullptr == root)
		return;

	std::queue<Node *> q;

	q.push(root);
	q.push(nullptr);

	Node *n;
	while (q.size() > 0)
	{
		n = q.front();
		q.pop();

		if (nullptr == n) {
			std::cout << std::endl;
			if (q.size() == 0)
				break;
			q.push(nullptr);
			continue;
		}

		std::cout << "    ";
		for (int i = 0; i < n->keycount; i++) {
			std::cout << n->keys[i] << " ";
		}

		if (!n->isLeaf) {
			for (int i = 0; i <= n->keycount; i++) {
				q.push(n->childs[i]);
			}
		}
	}


}

btree::BTree23::BTree23() : root(nullptr)
{
}

btree::BTree23::~BTree23()
{
	FreeTree();
}

int btree::BTree23::FindMaxKey(Node * n)
{
	if (nullptr == n)
		return 0;
	if (n->isLeaf) {
		return n->keys[n->keycount - 1];
	}
	else {
		return FindMaxKey(n->childs[n->keycount]);
	}
}

int btree::BTree23::FindMinKey(Node * n)
{
	if (nullptr == n)
		return 0;
	if (n->isLeaf) {
		return n->keys[0];
	}
	else {
		return FindMinKey(n->childs[0]);
	}
}

bool btree::BTree23::SearchNode(Node *n, int key)
{
	if (n->isLeaf) {
		for (int i = 0; i < n->keycount; i++) {
			if (key == n->keys[i])
				return true;
		}

		return false;
	}
	else {
		int i = n->keycount - 1;
		while (i >= 0) {
			if (key > n->keys[i])
				break;
			else if (key == n->keys[i])
				return true;
			i--;
		}
		return SearchNode(n->childs[i + 1], key);
	}
}

void btree::BTree23::InsertNonFullNode(Node * n, int key)
{
	if (n->isLeaf) {
		int i = n->keycount - 1;
		while (i >= 0) {
			if (n->keys[i] < key)
				break;
			else if (n->keys[i] == key)
				return;
			n->keys[i + 1] = n->keys[i];
			i--;
		}
		n->keys[i + 1] = key;
		n->keycount++;
	}
	else {
		int i = n->keycount - 1;
		while (i >= 0) {
			if (n->keys[i] < key)
				break;
			else if (n->keys[i] == key)
				return;
			i--;
		}
		i = i + 1;
		if (n->childs[i]->keycount == BTREE_23_NODE_KEY_COUNT) {
			SplitChildNode(n, i, n->childs[i]);

			if (n->keys[i] < key)
				i = i + 1;
		}
		
		InsertNonFullNode(n->childs[i], key);
	}
}

void btree::BTree23::SplitChildNode(Node * p, int index, Node *n)
{
	int middleindex = BTREE_23_NODE_KEY_COUNT / 2;
	int t = BTREE_23_NODE_KEY_COUNT - middleindex - 1;

	Node *right = Node::GetNode();
	right->isLeaf = n->isLeaf;
	right->keycount = t;
	n->keycount = BTREE_23_NODE_KEY_COUNT - t - 1;
	for (int i = 0; i < t; i++) {
		right->keys[i] = n->keys[i + middleindex + 1];
	}
	if (!n->isLeaf) {
		for (int i = 0; i <= t; i++) {
			right->childs[i] = n->childs[i + middleindex + 1];
		}
	}

	for (int i = p->keycount; i > index; i--)
		p->childs[i + 1] = p->childs[i];
	p->childs[index + 1] = right;
	for (int i = p->keycount - 1; i >= index; i--)
		p->keys[i + 1] = p->keys[i];
	p->keys[index] = n->keys[middleindex];

	p->keycount++;
}

void btree::BTree23::DeleteNode(Node * n, int key)
{
	if (n->isLeaf) {
		int i = n->keycount - 1;
		while (i >= 0) {
			if (n->keys[i] < key)
				return;
			else if (n->keys[i] == key)
				break;
			i--;
		}

		if (i >= 0) {
			for (int j = i + 1; j < n->keycount; j++) {
				n->keys[j - 1] = n->keys[j];
			}
			n->keycount--;
		}
	}
	else {
		int i = n->keycount - 1;
		bool findinkey = false;
		while (i >= 0) {
			if (n->keys[i] < key)
				break;
			else if (n->keys[i] == key) {
				findinkey = true;
				break;
			}
			i--;
		}
		
		int t = BTREE_23_NODE_KEY_COUNT_HALF_CEIL;

		// find key in internal node
		if (findinkey) {
			if (n->childs[i]->keycount >= t) {
				int nkey = FindMaxKey(n->childs[i]);
				n->keys[i] = nkey;
				DeleteNode(n->childs[i], nkey);
			}
			else if (n->childs[i + 1]->keycount >= t) {
				int nkey = FindMinKey(n->childs[i + 1]);
				n->keys[i] = nkey;
				DeleteNode(n->childs[i + 1], nkey);
			}
			else {
				// merge childs[i + 1] and key to childs[i]
				MergeChildNode(n, i);
				// check if is keycount == 0
				if (n->keycount == 0 && n == root) {
					root = n->childs[i];
					Node::GiveBack(n);
					DeleteNode(root, key);
				}
				else {
					DeleteNode(n->childs[i], key);
				}
				
			}
		}
		else {
			i++;
			// find key in internal node's childs
			if (n->childs[i]->keycount >= t) {
				DeleteNode(n->childs[i], key);
			}
			else if (i < n->keycount) { // merge bigger one 
				if (n->childs[i + 1]->keycount >= t) {
					// move sibling child's first key and first child to childs[i]
					Node *left = n->childs[i];
					Node *right = n->childs[i + 1];

					
					left->keys[left->keycount] = n->keys[i];
					n->keys[i] = right->keys[0];
					
					left->childs[left->keycount + 1] = right->childs[0];
					left->keycount++;

					for (int j = 0; j < right->keycount - 1; j++) {
						right->keys[j] = right->keys[j + 1];
						right->childs[j] = right->childs[j + 1];
						right->childs[j + 1] = right->childs[j + 2];
					}

					right->keycount--;
				}
				else {
					// merge sibling child and key to childs[i]
					MergeChildNode(n, i);
					// check if is keycount == 0
					if (n->keycount == 0 && n == root) {
						root = n->childs[i];
						Node::GiveBack(n);
						DeleteNode(root, key);
						return;
					}
				}
				DeleteNode(n->childs[i], key);
			}
			else if (i > 0) { // merge litte one
				if (n->childs[i - 1]->keycount >= t) {
					Node *left = n->childs[i - 1];
					Node *right = n->childs[i];

					for (int j = right->keycount; j > 0; j++) {
						right->keys[j] = right->keys[j - 1];
						right->childs[j + 1] = right->childs[j];
						right->childs[j] = right->childs[j - 1];
					}
					
					right->keys[0] = n->keys[i - 1];
					n->keys[i - 1] = left->keys[left->keycount - 1];
					right->childs[0] = left->childs[left->keycount];

					left->keycount--;
					right->keycount++;
				}
				else {
					MergeChildNode(n, i - 1);
					if (n->keycount == 0 && n == root) {
						root = n->childs[i - 1];
						Node::GiveBack(n);
						DeleteNode(root, key);
						return;
					}
				}
				DeleteNode(n->childs[i - 1], key);
			}
		}
	}
}

void btree::BTree23::MergeChildNode(Node *n, int index)
{
	Node *mergenode = n->childs[index];
	Node *bemergednode = n->childs[index + 1];
	mergenode->keys[mergenode->keycount] = n->keys[index];
	mergenode->keycount++;
	for (int j = 0; j < bemergednode->keycount; j++) {
		mergenode->keys[mergenode->keycount + j] = bemergednode->keys[j];
		mergenode->childs[mergenode->keycount + j] = bemergednode->childs[j];
	}
	mergenode->childs[mergenode->keycount + bemergednode->keycount] = bemergednode->childs[bemergednode->keycount];

	mergenode->keycount += bemergednode->keycount;

	// remove key and childs[i + 1] from n
	for (int j = index; j < n->keycount - 1; j++) {
		n->keys[j] = n->keys[j + 1];
		n->childs[j + 1] = n->childs[j + 2];
	}
	n->keycount--;
}

