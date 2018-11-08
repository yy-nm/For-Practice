package avl

import (
	"fmt"
)

type Node struct {
	lchild *Node
	rchild *Node
	height int
	value  int
}

type Tree struct {
	root *Node
}

func max(l, r int) int {
	if r > l {
		return r
	} else {
		return l
	}
}

func getHeight(n *Node) {
	if n == nil {
		return
	}

	lheight := -1
	if n.lchild != nil {
		lheight = n.lchild.height
	}

	rheight := -1
	if n.rchild != nil {
		rheight = n.rchild.height
	}

	n.height = max(lheight, rheight) + 1
}

func checkRotate(t *Tree, parents []*Node, v int) {
	sz := len(parents) - 1

	for sz >= 0 {
		p := parents[sz]
		sz--

		lheight := -1
		if p.lchild != nil {
			lheight = p.lchild.height
		}

		rheight := -1
		if p.rchild != nil {
			rheight = p.rchild.height
		}

		var subtree *Node
		if rheight+1 >= lheight && lheight+1 >= rheight {
			p.height = max(lheight, rheight) + 1
		} else if rheight > lheight {
			if p.value < v && v < p.rchild.value {
				// double rotate
				subtree = p.rchild.lchild
				n := p.rchild

				p.rchild = subtree.lchild
				n.lchild = subtree.rchild

				subtree.lchild = p
				subtree.rchild = n
			} else {
				// single rotate
				subtree = p.rchild

				p.rchild = subtree.lchild
				subtree.lchild = p
			}

			getHeight(subtree.lchild)
			getHeight(subtree.rchild)
			getHeight(subtree)
		} else if lheight > rheight {
			if v < p.value && p.lchild.value < v {
				// double rotate
				subtree = p.lchild.rchild
				n := p.lchild

				p.lchild = subtree.rchild
				n.rchild = subtree.lchild

				subtree.rchild = p
				subtree.lchild = n
			} else {
				// single rotate
				subtree = p.lchild

				p.lchild = subtree.rchild

				subtree.rchild = p
			}

			getHeight(subtree.lchild)
			getHeight(subtree.rchild)
			getHeight(subtree)
		}

		// check up node combined
		if subtree != nil {
			if sz < 0 {
				t.root = subtree
			} else {
				p := parents[sz]

				if p.value > subtree.value {
					p.lchild = subtree
				} else {
					p.rchild = subtree
				}
			}
		}
	}
}

func (t *Tree) AddElement(v int) {
	if t == nil {
		return
	}

	parents := make([]*Node, 0)
	n := t.root

	for n != nil {
		parents = append(parents, n)
		if n.value < v {
			n = n.rchild
		} else if v < n.value {
			n = n.lchild
		} else {
			return
		}
	}

	if len(parents) == 0 {
		t.root = &Node{value: v}
	} else {
		n = parents[len(parents)-1]
		if n.value < v {
			n.rchild = &Node{value: v}
		} else {
			n.lchild = &Node{value: v}
		}
	}

	checkRotate(t, parents, v)
}

func (t *Tree) RemoveElement(v int) {
	if t == nil {
		return
	}

	parents := make([]*Node, 0)

	n := t.root

	for n != nil {
		parents = append(parents, n)
		if n.value < v {
			n = n.rchild
		} else if v < n.value {
			n = n.lchild
		} else {
			break
		}
	}

	if n == nil {
		return
	}

	childs := make([]*Node, 0)
	if n.rchild != nil {
		childs = append(childs, n.rchild)
		n = n.rchild
		for n.lchild != nil {
			childs = append(childs, n.lchild)
			n = n.lchild
		}
	} else if n.lchild != nil {
		childs = append(childs, n.lchild)
		n = n.lchild
		for n.rchild != nil {
			childs = append(childs, n.rchild)
			n = n.rchild
		}
	}

	if len(childs) == 0 {
		// has no childs
		sz := len(parents) - 1
		if sz == 0 {
			t.root = nil
		} else {
			if parents[sz-1].value > v {
				parents[sz-1].lchild = nil
			} else {
				parents[sz-1].rchild = nil
			}
		}
	} else {
		psz := len(parents) - 1
		csz := len(childs) - 1

		plastone := parents[psz]
		clastone := childs[csz]

		if csz == 0 {
			if plastone.value < clastone.value {
				plastone.rchild = clastone.rchild
			} else {
				plastone.lchild = clastone.lchild
			}
		} else {
			clasttwo := childs[csz-1]
			if clasttwo.value < clastone.value {
				clasttwo.rchild = clastone.lchild
			} else {
				clasttwo.lchild = clastone.rchild
			}
		}

		for i := 0; i < len(childs)-1; i++ {
			parents = append(parents, childs[i])
		}

		parents[psz].value = childs[csz].value
		v = childs[csz].value
	}

	checkRotate(t, parents, v)
}

func (t *Tree) SearchElement(v int) bool {
	if t == nil {
		return false
	}

	n := t.root
	for n != nil {
		if n.value < v {
			n = n.rchild
		} else if v < n.value {
			n = n.lchild
		} else {
			return true
		}
	}

	return false
}

func (t *Tree) FreeTree() {
	if t != nil {
		t.root = nil
	}
}

func traversePreorder(n *Node) {
	if n == nil {
		return
	}

	fmt.Printf("%v ", n.value)
	traversePreorder(n.lchild)
	traversePreorder(n.rchild)
}

func (t *Tree) TraversePreorder() {
	if t == nil {
		return
	}
	traversePreorder(t.root)
}

func traverseInorder(n *Node) {
	if n == nil {
		return
	}

	traverseInorder(n.lchild)
	fmt.Printf("%v ", n.value)
	traverseInorder(n.rchild)
}

func (t *Tree) TraverseInorder() {
	if t == nil {
		return
	}
	traverseInorder(t.root)
}

func traversePostorder(n *Node) {
	if n == nil {
		return
	}

	traversePostorder(n.lchild)
	traversePostorder(n.rchild)
	fmt.Printf("%v ", n.value)
}

func (t *Tree) TraversePostorder() {
	if t == nil {
		return
	}
	traversePostorder(t.root)
}
