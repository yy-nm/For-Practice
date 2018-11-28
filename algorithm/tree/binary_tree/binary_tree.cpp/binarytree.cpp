

#include "binarytree.h"


#include <iostream>



void binarytree::Tree::Add(int k)
{
	Node *n = root;
	Node *p = n;

	while (n != nullptr) {
		p = n;

		if (n->k < k) {
			n = n->rchild;
		}
		else if (k < n->k) {
			n = n->lchild;
		}
		else {
			return;
		}
	}

	if (nullptr == p) {
		root = GetNode();
		root->k = k;
	}
	else {
		if (p->k < k) {
			p->rchild = GetNode();
			p->rchild->k = k;
		}
		else {
			p->lchild = GetNode();
			p->lchild->k = k;
		}
	}
}

void binarytree::Tree::Remove(int k)
{
	Node *n = root;
	Node *p = n;
	while (n != nullptr)
	{
		if (n->k < k) {
			p = n;
			n = n->rchild;
		}
		else if (k < n->k) {
			p = n;
			n = n->lchild;
		}
		else {
			break;
		}
	}

	if (nullptr == n)
		return;

	Node *replaceone = nullptr;
	Node *replaceonep = nullptr;
	if (n->lchild != nullptr) {
		replaceone = n->lchild;
		while (replaceone->rchild != nullptr) {
			replaceonep = replaceone;
			replaceone = replaceone->rchild;
		}

		if (replaceonep != nullptr) {
			replaceonep->rchild = replaceone->lchild;
		}
		else {
			n->lchild = replaceone->lchild;
		}
		n->k = replaceone->k;
	}
	else if (n->rchild != nullptr) {
		replaceone = n->rchild;
		while (replaceone->lchild != nullptr)
		{
			replaceonep = replaceone;
			replaceone = replaceonep->lchild;
		}

		if (replaceonep != nullptr) {
			replaceonep->lchild = replaceonep->rchild;
		}
		else {
			n->rchild = replaceone->rchild;
		}
		n->k = replaceone->k;
	}
	else {
		// root value
		if (p == n) {
			DesctroyNode(root);
			root = nullptr;
		}
		else {
			if (p->k < n->k) {
				p->rchild = nullptr;
			}
			else {
				p->lchild = nullptr;
			}

			DesctroyNode(n);
		}
	}
}

bool binarytree::Tree::Search(int k)
{
	Node *n = root;
	while (n != nullptr)
	{
		if (n->k < k) {
			n = n->rchild;
		}
		else if (k < n->k) {
			n = n->lchild;
		}
		else {
			return true;
		}
	}

	return false;
}

void binarytree::Tree::TraversePreorder()
{
	TraverseNodePreorder(root);
}

void binarytree::Tree::TraverseInorder()
{
	TraverseNodeInorder(root);
}

void binarytree::Tree::TraversePostorder()
{
	TraverseNodePostorder(root);
}

void binarytree::Tree::TraverseNodePreorder(Node * n)
{
	if (nullptr == n)
		return;
	std::cout << n->k << " ";
	TraverseNodePreorder(n->lchild);
	TraverseNodePreorder(n->rchild);

}

void binarytree::Tree::TraverseNodeInorder(Node * n)
{
	if (nullptr == n)
		return;
	TraverseNodeInorder(n->lchild);
	std::cout << n->k << " ";
	TraverseNodeInorder(n->rchild);
}

void binarytree::Tree::TraverseNodePostorder(Node * n)
{
	if (nullptr == n)
		return;
	TraverseNodePostorder(n->lchild);
	TraverseNodePostorder(n->rchild);
	std::cout << n->k << " ";
}

binarytree::Tree::Node* binarytree::Tree::GetNode()
{
	return new Node();
}

void binarytree::Tree::DesctroyNode(Node * n)
{
	if (n != nullptr) {
		delete n;
	}
}
