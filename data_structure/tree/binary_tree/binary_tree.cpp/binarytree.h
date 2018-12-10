

#ifndef _BINARY_TREE_
#define _BINARY_TREE_

namespace binarytree
{
	class Tree
	{
	protected:
		struct Node {
			Node *lchild;
			Node *rchild;
			int k;

			Node() : lchild(nullptr), rchild(nullptr), k(0) {}
			~Node() {
				if (lchild != nullptr) {
					delete lchild;
					lchild = nullptr;
				}
				if (rchild != nullptr) {
					delete rchild;
					rchild = nullptr;
				}
			}
		};

		Node *root;

	public:
		Tree() : root(nullptr) {}
		virtual ~Tree() {
			if (root != nullptr) {
				delete root;
				root = nullptr;
			}
		}

		void Add(int k);
		void Remove(int k);
		bool Search(int k);

		void TraversePreorder();
		void TraverseInorder();
		void TraversePostorder();

	protected:

		void TraverseNodePreorder(Node *n);
		void TraverseNodeInorder(Node *n);
		void TraverseNodePostorder(Node *n);

		virtual Node* GetNode();
		virtual void DesctroyNode(Node *n);

	};
}



#endif



