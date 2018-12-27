

#ifndef _TRIE_H_
#define _TRIE_H_


#include <string>
#include <deque>


namespace trie
{
	class Trie
	{
	protected:

		typedef unsigned char byte;

		struct Node;

		struct LinkedNode
		{
			struct Node *child;
			struct LinkedNode *next;
			byte value;

			LinkedNode() : child(nullptr), next(nullptr), value(0) {}
			~LinkedNode() {
				if (nullptr != child)
					delete child;
				if (nullptr != next)
					delete next;
			}
		};
		struct Children
		{
			struct LinkedNode *node;
			int childrenMap[8];
			int childcount;

			Children() : node(nullptr), childcount(0) {
				childrenMap[0] = childrenMap[1] = childrenMap[2] = childrenMap[3] = 0;
				childrenMap[4] = childrenMap[5] = childrenMap[6] = childrenMap[7] = 0;
			}
			~Children() {
				if (nullptr != node)
					delete node;
			}
		};

		struct Node 
		{
			struct Children children;
			bool isEnd;

			Node() : isEnd(false) {}
		};

		struct Node root;


		static struct Node* GetNode();
		static void GiveBack(struct Node *p);
		static struct LinkedNode* GetLinkedNode();
		static void GiveBack(struct LinkedNode *p);

		void PrintString(struct Node *n, std::deque<char> &words);

	public:

		Trie() {}
		~Trie() {}

		void Insert(const char *str, size_t len);
		void Insert(std::string &str) {
			Insert(str.c_str(), str.size());
		}

		bool Search(const char *str, size_t len);
		bool Search(std::string &str) {
			return Search(str.c_str(), str.size());
		}

		void Delete(const char *str, size_t len);
		void Delete(std::string &str) {
			Delete(str.c_str(), str.size());
		}

		void PrintAllString();
	};
}

#endif