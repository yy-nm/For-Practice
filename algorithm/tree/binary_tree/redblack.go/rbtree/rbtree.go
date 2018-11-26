package rbtree

import (
	"fmt"
)

type Node struct {
	lchild *Node
	rchild *Node
	parent *Node
	k      int
	black  bool
}

type Tree struct {
	root *Node
}

func leftRotation(t *Tree, n *Node) {
	if t == nil || n == nil {
		return
	}
	y := n.rchild

	n.rchild = y.lchild
	if y.lchild != nil {
		y.lchild.parent = n
	}

	y.parent = n.parent
	if n.parent != nil {
		if n.k < n.parent.k {
			n.parent.lchild = y
		} else {
			n.parent.rchild = y
		}
	}

	y.lchild = n
	n.parent = y

	if y.parent == nil {
		t.root = y
	}
}

func rightRotation(t *Tree, n *Node) {
	if t == nil || n == nil {
		return
	}

	y := n.lchild

	n.lchild = y.rchild
	if y.rchild != nil {
		y.rchild.parent = n
	}

	y.parent = n.parent
	if n.parent != nil {
		if n.k < n.parent.k {
			n.parent.lchild = y
		} else {
			n.parent.rchild = y
		}
	}

	y.rchild = n
	n.parent = y

	if y.parent == nil {
		t.root = y
	}
}

func checkNodeRed(n *Node) bool {
	return n != nil && !n.black
}

func checkNodeBlack(n *Node) bool {
	return n == nil || n.black
}

func insertFixup(t *Tree, n *Node) {

	for n != nil {
		p := n.parent
		if p == nil || p.black {
			break
		}
		pp := p.parent
		if pp == nil {
			break
		}

		// left node
		if p.k < pp.k {
			ppr := pp.rchild
			if checkNodeRed(ppr) {
				ppr.black = true
				p.black = true
				pp.black = false
				n = pp
			} else {
				// make insert right child become insert left child via left rotation
				if p.k < n.k {
					leftRotation(t, p)
					n = p
				}
				n.parent.black = true
				pp.black = false
				rightRotation(t, pp)
			}
		} else {
			ppl := pp.lchild
			if checkNodeRed(ppl) {
				ppl.black = true
				p.black = true
				pp.black = false
				n = pp
			} else {
				// make insert left child become insert right child via right rotation
				if n.k < p.k {
					rightRotation(t, n)
					n = p
				}

				n.parent.black = true
				pp.black = false
				leftRotation(t, pp)
			}
		}
	}

	if t.root != nil {
		t.root.black = true
	}
}

func (t *Tree) AddElement(k int) {
	if t == nil {
		return
	}

	n := t.root
	p := n

	for n != nil {
		p = n
		if n.k < k {
			n = n.rchild
		} else if k < n.k {
			n = n.lchild
		} else {
			return
		}
	}

	n = &Node{k: k}
	if p == nil {
		t.root = n
	} else {
		if p.k < n.k {
			p.rchild = n
		} else {
			p.lchild = n
		}
		n.parent = p
	}

	insertFixup(t, n)
}

func getMaxOne(n *Node) *Node {
	if n != nil {
		for n.rchild != nil {
			n = n.rchild
		}
	}

	return n
}

func transplant(t *Tree, from *Node, to *Node) {
	if from.parent == nil {
		t.root = to
	} else if from.parent.k < from.k {
		from.parent.rchild = to
	} else if from.k < from.parent.k {
		from.parent.lchild = to
	}

	if to != nil {
		to.parent = from.parent
	}
}

func clearTmp(n *Node) {
	if n == nil || n.parent == nil {
		return
	}
	if n == n.parent.lchild {
		n.parent.lchild = nil
	} else if n == n.parent.rchild {
		n.parent.rchild = nil
	}
}

func removeFixup(t *Tree, n *Node, use_tmp bool) {

	for n != t.root && n.black {
		// left node
		if n == n.parent.lchild {
			r := n.parent.rchild
			if r != nil && !r.black {
				r.black = true
				n.parent.black = false
				leftRotation(t, n.parent)
				r = n.parent.rchild
			}
			if r != nil && checkNodeBlack(r.lchild) && checkNodeBlack(r.rchild) {
				r.black = false
				if use_tmp {
					use_tmp = false
					clearTmp(n)
				}
				n = n.parent
			} else if r != nil {
				if checkNodeBlack(r.rchild) {
					r.lchild.black = true
					r.black = false
					rightRotation(t, r)
					r = n.parent.rchild
				}

				r.rchild.black = true
				r.black = n.parent.black
				n.parent.black = true
				leftRotation(t, n.parent)

				if use_tmp {
					use_tmp = false
					clearTmp(n)
				}

				n = t.root
			}
		} else {
			l := n.parent.lchild
			if l != nil && !l.black {
				l.black = true
				n.parent.black = false
				rightRotation(t, n.parent)
				l = n.parent.lchild
			}

			if l != nil && checkNodeBlack(l.lchild) && checkNodeBlack(l.rchild) {
				l.black = false
				if use_tmp {
					use_tmp = false
					clearTmp(n)
				}
				n = n.parent
			} else if l != nil {
				if checkNodeBlack(l.lchild) {
					l.rchild.black = true
					l.black = false
					leftRotation(t, l)
					l = n.parent.lchild
				}

				l.lchild.black = true
				l.black = n.parent.black
				n.parent.black = true
				rightRotation(t, n.parent)

				if use_tmp {
					use_tmp = false
					clearTmp(n)
				}

				n = t.root
			}
		}
	}

	if n != nil {
		n.black = true
	}
}

func (t *Tree) RemoveElement(k int) {
	if t == nil {
		return
	}

	n := t.root

	for n != nil {
		if n.k < k {
			n = n.rchild
		} else if k < n.k {
			n = n.lchild
		} else {
			break
		}
	}

	if n == nil {
		return
	}

	var tmp = &Node{black: true}
	use_tmp := false
	isblack := n.black

	var replaceone *Node
	if n.lchild == nil {
		replaceone = n.rchild
		if replaceone == nil {
			replaceone = tmp
			use_tmp = true
		}
		transplant(t, n, replaceone)
	} else if n.rchild == nil {
		replaceone = n.lchild
		transplant(t, n, replaceone)
	} else {
		one := getMaxOne(n.lchild)
		isblack = one.black

		replaceone = one.lchild
		if replaceone == nil {
			replaceone = tmp
			use_tmp = true
		}

		if one.parent == n {
			replaceone.parent = n
		} else {
			transplant(t, one, replaceone)
			one.lchild = n.lchild
			one.lchild.parent = one
		}

		transplant(t, n, one)
		one.rchild = n.rchild
		one.rchild.parent = one
		one.black = n.black
	}

	if isblack {
		removeFixup(t, replaceone, use_tmp)
	} else if use_tmp {
		clearTmp(replaceone)
	}
}

func (t *Tree) SearchElement(k int) bool {
	if t == nil {
		return false
	}

	n := t.root
	for n != nil {
		if n.k < k {
			n = n.rchild
		} else if k < n.k {
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

	fmt.Printf("%v(%t) ", n.k, n.black)
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
	fmt.Printf("%v(%t) ", n.k, n.black)
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
	fmt.Printf("%v(%t) ", n.k, n.black)
}

func (t *Tree) TraversePostorder() {
	if t == nil {
		return
	}
	traversePostorder(t.root)
}
