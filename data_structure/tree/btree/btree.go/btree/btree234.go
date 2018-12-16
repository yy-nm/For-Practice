package btree

import (
	"fmt"
)

const (
	t                = 2
	need_split_count = 3
)

type node struct {
	isleaf   bool
	keys     [2*t - 1]int
	keycount int
	childs   [2 * t]*node
}

type BTree234 struct {
	root *node
}

func GetEmptyBTree234() *BTree234 {
	return &BTree234{
		root: &node{
			isleaf:   true,
			keycount: 0,
		},
	}
}

func splitChildNode(p *node, index int, n *node) {
	middleindex := need_split_count / 2

	rightnode := &node{
		isleaf:   n.isleaf,
		keycount: 0,
	}

	rightnode.keycount = middleindex
	n.keycount = middleindex

	for i := 0; i < middleindex; i++ {
		rightnode.keys[i] = n.keys[i+t]
	}
	for i := 0; i <= middleindex; i++ {
		rightnode.childs[i] = n.childs[i+t]
	}

	for i := p.keycount; i > index; i-- {
		p.keys[i] = p.keys[i-1]
	}
	for i := p.keycount; i > index; i-- {
		p.childs[i+1] = p.childs[i]
	}
	p.keys[index] = n.keys[middleindex]
	p.childs[index+1] = rightnode
	p.keycount++
}

func insertNode(n *node, k int) {
	if n.isleaf {
		index := n.keycount - 1
		for index >= 0 {
			if n.keys[index] < k {
				break
			} else if n.keys[index] == k {
				return
			}
			n.keys[index+1] = n.keys[index]
			index--
		}
		n.keys[index+1] = k
		n.keycount++
	} else {
		index := n.keycount - 1
		for index >= 0 {
			if n.keys[index] < k {
				break
			} else if n.keys[index] == k {
				return
			}
			index--
		}
		index++

		if n.childs[index].keycount == need_split_count {
			splitChildNode(n, index, n.childs[index])
			if n.keys[index] < k {
				index++
			}
		}

		insertNode(n.childs[index], k)
	}
}

func (t *BTree234) AddElement(k int) {
	if t == nil {
		return
	}

	if t.SearchElement(k) {
		return
	}

	if t.root == nil {
		t.root = &node{
			isleaf:   true,
			keycount: 0,
		}
	}

	n := t.root

	if n == t.root && n.keycount == need_split_count {
		nroot := &node{
			isleaf:   false,
			keycount: 0,
		}
		t.root = nroot
		nroot.childs[0] = n
		splitChildNode(nroot, 0, n)
		insertNode(nroot, k)
	} else {
		insertNode(n, k)
	}
}

func (t *BTree234) SearchElement(k int) bool {
	if t == nil {
		return false
	}

	n := t.root

	for n != nil {
		index := n.keycount - 1
		for index >= 0 {
			if n.keys[index] < k {
				break
			} else if n.keys[index] == k {
				return true
			}
			index--
		}
		index++
		if n.isleaf {
			break
		} else {
			n = n.childs[index]
		}
	}

	return false
}

func findMaxKeyInSubTree(n *node) int {
	if n.isleaf {
		return n.keys[n.keycount-1]
	} else {
		return findMaxKeyInSubTree(n.childs[n.keycount])
	}
}

func findMinKeyInSubTree(n *node) int {
	if n.isleaf {
		return n.keys[0]
	} else {
		return findMinKeyInSubTree(n.childs[0])
	}
}

func mergeChildNode(n *node, index int) {
	key := n.keys[index]
	left := n.childs[index]
	right := n.childs[index+1]

	left.keys[left.keycount] = key

	for i := 0; i < right.keycount; i++ {
		left.keys[left.keycount+1+i] = right.keys[i]
		left.childs[left.keycount+1+i] = right.childs[i]
	}
	left.childs[left.keycount+1+right.keycount] = right.childs[right.keycount]
	left.keycount += right.keycount + 1

	for i := index; i < n.keycount-1; i++ {
		n.keys[i] = n.keys[i+1]
		n.childs[i+1] = n.childs[i+2]
	}
	n.keycount--
}

