

#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_

namespace bplustree
{
	template<size_t n>
	class BPlusTree {
	protected:
		struct Node {
			bool isLeaf;
			int keys[n];
			int keycount;
			struct Node *childs[n + 1];

			Node() : isLeaf(false), keycount(0) {
				for (int i = 0; i < n; i++) {
					keys[i] = 0;
				}
				for (int i = 0; i <= n; i++) {
					childs[i] = nullptr;
				}
			}

			~Node();

			static void GiveBack(Node *n);
			static void Node* GetNode();
		};

		struct Node *root;

	public:
		static BPlusTree* CreateEmptyTree();

		bool Search(int key);
		void Insert(int key);
		void Delete(int key);
		void FreeTree();

		void TraverseTree();

	protected:
		BPlusTree();
		~BPlusTree();

		// 
		void SplitChild(Node *p, Node *n);


	};
}

#endif