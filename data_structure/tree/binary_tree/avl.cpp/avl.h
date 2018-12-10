

#ifndef _AVL_H_
#define _AVL_H_

#include <stack>
#include <iostream>

namespace avl
{

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define GET_LEFT_NODE_HEIGHT(n) ((n)->lchild == nullptr ? -1 : (n)->lchild->height)
#define GET_RIGHT_NODE_HEIGHT(n) ((n)->rchild == nullptr ? -1 : (n)->rchild->height)

	template<class T>
	class Tree {

	protected:
		struct Node {
			Node *lchild;
			Node *rchild;
			int height;
			T value;

			Node() : value(default(T)), lchild(nullptr), rchild(nullptr), height(0) {}
			Node(T &value) : value(value), lchild(nullptr), rchild(nullptr), height(0) {}

			virtual ~Node() 
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
		};

		Node *root;

	public:
		Tree() : root(nullptr) {}
		virtual ~Tree() 
		{
			if (root != nullptr) {
				delete root;
				root = nullptr;
			}
		}

		void Add(T &value)
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
		void Remove(T &value) 
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
					childs.push(node->lchild);
					node = node->lchild;
				}
			}
			else if (nullptr != node->lchild) {
				childs.push(node->lchild);
				node = node->lchild;
				while (nullptr != node->rchild) {
					childs.push(node->rchild);
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
						parents.top()->rchild = n->rchild;
						n->rchild = nullptr;
					}
					else {
						parents.top()->lchild = n->lchild;
						n->lchild = nullptr;
					}
				}
				else {
					if (childs.top()->value < n->value) {
						childs.top()->rchild = n->lchild;
						n->lchild = nullptr;
					}
					else {
						childs.top()->lchild = n->rchild;
						n->rchild = nullptr;
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
				value = n->value;
				delete n;
				n = nullptr;
			}

			CheckRotate(parents, value);
		}
		bool Search(T &value)
		{
			Node *node = root;
			while (nullptr != node) {
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

		void TraversePreorder()
		{
			TraverseNodePreorder(root);
		}
		void TraverseInorder()
		{
			TraverseNodeInorder(root);
		}
		void TraversePostorder()
		{
			TraverseNodePostorder(root);
		}

	protected:
		void TraverseNodePreorder(Node *n)
		{
			if (nullptr == n)
				return;
			std::cout << n->value << " ";
			TraverseNodePreorder(n->lchild);
			TraverseNodePreorder(n->rchild);
		}
		void TraverseNodeInorder(Node *n)
		{
			if (nullptr == n)
				return;
			TraverseNodeInorder(n->lchild);
			std::cout << n->value << " ";
			TraverseNodeInorder(n->rchild);
		}
		void TraverseNodePostorder(Node *n)
		{
			if (nullptr == n)
				return;
			TraverseNodePostorder(n->lchild);
			TraverseNodePostorder(n->rchild);
			std::cout << n->value << " ";
		}

		void CheckRotate(std::stack<Tree<T>::Node *> &parents, T &value)
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
					if (p->value < value && value < p->rchild->value) {
						// double rotate
						subtree = p->rchild->lchild;

						Node *n = p->rchild;

						n->lchild = subtree->rchild;
						p->rchild = subtree->lchild;

						subtree->rchild = n;
						subtree->lchild = p;
					}
					else {
						// single rotate
						subtree = p->rchild;

						p->rchild = subtree->lchild;
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
					if (value < p->value && p->lchild->value < value) {
						// double rotate

						subtree = p->lchild->rchild;
						Node *n = p->lchild;

						n->rchild = subtree->lchild;
						p->lchild = subtree->rchild;

						subtree->lchild = n;
						subtree->rchild = p;
					}
					else {
						// single rotate
						subtree = p->lchild;

						p->lchild = subtree->rchild;
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
	};
	
	

}


#endif