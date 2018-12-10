

#include "btree.h"

using namespace btree;


btree::BTree234::Node::~Node()
{
	for (int i = 0; i < keycount + 1; i++)
		GiveBack(childs[i]);
}

void btree::BTree234::Node::GiveBack(Node * n)
{
	if (nullptr != n)
		delete n;
}

BTree234::Node * btree::BTree234::Node::GetNode()
{
	return new Node();
}

BTree234 * btree::BTree234::CreateEmptyTree()
{
	BTree234 *t = new BTree234();

	t->root = Node::GetNode();
	t->root->isLeaf = true;

	return t;
}

bool btree::BTree234::Search(int key)
{
	if (nullptr == root)
		return false;

	return SearchNode(root, key);
}

void btree::BTree234::Insert(int key)
{
	if (nullptr == root) {
		root = Node::GetNode();
		root->isLeaf = true;
	}

	Node *n = root;

	if (n->keycount == BTREE_234_NODE_KEY_COUNT) {
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

void btree::BTree234::Delete(int key)
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

void btree::BTree234::FreeTree()
{
	if (nullptr != root) {
		Node::GiveBack(root);
		root = nullptr;
	}
}

void btree::BTree234::TraverseTree()
{
}

btree::BTree234::BTree234() : root(nullptr)
{
}

btree::BTree234::~BTree234()
{
	FreeTree();
}

bool btree::BTree234::SearchNode(Node *n, int key)
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
		}
		return SearchNode(n->childs[i + 1], key);
	}
}

void btree::BTree234::InsertNonFullNode(Node * n, int key)
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
		if (n->childs[i]->keycount == BTREE_234_NODE_KEY_COUNT) {
			SplitChildNode(n, i, n->childs[i]);
		}
		if (n->keys[i] < key)
			i = i + 1;
		InsertNonFullNode(n->childs[i], key);
	}
}

void btree::BTree234::SplitChildNode(Node * p, int index, Node *n)
{
	int t = BTREE_234_NODE_KEY_COUNT_HALF_CEIL - 1;

	Node *right = Node::GetNode();
	right->isLeaf = n->isLeaf;
	right->keycount = t;
	n->keycount = t;
	for (int i = 0; i < t; i++) {
		right->keys[i] = n->keys[i + t + 1];
	}
	if (!n->isLeaf) {
		for (int i = 0; i <= t; i++) {
			right->childs[i] = n->childs[i + t + 1];
		}
	}

	for (int i = p->keycount; i > index; i--)
		p->childs[i + 1] = p->childs[i];
	p->childs[index + 1] = right;
	for (int i = p->keycount - 1; i >= index; i--)
		p->keys[i + 1] = p->keys[i];
	p->keys[index] = n->keys[t];

	n->keycount++;
}

void btree::BTree234::DeleteNode(Node * n, int key)
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
		
		int t = BTREE_234_NODE_KEY_COUNT_HALF_CEIL;

		// find key in internal node
		if (findinkey) {
			if (n->childs[i]->keycount >= t) {
				int nkey = n->childs[i]->keys[n->childs[i]->keycount - 1];
				n->keys[i] = nkey;
				DeleteNode(n->childs[i], nkey);
			}
			else if (n->childs[i + 1]->keycount >= t) {
				int nkey = n->childs[i + 1]->keys[0];
				n->keys[i] = nkey;
				DeleteNode(n->childs[i + 1], nkey);
			}
			else {
				// merge childs[i] and childs[i + 1] and key
			}
		}
		else {
			// find key in internal node's childs

		}
	}
}