func deleteNote(tree *BTree234, n *node, k int) {
	if n.isleaf {
		index := n.keycount - 1
		for index >= 0 {
			if n.keys[index] < k {
				return
			} else if n.keys[index] == k {
				break
			}
			index--
		}

		if index >= 0 {
			for i := index; i < n.keycount-1; i++ {
				n.keys[i] = n.keys[i+1]
			}
			n.keycount--
		}
	} else {
		index := n.keycount - 1
		findinkeys := false
		for index >= 0 {
			if n.keys[index] < k {
				break
			} else if n.keys[index] == k {
				findinkeys = true
				break
			}
			index--
		}

		if findinkeys {
			if n.childs[index].keycount >= t {
				nkey := findMaxKeyInSubTree(n.childs[index])
				n.keys[index] = nkey
				deleteNote(tree, n.childs[index], nkey)
			} else if n.childs[index+1].keycount >= t {
				nkey := findMinKeyInSubTree(n.childs[index+1])
				n.keys[index] = nkey
				deleteNote(tree, n.childs[index+1], nkey)
			} else {
				// merge childs[index + 1] and keys[index] to childs[index]
				mergeChildNode(n, index)
				if n.keycount == 0 && n == tree.root {
					tree.root = n.childs[index]
				}
				deleteNote(tree, n.childs[index], k)
			}
		} else {
			index++
			if n.childs[index].keycount >= t {
				deleteNote(tree, n.childs[index], k)
			} else if index < n.keycount {
				if n.childs[index+1].keycount >= t {
					// move a key from keys[index] to childs[index]
					// move a key from childs[index + 1] to key[index]
					// move a child from childs[index + 1] to childs[index]
					thegeter := n.childs[index]
					thegiver := n.childs[index+1]
					thegeter.keys[thegeter.keycount] = n.keys[index]
					n.keys[index] = thegiver.keys[0]
					thegeter.childs[thegeter.keycount+1] = thegiver.childs[0]
					thegeter.keycount++

					for i := 1; i < thegiver.keycount; i++ {
						thegiver.keys[i-1] = thegiver.keys[i]
						thegiver.childs[i-1] = thegiver.childs[i]
					}
					thegiver.childs[thegiver.keycount-1] = thegiver.childs[thegiver.keycount]
					thegiver.keycount--
				} else {
					// merge
					mergeChildNode(n, index)
					if n.keycount == 0 && n == tree.root {
						tree.root = n.childs[index]
					}
				}
				deleteNote(tree, n.childs[index], k)
			} else if index > 0 {
				if n.childs[index-1].keycount >= t {
					// move a key from keys[index - 1] to childs[index]
					// move a key from childs[index - 1] to keys[index + 1]
					// move a child from childs[index - 1] to childs[index]
					thegeter := n.childs[index]
					thegiver := n.childs[index-1]

					for i := thegeter.keycount; i > 0; i-- {
						thegeter.keys[i] = thegeter.keys[i-1]
						thegeter.childs[i+1] = thegeter.childs[i]
						thegeter.childs[i] = thegeter.childs[i-1]
					}
					thegeter.keys[0] = n.keys[index-1]
					n.keys[index-1] = thegiver.keys[thegiver.keycount-1]
					thegeter.childs[0] = thegiver.childs[thegiver.keycount]

					thegeter.keycount++
					thegiver.keycount--

					deleteNote(tree, n.childs[index], k)
				} else {
					mergeChildNode(n, index-1)
					if n.keycount == 0 && n == tree.root {
						tree.root = n.childs[index-1]
					}
					deleteNote(tree, n.childs[index-1], k)
				}
			}
		}
	}
}

func (t *BTree234) DeleteElement(k int) {
	if t == nil {
		return
	}

	if !t.SearchElement(k) {
		return
	}

	deleteNote(t, t.root, k)
}

func (t *BTree234) FreeTree() {
	if t != nil {
		t.root = nil
	}
}

func (t *BTree234) TraverseTree() {
	if t == nil || t.root == nil {
		return
	}

	q := make([]*node, 0)
	q = append(q, t.root)
	q = append(q, nil)

	index := 0
	for len(q) > index {
		n := q[index]
		index++
		if n == nil {
			fmt.Println()
			if index >= len(q) {
				break
			}
			q = append(q, nil)
		} else {
			fmt.Print("    ")
			for i := 0; i < n.keycount; i++ {
				fmt.Print(n.keys[i])
				fmt.Print(" ")
			}
			if !n.isleaf {
				for i := 0; i <= n.keycount; i++ {
					q = append(q, n.childs[i])
				}
			}
		}
	}
}
