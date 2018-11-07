

#include "avl.h"


#include <stack>

using namespace avl;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define GET_LEFT_NODE_HEIGHT(n) ((n)->lchild == nullptr ? -1 : (n)->lchild->height)
#define GET_RIGHT_NODE_HEIGHT(n) ((n)->rchild == nullptr ? -1 : (n)->rchild->height)


/*
template<class T>
Tree<T>::Node::Node() : value(default(T)), lchild(nullptr), rchild(nullptr), height(0)
{
}

template<class T>
Tree<T>::Node::Node(T &value) : value(value), lchild(nullptr), rchild(nullptr), height(0)
{
}

template<class T>
Tree<T>::Node::~Node()
{
	if (rchild != nullptr) {
		delete rchild;
		rchild = nullptr;
	}

	if (lchild != nullptr) {
		delete lchild;
		lchild = nullptr;
	}
}



template<class T>
inline Tree<T>::Tree() : root(nullptr)
{
}

template<class T>
Tree<T>::~Tree()
{
	if (root != nullptr) {
		delete root;
		root = nullptr;
	}
}

template<class T>
void Tree<T>::Add(T & value)
{
	std::stack<Node *> parents;

	// find postion
	Node *node = root;
	while (nullptr != node) {
		parents.push(node);
		if (node->value < value) {
			node = node->rchild;
		}
		else if (value < node->value) {
			node = node->lchild;
		}
		else {
			// value already exists
			return;
		}
	}

	if (parents.size() == 0) {
		node = new Node(value);
		root = node;
	}
	else {
		node = new Node(value);
		Node *p = parents.top();
		if (value < p->value) {
			p->lchild = node;
		}
		else {
			p->rchild = node;
		}

		CheckRotate(parents, value);
	}
}

template<class T>
void Tree<T>::Remove(T & value)
{
	std::stack<Node *> parents;

	// find postion
	Node *node = root;
	while (nullptr != node) {
		parents.push(node);
		if (node->value < value) {
			node = node->rchild;
		}
		else if (value < node->value) {
			node = node->lchild;
		}
		else {
			// value already exists
			break;
		}
	}

	if (nullptr == node)
		return;

	std::stack<Node *> childs;
	if (nullptr != node->rchild) {
		childs.push(node->rchild);
		node = node->rchild;
		while (nullptr != node->lchild) {
			childs.push(node);
			node = node->lchild;
		}
	}
	else if (nullptr != node->lchild) {
		childs.push(node->lchild);
		node = node->lchild;
		while (nullptr != node->rchild) {
			childs.push(node);
			node = node->rchild;
		}
	}

	if (childs.size() == 0) {
		node = parents.top();
		parents.pop();
		delete node;

		if (parents.size() == 0) {
			root = nullptr;
		}
		else {
			if (parents.top()->value < value) {
				parents.top()->rchild = nullptr;
			}
			else {
				parents.top()->lchild = nullptr;
			}
		}
	}
	else {
		parents.top()->value = childs.top()->value;
		
		Node *n = childs.top();
		childs.pop();
		if (childs.size() == 0) {
			if (value < n->value) {
				parents.top()->rchild = nullptr;
			}
			else {
				parents.top()->lchild = nullptr;
			}
		}
		else {
			if (childs.top()->value < n->value) {
				childs.top()->rchild = nullptr;
			}
			else {
				childs.top()->lchild = nullptr;
			}

			// get childs's node to parents
			std::stack<Node *> tmp;
			while (childs.size() > 0) {
				tmp.push(childs.top());
				childs.pop();
			}

			while (tmp.size() > 0) {
				parents.push(tmp.top());
				tmp.pop();
			}
		}
	}

	CheckRotate(parents, value);
}

template<class T>
bool Tree<T>::Search(T & value)
{
	Node *node = root;
	if (nullptr != node) {
		if (node->value < value) {
			node = node->rchild;
		}
		else if (value < node->value) {
			node = node->lchild;
		}
		else {
			return true;
		}
	}

	return false;
}

template<class T>
void Tree<T>::TraversePreorder()
{
	TraverseNodePreorder(root);
}

template<class T>
void Tree<T>::TraverseInorder()
{
	TraverseNodeInorder(root);
}

template<class T>
void Tree<T>::TraversePostorder()
{
	TraverseNodePostorder(root);
}

template<class T>
void avl::Tree<T>::TraverseNodePreorder(Node *n)
{
	if (nullptr == n)
		return;
	std::cout << n->value << " ";
	TraverseNodePreorder(n->lchild);
	TraverseNodePreorder(n->rchild);
}

template<class T>
void avl::Tree<T>::TraverseNodeInorder(Node *n)
{
	if (nullptr == n)
		return;
	TraverseNodeInorder(n->lchild);
	std::cout << n->value << " ";
	TraverseNodeInorder(n->rchild);
}

template<class T>
void avl::Tree<T>::TraverseNodePostorder(Node *n)
{
	if (nullptr == n)
		return;
	TraverseNodePostorder(n->lchild);
	TraverseNodePostorder(n->rchild);
	std::cout << n->value << " ";
}

template<class T>
void Tree<T>::CheckRotate(std::stack<Tree<T>::Node*>& parents, T & value)
{
	int lheight = 0;
	int rheight = 0;
	while (parents.size() > 0) {
		Node *p = parents.top();
		parents.pop();

		lheight = -1;
		if (nullptr != p->lchild) {
			lheight = p->lchild->height;
		}

		rheight = -1;
		if (nullptr != p->rchild) {
			rheight = p->rchild->height;
		}

		Node *subtree = nullptr;

		if (rheight + 1 >= lheight && lheight + 1 >= rheight) {
			p->height = MAX(rheight, lheight) + 1;
		}
		else if (rheight > lheight) {
			if (p->value < value && p->rchild->value < value) {
				// single rotate
				subtree = p->rchild;

				p->rchild = subtree->lchild;
				subtree->lchild = p;
			}
			else {
				// double rotate
				subtree = p->rchild->lchild;

				Node *n = p->rchild;

				n->lchild = subtree->rchild;
				p->rchild = subtree->lchild;
				
				subtree->rchild = n;
				subtree->lchild = p;
			}

			// calc height
			if (nullptr != subtree->lchild) {
				subtree->lchild->height = MAX(GET_LEFT_NODE_HEIGHT(subtree->lchild), GET_RIGHT_NODE_HEIGHT(subtree->lchild)) + 1;
			}
				
			if (nullptr != subtree->rchild) {
				subtree->rchild->height = MAX(GET_LEFT_NODE_HEIGHT(subtree->rchild), GET_RIGHT_NODE_HEIGHT(subtree->rchild)) + 1;
			}
			subtree->height = MAX(GET_LEFT_NODE_HEIGHT(subtree), GET_RIGHT_NODE_HEIGHT(subtree)) + 1;
		}
		else if (lheight > rheight) {
			if (value < p->value && value < p->lchild->value) {
				// single rotate
				subtree = p->lchild;

				p->lchild = subtree->rchild;
				subtree->rchild = p;
			}
			else {
				// double rotate

				subtree = p->lchild->rchild;
				Node *n = p->lchild;

				n->rchild = subtree->lchild;
				p->lchild = subtree->rchild;

				subtree->lchild = n;
				subtree->rchild = p;
			}

			if (nullptr != subtree->lchild) {
				subtree->lchild->height = MAX(GET_LEFT_NODE_HEIGHT(subtree->lchild), GET_RIGHT_NODE_HEIGHT(subtree->lchild)) + 1;
			}
				
			if (nullptr != subtree->rchild) {
				subtree->rchild->height = MAX(GET_LEFT_NODE_HEIGHT(subtree->rchild), GET_RIGHT_NODE_HEIGHT(subtree->rchild)) + 1;
			}
			subtree->height = MAX(GET_LEFT_NODE_HEIGHT(subtree), GET_RIGHT_NODE_HEIGHT(subtree)) + 1;
		}

		if (nullptr != subtree) {
			if (parents.size() > 0) {
				p = parents.top();
				if (p->value < subtree->value)
					p->rchild = subtree;
				else
					p->lchild = subtree;
			}
			else {
				root = subtree;
			}
		}
	}
}

//*/
