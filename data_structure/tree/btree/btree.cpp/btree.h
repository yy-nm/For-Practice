
#ifndef _BTREE_H_
#define _BTREE_H_

#define BTREE_234_NODE_KEY_COUNT 3
#define BTREE_234_NODE_KEY_COUNT_HALF_CEIL 2


namespace btree
{
	class BTree234 {

	protected:
		struct Node {
			bool isLeaf;
			int keys[BTREE_234_NODE_KEY_COUNT];
			int keycount;
			struct Node *childs[BTREE_234_NODE_KEY_COUNT + 1];

			Node() : isLeaf(false), keycount(0) {
				for (int i = 0; i < BTREE_234_NODE_KEY_COUNT; i++)
					keys[i] = 0;
				for (int i = 0; i < BTREE_234_NODE_KEY_COUNT + 1; i++)
					childs[i] = nullptr;
			}
			~Node();

			static void GiveBack(Node *n);
			static Node *GetNode();
		};

		struct Node *root;


	public:
		static BTree234 *CreateEmptyTree();

		bool Search(int key);
		void Insert(int key);
		void Delete(int key);
		void FreeTree();

		void TraverseTree();
	protected:
		BTree234();
		virtual ~BTree234();

		bool SearchNode(Node *n, int key);
		void InsertNonFullNode(Node *n, int key);
		void SplitChildNode(Node *p, int index, Node *n);
		void DeleteNode(Node *n, int key);
		void MergeChildNode(Node *n, int index);
		
	};
}

#endif