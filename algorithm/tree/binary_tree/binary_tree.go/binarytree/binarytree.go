package binarytree

import (
	"fmt"
)

type Node struct {
	lchild, rchild *Node
	value          int
}

type Tree struct {
	root *Node
}

func (t *Tree) AddElement(v int) {
	if t == nil {
		return
	}

	n := t.root
	var p *Node
	for n != nil {
		p = n
		if n.value < v {
			n = n.rchild
		} else if v < n.value {
			n = n.lchild
		} else {
			return
		}
	}

	if p == nil {
		t.root = &Node{value: v}
	} else {
		if p.value < v {
			p.rchild = &Node{value: v}
		} else {
			p.lchild = &Node{value: v}
		}
	}
}

func (t *Tree) RemoveElement(v int) {
	if t == nil {
		return
	}

	n := t.root
	var p *Node
	for n != nil {
		p = n
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

	var c *Node
	var cp *Node

	if n.rchild != nil {
		cp = n
		c = n.rchild
		for c.lchild != nil {
			cp = c
			c = c.lchild
		}
	} else if n.lchild != nil {
		cp = n
		c = n.lchild
		for c.rchild != nil {
			cp = c
			c = c.rchild
		}
	}

	if c == nil && cp == nil {
		if p.value < n.value {
			p.rchild = nil
		} else {
			p.lchild = nil
		}
	} else {
		var cc *Node
		if c.lchild != nil {
			cc = c.lchild
		} else if c.rchild != nil {
			cc = c.rchild
		}

		if c.value < cp.value {
			cp.lchild = cc
		} else {
			cp.rchild = cc
		}

		n.value = c.value
	}
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

// traverse tree

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
