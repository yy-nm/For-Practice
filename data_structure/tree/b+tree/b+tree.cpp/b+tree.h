

#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_


#include <stack>
#include <queue>
#include <cassert>

#include <iostream>


namespace bplustree
{
	template<size_t B>
	class BPlusTree {
		static_assert(B > 2, "b+tree node count must be more than 1!");
	protected:
		struct Node {
			bool isLeaf;
			int keys[B];
			int keycount;
			struct Node *childs[B + 1];

			Node() : isLeaf(false), keycount(0)
			{
				for (int i = 0; i < B; i++) {
					keys[i] = 0;
				}
				for (int i = 0; i <= B; i++) {
					childs[i] = nullptr;
				}
			}

			~Node()
			{
				for (int i = 0; keycount > 0 && i < keycount && !isLeaf; i++)
					GiveBack(childs[i]);
			}

			static void GiveBack(Node *n)
			{
				if (nullptr != n)
					delete n;
			}
			static Node* GetNode()
			{
				return new Node();
			}
		};

		struct Node *root;

	public:
		static BPlusTree* CreateEmptyTree()
		{
			BPlusTree *t = new BPlusTree();

			t->root = Node::GetNode();
			t->root->isLeaf = true;

			return t;
		}

		bool Search(int key)
		{
			if (nullptr == root) {
				return false;
			}
			return SearchNode(root, key);
		}
		void Insert(int key)
		{

			if (Search(key))
				return;

			if (nullptr == root) {
				root = Node::GetNode();
				root->isLeaf = true;
			}
			InsertNode(root, key);
		}
		void Delete(int key)
		{
			if (!Search(key))
				return;

			if (nullptr == root) {
				return;
			}

			DeleteNode(root, key);
		}

		void FreeTree()
		{
			if (nullptr != root) {
				Node::GiveBack(root);
				root = nullptr;
			}
		}

		// traverse tree's hierarchy
		void TraverseTree() 
		{
			std::cout << std::endl;

			if (nullptr == root) {
				return;
			}

			std::queue<Node*> q;

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
					if (!n->isLeaf)
						q.push(n->childs[i]);
				}
			}
		}

		// traverse data in bottom of tree
		void TraverseData()
		{
			if (nullptr == root) {
				return;
			}

			Node *n = root;

			while (!n->isLeaf)
			{
				n = n->childs[0];
			}

			while (nullptr != n) {
				for (int i = 0; i < n->keycount; i++) {
					std::cout << n->keys[i] << " ";
				}
				std::cout << "    ";
				n = n->childs[B];
			}
		}

	protected:
		BPlusTree() : root(nullptr) {}
		~BPlusTree() { FreeTree(); }


		bool SearchNode(Node *n, int k)
		{
			if (n->isLeaf) {
				for (int i = 0; i < n->keycount; i++) {
					if (k == n->keys[i])
						return true;
				}
				return false;
			}
			else {
				int index = n->keycount - 1;
				while (index >= 0) {
					if (n->keys[index] <= k)
						break;
					index--;
				}
				if (index < 0)
					return false;
				return SearchNode(n->childs[index], k);
			}
		}

		void InsertNode(Node *n, int k)
		{
			std::stack<Node *> parents;

			while (!n->isLeaf) {
				parents.push(n);

				int index = n->keycount - 1;
				while (index >= 0) {
					if (n->keys[index] < k) {
						break;
					}
					index--;
				}

				if (index < 0) {
					index = 0;
				}

				n = n->childs[index];
			}

			if (n->keycount == B) {
				SplitLeafChild(n, k, parents);
			} else {
				ModifyNodeKeys(n, k, parents);
			}
		}

		// the node keycount must smaller than 
		void ModifyNodeKeys(Node *n, int k, std::stack<Node *> &parents)
		{
			int index = n->keycount - 1;
			while (index >= 0) {
				if (n->keys[index] < k) {
					break;
				}
				n->keys[index + 1] = n->keys[index];
				index--;
			}
			index++;
			if (index > 0) {
				n->keys[index] = k;
			}
			else {
				// modify parent's keys
				int oldkey = n->keys[index];
				n->keys[index] = k;

				Node *p = nullptr;
				bool goon = false;
				while (parents.size() > 0) {
					p = parents.top();
					parents.pop();
					goon = false;

					for (int i = 0; i < p->keycount; i++) {
						if (p->keys[i] == oldkey) {
							p->keys[i] = k;
							if (i == 0)
								goon = true;
							break;
						}
					}

					if (!goon) {
						break;
					}
				}
			}
			n->keycount++;
		}

		void ModifyParentKeys(int oldk, int newk, std::stack<Node *> &parents)
		{
			if (parents.size() == 0) {
				return;
			}
			
			Node *p = parents.top();
			parents.pop();

			int oldpk = p->keys[0];

			for (int i = 0; i < p->keycount; i++) {
				if (p->keys[i] == oldk) {
					p->keys[i] = newk;
					break;
				}
			}

			if (oldpk != p->keys[0])
				ModifyParentKeys(oldpk, p->keys[0], parents);
		}

		void InsertInternalNode(int oldk, Node *nl, Node *nr, std::stack<Node *> &parents)
		{
			Node *p = nullptr;
			// handle root stuff, tree grows
			if (parents.size() == 0) {
				p = Node::GetNode();
				p->isLeaf = false;
				root = p;
				
				p->childs[0] = nl;
				p->childs[1] = nr;

				p->keys[0] = nl->keys[0];
				p->keys[1] = nr->keys[0];

				p->keycount = 2;
			}
			else {
				p = parents.top();
				parents.pop();

				int index = -1;
				for (int i = 0; i < p->keycount; i++) {
					if (p->keys[i] == oldk) {
						index = i;
						break;
					}
				}

				assert(index >= 0);

				if (p->keycount < B) {
					int oldpk = p->keys[0];
					for (int i = p->keycount - 1; i > index; i--) {
						p->childs[i + 1] = p->childs[i];
						p->keys[i + 1] = p->keys[i];
					}
					p->childs[index + 1] = nr;
					p->keys[index + 1] = nr->keys[0];
					p->keys[index] = nl->keys[0];

					if (index == 0 && p->keys[0] != oldpk) {
						ModifyParentKeys(oldpk, p->keys[0], parents);
					}
					p->keycount++;
				}
				else {
					int k = nr->keys[0];

					int pkey = p->keys[0];
					Node *pl = p;
					Node *pr = Node::GetNode();
					pr->isLeaf = pl->isLeaf;

					int middleindex = (B + 1) / 2 + (B + 1) % 2;
					bool isleft = (index + 1) < middleindex;
					int count = (B + 1) / 2;
					if (isleft) {
						middleindex--;
					}
					else {
						count--;
					}

					for (int i = 0; i < count; i++) {
						pr->childs[i] = pl->childs[i + middleindex];
						pr->keys[i] = pr->childs[i]->keys[0];
					}
					pr->keycount = count;
					pl->keycount -= count;

					Node *insertnode = pr;
					if (isleft) {
						insertnode = pl;
					}

					int pindex = insertnode->keycount - 1;
					while (pindex >= 0) {
						if (insertnode->keys[pindex] < k)
							break;
						insertnode->keys[pindex + 1] = insertnode->keys[pindex];
						insertnode->childs[pindex + 1] = insertnode->childs[pindex];
						pindex--;
					}
					insertnode->keys[pindex + 1] = k;
					insertnode->childs[pindex + 1] = nr;
					insertnode->keycount++;
					
					if (index < pl->keycount) {
						pl->keys[index] = nl->keys[0];
					}
					else {
						pr->keys[index - pl->keycount] = nl->keys[0];
					}

					InsertInternalNode(pkey, pl, pr, parents);
				}
			}
		}

		// 
		void SplitLeafChild(Node *n, int k, std::stack<Node *> &parents)
		{
			int pkey = n->keys[0];

			Node *l = n;
			Node *r = Node::GetNode();
			r->isLeaf = l->isLeaf;
			int middleindex = (B + 1) / 2 + (B + 1) % 2;
			bool isleft = n->keys[middleindex - 1] > k;
			int count = (B + 1) / 2;

			if (isleft) {
				middleindex--;
			}
			else {
				count--;
			}
			
			for (int i = 0; i < count; i++) {
				r->keys[i] = l->keys[i + middleindex];
			}
			r->keycount = count;
			l->keycount -= count;

			Node *insertnode = r;
			if (isleft) {
				insertnode = l;
			}

			int index = insertnode->keycount - 1;
			while (index >= 0) {
				if (insertnode->keys[index] < k)
					break;
				insertnode->keys[index + 1] = insertnode->keys[index];
				index--;
			}

			insertnode->keys[index + 1] = k;
			insertnode->keycount++;

			// b+tree feature
			r->childs[B] = l->childs[B];
			l->childs[B] = r;

			InsertInternalNode(pkey, l, r, parents);
		}


		void DeleteNode(Node *n, int k)
		{
			std::stack<Node *> parents;

			while (!n->isLeaf) {
				parents.push(n);

				for (int i = n->keycount - 1; i >= 0; i--) {
					if (n->keys[i] <= k) {
						n = n->childs[i];
						break;
					}
				}
			}

			int mincount = (B + 1) / 2;
			if (n->keycount > mincount || parents.size() == 0) {
				int index = -1;
				for (int i = 0; i < n->keycount; i++) {
					if (n->keys[i] == k) {
						index = i;
						break;
					}
				}

				assert(index >= 0);
				
				int oldk = n->keys[0];
				for (int i = index; i < n->keycount - 1; i++) {
					n->keys[i] = n->keys[i + 1];
				}
				n->keycount--;

				if (oldk != n->keys[0]) {
					ModifyParentKeys(oldk, n->keys[0], parents);
				}
			}
			else {

				int oldk = n->keys[0];
				int index = -1;
				for (int i = 0; i < n->keycount; i++) {
					if (n->keys[i] == k) {
						index = i;
						break;
					}
				}

				assert(index >= 0);

				for (int i = index; i < n->keycount - 1; i++) {
					n->keys[i] = n->keys[i + 1];
				}
				n->keycount--;

				KeepNodeKeyCountCorrect(n, oldk, parents);
			}
		}

		void KeepNodeKeyCountCorrect(Node *n, int k, std::stack<Node *> &parents)
		{
			int oldk = k;
			Node *p = parents.top();
			int pindex = -1;
			for (int i = 0; i < p->keycount; i++) {
				if (oldk == p->keys[i]) {
					pindex = i;
					break;
				}
			}

			assert(pindex >= 0);

			int mincount = (B + 1) / 2;
			// extra one key from sibling node
			if (pindex < p->keycount - 1 && p->childs[pindex + 1]->keycount > mincount) {
				Node *r = p->childs[pindex + 1];
				n->keys[n->keycount] = r->keys[0];
				if (!n->isLeaf) {
					n->childs[n->keycount] = r->childs[0];
				}
				n->keycount++;

				int oldrk = r->keys[0];
				for (int i = 1; i < r->keycount; i++) {
					r->keys[i - 1] = r->keys[i];
					if (!r->isLeaf) {
						r->childs[i - 1] = r->childs[i];
					}
				}
				r->keycount--;

				std::stack<Node *> ps(parents);
				ModifyParentKeys(oldrk, r->keys[0], ps);
				if (oldk != n->keys[0]) {
					ModifyParentKeys(oldk, n->keys[0], parents);
				}
			}
			else if (pindex > 0 && p->childs[pindex - 1]->keycount > mincount) {
				Node *l = p->childs[pindex - 1];

				for (int i = n->keycount - 1; i >= 0; i--) {
					n->keys[i + 1] = n->keys[i];
					if (!n->isLeaf) {
						n->childs[i + 1] = n->childs[i];
					}
				}
				n->keys[0] = l->keys[l->keycount - 1];
				if (!n->childs) {
					n->childs[0] = l->childs[l->keycount - 1];
				}
				n->keycount++;
				l->keycount--;

				ModifyParentKeys(oldk, n->keys[0], parents);
			}
			else {
				// consider to merge with sibling node
				if (pindex < p->keycount - 1) {
					MergeNode(n, oldk, parents);
				}
				else if (pindex > 0) {
					MergeNode(p->childs[pindex - 1], p->childs[pindex - 1]->keys[0], parents);
				}
			}
		}

		void MergeNode(Node *n, int k, std::stack<Node *> &parents)
		{
			Node *p = parents.top();
			parents.pop();

			int oldpk = p->keys[0];

			int pindex = -1;
			for (int i = 0; i < p->keycount; i++) {
				if (k == p->keys[i]) {
					pindex = i;
					break;
				}
			}

			assert(pindex >= 0);

			Node *l = p->childs[pindex];
			Node *r = p->childs[pindex + 1];

			int lcount = l->keycount;
			for (int i = 0; i < r->keycount; i++) {
				l->keys[lcount + i] = r->keys[i];
				if (!l->isLeaf)
					l->childs[lcount + i] = r->childs[i];
			}

			if (l->isLeaf) {
				l->childs[B] = r->childs[B];
			}

			l->keycount += r->keycount;
			r->keycount = 0;
			Node::GiveBack(r);

			for (int i = pindex + 1; i < p->keycount - 1; i++) {
				p->keys[i] = p->keys[i + 1];
				p->childs[i] = p->childs[i + 1];
			}
			p->keycount--;
			p->keys[0] = p->childs[0]->keys[0];


			int mincount = (B + 1) / 2;
			// handle p is root 
			if (parents.size() == 0) {
				if (p->keycount == 1) {
					root = p->childs[0];
					p->keycount = 0;
					Node::GiveBack(p);
				}
			}
			else if (p->keycount < mincount) {
				KeepNodeKeyCountCorrect(p, oldpk, parents);
			}
			else if (oldpk != p->keys[0]) {
				ModifyParentKeys(oldpk, p->keys[0], parents);
			}
		}


	};
}

#endif